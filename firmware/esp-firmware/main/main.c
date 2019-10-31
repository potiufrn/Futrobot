/********************************
By: Luís Gabriel Pereira Condados - 31/10/2019
*********************************/
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

#define DEVICE_NAME "ESP_ROBO_1"

//Motor 1 (Esquerdo)
#define  GPIO_PWM_LEFT       23  //Controla a velocidade do motor A (Esquerdo)
#define  GPIO_A1N1_LEFT      21 //Sentido motor A
#define  GPIO_A1N2_LEFT      19 //Sentido motor A
// Modo Standby do driver
#define  GPIO_STBY        18
//Motor 2 (Direito)
#define  GPIO_PWM_RIGHT       16  //  Controla a velocidade do motor B (Direito)
#define  GPIO_B1N1_RIGHT       5  // Sentido motor
#define  GPIO_B1N2_RIGHT      17  // Sentido motor B
//############################### Variaveis Globais ######################################

//Identificador do Bluetooth
static uint32_t bt_handle = 0;
//########################################################################################
static void esp_spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
static void controlSignal(bool frontLeft, bool frontRight, float pwmLeft, float pwmRight);
//######################################################################################
static void config_gpio(){
  //config. GPIO
  #define GPIO_OUTPUT_PIN_SEL (1ULL << GPIO_STBY) | (1ULL << GPIO_A1N1_LEFT)  | (1ULL << GPIO_A1N2_LEFT) | \
                              (1ULL << GPIO_B1N1_RIGHT) | (1ULL << GPIO_B1N2_RIGHT)
  //##### SET GPIO CONFIG ########
  gpio_config_t io_conf;
  io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
  io_conf.pull_down_en = 0;
  io_conf.pull_up_en = 0;
  //configure GPIO with the given settings
  gpio_config(&io_conf);

  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM_LEFT);
  mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0A, GPIO_PWM_RIGHT);

  mcpwm_config_t pwm_config;
  pwm_config.frequency = 10000;    //frequency = 10kHz
  pwm_config.cmpr_a = 0.0;       //duty cycle of PWMxA = 0.0%
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);   //Configure PWM0A & PWM0B with above settings
  mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_0, &pwm_config);   //Configure PWM0A & PWM0B with above settings

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
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, pwmLeft + (pwmLeft > 0.0)*8.0);
  //set PWM motor direito
  mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_0, MCPWM0A, pwmRight+ (pwmRight > 0.0)*8.0);
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
        if((param->data_ind.data[0] & 0b11111100) == 0b10101000)
        {
            controlSignal(param->data_ind.data[0] & 0x02    , param->data_ind.data[0] & 0x01,
                          100*param->data_ind.data[1]/255.0 , 100*param->data_ind.data[2]/255.0);
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
