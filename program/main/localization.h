#ifndef _LOCALIZATION_H_
#define _LOCALIZATION_H_

#include "futdata.h"

//#define VEL_BOLA_PARADA 0.025 //CHECAR
#define TEMPO_BOLA_FUTURA 0.1

class Localization : public virtual FutData
{
private:
public:
  Localization( TEAM team, SIDE side, GAME_MODE gameMode);
  ~Localization();
  bool localization();
};

#endif
