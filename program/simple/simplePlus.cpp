#include <iostream>          //cout, cerr, cin
#include <stdio.h>           //printf
#include <unistd.h>          //sleep
#include <bluetoothAction.h>
#include <omp.h>

#include "../../firmware/esp-firmware-test/main/common.h"

#define MAC_ESP_TEST   "30:AE:A4:3B:A4:26"
#define MAC_ESP_ROBO_1 "30:AE:A4:20:0E:12"
// #define MAC_ESP_ROBO_2

using namespace std;

BluetoothAction btAction;

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
  uint8_t  sense[2]; // 0 -> back, 1 -> front

	ref_b[0] = (!F_IS_NEG(vec_f[0]) << 15)  | (uint16_t)(ABS_F(vec_f[0])*32767.0);
  ref_b[1]= (!F_IS_NEG(vec_f[1]) << 15)   | (uint16_t)(ABS_F(vec_f[1])*32767.0);

  bitstream[0] = (ref_b[0] & 0xFF00) >> 8;
  bitstream[1] = (ref_b[0] & 0x00FF);
  bitstream[2] = (ref_b[1] & 0xFF00) >> 8;
  bitstream[3] = (ref_b[1] & 0x00FF);
}

void _printMainMenu(){
  printf("****************MENU DE AÇÕES*************\n");
  printf("01 -> ENCERRAR O PROGRAMA\n");
  printf("02 -> CONECTAR\n");
  printf("03 -> DESCONECTAR\n");
  printf("04 -> PING\n");
  printf("05 -> ENVIAR REFERÊNCIAS\n");
  printf("06 -> ENVIAR SINAL DE CONTROLE\n");
  printf("07 -> INICIAR CALIBRAÇÃO DO CONTROLADOR\n");
  printf("08 -> INICIAR IDENTIFICAÇÃO\n");
  printf("09 -> PEDIR DADOS DA CALIBRACAO\n");
  printf("10 -> PEDIR DADOS DA IDENTIFICACAO\n");
  printf("11 -> PEDIR OMEGAS ATUAIS\n");
};

void _pause(const char* msg = ""){
  printf("%s\n(PRESSIONE QUALQUER TECLA PARA CONTINUAR)\n", msg);
  cin.ignore(1,'\n');
  cin.get();
}

void _printListMACs(){
  printf("****************MACs Conhecidos*************\n");
  vector<string> addrs = btAction.getDest();
  for(int i = 0; i < addrs.size(); i++)
  {
    printf("%d -> %s\n",i, addrs[i].c_str());
  }
};

int main(int argc, char** argv)
{
  double time_stemp[2];
  uint8_t *bitstream;
  float   *vec_float;
  int     idBt = -1;
  int     choice;
  char    trash;

  btAction.setBluetoothAddr(MAC_ESP_TEST);
  btAction.setBluetoothAddr(MAC_ESP_ROBO_1);

  bool run = true;
  int  send = 0,rec = 0;
  while(run)
  {

    system("clear");
    _printMainMenu();
    cin >> choice;

    if(choice != 2 && idBt == -1 && choice != 1)
    {
      _pause("Nenhum dispositivo conectado!");
      continue;
    }

    switch (choice){
    case 1: //terminar
      run = false;
      break;
    case 2://conectar
      _printListMACs();
      cin >> idBt;
      btAction.initBluetoothById(idBt);
      _pause();
      break;
    case 3://desconectar
      btAction.closeBluetoothById(idBt);
      _pause("Dispositivo desconectado!");
      idBt = -1;
      break;
    case 4: //ping
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
    case 5://omegas ref
    case 6://omegas sinal de controle
      vec_float = new float[2];
      bitstream = new uint8_t[5];
      memset(bitstream, 0, 5*sizeof(uint8_t));
      memset(vec_float, 0, 2*sizeof(float));

      printf("Referencia do motor esquerdo ? (-1.0 a 1.0)\n");
      cin >> vec_float[0];
      printf("Referencia do motor direito ? (-1.0 a 1.0)\n");
      cin >> vec_float[1];

      encodeFloat(vec_float, bitstream+1);

      if(choice == 5){
        bitstream[0] = CMD_HEAD | CMD_SET_POINT;
      }else{ //control signal
        bitstream[0] = CMD_HEAD | CMD_CONTROL_SIGNAL;
      }

      btAction.sendBluetoothMessage(idBt, bitstream, 5*sizeof(uint8_t));

      delete[] bitstream;
      delete[] vec_float;

      break;
    case 7://calibrar
      bitstream = new uint8_t;
      bitstream[0] = CMD_HEAD | CMD_CALIBRATION;
      btAction.sendBluetoothMessage(idBt, bitstream, 1*sizeof(uint8_t));
      delete bitstream;
      break;
    case 8://identificar
      bitstream = new uint8_t;
      bitstream[0] = CMD_HEAD | CMD_IDENTIFY;
      btAction.sendBluetoothMessage(idBt, bitstream, 1*sizeof(uint8_t));
      delete bitstream;

      break;
    case 9://dados da calibracao
      bitstream = new uint8_t[1];
      vec_float = new float[8];

      bitstream[0] = CMD_HEAD | CMD_REQ_CAL;
      btAction.sendBluetoothMessage(idBt, bitstream, 1*sizeof(uint8_t));
      btAction.recvBluetoothMessage(idBt, (uint8_t*)vec_float, 8*sizeof(float));

      printf("Coeficientes da calibracao:\n");
      printf("Left  Front  => a = %f , b = %f \n", vec_float[0], vec_float[1]);
      printf("Left  Back   => a = %f , b = %f \n", vec_float[2], vec_float[3]);
      printf("Right Front  => a = %f , b = %f \n", vec_float[4], vec_float[5]);
      printf("Right Back   => a = %f , b = %f \n", vec_float[6], vec_float[7]);
      _pause();

      delete[] bitstream;
      delete[] vec_float;

      break;
    case 10://dados da identificacao
      bitstream = new uint8_t;

      bitstream[0] = CMD_HEAD | CMD_REQ_IDENT;
      btAction.sendBluetoothMessage(idBt, bitstream, 1*sizeof(uint8_t));
      delete   bitstream;

      break;
    case 11://solicitar omegas
      bitstream = new uint8_t[1];
      vec_float = new float[2];

      memset(vec_float, 0, 2*sizeof(float));

      bitstream[0] = CMD_HEAD | CMD_REQ_OMEGA;
      btAction.sendBluetoothMessage(idBt, bitstream, 1*sizeof(uint8_t));
      rec = btAction.recvBluetoothMessage(idBt, (uint8_t*)vec_float, 2*sizeof(float), 2);

      if(rec != 2*sizeof(float))
      {
        printf("Omega Left:%f \t Omega Right:%f\n", vec_float[0], vec_float[1]);
        _pause("Erro na leitura");
        break;
      }
      printf("Omega Left:%f \t Omega Right:%f\n", vec_float[0], vec_float[1]);
      _pause();

      delete[] bitstream;
      delete[] vec_float;

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
