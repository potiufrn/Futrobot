/* Codigo receptor bluetooth para o futebol de robos 2015
   Feito por Elitelma Souza - 2012924160 */
   
//Motor 1 - Pinos e PWM
int PWM_A = 5; ///Controla a velocidade do motor A
int A1N2 = 6; //Sentido motor A
int A1N1 = 7; //Sentido motor A

// Modo Standby do driver
int STBY = 8;

//Motor 2 - Pinos e PWM
int B1N1 = 9; // Sentido motor B
int B1N2 = 10; // Sentido motor
int PWM_B = 11; //  Controla a velocidade do motor B

double aux = 0; //Auxiliar para evitar overflow
int compensador = 15; //Valor máximo que vai ser adicionado a todas as PWMs diferentes de 0

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // SoftwareSerial "com port" data rate. JY-MCU v1.03 defaults to 9600.
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  pinMode(13, OUTPUT);
  pinMode(PWM_A, OUTPUT);
  pinMode(A1N2, OUTPUT);
  pinMode(A1N1, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(B1N1, OUTPUT);
  pinMode(B1N2, OUTPUT);
  pinMode(PWM_B, OUTPUT);
}


/*
* 1- AO RECEBER O PRIMEIRO BYTE, CHECAR SE ESTA CORRETO ATRAVES DA MASCARA '0xa8', e so passar a receber os demais bytes de pwm APENAS SE
*A CHECAGEM ANTERIOR ESTIVER OK.
 /* para implementar                                         0xA8          0xFB       
 primeiro byte criar uma variavel para fazer um and logico 1010 10xx & 1111 1100
*
*/
void loop() {
  //unsig ned char cabecalho;
  int dado;
  bool frente_left, frente_right;
  int pwm_left, pwm_right;
  
  do
  {
    dado = Serial.read();
  } while ( dado==-1 ||(dado & 0xFC) != 0xA8 );
  frente_left = ( dado & 0x02 ? true : false );
  frente_right = ( dado & 0x01 ? true : false );
  do {
    pwm_left = Serial.read();
  } while(pwm_left==-1);
  do{
    pwm_right = Serial.read();
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
    analogWrite(PWM_A, pwm);
  } else {
    digitalWrite(B1N1, pin1n1);
    digitalWrite(B1N2, !pin1n1);
    analogWrite(PWM_B, pwm);
  }

  //STANDBY EM ALTO
  digitalWrite(STBY, HIGH);

}
