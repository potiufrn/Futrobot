#ifndef _TRANSMISSION_H_
#define _TRANSMISSION_H_

/******************************************************/
/* Funções de comunicação com os robôs                */
/******************************************************/


#include "futdata.h"


#ifndef _SO_SIMULADO_
#ifndef TRANSMISSOR_VELHO
#include "USBrobot.h"
#else
#include "ftdi.h"
#endif //#ifndef TRANSMISSOR_VELHO
#endif //#ifndef _SO_SIMULADO_


class Transmission :
  public virtual FutData
{
 
private:
#ifndef _SO_SIMULADO_
#ifndef TRANSMISSOR_VELHO
  USBrobot usb_handler;
#else
  FTDI ftdi;
#endif //#ifndef TRANSMISSOR_VELHO
#endif //#ifndef _SO_SIMULADO_

public:
  Transmission(TEAM team, SIDE side, GAME_MODE mode);
  ~Transmission();
  bool transmission();

};

#endif //_TRANSMISSION_H_
