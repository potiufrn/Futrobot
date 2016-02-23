//DANIEL MORAIS - 22.09.2015
//FUTEBOL DE ROBÔS - JERIMUM

using namespace std;
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>

/*Mesmas variáveis do código original*/
#define LEN_BUFFER 8  // número de bytes
int idRobo;
double ue,ud; // tensão do motor esquerdo e direito
unsigned char dados[LEN_BUFFER];

unsigned char pwmDouble2Char( double pwmDouble );


int main ()
{
    for(int i=0;i<8;i++){
                dados[i]=0;
    }
    int serial_fd; //process ID info for forking and file descriptor for serial stream
    struct termios port_config; //sets up termios configuration structure for the serial port
    cfsetispeed(&port_config, B115200);// taxa de transmissão de dados de entrada
    cfsetospeed(&port_config, B115200);// taxa de transmissão de dados de saida

    char confirma, resposta; // imprimir da porta serial
    bool espera = true;

    bool flag=true;
    const char *device = "/dev/ttyACM0"; //ttyACM0 sets where the serial port is plugged in
    serial_fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY); //opens serial port in device slot

    if(serial_fd == -1)  //-1 is the error message for failed open port
    {
        cout << "Falha ao abrir a porta serial :( \n";
    }

    else
    {
        cout << "Porta aberta com sucesso! Conectado! \n\n";
        cout << "=========== IDENTIFICADOR DE ROBÔ ===========\n\n";

        //do
        while(1)
        {
            //cout<<"Identificar robô? [ S/N ]"<<endl;
            //cin>>confirma;

            //if(confirma == 's'|| confirma == 'S')
            {

                cout << "Informe o robô (0, 1 ou 2):" << endl;
                cin >> idRobo;
                cout << "\nDigite o valor de tensao para o motor esquerdo (-1 a 1): ";
                cin >> ue;

                cout << "Digite o valor de tensao para o motor direito (-1 a 1): ";
                cin >> ud;

                if(ue >= -1.0 && ue <= 1.0 && ud >= -1.0 && ud <= 1.0)
                {

                    dados[0] = 0x80; //Valor de inicio do Cabeçalho  //está enviando um numero errado!//deve enviar 0x80
                    dados[(idRobo*2)+1] = pwmDouble2Char( ue );  //tensão no motor esquerdo
                    dados[(idRobo*2)+2] = pwmDouble2Char( ud );  //tensão no motor direito
                    dados[7] = 0x7F; //Valor de fim de Cabeçalho
                    
                    //write(serial_fd, &dados, 8);
		    write(serial_fd, &dados, 8);
		    usleep(30000);
                    cout << "Enviado: ";
                    for(int i = 0; i < sizeof(dados); i++)
                    {
                        cout <<hex<<(int)dados[i]<<" ";  //internal ou hex
                    }
                    cout << endl;

                }
                else
                {
                    cout << "Valores invalidos!";
                }

                cout << "\n\n";
            }
            //limpar dados!
            for(int i=0;i<8;i++)
            {
                dados[i]=0;
            }

        }
        //while(confirma!='n' && confirma!='N');

    }//fim do else

    //close(serial_fd);
    return 0;
}

unsigned char pwmDouble2Char( double pwmDouble )
{
    unsigned char retorno = 0;
    double absPWM = pwmDouble;

    if( absPWM < 0.0 )
    {
        retorno = 128;
        absPWM = fabs( pwmDouble );
    }
    if( absPWM>=1.0 )
        absPWM = 0.99999999;

    retorno += (unsigned char)((int)(absPWM*128.0));
    return retorno;
}
