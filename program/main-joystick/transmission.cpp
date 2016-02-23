#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "transmission.h"


//const double treshold = 1.0/8.0;
//const double limiarZero = 0.00781;

using namespace::std;

#ifndef _SO_SIMULADO_
#ifndef TRANSMISSOR_VELHO
static void pwmDouble2Char2( double pwmDouble , unsigned char *data)
{
  int intPWM = 0;
  data[0] = 0;
  data[1] = 0;
  double absPWM = pwmDouble;
  if( absPWM < 0.0 ) {
    data[0] = 0x04; //Seta bit de sentido reverso
    absPWM = fabs( pwmDouble );
  }
  
  if( absPWM>=1.0 )
    absPWM = 1.0; //Satura valor maximo em 1.0

  intPWM = (int)(absPWM*1023.0);
  // cout << dec << intPWM << endl;
  data[1] = (unsigned char)(intPWM & 0xFF);
  intPWM = intPWM >> 8;
  data[0] = data[0] | ((unsigned char)(intPWM & 0x03));
}
#else //transmissor velho
static unsigned char pwmDouble2Char( double pwmDouble )
{
  unsigned char retorno = 0;
  double absPWM = pwmDouble;

  if( absPWM < 0.0 ) {
    retorno = 128;
    absPWM = fabs( pwmDouble );
  }
  if( absPWM>=1.0 )
    absPWM = 0.99999999;

  retorno += (unsigned char)((int)(absPWM*128.0));
  return retorno;
}
#endif //#ifndef TRANSMISSOR_VELHO
#endif //#ifdef _SO_SIMULADO_

Transmission::Transmission(TEAM team, SIDE side, GAME_MODE mode):
  FutData(team,side,mode)
#ifndef _SO_SIMULADO_
#ifndef TRANSMISSOR_VELHO
  ,usb_handler()
#else // //transmissor velho
  ,ftdi()
#endif //#ifndef TRANSMISSOR_VELHO
#endif //_SO_SIMULADO_
{
#ifndef _SO_SIMULADO_
#ifndef TRANSMISSOR_VELHO
  
#else // transmissor velho
  const char * time = "FTCEMQ0G";
  if (team == BLUE_TEAM) {
    time = "FTCEMPRR";
  }
  if(gameMode() == REAL_MODE){
    // if(!usbSerial.open(FT_BAUD_57600)){
    //   cerr << "Erro ao abrir o dispositivo USB\n";
    //   int usb, ft;
    //   usbSerial.status(usb,ft);
    //   cerr << "STATUS_USB: "<< usb << " STATUS_FT: "<< ft << endl;
    //   exit(1);
    // }
    // else {
    //   cout << "Dispositivo USB aberto." << endl;
    // }
    ftdi.ftdi_open();
  }
#endif //#ifndef TRANSMISSOR_VELHO
#endif
}

Transmission::~Transmission()
{
#ifndef _SO_SIMULADO_
#ifndef TRANSMISSOR_VELHO
  
#else // transmissor velho
  if(gameMode() == REAL_MODE){
    // if(!usbSerial.close()){
    //   cerr << "Erro ao fechar o dispositivo USB\n";
    //   int usb, ft;
    //   usbSerial.status(usb,ft);
    //   cerr << "STATUS_USB: "<< usb << " STATUS_FT: "<< ft << endl;
    //   exit(1);
    // }
    ftdi.ftdi_close();
  }
#endif //#ifndef TRANSMISSOR_VELHO
#endif //#ifndef _SO_SIMULADO_
}

bool Transmission::transmission()
{
#ifndef _SO_SIMULADO_
#ifndef TRANSMISSOR_VELHO
#define LEN_BUFFER  14
  if(gameMode() == REAL_MODE){  
    unsigned char dados[LEN_BUFFER];
    
    dados[0] = 0x80; //Valor de inicio do Cabeçalho
    for( int i=0; i<3; i++ ) {
      pwmDouble2Char2(pwm.me[i].left,&dados[(i*4)+1]);
      pwmDouble2Char2(pwm.me[i].right,&dados[(i*4)+3]);
    }
    dados[LEN_BUFFER-1] = 0x7F; //Valor de fim de Cabeçalho
    try{
      usb_handler.writeDevice(dados, sizeof(dados));
    }catch(USBException ex){
      cout << ex.what() << endl;
    }
    
    return false;
  }
#else // transmissor velho
#define LEN_BUFFER  8
  if(gameMode() == REAL_MODE){  
    unsigned char dados[LEN_BUFFER];
    
    dados[0] = 0x80; //Valor de inicio do Cabeçalho
    for( int i=0; i<3; i++ ) {
      dados[(i*2)+1] = pwmDouble2Char( pwm.me[i].left );
      dados[(i*2)+2] = pwmDouble2Char( pwm.me[i].right );
    }
    dados[7] = 0x7F; //Valor de fim de Cabeçalho
    
    // if(!usbSerial.write((unsigned char) 0x80,dados,LEN_BUFFER+1)){
    //   int usb,ft;
    //   usbSerial.status(usb,ft);
    //   cerr << "STATUS_USB: "<< usb << " STATUS_FT: "<< ft << endl;
    //   return true;
    // }
    ftdi.ftdi_write(dados,sizeof(dados));
    return false;
  }
#endif //#ifndef TRANSMISSOR_VELHO
#endif //#ifndef _SO_SIMULADO_
  
  
  SINAL_RADIO mySignal;
  mySignal.id = id_pos;
  for( int i=0; i<3; i++ ) {
    mySignal.c.me[i] = pwm.me[i];
  }
  if(sock.write(&mySignal,sizeof(SINAL_RADIO))){
    cerr << "Erro na escrita no socket" << endl;
    return true;
  }
  
  return false;
}


