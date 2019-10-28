/**
 *  Programa teste para o controle local FeedForward
 *  O programa ira identificar a relacao entre PWM e Velocidade de cada motor para cada
 *  sentido de rotacao
 **/
#include <stdio.h>
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "freertos/queue.h"
// #include "esp_timer.h"

#include "driver/periph_ctrl.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/pcnt.h"
#include "driver/timer.h"
#include "driver/mcpwm.h"

#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

#define DELTA_T  0.10   // DELTA_T segundos
#define DELTA_T_us DELTA_T*1000000.0 //DELTA_T em us


//Motor 1 (Esquerdo)
#define  GPIO_PWM_A       23  //Controla a velocidade do motor A (Esquerdo)
#define  GPIO_A1N1        21 //Sentido motor A
#define  GPIO_A1N2        19 //Sentido motor A
#define  GPIO_OUTA_CAP0_E 15 //Sinal de saida do encoder Esquerdo (usado para calcular a velocidade)
#define  GPIO_OUTB_E      2  //Sinal em quadrado com relacao ao CAP0A do motor 1 (usado para identificar o sentido de rotacao)
// Modo Standby do driver
#define  GPIO_STBY        18
//Motor 2 (Direito)
#define  GPIO_PWM_B       16 //  Controla a velocidade do motor B (Direito)
#define  GPIO_B1N1        5  // Sentido motor
#define  GPIO_B1N2        17 // Sentido motor B
#define  GPIO_OUTA_CAP1_D 33 //Sinal de saida do encoder Direito (Usado para calcular a velocidade)
#define  GPIO_OUTB_D      32 //Sinal em quadrado com relacao ao CAP1A do motor 2 (usado para calcular o sentido de rotacao)

#define CAP0_INT_EN BIT(27)  //Capture 0 interrupt bit
#define CAP1_INT_EN BIT(28)  //Capture 1 interrupt bit

enum LIMITES_OHMEGA{
  MIN_FRONT,
  MAX_FRONT,
  MIN_BACK,
  MAX_BACK
};
//############# Variaveis Globais #################
/*flags para auxiliar a identificar velocidade nula (quando o encoder nao gera interrupcao)
*/
static mcpwm_dev_t *MCPWM[2] = {&MCPWM0, &MCPWM1};

time_t lastTime_RigthEncoder, lastTime_LeftEncoder;
double ohmega_rigth, ohmega_left; //rad/s
double ohmega_rigth_lim[4], ohmega_left_lim[4]; //velocidades limites consideradas serao armazenadas nesses vetores
long unsigned int pcnt_rigth = 0, pcnt_left = 0;
//#################################################

static void IRAM_ATTR isr_handler()
{
    uint32_t mcpwm_intr_status;
    mcpwm_intr_status = MCPWM[MCPWM_UNIT_0]->int_st.val; //Read interrupt status

    if (mcpwm_intr_status & CAP0_INT_EN) { //Check for interrupt on rising edge on CAP0 signal
        //Interrupcao do encoder esquerdo
        pcnt_left++;
    }
    if (mcpwm_intr_status & CAP1_INT_EN) { //Check for interrupt on rising edge on CAP1 signal
      //Interrupcao do encoder direito
      pcnt_rigth++;
    }
    MCPWM[MCPWM_UNIT_0]->int_clr.val = mcpwm_intr_status;
}

//Interrupcao de timeout, mede velocidade de cada motor.
//Chamado a cada 5ms
static void periodic_callback(void* arg)
{
  // int64_t time_since_boot = esp_timer_get_time();
  printf("Velocidade E:%E pulsos/s\n", pcnt_left/DELTA_T);
  printf("Velocidade D:%E pulsos/s\n", pcnt_rigth/DELTA_T);
  pcnt_left = 0;
  pcnt_rigth= 0;
}

static void setup(){
  //config. GPIO
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

  //Config. MCPWM
  //Mapeando pinos para o Modulo de controle de PWM dedicado
  mcpwm_pin_config_t pin_config = {
      .mcpwm0a_out_num = GPIO_PWM_A,
      .mcpwm0b_out_num = GPIO_PWM_B,
      .mcpwm_cap0_in_num   = GPIO_OUTA_CAP0_E,
      .mcpwm_cap1_in_num   = GPIO_OUTA_CAP1_D
  };
  mcpwm_set_pin(MCPWM_UNIT_0, &pin_config);
  gpio_pulldown_en(GPIO_OUTA_CAP0_E);    //Enable pull down on CAP0   signal
  gpio_pulldown_en(GPIO_OUTA_CAP1_D);    //Enable pull down on CAP1   signal

  mcpwm_config_t pwm_config;
  pwm_config.frequency = 1000;    //frequency = 1000Hz
  pwm_config.cmpr_a = 0.0;       //duty cycle of PWMxA = 0.0%
  pwm_config.cmpr_b = 0.0;       //duty cycle of PWMxb = 0.0%
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);   //Configure PWM0A & PWM0B with above settings

  mcpwm_capture_enable(MCPWM_UNIT_0, MCPWM_SELECT_CAP0, MCPWM_POS_EDGE, 0);  //capture signal on rising edge, prescale = 0 i.e. 800,000,000 counts is equal to one second
  mcpwm_capture_enable(MCPWM_UNIT_0, MCPWM_SELECT_CAP1, MCPWM_POS_EDGE, 0);  //capture signal on rising edge, prescale = 0 i.e. 800,000,000 counts is equal to one second

  //enable interrupt, so each this a rising edge occurs interrupt is triggered
  MCPWM[MCPWM_UNIT_0]->int_ena.val = CAP0_INT_EN | CAP1_INT_EN;  //Enable interrupt on  CAP0, CAP1 and CAP2 signal
  mcpwm_isr_register(MCPWM_UNIT_0, isr_handler, NULL, ESP_INTR_FLAG_IRAM, NULL);  //Set ISR Handler

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
  ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, DELTA_T_us));
  vTaskDelete(NULL);
}

void app_main()
{
  //configuracoes iniciais
  xTaskCreatePinnedToCore(setup, "setup", 1024, NULL, 5, NULL, 0);
  xTaskCreatePinnedToCore(time_init, "timer init", 1024, NULL, 5, NULL, 1);

  gpio_set_level(GPIO_A1N1, 1);
  gpio_set_level(GPIO_A1N2, 0);
  //set modo de rotacao do motor direito
  gpio_set_level(GPIO_B1N1, 1);
  gpio_set_level(GPIO_B1N2, 0);

  // //set PWM motor esquerdo
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, 20.0);
  // //set PWM motor direito
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0B, 20.0);

  //driver on
  gpio_set_level(GPIO_STBY, 1);
}
