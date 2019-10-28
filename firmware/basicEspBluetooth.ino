#include "BluetoothSerial.h"


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


//Motor 1 - Pinos e PWM
int PWM_A = 36; ///Controla a velocidade do motor A (Esquerdo)
int A1N2 = 20; //Sentido motor A
int A1N1 = 18; //Sentido motor A

// Modo Standby do driver
int STBY = 17;

//Motor 2 - Pinos e PWM
int B1N1 = 16; // Sentido motor B
int B1N2 = 15; // Sentido motor

//Motor 2 - Pinos e PWM
int PWM_B = 39; //  Controla a velocidade do motor B (Direito)

double aux = 0; //Auxiliar para evitar overflow
int compensador = 15; //Valor máximo que vai ser adicionado a todas as PWMs diferentes de 0

BluetoothSerial SerialBT;

void setup() {
  pinMode(PWM_A, OUTPUT);//Definimos o pino 2 (LED) como saída.
  ledcAttachPin(PWM_A, 0);//Atribuimos o pino 2 ao canal 0.
  ledcSetup(0, 1000, 8);//Atribuimos ao canal 0 a frequencia de 1000Hz com resolucao de 10bits.

  pinMode(PWM_B, OUTPUT);//Definimos o pino 2 (LED) como saída.
  ledcAttachPin(PWM_B, 1);
  ledcSetup(0, 1000, 8);//Atribuimos ao canal 0 a frequencia de 1000Hz com resolucao de 10bits.

  Serial.begin(115200);
  SerialBT.begin("ESP32_TEST"); //Bluetooth device name
  // Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop(){
  //unsig ned char cabecalho;
  int dado;
  bool frente_left, frente_right;
  int pwm_left, pwm_right;

  do
  {
    dado = SerialBT.read();
  } while ( dado==-1 ||(dado & 0xFC) != 0xA8 );
  frente_left = ( dado & 0x02 ? true : false );
  frente_right = ( dado & 0x01 ? true : false );
  do {
    pwm_left = SerialBT.read();
  } while(pwm_left==-1);
  do{
    pwm_right = SerialBT.read();
  }while(pwm_right==-1);

  //Compensar a zona morta
  //Adiciona, no máximo, "compensador" à pwm
  //O valor adicionado decai linearmente com a pwm; pwms maiores recebem uma menor compensação
  //Se pwm for 0 ou 255, não há modificação
  if ((pwm_left != 0) & (pwm_left < 255)) {
    aux = pwm_left*(1-(compensador/255)) + compensador;
    pwm_left = aux;
  }
  if ((pwm_right != 0) & (pwm_right < 255)) {
    aux = pwm_right*(1-(compensador/255)) + compensador;
    pwm_right = aux;
  }

  acelerarMotor(true, frente_left, pwm_left);
  acelerarMotor(false, frente_right, pwm_right);
}

void acelerarMotor( boolean motorLeft, boolean paraFrente, int pwm) {
  /*
  Essa função é responsável pela aceleração do motor, ela recebe a identificação do motor, o sentido e a velocidade que deve girar
    - motorLeft: true: acelera o LEFT;  false: acelera o RIGHT
    - paraFrente: true: acelera para frente;   false: acelera para tr
    - pwm: 0 a 255, sendo 0 = parado e 255= velocidade máxima
  */

  boolean pin1n1;// variavel para controlar os A1N1 ou B1N1 ( isso vai depender de que motor vamos acionar)
  /*
  Aqui dizemos a direção que deve girar o motor
  No sentido horário temos: A1N1/B1N1 = HIGH e A1N2/B1N2 = LOW
  No sentido anti-horário temos: A1N1/B1N1 = LOW e A1N2/B1N2 = HIGH
  */
  if (motorLeft == paraFrente)
    pin1n1 = HIGH;
  else
    pin1n1 = LOW;

  //Seleciona o motor que vai ser ligado, define a direção e a velocidade
  if (motorLeft) {
    digitalWrite(A1N1, pin1n1);
    digitalWrite(A1N2, !pin1n1);
    // analogWrite(PWM_A, pwm);
    // mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, pwm/255.0);
    ledcWrite(0,pwm);
  } else {
    digitalWrite(B1N1, pin1n1);
    digitalWrite(B1N2, !pin1n1);
    // analogWrite(PWM_B, pwm);
    // mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, pwm/255.0);
    ledcWrite(1,pwm);
  }

  //STANDBY EM ALTO
  // digitalWrite(STBY, HIGH);
}
