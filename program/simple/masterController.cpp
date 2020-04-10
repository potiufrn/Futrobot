#include <string>
#include <fstream>
#include <iostream>          //cout, cerr, cin
#include <cstdio>           //printf
#include <unistd.h>          //sleep
#include <bluetoothAction.h>
#include <cmath>
#include <omp.h>


#define MAC_ESP_TEST   "30:AE:A4:3B:A4:26"
#define MAC_ESP_ROBO_1 "30:AE:A4:20:0E:12"
#define MAC_ESP_ROBO_2 "30:AE:A4:13:F8:AE"
#define MAC_ESP_ROBO_3 "30:AE:A4:20:0E:12"
#define MAC_ESP_ROBO_4 "30:AE:A4:41:74:F6"

#define F_IS_NEG(x) (*(uint32_t*)&(x) >> 31)
#define ABS_F(x) (((x)<0.0)?-(x):(x))

//Comandos do bluetooth
#define CMD_HEAD           0xA0

#define CMD_REQ_CAL        0x00
#define CMD_REQ_OMEGA      0x03

#define CMD_CALIBRATION    0x04
#define CMD_IDENTIFY       0x05
// #define CMD_SET_KP         0x06

#define CMD_REF            0x0A
#define CMD_CONTROL_SIGNAL 0x0B

#define CMD_PING           0x0F

#define RADIUS    1.5/100 //metros
#define REDUCTION 30 //30x1

enum OPTION{
  _rec_omegas  = 1,
  _connect     = 2,
  _disconnect  = 3,
  _ping        = 4,
  _send_ref    = 5,
  _send_pwm    = 6,
  _calibration = 7,
  _identify    = 8,
  _graphic     = 9,
  _rec_coef    = 10,
  _close       = 11
};

using namespace std;

BluetoothAction btAction;

typedef struct
{
  double ang; //coef. angular da reta
  double lin;  //coef. linear da reta
}coefLine_t;

typedef struct
{
  double omegaMax;
  double Kp[4];        //ganho do controlador
  double K[2];         //ganho do sistema, motor esquerdo e direito
  double tau[2];       //constante de tempo, motor esquerdo e direito
  coefLine_t coef[4];
}parameters_t;

typedef struct
{
  double  rawOmega;  //ultimo omega medido, sem filtro
  double  omega;     //omega filtrado
}encoder_data_t;

string getDate()
{
  time_t rawtime;
  tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  std::string strTime = asctime(timeinfo);
  strTime.replace(strTime.find(' '), 1, "_");
  strTime.replace(strTime.find(' '), 1, "_");
  strTime.replace(strTime.find(' '), 1, "_");
  strTime.replace(strTime.find(' '), 1, "_");
  return strTime;
}

void saveToFile(encoder_data_t vec_data[], const int size, const float timeout, const double omegaRef, const string &fileName);

void bytes2float(const uint8_t *bitstream, float*f, uint32_t num_float)
{
  memcpy((float*)bitstream, f, num_float*sizeof(float));
}

void float2bytes(const float*f, uint8_t *bitstream, uint32_t num_float)
{
  memcpy((uint8_t*)f, bitstream, num_float*sizeof(float));
}

void encodeFloat(const float* vec_f, uint8_t *bitstream)
{
  uint16_t ref_b[2];

	ref_b[0] = (!F_IS_NEG(vec_f[0]) << 15)  | (uint16_t)(ABS_F(vec_f[0])*32767.0);
  ref_b[1]= (!F_IS_NEG(vec_f[1]) << 15)   | (uint16_t)(ABS_F(vec_f[1])*32767.0);

  bitstream[0] = (ref_b[0] & 0xFF00) >> 8;
  bitstream[1] = (ref_b[0] & 0x00FF);
  bitstream[2] = (ref_b[1] & 0xFF00) >> 8;
  bitstream[3] = (ref_b[1] & 0x00FF);
}


void _printMainMenu()
{
  printf("****************MENU DE AÇÕES*************\n");
  printf("%d -> PEDIR OMEGAS ATUAIS\n", OPTION::_rec_omegas);
  printf("************************************\n");
  printf("%d -> CONECTAR\n",OPTION::_connect);
  printf("%d -> DESCONECTAR\n",OPTION::_disconnect);
  printf("%d -> PING\n",OPTION::_ping);
  printf("************************************\n");
  printf("%d -> ENVIAR REFERÊNCIAS\n",OPTION::_send_ref);
  printf("%d -> ENVIAR PWM\n",OPTION::_send_pwm);
  printf("************************************\n");
  // printf("%d -> ALTERAR Kp\n", OPTION::_send_kp);
  printf("%d -> CALIBRAR CONTROLADOR\n",OPTION::_calibration);
  printf("%d -> IDENTIFICAÇÃO\n",OPTION::_identify);
  printf("%d -> VISUALIZAR GRAFICOS\n",OPTION::_graphic);
  printf("%d -> PEDIR DADOS DA CALIBRACAO\n",OPTION::_rec_coef);
  printf("************************************\n");
  printf("%d -> ENCERRAR O PROGRAMA\n",OPTION::_close);
};

void _pause(const char* msg = ""){
  printf("%s\n(PRESSIONE QUALQUER TECLA PARA CONTINUAR)\n", msg);
  cin.ignore(1,'\n');
  cin.get();
}

void _printListMACs(){
  printf("****************MACs Conhecidos*************\n");
  vector<string> addrs = btAction.getDest();
  for(int i = 0; i < (int)addrs.size(); i++)
  {
    printf("%d -> Robo_%d = %s\n",i, i, addrs[i].c_str());
  }
};

int main(int argc, char** argv)
{
  double  time_stemp[2];
  uint8_t *bitstream;
  int     idBt = -1;
  int     choice;

  btAction.setBluetoothAddr(MAC_ESP_TEST);
  btAction.setBluetoothAddr(MAC_ESP_ROBO_1);
  btAction.setBluetoothAddr(MAC_ESP_ROBO_2);
  btAction.setBluetoothAddr(MAC_ESP_ROBO_3);
  btAction.setBluetoothAddr(MAC_ESP_ROBO_4);


  bool run = true;
  int  send = 0,rec = 0;
  while(run)
  {

    system("clear");
    _printMainMenu();
    cin >> choice;

    if(idBt == -1 && (choice != _graphic &&
                      choice != _connect &&
                      choice != _close))
    {
          _pause("Nenhum dispositivo conectado!");
          continue;
    }

    switch (choice){
    case OPTION::_rec_omegas://solicitar omegas
    {
      bitstream = new uint8_t[1];
      double *vec_double = new double[2];

      memset(vec_double, 0, 2*sizeof(double));

      bitstream[0] = CMD_HEAD | CMD_REQ_OMEGA;
      btAction.sendBluetoothMessage(idBt, bitstream, 1*sizeof(uint8_t));
      rec = btAction.recvBluetoothMessage(idBt, (uint8_t*)vec_double, 2*sizeof(double), 2);

      if(rec != 2*sizeof(double))
      {
        _pause("Erro na leitura");
        break;
      }
      printf("   Omega Left:%f rad/s     Omega Right:%f rad/s\n", vec_double[0], vec_double[1]);
      printf("Velocity Left:%f m/s   Velocity Right:%f m/s\n", vec_double[0]*RADIUS/REDUCTION, vec_double[1]*RADIUS/REDUCTION);
      _pause();

      delete[] bitstream;
      delete[] vec_double;
      break;
    }
    case OPTION::_connect://conectar
      _printListMACs();
      cin >> idBt;
      btAction.initBluetoothById(idBt);
      _pause();
      break;
    case OPTION::_disconnect://desconectar
      btAction.closeBluetoothById(idBt);
      _pause("Dispositivo desconectado!");
      idBt = -1;
      break;
    case OPTION::_ping: //_ping
      bitstream = new uint8_t[64];

      memset(bitstream, 'A', 64);
      bitstream[0] = CMD_HEAD | CMD_PING;

      time_stemp[0] = omp_get_wtime();
      send = btAction.sendBluetoothMessage(idBt, bitstream, 64);
      rec  = btAction.recvBluetoothMessage(idBt, bitstream, 64, 1);
      time_stemp[1] = omp_get_wtime();

      printf("send:%d  \t rec:%d bytes \t time=%0.2fms\n",send, rec, (time_stemp[1] - time_stemp[0])*1000.0);
      delete[] bitstream;
      _pause();

      break;
    case OPTION::_send_ref://omegas ref
    case OPTION::_send_pwm://omegas sinal de controle
    {
      float *vec_float = new float[2];
      bitstream = new uint8_t[5];
      memset(bitstream, 0, 5*sizeof(uint8_t));
      memset(vec_float, 0, 2*sizeof(float));

      printf("Referencia do motor esquerdo ? (-1.0 a 1.0)\n");
      cin >> vec_float[0];
      printf("Referencia do motor direito ? (-1.0 a 1.0)\n");
      cin >> vec_float[1];

      encodeFloat(vec_float, bitstream+1);

      if(choice == 5){
        bitstream[0] = CMD_HEAD | CMD_REF;
      }else{ //control signal
        bitstream[0] = CMD_HEAD | CMD_CONTROL_SIGNAL;
      }

      btAction.sendBluetoothMessage(idBt, bitstream, 5*sizeof(uint8_t));
      delete[] bitstream;
      delete[] vec_float;
      break;
    }
    case OPTION::_calibration://calibrar
      bitstream = new uint8_t;
      bitstream[0] = CMD_HEAD | CMD_CALIBRATION;
      btAction.sendBluetoothMessage(idBt, bitstream, 1*sizeof(uint8_t));
      delete bitstream;
      break;
    case OPTION::_identify://identificar
    {
      encoder_data_t *vec_data = NULL;
      double omegaRef;
      uint8_t motor, typeC;
      float setpoint;
      int size;
      int sumRec = 0;

      cout << "Motor ? (0 -> Left \t 1 -> Right): ";
      cin >> motor;

      cout << "Setpoint: ";
      cin >> setpoint;

      cout << "Desabilitar controlador ? 0(nao), 1(sim): ";
      cin >> typeC;

      bitstream  = new uint8_t[2 + 1*sizeof(float)];

      bitstream[0]                   = CMD_HEAD | CMD_IDENTIFY;
      bitstream[1]                   = ((motor << 7)  | typeC) & 0b10000001;
      *(float*)&bitstream[2 + 0*sizeof(float)] = setpoint;

      //envia comando para iniciar a identificação
      btAction.sendBluetoothMessage(idBt, bitstream, 2 + 1*sizeof(float));
      time_stemp[0] = omp_get_wtime();

      printf("Esperando...\n");
      //aguarda receber a informação da quantidade de medições
      rec = btAction.recvBluetoothMessage(idBt, (uint8_t*)&size, sizeof(int), 10);
      if(rec == -1)
      {
        _pause("timeout! erro ao receber informacao do size");
        continue;
      }
      else printf("Medicoes:%d\n", size);
      sumRec += rec;
      vec_data = new encoder_data_t[size];
      memset(vec_data, 0, size*sizeof(encoder_data_t));
      //aguarda receber a informação da quantidade de medições
      rec = btAction.recvBluetoothMessage(idBt, (uint8_t*)&omegaRef, sizeof(double), 10);
      if(rec == -1)
      {
        _pause("timeout! erro ao receber informacao do omega de referencia");
        continue;
      }
      else printf("Omega ref:%lf\n", omegaRef);
      sumRec += rec;

      for(int i = 0; i < size; i += 50)
      {
        rec = btAction.recvBluetoothMessage(idBt, (uint8_t*)&vec_data[i], 50*sizeof(encoder_data_t), 10);
        printf("Recebi:%d bytes\n", rec);
        if(rec == -1)
          puts("timeout!");
        sumRec += rec;
      }
      time_stemp[1] = omp_get_wtime();
      printf("Tempo decorrido: %f s\n", time_stemp[1] - time_stemp[0]);

      printf("%d Bytes recebidos, deseja salvar ? (0/1)", sumRec);
      cin >> choice;

      if(choice == 1)
      {
        char fileName[50];
        cout << "Que nome devo colocar no arquivo ? ";
        scanf("%50s", fileName);

        saveToFile(vec_data, size, 2.0, omegaRef, string(fileName));
        cout << "Salvando...\n";
        printf("Salvo! Em: %s\n", (string("etc/") + string(fileName)).c_str());
        _pause();
      }else{
        printf("Tudo bem então...\n");
        _pause();
      }

      delete[] bitstream;
      delete[] vec_data;
      break;
    }
    case OPTION::_graphic://graficos
      system("python3 etc/_pyplotter.py");
    break;
    case OPTION::_rec_coef://dados da calibracao
      bitstream = new uint8_t[1];
      parameters_t parameters;
      bitstream[0] = CMD_HEAD | CMD_REQ_CAL;
      btAction.sendBluetoothMessage(idBt, bitstream, 1*sizeof(uint8_t));
      rec = btAction.recvBluetoothMessage(idBt, (uint8_t*)&parameters, sizeof(parameters_t), 5);

      printf("Coeficientes da calibracao tamanho total:%d bytes\n", rec);
      printf("Omega Max: %f rad/s = %f m/s\n", parameters.omegaMax, parameters.omegaMax*RADIUS/(REDUCTION));//reducao de 30 e 24 interrupcoes por volta
      printf("Left  Front  => a = %f , b = %f \n", parameters.coef[0].ang, parameters.coef[0].lin);
      printf("Left  Back   => a = %f , b = %f \n", parameters.coef[1].ang, parameters.coef[1].lin);
      printf("Right Front  => a = %f , b = %f \n", parameters.coef[2].ang, parameters.coef[2].lin);
      printf("Right Back   => a = %f , b = %f \n", parameters.coef[3].ang, parameters.coef[3].lin);
      printf("Left  Kp_frente = %.12f, Kp_tras %.12f\n", parameters.Kp[0], parameters.Kp[1]);
      printf("Right Kp_frente = %.12f, Kp_tras %.12f\n", parameters.Kp[2], parameters.Kp[3]);
      printf("Left  K = %.12f\n", parameters.K[0]);
      printf("Right K = %.12f\n", parameters.K[1]);
      printf("Left  tau = %.12f\n", parameters.tau[0]);
      printf("Right tau = %.12f\n", parameters.tau[1]);
      _pause();

      delete[] bitstream;
      break;
    case OPTION::_close: //terminar
      run = false;
      break;
    default:
      break;
    }
  }

  printf("Encerrando conexao e Fechando o programa\n");
  if(idBt != -1)
    btAction.closeBluetoothById(idBt);

  return 0;
}

void
saveToFile(encoder_data_t vec_data[],
           const int size,
           const float timeout,
           const double omegaRef,
           const string &fileName)
{
  string fileR = string("etc/raw_") + fileName + string(".out");
  string fileF = string("etc/filtered_") + fileName + string(".out");

  ofstream outR(fileR);
  ofstream outF(fileF);

  outR << size << ',' << timeout << ',' << omegaRef;
  outF << size << ',' << timeout << ',' << omegaRef;

  cout << "Save to File\n";
  for(int i = 0; i < size; i++){
    outR << ',' << vec_data[i].rawOmega;
    outF << ',' << vec_data[i].omega;
  }

  outR.close();
  outF.close();
}
