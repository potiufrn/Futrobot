/***************************************************/
/*   Definições das paredes do campo               */
/***************************************************/

#ifndef _CAMPO_H_
#define _CAMPO_H_

#include "../data.h"

#define NUM_ARESTAS 16

class Campo {
 private:
  Coord2 ponto[NUM_ARESTAS+1];
 public:
  Campo();
  inline Coord2 vertice(int i) {return ponto[i];}
};

#endif
