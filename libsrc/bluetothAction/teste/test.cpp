#include <stdio.h>
#include <omp.h>
#include "../bluetoothAction.h"

// #define LEN_MSG 3
int LEN_MSG = 12;

int main(int argc, char** argv)
{
  BluetoothAction btAction;
  uint8_t buffer[12];
  int length;
  int idBt;
  double tsend[2], trecv[2];

  printf("Programa de Teste do Bluetoth Action\n");

  idBt = btAction.setBluetoothAddr("30:AE:A4:3B:A4:26");
  btAction.initBluetoothById(idBt);

  memset(buffer, 0, LEN_MSG);

  const unsigned char msg[LEN_MSG] = "HELLO WORLD";


  while(1)
  {
    tsend[0] = omp_get_wtime();
    btAction.sendBluetoothMessage(idBt, LEN_MSG, msg);
    length = btAction.recvBluetoothMessage(idBt, buffer, LEN_MSG);
    tsend[1] = omp_get_wtime();

    if(length != -1)
    {
      printf("Tempo para enviar e receber:%f\n", tsend[1] - tsend[0]);
      printf("Mensagem recebida: %s\n", buffer);
    }
    else{
      printf("ERRO com RECV!\n");
    }
  }

  printf("Encerrando conexao\n");
  btAction.closeBluetoothById(idBt);
  return 0;
}
