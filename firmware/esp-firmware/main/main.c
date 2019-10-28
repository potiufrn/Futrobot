/********************************
               (FRENTE)
(MOTOR LEFT)||  ROBÔ   ||(MOTOR HIGHT)
               (TRÁS)
<<-- (sentido de rotação para ir para a frente)


Informações sobre o controle do Driver:
MOTOR ESQUERDO(LEFT)                   => CANAL A DO DRIVER
MOTOR DIREITO(RIGHT)                   => CANAL B DO DRIVER

DESABILITAR DRIVER/MOTORES             => GPIO_STBY = LOW   (LOGICA INVERTIDA)
HABILITAR DRIVER/MOTORES               => GPIO_STBY = HIGH  (LOGICA INVERTIDA)
PWM MOTOR DIREITO                      => GPIO_PWM_B
PWM MOTOR ESQUERDO                     => GPIO_PWM_A
MOTOR DIREITO  PRA FRENTE              => GPIO_B1N1 = HIGH E GPIO_B1N2 = LOW
MOTOR ESQUERDO PRA FRENTE              => GPIO_A1N1 = HIGH E GPIO_A1N2 = LOW
ANDAR PARA FRENTE                      => GPIO_A1N1 E GPIO_B1N1 = HIGH E GPIO_A1N2 E GPIO_B1N2 = LOW
ANDAR PARA TRÁS                        => GPIO_A1N2 E GPIO_B1N2 = HIGH E GPIO_A1N1 E GPIO_B1N1 = LOW


By: Luís Gabriel Pereira Condados - 16/10/2019
*********************************/
#include "nvs.h"
#include "nvs_flash.h"

//Bluetooth
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"

//FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

//Motor Controller PWM
#include "driver/mcpwm.h"
#include "driver/gpio.h"

//C
#include <stdio.h>

//Motor 1 - Pinos e PWM
int GPIO_PWM_A = GPIO_NUM_3; ///Controla a velocidade do motor A (Esquerdo)
int GPIO_A1N2  = GPIO_NUM_21; //Sentido motor A
int GPIO_A1N1  = GPIO_NUM_19; //Sentido motor A

// Modo Standby do driver
int GPIO_STBY = GPIO_NUM_18;

//Motor 2 - Pinos e PWM
int GPIO_B1N2 = GPIO_NUM_17; // Sentido motor B
int GPIO_B1N1 = GPIO_NUM_5; // Sentido motor
//Motor 2 - Pinos e PWM
int GPIO_PWM_B = GPIO_NUM_16; //  Controla a velocidade do motor B (Direito)

//############ Variaveis Globais ###############

//####################################################################################################
//                    mode_rotate
//Left  to front : 0bxxxxxx1x  | Left  to Back : 0bxxxxxx0x
//Right to front : 0bxxxxxxx1  | Right to Back : 0bxxxxxxx0
void motorControl(const uint8_t mode_rotate, const uint8_t pwmLeft, const uint8_t pwmRight);
void setup();

bool initBluetooth(const char* deviceName, esp_spp_cb_t *esp_spp_cb);
static void esp_spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
//####################################################################################################

//Tarefa principal, que cria as demais tarefas
void
app_main(){

  setup();
  xTaskCreatePinnedToCore(motorManager, "Motor Manager", 1024, NULL, 5, NULL, 1);
}

//Funcao que configura e inicializa o Bluetooth
bool
initBluetooth(const char* deviceName, esp_spp_cb_t *esp_spp_cb)
{
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

  esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK){
      //ESP_LOGE(SPP_TAG, "%s initialize controller failed: %s\n", _func_, esp_err_to_name(ret));
      return false;
  }

  if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK){
      //ESP_LOGE(SPP_TAG, "%s enable controller failed: %s\n", _func_, esp_err_to_name(ret));
      return false;
  }

  if ((ret = esp_bluedroid_init()) != ESP_OK) {
      //ESP_LOGE(SPP_TAG, "%s initialize bluedroid failed: %s\n", _func_, esp_err_to_name(ret));
      return false;
  }

  if ((ret = esp_bluedroid_enable()) != ESP_OK) {
      //ESP_LOGE(SPP_TAG, "%s enable bluedroid failed: %s\n", _func_, esp_err_to_name(ret));
      return false;
  }

  if ((ret = esp_spp_register_callback(esp_spp_cb)) != ESP_OK) {
      //ESP_LOGE(SPP_TAG, "%s spp register failed: %s\n", _func_, esp_err_to_name(ret));
      return false;
  }

  if ((ret = esp_spp_init(ESP_SPP_MODE_CB)) != ESP_OK) {
      //ESP_LOGE(SPP_TAG, "%s spp init failed: %s\n", _func_, esp_err_to_name(ret));
      return false;
  }

  esp_bt_dev_set_device_name(deviceName);

  return true;
}

//Funcao de tratamento de eventos do bluetooth
static void esp_spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  uint8_t bt_data[3];

    switch (event){
    case ESP_SPP_INIT_EVT:
        esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
        esp_spp_start_srv(ESP_SPP_SEC_NONE, ESP_SPP_ROLE_SLAVE, 0, "ESP32_SPP_SERVER");
        break;
    case ESP_SPP_DISCOVERY_COMP_EVT:
        printf("Evento: ESP_SPP_DISCOVERY_COMP_EVT\n");
        break;
    case ESP_SPP_OPEN_EVT:
        printf("Evento: ESP_SPP_OPEN_EVT\n");
        break;
    case ESP_SPP_CLOSE_EVT:
        printf("Evento: ESP_SPP_CLOSE_EVT\n");
        break;
    case ESP_SPP_START_EVT:
        printf("Evento: ESP_SPP_START_EVT\n");
        break;
    case ESP_SPP_CL_INIT_EVT:
        printf("Evento: ESP_SPP_CL_INIT_EVT\n");
        break;
    case ESP_SPP_DATA_IND_EVT:
        printf("Mensagem recebida! tamanho:%d\n", param->data_ind.len);
        // printf("Tamanho: %d da mensagem: %s\n", param->data_ind.len, param->data_ind.data);
        if((param->data_ind.data[0] & 0b11111100) == 0b10101000) //check head
          motorControl(param->data_ind.data[0], param->data_ind.data[1], param->data_ind.data[2]);
        break;
    case ESP_SPP_CONG_EVT:
        printf("Evento: ESP_SPP_CONG_EVT\n");
        break;
    case ESP_SPP_WRITE_EVT:
        printf("Evento: ESP_SPP_WRITE_EVT\n");
        break;
    case ESP_SPP_SRV_OPEN_EVT:
        printf("Evento: ESP_SPP_SRV_OPEN_EVT\n");
        break;
    default:
        break;
    }
}

//####################################################################################################

//Funcao que realiza as configuracoes dos pinos
void
setup()
{
  #define GPIO_OUTPUT_PIN_SEL (1ULL << GPIO_STBY) | (1ULL << GPIO_A1N1)  | (1ULL << GPIO_A1N2) | \
                              (1ULL << GPIO_B1N1) | (1ULL << GPIO_B1N2)
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

  // gpio_config_t io_conf;
  // //disable interrupt
  // io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
  // //set as output mode
  // io_conf.mode = GPIO_MODE_OUTPUT;
  // //bit mask of the pins that you want to set,e.g.GPIO18/19
  // io_conf.pin_bit_mask = (1ULL << GPIO_STBY) | (1ULL << GPIO_A1N1)  | (1ULL << GPIO_A1N2) |
  //                        (1ULL << GPIO_B1N1) | (1ULL << GPIO_B1N2);
  // //disable pull-down mode
  // io_conf.pull_down_en = 0;
  // //disable pull-up mode
  // io_conf.pull_up_en = 0;
  // //configure GPIO with the given settings
  // gpio_config(&io_conf);

  //##### SET PWM CONFIG ########
  mcpwm_config_t pwm_config;
  pwm_config.frequency = 1000;    //frequency = 1000Hz
  pwm_config.cmpr_a = 0.0;       //duty cycle of PWMxA = 60.0%
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);   //Configure MCPWM 0 PWM0A
  mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_0, &pwm_config);   //Configure MCPWM 1 PWM0A

  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM_A);
  mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0A, GPIO_PWM_B);


  initBluetooth("ESP32_DE_GABRIEL", esp_spp_callback);
}

void
motorControl(const uint8_t mode_rotate, const uint8_t pwmLeft, const uint8_t pwmRight)
{
  static float pwmA, pwmB;
  //set modo de rotacao do motor esquerdo
  gpio_set_level(GPIO_A1N1, mode_rotate && 0x02);
  gpio_set_level(GPIO_A1N2, !(mode_rotate && 0x02));
  //set modo de rotacao do motor direito
  gpio_set_level(GPIO_B1N1, mode_rotate && 0x01);
  gpio_set_level(GPIO_B1N2, !(mode_rotate && 0x01));

  //set PWM motor esquerdo
  pwmA = pwmLeft/255.0;
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, pwmA);
  //set PWM motor direito
  pwmB = pwmRight/255.0;
  mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_0, MCPWM0A, pwmB);

  //driver on
  gpio_set_level(GPIO_STBY, 1);
}
