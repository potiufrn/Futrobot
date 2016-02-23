#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <math.h>
#include <stdlib.h>
#include <time.h>

static inline double sgn(double x) {
  return ( x>0.0 ? +1.0 : (x<0.0 ? -1.0 : 0.0) );
}

// Um arco tangente que funciona para ângulos de +- 90 graus
static inline double arc_tang(double y, double x)
{
  return (x != 0.0 ? atan2(y,x) : (y>=0 ? M_PI/2.0 : -M_PI/2.0));
}

// Retorna um ângulo equivalente no intervalo entre -180 graus e +180 graus
static inline double ang_equiv(double theta)
{
  return (arc_tang(sin(theta), cos(theta)));
}

// Retorna um ângulo rebatido para o 1o ou o 4o quadrantes
static inline double ang_equiv2(double theta)
{
  return asin(sin(theta));
}

// Um tan que não dá erro quanto theta=90graus
static inline double mytan(double x) {
  return tan(x);
  //return fabs(ang_equiv2(x))<1.5608 ? tan(x) : 100.0;
}

// Eleva ao quadrado
static inline double pow2(double x)
{
  return (x*x);
}

// Retornam o máximo e mínimo
static inline double mymax(double x, double y) {
  return (x>y ? x : y);
}
static inline double mymax(double x, double y, double z) {
  return (x>y ? (x>z ? x : z) : (y>z ? y : z) );
}
static inline double mymin(double x, double y) {
  return (x<y ? x : y);
}
static inline double mymin(double x, double y, double z) {
  return (x<y ? (x<z ? x : z) : (y<z ? y : z) );
}

//funcao degrau
static inline double d(double x){return (x>=0?1.0:0.0);};

 //TODO: Definir uma formula para essa ZM e sair alterando os codigos
 //para todos estarem de acordo com a mesma. Por exemplo, o calculo
 //atual considerou D_f > 0 e d_b < 0; já o utilizado em RLSIdent
 //considerou ambos D_f,D_b>0.
/* static inline double ZM(double u, double D_f,double D_b){ */
/*   return (u-D_f)*d(u-D_f)+(u+D_b)*d(-u-D_b); */
/* } */

static inline double sature(double value, double min, double max)
{return ((value < min)?(min):((value > max)?(max):(value)));};

//Função que gera um valor aleatório baseado na distribuição gaussiana
double rnd_gauss(const double &med, const double &desv);
#endif
