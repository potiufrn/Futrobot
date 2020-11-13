#ifndef _LOCALIZATION_H_
#define _LOCALIZATION_H_

#include "futdata.h"

class Localization : public virtual FutData
{
private:
public:
  Localization( TEAM team, SIDE side, GAME_MODE gameMode);
  ~Localization();
  bool localization();
};

#endif
