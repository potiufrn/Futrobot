#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <bluetoothAction.h>

using namespace::std;


const int LEN_BUFFER = 3;
const int MAX_PWM_ARDUINO = 256;

BluetoothAction btAction;// = BluetoothAction;

//COPIA DA STRUCT DO CONTROLE
struct pwmRobo{
	double left,right;
};

pwmRobo me[3];

uint8_t pwmToByte(double pwm){

	pwm=fabs(pwm);
	if(pwm>=1.0){
		pwm=0.999999999;
	}
	uint8_t byte = MAX_PWM_ARDUINO*pwm; //256 é o valor máximo de um byte para o arduino
	return byte;
}

bool transmission(){
	static uint8_t dados[LEN_BUFFER];
	for(int i=0;i<3;i++){
		// os seis primeiros bits do primeiro byte são 1010 10xx = 0xA8
		dados[0]=0xA8;
		// o sétimo bit do primeiro byte é o sentido do motor left
		// 1= para frente , 0= para trás.
		if(me[i].left>= 0.0){
			dados[0]+=0x02;
		}
		// o oitavo bit do primeiro byte é o sentido do motor right
		// 1= para frente , 0= para trás.
		if(me[i].right>= 0.0){
			dados[0]+=0x01;
		}
		// o segundo byte é a velocidade do motor left
		dados[1]=pwmToByte(me[i].left);
		// o terceiro byte é a velocidade do motor right
		dados[2]=pwmToByte(me[i].right);
		cout<<"dados  = "<<(int)dados[0] << ' ' <<(int)dados[1]<< ' ' <<(int)dados[2]<<endl;
		btAction.sendBluetoothMessage(i,dados);
	}
	return false;
}


int main(){
	int idRobo;

	cout << "----------------------------------------------------------\n";
	cout << "\tSimple Bluetooth Zone :: 'Powered by BluetoothAction.h' \n";
	cout << "----------------------------------------------------------\n";

	/*scan all active bluetooths*/
	//findActiveBluetoothDevice();

	/*seta endereços dos robôs*/
	btAction.setBluetoothAddr("20:17:03:06:25:29");
	btAction.setBluetoothAddr("20:17:03:06:00:34");
	btAction.setBluetoothAddr("20:17:03:06:17:86");

	/*identifica bluetooths proximos*/
	//btAction.findActiveBluetoothDevice();

	/*faz a conecção com os bluetooths with RFCOMM protocol*/
	btAction.initBluetoothDevices(btAction.getNumBTDevices());

  /*
  for (int i=0; i<1000; i++)
  {
    me[0].left = sin(2*M_PI*i/1000.0);
    me[0].right = sin(2*M_PI*i/1000.0);

    me[1].left = cos(2*M_PI*i/1000.0);
    me[1].right = cos(2*M_PI*i/1000.0);

    me[2].left = sin(2*M_PI*i/1000.0);
    me[2].right = cos(2*M_PI*i/1000.0);

    cout << "PWMs:" << ' ' << me[0].left << ';' << me[0].right
		    << ' ' << me[1].left << ';' << me[1].right
		    << ' ' << me[2].left << ';' << me[2].right << endl;
    transmission();

    usleep(30000);
  }

  me[0].left = 0;
  me[0].right = 0;

  me[1].left = 0;
  me[1].right = 0;

  me[2].left = 0;
  me[2].right = 0;

  transmission();

  return 0;
  */
    do{/*
		for(int i=0;i<3;i++){
			me[i].left=0;
			me[i].right=0;
		}*/
		do{
			cout << "Digite o numero do robo(0, 1 ou 2) ou -1 para terminar: ";
			cin >> idRobo;
		}while(idRobo!=0 && idRobo!=1 && idRobo!=2 && idRobo!=-1);

		if(idRobo!=-1){
			cout << "\nDigite o valor de tensao para o motor esquerdo (-1 a 1): ";
			cin >> me[idRobo].left;
			cout << "Digite o valor de tensao para o motor direito (-1 a 1): ";
			cin >> me[idRobo].right;
			if(me[idRobo].left >= -1.0 && me[idRobo].left <= 1.0 &&
			   me[idRobo].right >= -1.0 && me[idRobo].right <= 1.0){
				transmission();
			}
			else{
				cout << "Valores invalidos!";
			}
			cout << "\n\n";
		}
    } while(idRobo != -1);
	return 0;
}
