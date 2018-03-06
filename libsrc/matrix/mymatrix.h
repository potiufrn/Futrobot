#ifndef _MYMATRIX_H_
#define _MYMATRIX_H_

#include "matrix.h"

#define MATRIX_BUFFER_SIZE 10
#define MATRIX_NUM_ITERACOES 20

MATRIX_STATUS matrix_error_int(MATRIX_STATUS err);

inline static double real(double x){return x;}
inline static double real(cdouble x){return x.real();}
inline static double conjugado(double x){return x;}
inline static cdouble conjugado(cdouble x){return conj(x);}
inline static double modulo2(double x){return x*x;}
inline static double modulo2(cdouble x){return norm(x);}
// Erro na biblioteca "complex":        ^^^^^^^
// O que C++ chama de norma não é a norma euclidiana...
inline static double modulo(double x){return fabs(x);}
inline static double modulo(cdouble x){return abs(x);}
inline static void swap(double &x, double &y){double prov=x; x=y; y=prov;}
inline static void swap(cdouble &x, cdouble &y){cdouble prov=x; x=y; y=prov;}

inline static double sign(double a, double b)
{
  return(b>=0.0 ? modulo(a) : -modulo(a));
}
inline static double sign(cdouble a, cdouble b)
{
  return(real(b)>=0.0 ? modulo(a) : -modulo(a));
}
inline static double pow2(double x)
{
  return (x*x);
}

#endif
