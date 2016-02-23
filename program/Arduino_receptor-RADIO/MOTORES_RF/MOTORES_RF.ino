//DANIEL MORAIS / GUSTAVO VIEIRA- 22.06.2015
//FUTEBOL DE ROBOS - JERIMUM
//PROGRAMA : NRF24L01 - RECEPTOR

#include <RF24_config.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

const uint8_t idRobo = 0;   //Nome do robo

//pinos para o UNO e PRO MINI sao iguais

//MOTOR A - Direita
const uint8_t IN1 = 2;
const uint8_t IN2 = 4;
const uint8_t PWM_A = 3;  //Pino do PWM
byte VEL_A = 0;        //Velocidade do motor A

//MOTOR B - Esquerda
const uint8_t IN3 = 5;
const uint8_t IN4 = 7;
const uint8_t PWM_B = 6;  //Pino do PWM
byte VEL_B = 0;        //Velocidade do motor B

bool SENT_A = 1;       // considere 1:frente e 0:tras referente aos sentidos dos motores
bool SENT_B = 1;

//Radio
//Vetor para receber os bytes
byte dados[7];

//Inicializa a placa nos pinos 9 (CE) e 10 (CS) do Arduino
RF24 radio(9, 10);

//Define o endereco para comunicacao entre os modulos
const uint64_t pipe = 0xE14BC8F482LL;

void setup() {
  //Inicializa Pinos
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(PWM_A, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(PWM_B, OUTPUT);

  //Inicializa a serial
  //Serial.begin(57600);
  //Inicializa a comunicacao
  radio.begin();
  //Entra em modo de recepcao
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  analogWrite(PWM_A, 50);
  analogWrite(PWM_B, 50);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(2000);     
}

void loop() {
  /*LEITURA DOS BYTES VINDOS PELO RF*/
  //Espera pelo cabeçalho do pacote de dados
  do{
    radio.read(dados, 1); 
  }while (dados[0] != B10000000)
  
  byte SENT[1];   
  
  radio.read(dados, 7);  
  
    if ((dados[6] & B100100) == B100100){ 
    if (idRobo == 0)
    {
      SENT[0] = ((dados[6] & B11000000)/0x064);
    }
    if (idRobo == 1)
    {
      SENT[0] = ((dados[6] & B00011000)/0x08);
    }  
    // complemento de 1  
    // Motor A
    PARA_FRENTE_A = (SENT & 0x02 == 0x02 ? true : false );
    VEL_A = dados[0 + (2 * idRobo)];
    // Motor B
    PARA_FRENTE_B = (SENT & 0x01 == 0x01 ? true : false );
    VEL_B = dados[1 + (2 * idRobo)];

    /*Estados do Motor A e B*/

    analogWrite(PWM_A, VEL_A);
    analogWrite(PWM_B, VEL_B);

    if (PARA_FRENTE_A) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    }
    else {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    }

    if (PARA_FRENTE_B) {
      digitalWrite(IN3,LOW);
      digitalWrite(IN4, HIGH);
    }
    else {
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
  }
}


