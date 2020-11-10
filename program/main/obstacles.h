#ifndef _OBSTACLES_H_
#define _OBSTACLES_H_

#include "futdata.h"


#define ZONA_INFL_MIN 0.075
#define ZONA_INFL_MAX 0.15
// #define ZONA_INFL_MIN 0.05
// #define ZONA_INFL_MAX 0.10

class Obstacles : public virtual FutData
{
private:
  //Obstacles deve alterar o valor de ref pra uma nova referencia
  //procurando desviar de possiveis obstaculos no caminho.
  void desvio_area(int i);

public:
  Obstacles(TEAM time, SIDE lado, GAME_MODE gameMode);
  ~Obstacles();
  bool obstacles();
};

#endif
