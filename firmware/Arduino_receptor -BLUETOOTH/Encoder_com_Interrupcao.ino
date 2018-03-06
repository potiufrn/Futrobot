const int chA1=2,chA2=4,chB1=3,chB2=12; //Entradas  para os canais do encoder(A,B). Mudar depois. //ch -> canal; A -> motor A;1->canal 1

bool leituraA,leituraB; //Teremos 2 posições, de acordo com o encoder, e temos dois pulsos.
int tetaA = 0, tetaB = 0; bool sentA, sentB;

const double Raio = 0.3; const double angulo=1;//360/(12*30) -> 360 graus(uma rotaçao) / 12 pulsos por revolução / redução de 30:1
const int time_control = 10; //10 ms

int pwm_recebidoA, pwm_lidoA, pwm_recebidoB, pwm_lidoB;
double w_lidoA, w_recebidoA, w_lidoB, w_recebidoB;
bool left_sense, right_sense;//1 direto-> frente. 0 inverso -> trás. //Vai variar conforme a roda

int pwm_left, pwm_right;

//Motor 1 (Esquerdo) - Pinos e PWM
int PWM_A = 5; // /Controla a velocidade do motor A
int RE2 = 6; // Sentido motor A
int RE1 = 7; // Sentido motor A

// Modo Standby do driver
int STBY = 8;

//Motor 2 (Direito) - Pinos e PWM
int RD1 = 9; // Sentido motor B
int RD2 = 10; // Sentido motor B
int PWM_B = 11; //  Controla a velocidade do motor B

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // SoftwareSerial "com port" data rate. JY-MCU v1.03 defaults to 9600.
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  pinMode(13, OUTPUT);
  pinMode(PWM_A, OUTPUT);
  pinMode(RE2, OUTPUT); //Saídas de controle da roda esquerda
  pinMode(RE1, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(RD1, OUTPUT); //Saídas de controle da roda direita
  pinMode(RD2, OUTPUT); 
  pinMode(PWM_B, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(chA1), analiseReadA, RISING); //Inicialização das portas
  attachInterrupt(digitalPinToInterrupt(chB1), analiseReadB, RISING); //Inicialização das portas  
  
  //Inicialização das interrupções do Timer1
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 624;// = (16*10^6) / (100*256) - 1 , where 100 is our frequency(=10ms) and 256 is the prescaler(I chose);
  // turn on CTC mode - clear time on compare
  TCCR1B |= (1 << WGM12);
  // Set CS12 bit for 256 prescaler
  TCCR1B |= (1 << CS12);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}

void loop() {
  int dado;
  //int pwm_left, pwm_right;
  // Read device output if available.
    do
    {
      dado = Serial.read();
    } while ( dado==-1 || (dado & 0xFC) != 0xA8 );   //Um bit -1 na transmissão significa que nenhum dado está armazenado no buffer;
    
    left_sense = (dado & 0x02 ? true : false);    //O dado tem o formato 1010 10xx, e caso a comparação lógica com 0000 0010 retorne 10, o sentido desejado de rotação da roda esquerda é tal que o robô vá para frente é para a frente, caso 00 o sentido é para trás, e 11 é bug;
    right_sense = (dado & 0x01? true : false);      //Semelhante à situação acima, mas para a roda direita a comparação é feita com 0000 0001. 
    //frente_left = ( dado & 0x02 ? true : false );  
    //frente_right = ( dado & 0x01 ? true : false );
    do {
      pwm_left = Serial.read();
    } while(pwm_left==-1);
    do{
      pwm_right = Serial.read();
    } while(pwm_right==-1);
}

ISR(TIMER1_COMPA_vect){//Fazer controle aqui
  w_lidoA = tetaA*100; w_lidoB = tetaB*100; //O teta é multiplicado por 100 porque a velocidade angular é obtidade de Delta_teta/10ms, logo trivial.
  tetaA = 0; tetaB = 0;
  Activate_Left_Motor(left_sense,pwm_left);
  Activate_Right_Motor(right_sense,pwm_right);
}

void analiseReadA(void){ //Perguntar a adelardo se essa função está 100% certa (provavelmente ele tem um jeito melhor)
  leituraA=digitalRead(chA2);
  if(leituraA == true){
    //sentidoA = true;
    tetaA++;
  }
  else{
    //sentidoA = 0;
    tetaA--;
  }
}


void analiseReadB(void){ //Perguntar a adelardo se essa função está 100% certa (provavelmente ele tem um jeito melhor)
  leituraB=digitalRead(chB2);
  if(leituraB == true){
    //sentidoA = true;
    tetaB++;
  }
  else{
    //sentidoA = 0;
    tetaB--;
  }
}

void Activate_Left_Motor(boolean sense, int pwm)
{
  boolean set;    //Variável para controlar o sentido de rotação dos motores
  set = (sense == true ? true : false);
  
  digitalWrite(RE1, set);
  digitalWrite(RE2, !set);
  analogWrite(PWM_A, pwm);
}

void Activate_Right_Motor(boolean sense, int pwm)
{
  boolean set;    //Variável para controlar o sentido de rotação dos motores  
  set = (sense == true ? true : false);
  
  digitalWrite(RD1, !set);
  digitalWrite(RD2, set);
  analogWrite(PWM_B, pwm);
}



