/**
 *  Programa teste para o controle local FeedForward
 *  O programa ira identificar a relacao entre PWM e Velocidade de cada motor para cada
 *  sentido de rotacao
 **/

#include <string.h>
#include "common.h"

/*************************************************************************************/
/****************************** CONFIGURACOES ****************************************/
/*************************************************************************************/
static void config_bluetooth();
static void mcpwm_init_isrLeft();
static void mcpwm_init_isrRight();
static void config_gpio();
static void periodics_func_config();

//interruptions and callbacks
static void IRAM_ATTR isr_EncoderLeft();
static void IRAM_ATTR isr_EncoderRight();
static void esp_spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
static void periodic_test_omega_zero_cb(void *arg);
static void periodic_controller(void *arg);

static void task_calc_Leftsense(void *arg);
static void task_calc_Rightsense(void *arg);

//my functions
static void func_controlSignal(const float pwmL,const float pwmR);
static void func_calibration();
/*************************************************************************************/
/****************************** VARIAVEIS GLOBAIS ************************************/
/*************************************************************************************/
static mcpwm_dev_t *MCPWM[2] = {&MCPWM0, &MCPWM1};

static xQueueHandle bt_queue = NULL;
static xQueueHandle encL_queue = NULL;
static xQueueHandle encR_queue = NULL;

static bool controller_enable = true;

struct CoefLine coefL[2];       //coef. das retas PWM(omega) para o motor esquerdo
struct CoefLine coefR[2];       //coef. das retas PWM(omega) para o motor direito
static float omega_max = 0.0;   //modulo da velocidade maxima do robo

static bool  omega_zero[2]     = {true, true};
static float omega_ref[2]      = {0.0, 0.0}; //-1.0 a 1.0
static float omega_current[2]  = {0.0, 0.0}; //-1.0 a 1.0
static float _sense[2]         = {1.0, 1.0};
//Identificador do Bluetooth
static uint32_t bt_handle = 0;
/*************************************************************************************/
/****************************** ROTINAS PRINCIPAIS ***********************************/
/*************************************************************************************/
void app_main()
{
  bt_data btdata;
  bt_queue   = xQueueCreate(1, sizeof(bt_data));
  encL_queue = xQueueCreate(1, 0);
  encR_queue = xQueueCreate(1, 0);

  /** configuracoes iniciais **/
  xTaskCreatePinnedToCore(config_gpio, "config_gpio", 4096, NULL, 5, NULL, 0);

  xTaskCreatePinnedToCore(config_bluetooth, "config_bluetooth", 2048, NULL, 5, NULL, 0);
  xTaskCreatePinnedToCore(periodics_func_config, "periodics_func_config", 4096, NULL, 4, NULL, 1);

  xTaskCreatePinnedToCore(mcpwm_init_isrLeft, "mcpwm 0", 4096, NULL, 4, NULL, 0);
  xTaskCreatePinnedToCore(mcpwm_init_isrRight, "mcpwm 1", 4096, NULL, 4, NULL, 0);

  xTaskCreatePinnedToCore(task_calc_Leftsense, "task_calc_Leftsense", 2048, NULL, 5, NULL, 0);
  xTaskCreatePinnedToCore(task_calc_Rightsense, "task_calc_Rightsense", 2048, NULL, 5, NULL, 0);

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
        //Falta fazer
        bitstream = (uint8_t*)malloc(8*sizeof(float));

        bitstream[0*sizeof(float)] = *(uint8_t*)&coefL[FRONT].alpha;
        bitstream[1*sizeof(float)] = *(uint8_t*)&coefL[FRONT].beta;
        bitstream[2*sizeof(float)] = *(uint8_t*)&coefL[BACK].alpha;
        bitstream[3*sizeof(float)] = *(uint8_t*)&coefL[BACK].beta;

        bitstream[4*sizeof(float)] = *(uint8_t*)&coefR[FRONT].alpha;
        bitstream[5*sizeof(float)] = *(uint8_t*)&coefR[FRONT].beta;
        bitstream[6*sizeof(float)] = *(uint8_t*)&coefR[BACK].alpha;
        bitstream[7*sizeof(float)] = *(uint8_t*)&coefR[BACK].beta;

        esp_spp_write(bt_handle, 8*sizeof(float), bitstream);

        free(bitstream);
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
static void IRAM_ATTR isr_EncoderLeft()
{
  static double old_time = 0.0;
  static double new_time;
  static uint8_t trash;

  new_time = get_time_sec();

  omega_current[LEFT] = _sense[LEFT]/(new_time - old_time);
  omega_zero[LEFT] = false;
  old_time = new_time;

  // if(omega_current[LEFT] < 100.0)
  xQueueSendFromISR(encL_queue, &trash, NULL);

  MCPWM[MCPWM_UNIT_0]->int_clr.val = MCPWM[MCPWM_UNIT_0]->int_st.val;
}

static void IRAM_ATTR isr_EncoderRight()
{
  static double old_time = 0;
  static double new_time;
  static uint8_t trash;

  new_time = get_time_sec();
  omega_current[RIGHT] = _sense[RIGHT]/(new_time - old_time);
  omega_zero[RIGHT]    = false;
  old_time = new_time;

  // if(omega_current[RIGHT] < 100.0)
  xQueueSendFromISR(encR_queue, &trash, NULL);

  MCPWM[MCPWM_UNIT_1]->int_clr.val = MCPWM[MCPWM_UNIT_1]->int_st.val;
}

static void task_calc_Leftsense(void *arg)
{
  uint8_t trash;
  bool level;
  while(1)
  {
    xQueueReceive(encL_queue, &trash, portMAX_DELAY);
    if(omega_current[LEFT] > 80.0)continue;
    level = gpio_get_level(GPIO_OUTB_CAP1_LEFT);
    _sense[LEFT] = (!level)*-1.0 + level*1.0f;
  }
}

static void task_calc_Rightsense(void *arg)
{
  uint8_t trash;
  bool level;
  while(1)
  {
    xQueueReceive(encR_queue, &trash, portMAX_DELAY);
    if(omega_current[RIGHT] > 80.0)continue;
    level = gpio_get_level(GPIO_OUTB_CAP1_RIGHT);
    _sense[RIGHT] = (level)*-1.0 + (!level)*1.0f;
  }
}

//Interrupcao de timeout, mede velocidade de cada motor.
//Chamado a cada 800ms
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

  gpio_set_level(GPIO_A1N1_LEFT, !front[LEFT]);
  gpio_set_level(GPIO_A1N2_LEFT, front[LEFT]);
  //set modo de rotacao do motor direito
  gpio_set_level(GPIO_B1N1_RIGHT, !front[RIGHT]);
  gpio_set_level(GPIO_B1N2_RIGHT, front[RIGHT]);

  gpio_set_level(GPIO_STBY, 0);
  //set PWM motor esquerdo
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, ABS_F(pwmL)*100.0);
  //set PWM motor direito
  mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_0, MCPWM0A, ABS_F(pwmR)*100.0);
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
  static int sense[2];

  static float difPhase;
  if(omega_current[LEFT] < 80.0 && (omega_current[LEFT] != 0.0))
  {
     difPhase = gpio_get_level(GPIO_OUTA_CAP0_LEFT) - gpio_get_level(GPIO_OUTB_CAP1_LEFT);
    _sense[LEFT] = difPhase;
    // printf("Motor Esquerdo => Sentido:%f\n", difPhase);
  }
  if(omega_current[RIGHT] < 80.0 && (omega_current[RIGHT] != 0.0))
  {
     difPhase = gpio_get_level(GPIO_OUTB_CAP1_RIGHT) - gpio_get_level(GPIO_OUTA_CAP0_RIGHT);
    _sense[RIGHT] = difPhase;
    // printf("Motor Direito => Sentido:%f\n", difPhase);
  }

  sense[LEFT]  = F_IS_NEG(omega_ref[LEFT]); //se false => sentido para "frente"
  sense[RIGHT] = F_IS_NEG(omega_ref[RIGHT]); //se false => sentido para "frente"

  if(!controller_enable)
  {
    func_controlSignal(omega_ref[LEFT], omega_ref[RIGHT]);
    return;
  }

  //controlador FeedForWard
  pwm[LEFT]  = coefL[sense[LEFT] ].alpha*omega_ref[LEFT]  +
               coefL[sense[LEFT] ].beta*(omega_ref[LEFT]  != 0 );
  pwm[RIGHT] = coefR[sense[RIGHT]].alpha*omega_ref[RIGHT] +
               coefR[sense[RIGHT]].beta*(omega_ref[RIGHT] != 0 );
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

  io_conf.mode = GPIO_MODE_INPUT;
  //bit mask of the pins that you want to set,e.g.GPIO18/19
  io_conf.pull_down_en = 1;
  io_conf.pin_bit_mask = (1ULL << GPIO_OUTB_CAP1_LEFT) | (1ULL << GPIO_OUTB_CAP1_RIGHT);
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
