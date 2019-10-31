/**
 *  Programa teste para o controle local FeedForward
 *  O programa ira identificar a relacao entre PWM e Velocidade de cada motor para cada
 *  sentido de rotacao
 **/
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
// #include "esp_timer.h"

#include "driver/periph_ctrl.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/pcnt.h"
#include "driver/timer.h"
#include "driver/mcpwm.h"

#include "nvs.h"
#include "nvs_flash.h"

//Bluetooth
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"

#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

#define DEVICE_NAME "ESP_ROBO_0"

#define Fs  300.0   //  freq. de amostragem
#define DELTA_T 1/Fs
#define DELTA_T_us DELTA_T*1000000.0 //DELTA_T em us

//Motor 1 (Esquerdo)
#define  GPIO_PWM_LEFT       23  //Controla a velocidade do motor A (Esquerdo)
#define  GPIO_A1N1_LEFT      21 //Sentido motor A
#define  GPIO_A1N2_LEFT      19 //Sentido motor A
#define  GPIO_OUTA_CAP0_LEFT 33 //Sinal de saida do encoder Direito (Usado para calcular a velocidade)
#define  GPIO_OUTB_CAP1_LEFT 32 //Sinal em quadrado com relacao ao CAP1A do motor 2 (usado para calcular o sentido de rotacao)
// Modo Standby do driver
#define  GPIO_STBY        18
//Motor 2 (Direito)
#define  GPIO_PWM_RIGHT      16  //  Controla a velocidade do motor B (Direito)
#define  GPIO_B1N1_RIGHT      5  // Sentido motor
#define  GPIO_B1N2_RIGHT      17  // Sentido motor B
#define  GPIO_OUTA_CAP0_RIGHT 15  //Sinal de saida do encoder Esquerdo (usado para calcular a velocidade)
#define  GPIO_OUTB_CAP1_RIGHT 2  //Sinal em quadrado com relacao ao CAP0A do motor 1 (usado para identificar o sentido de rotacao)

#define CAP0_INT_EN BIT(27)  //Capture 0 interrupt bit
#define CAP1_INT_EN BIT(28)  //Capture 1 interrupt bit

//#########################################################################################
enum{
  LEFT_FRONT,
  LEFT_BACK,
  RIGHT_FRONT,
  RIGHT_BACK
};
enum MOTOR{
  LEFT,
  RIGHT
};
struct CoefLine{
  float alpha;
  float beta;
};
//############################### Variaveis Globais ######################################
/*flags para auxiliar a identificar velocidade nula (quando o encoder nao gera interrupcao)
*/
static mcpwm_dev_t *MCPWM[2] = {&MCPWM0, &MCPWM1};

struct CoefLine coef[4];        //coef. das retas PWM(velocity)
static float maxVelocity = 0.0; //velocidade maxima considerada

static float velocity[2] = {0.0, 0.0};
static bool  velZero[2]  = {true, true};
//Identificador do Bluetooth
static uint32_t bt_handle = 0;
//########################################################################################
static void IRAM_ATTR isr_EncoderLeft();
static void IRAM_ATTR isr_EncoderRight();
static void esp_spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
static void periodic_callback();
static void motorControl(bool frontLeft, bool frontRight, float leftVelPercent, float rigthVelPercent);
static void controlSignal(bool frontLeft, bool frontRight, float pwmLeft, float pwmRight);
static void calibration();
double get_time_sec(void);
//######################################################################################

static void config_gpio(){
  //config. GPIO
  #define GPIO_OUTPUT_PIN_SEL (1ULL << GPIO_STBY) | (1ULL << GPIO_A1N1_LEFT)  | (1ULL << GPIO_A1N2_LEFT) | \
                              (1ULL << GPIO_B1N1_RIGHT) | (1ULL << GPIO_B1N2_RIGHT)
  //##### SET GPIO CONFIG ########
  gpio_config_t io_conf;
  //disable interrupt
  io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
  //set as output mode
  io_conf.mode = GPIO_MODE_OUTPUT;
  //bit mask of the pins that you want to set,e.g.GPIO18/19
  io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
  //disable pull-down mode
  io_conf.pull_down_en = 0;
  //disable pull-up mode
  io_conf.pull_up_en = 0;
  //configure GPIO with the given settings
  gpio_config(&io_conf);

  mcpwm_pin_config_t pin_configLeft = {
      .mcpwm0a_out_num = GPIO_PWM_LEFT,
      .mcpwm_cap0_in_num   = GPIO_OUTA_CAP0_LEFT,
  };
  mcpwm_set_pin(MCPWM_UNIT_0, &pin_configLeft);

  mcpwm_pin_config_t pin_configRight = {
      .mcpwm0a_out_num = GPIO_PWM_RIGHT,
      .mcpwm_cap0_in_num   = GPIO_OUTA_CAP0_RIGHT,
  };
  mcpwm_set_pin(MCPWM_UNIT_1, &pin_configRight);

  gpio_pulldown_en(GPIO_OUTA_CAP0_LEFT);    //Enable pull down on CAP0   signal
  gpio_pulldown_en(GPIO_OUTA_CAP0_RIGHT);    //Enable pull down on CAP0   signal

  mcpwm_config_t pwm_config;
  pwm_config.frequency = 10000;    //frequency = 10kHz
  pwm_config.cmpr_a = 0.0;       //duty cycle of PWMxA = 0.0%
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);   //Configure PWM0A & PWM0B with above settings
  mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_0, &pwm_config);   //Configure PWM0A & PWM0B with above settings

  mcpwm_capture_enable(MCPWM_UNIT_0, MCPWM_SELECT_CAP0, MCPWM_POS_EDGE, 0);  //capture signal on rising edge, prescale = 0 i.e. 800,000,000 counts is equal to one second
  mcpwm_capture_enable(MCPWM_UNIT_1, MCPWM_SELECT_CAP0, MCPWM_POS_EDGE, 0);  //capture signal on rising edge, prescale = 0 i.e. 800,000,000 counts is equal to one second

  vTaskDelete(NULL);
}

static void mcpwm_init_isrLeft()
{
  MCPWM[MCPWM_UNIT_0]->int_ena.val = CAP0_INT_EN;  //Enable interrupt on  CAP0, CAP1 and CAP2 signal
  mcpwm_isr_register(MCPWM_UNIT_0, isr_EncoderLeft, NULL, ESP_INTR_FLAG_IRAM, NULL);  //Set ISR Handle
  vTaskDelete(NULL);
}

static void mcpwm_init_isrRight()
{
  MCPWM[MCPWM_UNIT_1]->int_ena.val = CAP0_INT_EN;  //Enable interrupt on  CAP0, CAP1 and CAP2 signal
  mcpwm_isr_register(MCPWM_UNIT_1, isr_EncoderRight, NULL, ESP_INTR_FLAG_IRAM, NULL);  //Set ISR Handle
  vTaskDelete(NULL);
}

void time_init(){
  // //Config. Timer
  const esp_timer_create_args_t periodic_timer_args = {
          .callback = &periodic_callback,
          /* name is optional, but may help identify the timer when debugging */
          .name = "periodic_timer"
  };
  esp_timer_handle_t periodic_timer;
  ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
  // ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, DELTA_T_us));
  ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 0.8*1000000));
  vTaskDelete(NULL);
}

static void config_bluetooth()
{
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      nvs_flash_erase();
      nvs_flash_init();
  }
  esp_bt_controller_mem_release(ESP_BT_MODE_BLE);

  esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  esp_bt_controller_init(&bt_cfg);
  esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);
  esp_bluedroid_init();
  esp_bluedroid_enable();
  esp_spp_register_callback(esp_spp_callback);
  esp_spp_init(ESP_SPP_MODE_CB);
  esp_bt_dev_set_device_name(DEVICE_NAME);

  vTaskDelete(NULL);
}

void app_main()
{
  //configuracoes iniciais
  xTaskCreatePinnedToCore(config_bluetooth, "config_bluetooth", 2048, NULL, 5, NULL, 0);
  xTaskCreatePinnedToCore(config_gpio, "config_gpio", 4096, NULL, 5, NULL, 0);
  xTaskCreatePinnedToCore(mcpwm_init_isrLeft, "mcpwm 0", 4096, NULL, 4, NULL, 0);
  xTaskCreatePinnedToCore(mcpwm_init_isrRight, "mcpwm 1", 4096, NULL, 4, NULL, 0);
  xTaskCreatePinnedToCore(time_init, "timer init", 2048, NULL, 5, NULL, 1);

  // while (1) {
  //   vTaskDelay(500/portTICK_PERIOD_MS);
  //   printf("Esquerdo Vel:%lf\n", velocity[LEFT]);
  //   printf("Direito  Vel:%lf\n", velocity[RIGHT]);
  // }
}

static void IRAM_ATTR isr_EncoderLeft()
{
  static double old_time = 0;
  double new_time = get_time_sec();

  velocity[LEFT] = 1.0/(new_time - old_time);
  old_time = new_time;
  velZero[LEFT] = false;

  uint32_t mcpwm_intr_status;
  mcpwm_intr_status = MCPWM[MCPWM_UNIT_0]->int_st.val; //Read interrupt status
  MCPWM[MCPWM_UNIT_0]->int_clr.val = mcpwm_intr_status;
}

static void IRAM_ATTR isr_EncoderRight()
{
  static double old_time = 0;
  double new_time = get_time_sec();

  velocity[RIGHT] = 1.0/(new_time - old_time);
  old_time = new_time;
  velZero[RIGHT]  = false;

  uint32_t mcpwm_intr_status;
  mcpwm_intr_status = MCPWM[MCPWM_UNIT_1]->int_st.val; //Read interrupt status
  MCPWM[MCPWM_UNIT_1]->int_clr.val = mcpwm_intr_status;
}

//Interrupcao de timeout, mede velocidade de cada motor.
//Chamado a cada 5ms
static void periodic_callback()
{
  if(velZero[LEFT])velocity[LEFT] = 0.0;
  if(velZero[RIGHT])velocity[RIGHT] = 0.0;

  velZero[LEFT] = true;
  velZero[RIGHT]= true;
}

static void controlSignal(bool frontLeft, bool frontRight, float pwmLeft, float pwmRight)
{
  gpio_set_level(GPIO_A1N1_LEFT, !frontLeft);
  gpio_set_level(GPIO_A1N2_LEFT, frontLeft);
  //set modo de rotacao do motor direito
  gpio_set_level(GPIO_B1N1_RIGHT, !frontRight);
  gpio_set_level(GPIO_B1N2_RIGHT, frontRight);

  gpio_set_level(GPIO_STBY, 0);
  //set PWM motor esquerdo
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, pwmLeft);
  //set PWM motor direito
  mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_0, MCPWM0A, pwmRight);
  //driver on
  gpio_set_level(GPIO_STBY, 1);
}

//Funcao de tratamento de eventos do bluetooth
static void
esp_spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    switch (event){
    case ESP_SPP_INIT_EVT:
        esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
        esp_spp_start_srv(ESP_SPP_SEC_NONE, ESP_SPP_ROLE_SLAVE, 0, "ESP32_SPP_SERVER");
        break;
    case ESP_SPP_DISCOVERY_COMP_EVT:
        break;
    case ESP_SPP_OPEN_EVT:
      bt_handle = param->open.handle;
        break;
    case ESP_SPP_CLOSE_EVT:
      bt_handle = 0;
        break;
    case ESP_SPP_START_EVT:
        break;
    case ESP_SPP_CL_INIT_EVT:
        break;
    case ESP_SPP_DATA_IND_EVT:
        // printf("Tamanho: %d da mensagem: %s\n", param->data_ind.len, param->data_ind.data);
        if((param->data_ind.data[0] & 0b11110000) == 0b10100000 )
        {
          if((param->data_ind.data[0] & 0b00001100) == 0b00000100) //calibracao  01
          {
            calibration();
            break;
          }

          if((param->data_ind.data[0] & 0b00001100) == 0b00001000) //controlSignal 10
          {
              motorControl(param->data_ind.data[0] & 0x02, param->data_ind.data[0] & 0x01,
                           param->data_ind.data[1]/255.0 , param->data_ind.data[2]/255.0);
              break;
          }

          if((param->data_ind.data[0] & 0b00001100) == 0b00001100) //controlSignal 11
          {
              controlSignal(param->data_ind.data[0] & 0x02    , param->data_ind.data[0] & 0x01,
                            100*param->data_ind.data[1]/255.0 , 100*param->data_ind.data[2]/255.0);
              break;
          }
        }
        break;
    case ESP_SPP_CONG_EVT:
        break;
    case ESP_SPP_WRITE_EVT:
        break;
    case ESP_SPP_SRV_OPEN_EVT:
      bt_handle = param->open.handle;
        break;
    default:
        break;
    }
}

static void
motorControl(bool frontLeft, bool frontRight, float leftVelPercent, float rigthVelPercent)
{
  float pwmLeft, pwmRight;
  float left_vel  = leftVelPercent*maxVelocity;
  float rigth_vel = rigthVelPercent*maxVelocity;

  if(frontLeft)
  {
    pwmLeft = coef[LEFT_FRONT].alpha*left_vel + coef[LEFT_FRONT].beta*( left_vel != 0 );
  }else{
    pwmLeft = coef[LEFT_BACK].alpha*left_vel  + coef[LEFT_BACK].beta*( left_vel  != 0 );
  }

  if(frontRight)
  {
    pwmRight = coef[RIGHT_FRONT].alpha*rigth_vel + coef[RIGHT_FRONT].beta*( rigth_vel != 0 );
  }else{
    pwmRight = coef[RIGHT_BACK].alpha*rigth_vel  + coef[RIGHT_BACK].beta*( rigth_vel  != 0 );
  }
  controlSignal(frontLeft, frontRight, pwmLeft, pwmRight);
}

double get_time_sec(void)
{
  static struct timeval tbase={-1,-1};
  struct timeval t;

  gettimeofday(&t,NULL);
  if (tbase.tv_sec==-1 && tbase.tv_usec==-1)
    {
      tbase = t;
    }
  return( t.tv_sec-tbase.tv_sec + (t.tv_usec-tbase.tv_usec)/1000000.0 );
}

static void calibration()
{
  #define PWM_MIN_INIT         20.0
  #define TIME_MS_WAIT_PWM_MIN 1000
  #define TIME_MS_WAIT_VEL_MAX 1500

  float velocity_max[4];
  float pwmMin[4];
  float minVelocityMax = 0.0;
  //##################################### Calibrar motor direito ##########################################//
  /*************************************** LEFT FRONT *************************************************************/
  //Aplica maximo PWM
  //aguarda 500ms e armazena as 100 ultimas velocidades do motor esquerdo
  //calcula a media dessas velocidades e armazena como sendo a maior velocidade desse motor para essa rotacao
  controlSignal(true, false, 100.0, 0.0);
  vTaskDelay(TIME_MS_WAIT_VEL_MAX/portTICK_PERIOD_MS);
  velocity_max[LEFT_FRONT] = velocity[LEFT];
  minVelocityMax = velocity_max[LEFT_FRONT];
  //Encontrando o PWM minimo
  for(float pwm = PWM_MIN_INIT; pwm > 0; pwm = pwm - 2.0)
  {
    controlSignal(true, false, pwm, 0.0);
    vTaskDelay(TIME_MS_WAIT_PWM_MIN/portTICK_PERIOD_MS);
    if(velocity[LEFT] == 0.0)
    {
      pwmMin[LEFT_FRONT] = pwm;
      pwm = 0;
    }
  }
  controlSignal(false, false, 0.0, 0.0);
  /*************************************** LEFT BACK *************************************************************/
  //Aplica maximo PWM
  //aguarda 500ms e armazena as 100 ultimas velocidades do motor esquerdo
  //calcula a media dessas velocidades e armazena como sendo a maior velocidade desse motor para essa rotacao
  controlSignal(false, false, 100.0, 0.0);
  vTaskDelay(TIME_MS_WAIT_VEL_MAX/portTICK_PERIOD_MS);
  velocity_max[LEFT_BACK] = velocity[LEFT];

  //Encontrando o PWM minimo
  for(float pwm = PWM_MIN_INIT; pwm > 0; pwm = pwm - 2.0)
  {
    controlSignal(false, false, pwm, 0.0);
    vTaskDelay(TIME_MS_WAIT_PWM_MIN/portTICK_PERIOD_MS);
    if(velocity[LEFT] == 0.0)
    {
      pwmMin[LEFT_BACK] = pwm;
      pwm = 0;
    }
  }
  controlSignal(false, false, 0.0, 0.0);
  //##################################### Calibrar motor direito ##########################################//
  /*************************************** RIGHTFRONT *************************************************************/
  //Aplica maximo PWM
  //aguarda 500ms e armazena as 100 ultimas velocidades do motor esquerdo
  //calcula a media dessas velocidades e armazena como sendo a maior velocidade desse motor para essa rotacao
  controlSignal(false, true, 0.0, 100.0);
  vTaskDelay(TIME_MS_WAIT_VEL_MAX/portTICK_PERIOD_MS);
  velocity_max[RIGHT_FRONT] = velocity[RIGHT];
  //Encontrando o PWM minimo
  for(float pwm = PWM_MIN_INIT; pwm > 0; pwm = pwm - 2.0)
  {
    controlSignal(false, true, 0.0, pwm);
    vTaskDelay(TIME_MS_WAIT_PWM_MIN/portTICK_PERIOD_MS);
    if(velocity[RIGHT] == 0)
    {
      pwmMin[RIGHT_BACK] = pwm;
      pwm = 0;
    }
  }
  controlSignal(false, false, 0.0, 0.0);

  /*************************************** RIGHTBACK *************************************************************/
  //Aplica maximo PWM
  //aguarda 500ms e armazena as 100 ultimas velocidades do motor esquerdo
  //calcula a media dessas velocidades e armazena como sendo a maior velocidade desse motor para essa rotacao
  controlSignal(false, false, 0.0, 100.0);
  vTaskDelay(TIME_MS_WAIT_VEL_MAX/portTICK_PERIOD_MS);
  velocity_max[RIGHT_BACK] = velocity[RIGHT];
  //Encontrando o PWM minimo
  for(float pwm = PWM_MIN_INIT; pwm > 0; pwm = pwm - 2.0)
  {
    controlSignal(false, false, 0.0, pwm);
    vTaskDelay(TIME_MS_WAIT_PWM_MIN/portTICK_PERIOD_MS);
    if(velocity[RIGHT] == 0)
    {
      pwmMin[RIGHT_BACK] = pwm;
      pwm = 0;
    }
  }
  controlSignal(false, false, 0.0, 0.0);

  //Calculo dos coef.
  coef[LEFT_FRONT].alpha = (100.0 - pwmMin[LEFT_FRONT])/(velocity_max[LEFT_FRONT]);
  coef[LEFT_FRONT].beta  = pwmMin[LEFT_FRONT];

  coef[LEFT_BACK].alpha = (100.0 - pwmMin[LEFT_BACK])/(velocity_max[LEFT_BACK]);
  coef[LEFT_BACK].beta  = pwmMin[LEFT_BACK];

  coef[RIGHT_FRONT].alpha = (100.0 - pwmMin[RIGHT_FRONT])/(velocity_max[RIGHT_FRONT]);
  coef[RIGHT_FRONT].beta  = pwmMin[RIGHT_FRONT];

  coef[RIGHT_BACK].alpha = (100.0 - pwmMin[RIGHT_BACK])/(velocity_max[RIGHT_BACK]);
  coef[RIGHT_BACK].beta  = pwmMin[RIGHT_BACK];

  //Menor das Maximas velocidades
  for(uint8_t i = 0; i < 4; i++)
  {
    minVelocityMax = (velocity_max[i] < minVelocityMax)?velocity_max[i]:minVelocityMax;
  }
  //Maior velocidade considerada
  maxVelocity = 0.80 * minVelocityMax;
}
