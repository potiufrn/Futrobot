#include "functions.h"

//Função que gera um valor aleatório baseado na distribuição gaussiana
double rnd_gauss(const double &med, const double &desv)
{
  static double swt = false;
  static double U=drand48();
  static double V=drand48();

  static double normal;

  if(swt){
    U=drand48();
    V=drand48();
    normal = sqrt(-2.0*log(U))*cos(2.0*M_PI*V);
    swt = false;
  }else{
    normal = sqrt(-2.0*log(U))*sin(2.0*M_PI*V);
    swt = true;
  }

  return normal*desv + med;
  
}
