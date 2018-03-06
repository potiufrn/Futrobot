#ifndef _FUTROBOT_H_
#define _FUTROBOT_H_


//#include <pthread.h>

#include "futdata.h"
#include "acquisition.h"
#include "localization.h"
#include "strategy.h"
#include "obstacles.h"
#include "control.h"
#include "transmission.h"
#include "export.h"
#include "../system.h"


class Futrobot
: //public Acquisition,
  //public Localization,
  public Strategy,
  //public Obstacles,
  public Control,
  public Transmission,
  //  public Export,
  public virtual FutData
{
  pthread_t thr_ger;
  double t_start, t_end_cap, t_end_acq, 
    t_end_loc, t_end_str, t_end_obs, 
    t_end_con, t_end_tra, t_end_exp;

 public:
  Futrobot( TEAM team, SIDE side, GAME_MODE gameMode);
  bool start_management();
  bool finish_management();
  void management();
  void print_state() const;

  //metodos de consulta
  inline CONFIG getPos() const {return FutData::pos; }
  inline REFERENCES getRef() const { return FutData::ref; }
  inline PWM_ROBOTS getPwm() const { return FutData::pwm; }
  inline double getDtAmostr() const { return FutData::dt_amostr; }
  //myTeam() -> em futdata.h
  //mySide() -> em futdata.h
  //gameState() -> em futdata.h
  
  
  //metodos de modificação
  inline void setRef(const REFERENCES &newRef){ FutData::ref = newRef; };
  //setTeam() -> em futdata.h
  //setSide() -> em futdata.h
  //setAdvantage() ?  -> em futdata.h
  //setGameState() ?  -> em futdata.h

  // private:
  //  bool exportdat();
};

#endif
