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

#include "driver/periph_ctrl.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/pcnt.h"
#include "driver/timer.h"


#define MOTOR_REDUCTION 30
#define DELTA_T 

enum LIMITES_OHMEGA{
  MIN_FRONT,
  MAX_FRONT,
  MIN_BACK,
  MAX_BACK
};
//############# Variaveis Globais #################
/*flags para auxiliar a identificar velocidade nula (quando o encoder nao gera interrupcao)
*/
time_t lastTime_RigthEncoder, lastTime_LeftEncoder;
double ohmega_rigth, ohmega_left; //rad/s
double ohmega_rigth_lim[4]; ohmega_left_lim[4]; //velocidades limites consideradas serao armazenadas nesses vetores
long unsigned int pcnt_rigth = 0, pcnt_left = 0;
//#################################################

//Interrupcao do encoder Direito
static void IRAM_ATTR pcnt_function_RigthEncoder(void *arg)
{
    pcnt_rigth++;
}

//Interrupcao do encoder Esquerdo
static void IRAM_ATTR pcnt_function_LeftEncoder(void *arg)
{
    pcnt_left++;
}

//Interrupcao de timeout, mede velocidade de cada motor.
static void periodic_callback(void* arg)
{

}

void app_main()
{
  pin_config();

}
