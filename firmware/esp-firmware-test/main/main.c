/**
 *  Programa teste para o controle local FeedForward
 *  O programa ira identificar a relacao entre PWM e Velocidade de cada motor para cada
 *  sentido de rotacao
 **/

#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"
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

#include "esp_attr.h"

#include "common.h"


#define GPIO_OUTPUT_PIN_SEL ((1ULL << GPIO_STBY)      | \
                             (1ULL << GPIO_A1N1_LEFT) | \
                             (1ULL << GPIO_A1N2_LEFT) | \
                             (1ULL << GPIO_B1N1_RIGHT)| \
                             (1ULL << GPIO_B1N2_RIGHT))

#define GPIO_INPUT_PIN_SEL ( (1ULL << GPIO_OUTA_LEFT)  | \
                             (1ULL << GPIO_OUTB_LEFT)  | \
                             (1ULL << GPIO_OUTA_RIGHT) | \
                             (1ULL << GPIO_OUTB_RIGHT))

/*************************************************************************************/
/********************************* CABEÇALHOS ****************************************/
/*************************************************************************************/
static void config_bluetooth();
static void config_gpio();
static void periodics_func_config();

//interruptions and callbacks
static void IRAM_ATTR isr_EncoderLeft(void* arg);
static void IRAM_ATTR isr_EncoderRight(void* arg);
static void esp_spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
static void periodic_test_omega_zero_cb(void *arg);
static void periodic_controller(void *arg);
//my functions
static void func_controlSignal(const float pwmL,const float pwmR);
static void func_calibration();
/*************************************************************************************/
/****************************** VARIAVEIS GLOBAIS ************************************/
/*************************************************************************************/
static xQueueHandle bt_queue = NULL;

static bool controller_enable = true;

struct CoefLine coefL[2];       //coef. das retas PWM(omega) para o motor esquerdo
struct CoefLine coefR[2];       //coef. das retas PWM(omega) para o motor direito
static float omega_max = 0.0;   //modulo da velocidade maxima do robo

static bool  omega_zero[2]     = {true, true};
static float omega_ref[2]      = {0.0, 0.0}; //-1.0 a 1.0
static float omega_current[2]  = {0.0, 0.0}; //-1.0 a 1.0
//Identificador do Bluetooth
static uint32_t bt_handle = 0;
/*************************************************************************************/
/****************************** ROTINAS PRINCIPAIS ***********************************/
/*************************************************************************************/
void app_main()
{
  bt_data btdata;
  bt_queue   = xQueueCreate(1, sizeof(bt_data));

  /** configuracoes iniciais **/
  //interrupcoes no nucleo 0
  xTaskCreatePinnedToCore(config_gpio, "config_gpio", 4096, NULL, 5, NULL, 0);
  xTaskCreatePinnedToCore(config_bluetooth, "config_bluetooth", 2048, NULL, 5, NULL, 0);

  //controlador no nucleo 1
  xTaskCreatePinnedToCore(periodics_func_config, "periodics_func_config", 4096, NULL, 4, NULL, 1);

  uint8_t  head, cmd;
  uint8_t  *bitstream;
  float    *vec_float;
  while(1){
      xQueueReceive(bt_queue, &btdata, portMAX_DELAY);

      head = btdata.data[0] & 0xF0;
      if(head != CMD_HEAD)
        continue;

      cmd = btdata.data[0] & 0x0F;

      switch (cmd) {
      case CMD_SET_POINT:
        if(!controller_enable)
          controller_enable = true;
        decodeFloat(btdata.data, &omega_ref[LEFT], &omega_ref[RIGHT]);
        break;
      case CMD_CONTROL_SIGNAL:
        if(controller_enable)
          controller_enable = false;
        decodeFloat(btdata.data, &omega_ref[LEFT], &omega_ref[RIGHT]);
        break;
      case CMD_PING:
        esp_spp_write(bt_handle, btdata.len, btdata.data);
        break;
      case CMD_IDENTIFY:
        //Falta fazer
        break;
      case CMD_CALIBRATION:
        func_calibration();
        break;
      case CMD_REQ_OMEGA:
        esp_spp_write(bt_handle, 2*sizeof(float), (uint8_t*)omega_current);
        break;
      case CMD_REQ_CAL:
        bitstream = (uint8_t*)malloc(8*sizeof(float));
        vec_float = (float*)malloc(8*sizeof(float));

        vec_float[0] = coefL[FRONT].alpha;
        vec_float[1] = coefL[FRONT].beta;
        vec_float[2] = coefL[BACK].alpha;
        vec_float[3] = coefL[BACK].beta;

        vec_float[4] = coefR[FRONT].alpha;
        vec_float[5] = coefR[FRONT].beta;
        vec_float[6] = coefR[BACK].alpha;
        vec_float[7] = coefR[BACK].beta;

        esp_spp_write(bt_handle, 8*sizeof(float), (uint8_t*)vec_float);
        free(vec_float);
        break;
      case CMD_REQ_IDENT:
        //Falta fazer
        // esp_spp_write(bt_handle, btdata.len, btdata.data);
        break;
      default:
        break;
      }
  }
}
//***************************************************************************************

// gettimeofday(&t,NULL);
// t.tv_sec + t.tv_usec/1000000.0;
static void IRAM_ATTR isr_EncoderLeft(void*arg)
{
  omega_zero[LEFT] = false;
  static struct timeval t;
  static int8_t  lookup_table[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
  static double  old_time = 0, new_time = 0;
  static uint8_t enc_v = 0;
  gettimeofday(&t, NULL);

  enc_v <<= 2;
  enc_v |= ((uint32_t)REG_READ(GPIO_IN1_REG) & 0x02);
  new_time =  t.tv_usec/1000000.0;
  omega_current[0] = lookup_table[enc_v & 0b1111]/(new_time - old_time);
  // omega_current[0] = 1.0/(new_time - old_time);
  old_time = new_time;
}
static void IRAM_ATTR isr_EncoderRight(void*arg)
{
  omega_zero[RIGHT] = false;
  static struct timeval t;
  static int8_t   lookup_table[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
  static double   old_time = 0, new_time = 0;
  static uint8_t  enc_v = 0;
  static uint32_t reg_read;

  gettimeofday(&t, NULL);

  enc_v <<= 2;
  //WARNING, trocar as GPIO dos canais A e B, para serem adjacentes, isso economizaria tempo na leitura
  reg_read = REG_READ(GPIO_IN_REG);
  enc_v |= (reg_read & LS(GPIO_OUTA_RIGHT)) >> (GPIO_OUTA_RIGHT-1);
  enc_v |= (reg_read & LS(GPIO_OUTB_RIGHT)) >> GPIO_OUTB_RIGHT;
  new_time =  t.tv_usec/1000000.0;
  omega_current[1] = lookup_table[enc_v & 0b1111]/(new_time - old_time);
  old_time = new_time;
}

//Chamado a cada 800ms
//Testa se houve interrupcao dos encoders, rotina para medir velocidade zero
static void periodic_test_omega_zero_cb(void *arg)
{
  if(omega_zero[LEFT])
  {
    omega_current[LEFT] = 0.0;
  }
  if(omega_zero[RIGHT])
  {
    omega_current[RIGHT] = 0.0;
  }
  omega_zero[LEFT] = true;
  omega_zero[RIGHT]= true;
}

static void func_controlSignal(const float pwmL,const float pwmR)
{
  static bool front[2]  = {false, false};
  front[LEFT]  = !F_IS_NEG(pwmL);
  front[RIGHT] = !F_IS_NEG(pwmR);

  gpio_set_level(GPIO_STBY, 0);
  gpio_set_level(GPIO_A1N1_LEFT, !front[LEFT]);
  gpio_set_level(GPIO_A1N2_LEFT, front[LEFT]);
  //set modo de rotacao do motor direito
  gpio_set_level(GPIO_B1N1_RIGHT, !front[RIGHT]);
  gpio_set_level(GPIO_B1N2_RIGHT, front[RIGHT]);

  //set PWM motor esquerdo
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, ABS_F(pwmL)*100.0);
  //set PWM motor direito
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0B, ABS_F(pwmR)*100.0);

  //driver on
  gpio_set_level(GPIO_STBY, (pwmL != 0.0) || (pwmR != 0.0)); //se algum pwm for diferente de zero, liga o drive (1)
}
//Funcao de tratamento de eventos do bluetooth
static void
esp_spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  bt_data btdata;
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
        memset(omega_ref, 0, 2*sizeof(float));
        break;
    case ESP_SPP_START_EVT:
        break;
    case ESP_SPP_CL_INIT_EVT:
        break;
    case ESP_SPP_DATA_IND_EVT:
        // param->data_ind.len e param->data_ind.data
        btdata.data = param->data_ind.data;
        btdata.len  = param->data_ind.len;
        xQueueSend(bt_queue, &btdata, NULL);
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
periodic_controller(void *arg)
{
  static float pwm[2];

  if(!controller_enable)
  {
    func_controlSignal(omega_ref[LEFT], omega_ref[RIGHT]);
    return;
  }

  //controlador FeedForWard
  // pwm[LEFT]  = coefL[sense[LEFT] ].alpha*omega_ref[LEFT]  +
  //              coefL[sense[LEFT] ].beta*(omega_ref[LEFT]  != 0 );
  // pwm[RIGHT] = coefR[sense[RIGHT]].alpha*omega_ref[RIGHT] +
               // coefR[sense[RIGHT]].beta*(omega_ref[RIGHT] != 0 );
  /****
  *aqui ficara o controlador BackWard (PID)
  ****/

  //aplicar sinal de controle
  func_controlSignal(pwm[LEFT], pwm[RIGHT]);
}
static void func_calibration()
{
  //desabilita a rotina de controle para realizar esse procedimento
  controller_enable = false;

  #define PWM_MIN_INIT          0.3  // 30%
  #define TIME_MS_WAIT_PWM_MIN 1000
  #define TIME_MS_WAIT_VEL_MAX 1500
  const float STEP = 1.0/32767.0;    //por causa da resolucao de 15 bits na transmissao da ref

  float omega_max_tmpL[2];
  float omega_max_tmpR[2];
  float pwmMinL[2];
  float pwmMinR[2];
  float minOmegaMax = 0.0;
  //##################################### Calibrar motor direito ##########################################//
  /*************************************** LEFT FRONT *************************************************************/
  //Aplica maximo PWM
  //aguarda 500ms e armazena as 100 ultimas velocidades do motor esquerdo
  //calcula a media dessas velocidades e armazena como sendo a maior velocidade desse motor para essa rotacao
  func_controlSignal(1.0, 0.0);
  vTaskDelay(TIME_MS_WAIT_VEL_MAX/portTICK_PERIOD_MS);
  omega_max_tmpL[FRONT] = omega_current[LEFT];
  minOmegaMax = omega_max_tmpL[FRONT];
  //Encontrando o PWM minimo
  for(float pwm = PWM_MIN_INIT; pwm > 0.0; pwm = pwm - STEP)
  {
    func_controlSignal(pwm, 0.0);
    vTaskDelay(TIME_MS_WAIT_PWM_MIN/portTICK_PERIOD_MS);
    if(omega_current[LEFT] == 0.0)
    {
      pwmMinL[FRONT] = pwm;
      pwm = 0.0;
    }
  }
  func_controlSignal(0.0, 0.0);
  /*************************************** LEFT BACK *************************************************************/
  //Aplica maximo PWM
  //aguarda 500ms e armazena as 100 ultimas velocidades do motor esquerdo
  //calcula a media dessas velocidades e armazena como sendo a maior velocidade desse motor para essa rotacao
  func_controlSignal(-1.0, 0.0);
  vTaskDelay(TIME_MS_WAIT_VEL_MAX/portTICK_PERIOD_MS);
  omega_max_tmpL[BACK] = omega_current[LEFT];

  //Encontrando o PWM minimo
  for(float pwm = PWM_MIN_INIT; pwm > 0; pwm = pwm - STEP)
  {
    func_controlSignal(-pwm, 0.0);
    vTaskDelay(TIME_MS_WAIT_PWM_MIN/portTICK_PERIOD_MS);
    if(omega_current[LEFT] == 0.0)
    {
      pwmMinL[BACK] = pwm;
      pwm = 0.0;
    }
  }
  func_controlSignal(0.0, 0.0);
  //##################################### Calibrar motor direito ##########################################//
  /*************************************** RIGHT FRONT *************************************************************/
  //Aplica maximo PWM
  //aguarda 500ms e armazena as 100 ultimas velocidades do motor esquerdo
  //calcula a media dessas velocidades e armazena como sendo a maior velocidade desse motor para essa rotacao
  func_controlSignal(0.0, 1.0);
  vTaskDelay(TIME_MS_WAIT_VEL_MAX/portTICK_PERIOD_MS);
  omega_max_tmpR[FRONT] = omega_current[RIGHT];
  //Encontrando o PWM minimo
  for(float pwm = PWM_MIN_INIT; pwm > 0; pwm = pwm - STEP)
  {
    func_controlSignal(0.0, pwm);
    vTaskDelay(TIME_MS_WAIT_PWM_MIN/portTICK_PERIOD_MS);
    if(omega_current[RIGHT] == 0)
    {
      pwmMinR[BACK] = pwm;
      pwm = 0;
    }
  }
  func_controlSignal(0.0, 0.0);
  /*************************************** RIGHT BACK *************************************************************/
  //Aplica maximo PWM
  //aguarda 500ms e armazena as 100 ultimas velocidades do motor esquerdo
  //calcula a media dessas velocidades e armazena como sendo a maior velocidade desse motor para essa rotacao
  func_controlSignal(0.0, -1.0);
  vTaskDelay(TIME_MS_WAIT_VEL_MAX/portTICK_PERIOD_MS);
  omega_max_tmpR[BACK] = omega_current[RIGHT];
  //Encontrando o PWM minimo
  for(float pwm = PWM_MIN_INIT; pwm > 0; pwm = pwm - 2.0)
  {
    func_controlSignal(0.0, -pwm);
    vTaskDelay(TIME_MS_WAIT_PWM_MIN/portTICK_PERIOD_MS);
    if(omega_current[RIGHT] == 0)
    {
      pwmMinR[BACK] = pwm;
      pwm = 0;
    }
  }

  func_controlSignal(0.0, 0.0);
  for(uint8_t i = 0; i < 2; i++)
  {
    //Calculo dos coef.
    coefL[i].alpha = (1.0 - pwmMinL[i])/(omega_max_tmpL[i]);
    coefL[i].beta  = pwmMinL[i];

    coefR[i].alpha = (1.0 - pwmMinR[i])/(omega_max_tmpR[i]);
    coefR[i].beta  = pwmMinR[i];

    //Menor das Maximas velocidades
    minOmegaMax = (omega_max_tmpL[i] < minOmegaMax)?omega_max_tmpL[i]:minOmegaMax;
    minOmegaMax = (omega_max_tmpR[i] < minOmegaMax)?omega_max_tmpR[i]:minOmegaMax;
  }
  //Maior velocidade considerada
  omega_max = 0.85 * minOmegaMax;
}

/*************************************************************************************/
/****************************** CONFIGURACOES ****************************************/
/*************************************************************************************/
void periodics_func_config()
{
  //Config. Timer
  const esp_timer_create_args_t periodic_test_args = {
          .callback = &periodic_test_omega_zero_cb,
          /* name is optional, but may help identify the timer when debugging */
          .name = "periodic_test_omega_zero_cb"
  };
  esp_timer_handle_t periodic_test_handle;
  ESP_ERROR_CHECK(esp_timer_create(&periodic_test_args, &periodic_test_handle));
  ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_test_handle,
                                           TIME_TEST_OMEGA_ZERO*1000)); //timer de 800ms, para medir velocidade zero

  const esp_timer_create_args_t periodic_controller_args = {
   .callback = &periodic_controller,
   /* name is optional, but may help identify the timer when debugging */
   .name = "periodic_controller"
  };
  esp_timer_handle_t periodic_controller_handle;
  ESP_ERROR_CHECK(esp_timer_create(&periodic_controller_args,
                                   &periodic_controller_handle));
  ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_controller_handle,
                                          TIME_CONTROLLER*1000)); //timer de 800ms, para medir velocidade zero

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

static void config_gpio(){
  //##### SET GPIO CONFIG ########
  gpio_config_t io_conf;
  io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
  io_conf.pull_down_en = 0;
  io_conf.pull_up_en = 0;
  gpio_config(&io_conf);

  io_conf.intr_type = GPIO_INTR_ANYEDGE;
  io_conf.mode = GPIO_MODE_INPUT;
  io_conf.pull_down_en = 1;
  io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
  gpio_config(&io_conf);

  gpio_install_isr_service(0);
  gpio_isr_handler_add(GPIO_OUTA_LEFT, isr_EncoderLeft, (void*) 1);
  gpio_isr_handler_add(GPIO_OUTB_LEFT, isr_EncoderLeft, (void*) 0);
  gpio_isr_handler_add(GPIO_OUTA_RIGHT,isr_EncoderRight,(void*) 1);
  gpio_isr_handler_add(GPIO_OUTB_RIGHT,isr_EncoderRight,(void*) 0);

  mcpwm_pin_config_t pin_configLeft = {
      .mcpwm0a_out_num = GPIO_PWM_LEFT,
      .mcpwm0b_out_num = GPIO_PWM_RIGHT,
  };
  mcpwm_set_pin(MCPWM_UNIT_0, &pin_configLeft);

  mcpwm_config_t pwm_config;
  pwm_config.frequency = 10000;    //frequency = 10kHz
  pwm_config.cmpr_a = 0.0;       //duty cycle of PWMxA = 0.0%
  pwm_config.cmpr_b = 0.0;       //duty cycle of PWMxA = 0.0%
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);   //Configure PWM0A & PWM0B with above settings

  vTaskDelete(NULL);
}
