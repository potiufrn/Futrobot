#include <iostream>
#include <stdio.h>
#include <bluetoothAction.h>
#include <omp.h>

// ESP de TEST : "30:AE:A4:3B:A4:26"
// #define MAC_BT "30:AE:A4:3B:A4:26" //TEST
// #define MAC_BT "30:AE:A4:20:0E:12"    //1

#define MAC_BT "30:AE:A4:20:0E:12"
#define PACKET_SIZE 5

#define CMD_HEAD           0xA0
#define CMD_CONTROL_SIGNAL 0x0B
#define CMD_SET_POINT      0x0A


enum{ LEFT, RIGHT };

#define F_IS_NEG(x) (*(uint32_t*)&(x) >> 31)

using namespace std;

int main(int argc, char** argv)
{
  BluetoothAction btAction;
  uint8_t         packet[PACKET_SIZE];
	float           ref_f[2];
	uint16_t        ref_b[2];
  int             idBt;

  printf("Programa de Teste do Bluetoth Action\n");

  idBt = btAction.setBluetoothAddr(MAC_BT);
  btAction.initBluetoothById(idBt);

	memset(packet, 0, PACKET_SIZE);


  #define ABS_F(x) (((x)<0.0)?-(x):(x))
  while(1)
  {
		// printf("Referencia Esquerda e Direita | -2 para terminar\n");
		cout << "Referencia para Esquerda e Direita | -2 para terminar\n";
		cin >> ref_f[LEFT];
		if(ref_f[0] == -2.0)
			break;
		cin >> ref_f[RIGHT];
		if(ref_f[1] == -2.0)
			break;

		ref_b[LEFT] = (!F_IS_NEG(ref_f[LEFT]) << 15)  | (uint16_t)(ABS_F(ref_f[LEFT])*32767.0);
    ref_b[RIGHT]= (!F_IS_NEG(ref_f[RIGHT]) << 15) | (uint16_t)(ABS_F(ref_f[RIGHT])*32767.0);

		packet[0] = CMD_HEAD | CMD_CONTROL_SIGNAL;
		packet[1] = (ref_b[LEFT] & 0xFF00) >> 8;
		packet[2] = (ref_b[LEFT] & 0x00FF);
		packet[3] = (ref_b[RIGHT] & 0xFF00) >> 8;
		packet[4] = (ref_b[RIGHT] & 0x00FF);

		printf("Left:%X \t Right:%X\n", ref_b[LEFT], ref_b[RIGHT]);

		btAction.sendBluetoothMessage(idBt, PACKET_SIZE, packet);
		// system("clear");
  }

  printf("Encerrando conexao\n");
  btAction.closeBluetoothById(idBt);
  return 0;
}
