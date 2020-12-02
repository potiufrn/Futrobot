#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "transmission.h"
#include "../comunicacao.h"
//static int cont=-100;
//static double x[1000];
//static double y[1000];
//static double tetta[1000];
//static double pwml[1000];
//static double pwmr[1000];

//const double treshold = 1.0/8.0;
//const double limiarZero = 0.00781;

using namespace ::std;

#ifndef _SO_SIMULADO_

#ifndef _TRANSMITION_BLUETOOTH_
static uint8_t pwm2Byte7bits(double pwm)
{
  // O primeiro bit eh o sinal (1=negativo 0=positivo ou nulo)
  uint8_t retorno = (pwm < 0.0 ? 128 : 0);
  pwm = fabs(pwm);
  if (pwm >= 1.0)
  {
    // Nao pode ser 1.0 exato para, quando truncar, nunca atingir o valor maximo 128
    // A funcao converte de 0 a 127
    pwm = 0.999999999;
  }
  retorno += (uint8_t)(128.0 * pwm);
  return retorno;
}
#else  //#ifndef _TRANSMITION_BLUETOOTH_
static uint8_t pwm2Byte8bits(double pwm)
{
  pwm = fabs(pwm);
  if (pwm >= 1.0)
  {
    // Nao pode ser 1.0 exato para, quando truncar, nunca atingir o valor maximo 256
    // A funcao converte de 0 a 255
    pwm = 0.999999999;
  }
  uint8_t retorno = (uint8_t)(256.0 * pwm);
  return retorno;
}

static void pwm2Bitstream(const float &left_f, const float &right_f, uint8_t *bitstream)
{
  static uint16_t ref_b[2];

  ref_b[0] = ((left_f > 0) << 15) | (uint16_t)(fabs(left_f) * 32767.0);
  ref_b[1] = ((right_f > 0) << 15) | (uint16_t)(fabs(right_f) * 32767.0);

  // bitstream[0] reservado para o HAED e CMD
  bitstream[1] = (ref_b[0] & 0xFF00) >> 8;
  bitstream[2] = (ref_b[0] & 0x00FF);
  bitstream[3] = (ref_b[1] & 0xFF00) >> 8;
  bitstream[4] = (ref_b[1] & 0x00FF);
}

#endif //#ifndef _TRANSMITION_BLUETOOTH_

#endif //#ifdef _SO_SIMULADO_

Transmission::Transmission(TEAM team, SIDE side, GAME_MODE mode) : FutData(team, side, mode)
{
#ifndef _SO_SIMULADO_
#ifndef _TRANSMITION_BLUETOOTH_
  struct termios port_config;          //sets up termios configuration structure for the serial port
  const char *device = "/dev/ttyACM0"; //ttyACM0 sets where the serial port

  cfsetispeed(&port_config, B115200); // taxa de transmissão de dados de entrada
  cfsetospeed(&port_config, B115200); // taxa de transmissão de dados de saida

  serial_fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY); //opens serial port in device slot
  if (serial_fd == -1)
  { //-1 is the error message for failed open port
    cerr << "Falha ao abrir a porta serial :( \n";
  }
#else  //#ifndef _TRANSMITION_BLUETOOTH_
  /*seta endereços dos robôs*/
  btAction.setBluetoothAddr(ID_ROBO_0);
  btAction.setBluetoothAddr(ID_ROBO_1);
  btAction.setBluetoothAddr(ID_ROBO_2);

  /*identifica bluetooths proximos*/
  //btAction.findActiveBluetoothDevice();

  /*faz a conecção com os bluetooths*/
  if (mode == REAL_MODE)
    btAction.initBluetoothDevices();
#endif //#ifndef _TRANSMITION_BLUETOOTH_
#endif
}
Transmission::~Transmission()
{
#ifndef _SO_SIMULADO_
#ifndef _TRANSMITION_BLUETOOTH_
  if (gameMode() == REAL_MODE)
  {
    close(serial_fd);
  }
#else  //#ifndef _TRANSMITION_BLUETOOTH_
#endif //#ifndef _TRANSMITION_BLUETOOTH_
#endif //#ifndef _SO_SIMULADO_
}

bool Transmission::transmission()
{
#ifndef _SO_SIMULADO_
  if (gameMode() == REAL_MODE)
  {
#ifndef _TRANSMITION_BLUETOOTH_
#define LEN_BUFFER 8
    static uint8_t dados[LEN_BUFFER];

    dados[0] = 0x80; //Valor de inicio do Cabeçalho
    for (int i = 0; i < 3; i++)
    {
      dados[(i * 2) + 1] = pwm2Byte7bits(pwm.me[i].left);
      dados[(i * 2) + 2] = pwm2Byte7bits(pwm.me[i].right);
    }
    dados[7] = 0x7F; //Valor de fim de Cabeçalho
    //write(serial_fd, &dados, LEN_BUFFER);

    if (write(serial_fd, &dados, LEN_BUFFER) != LEN_BUFFER)
    {
      cerr << "Erro na escrita no dispositivo serial" << endl;
      return true;
    }
#else //#ifndef _TRANSMITION_BLUETOOTH_
#define LEN_BUFFER 3
    //1 byte: HEAD | CMD |  2 bytes (pwm esquerdo) | 2 bytes (pwm motor direito)
    static uint8_t bitstream[5];
    // HEAD  | CMD
    // 0xA0  | 0x0B. Para enviar sinal de controle (sem controlador)
    bitstream[0] = 0xAB;
    // 0xA0  | 0x0A. Para enviar setpoint (com controlador)
    // bitstream[0] = 0xAA;
    for (int i = 0; i < 3; i++)
    {
      pwm2Bitstream(pwm.me[i].left, pwm.me[i].right, bitstream);

      if( btAction.sendBluetoothMessage(i, bitstream, 5 * sizeof(uint8_t)) )
      {
        cerr << "Erro na escrita no dispositivo bluetooth " << i << endl;
        //return true;
      }
    }
#endif //#ifndef _TRANSMITION_BLUETOOTH_

    return false;
  }

#endif //#ifndef _SO_SIMULADO_
  uint8_t dgram[MAX_DGRAM_SIZE];
  fira_message::sim_to_ref::Packet packet;
  fira_message::sim_to_ref::Command *command = packet.mutable_cmd()->add_robot_commands();

  for (int i = 0; i < 3; i++)
  {
    command->set_id(i);
    command->set_yellowteam((bool)myTeam());
    command->set_wheel_left(pwm.me[i].left   * 100.0);
    command->set_wheel_right(pwm.me[i].right * 100.0);

    memset(dgram, 0, MAX_DGRAM_SIZE);
    packet.SerializeToArray((void *)dgram, packet.ByteSize());
    if (sock_cmd.write(dgram, packet.ByteSize()) != SOCKET_OK)
    {
      std::cerr << "Falha ao enviar comando para o simulador!\n";
      return true;
    }
  }

  return false;
}
