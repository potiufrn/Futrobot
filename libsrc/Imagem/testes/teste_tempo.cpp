#include <iostream>
#include <stdlib.h>
#include <sys/time.h>

#include "imagem.h"

using namespace std;

// Retorna o intervalo de tempo em microssegundos
double deltaT(const struct timeval &tF, const struct timeval &tI)
{
  if (tF.tv_sec < tI.tv_sec) {
    cerr << "Problema: tempo final menor que inicial..." << endl;
    exit(1);
  }
  if (tF.tv_sec == tI.tv_sec) {
    if (tF.tv_usec < tI.tv_usec) {
      cerr << "Problema: tempo final menor que inicial..." << endl;
      exit(1);
    }
    return(tF.tv_usec - tI.tv_usec);
  }
  return(1000000.0*(tF.tv_sec-tI.tv_sec) + tF.tv_usec - tI.tv_usec);
}

// Gera um pixel aleatório
PxRGB aleatorio()
{
  return PxRGB(uint8_t(floor(255.0*drand48())),
	       uint8_t(floor(255.0*drand48())),
	       uint8_t(floor(255.0*drand48())));
}

// Dimensões da imagem de teste
#define LARGURA 1280
#define ALTURA 960

int main()
{
  srand48(time(NULL));

  ImagemRGB I(LARGURA,ALTURA), J(LARGURA,ALTURA);
  PxRGB Porig,Pcalc;
  float H,G,P;
  unsigned int N=LARGURA*ALTURA,v,u;

  struct timeval tIni, tFin;
  double t1,t2,t3,t4;

  // Geração da imagem original
  for (v=0; v<ALTURA; v++) {
    for (u=0; u<LARGURA; u++) {
      I[v][u] = aleatorio();
    }
  }

  // Tempo de leitura
  gettimeofday(&tIni,NULL);
  for (v=0; v<ALTURA; v++) {
    for (u=0; u<LARGURA; u++) {
      Porig = I[v][u];
    }
  }
  gettimeofday(&tFin,NULL);
  t1 = deltaT(tFin,tIni)/N;

  // Tempo de getHPG
  gettimeofday(&tIni,NULL);
  for (v=0; v<ALTURA; v++) {
    for (u=0; u<LARGURA; u++) {
      Porig = I[v][u];
      Porig.getHPG(H,P,G);
    }
  }
  gettimeofday(&tFin,NULL);
  t2 = deltaT(tFin,tIni)/N;

  // Tempo de H2PxRGB
  gettimeofday(&tIni,NULL);
  for (v=0; v<ALTURA; v++) {
    for (u=0; u<LARGURA; u++) {
      Porig = I[v][u];
      Porig.getHPG(H,P,G);
      Pcalc = H2PxRGB(H);
    }
  }
  gettimeofday(&tFin,NULL);
  t3 = deltaT(tFin,tIni)/N;

  // Tempo de atribuição
  gettimeofday(&tIni,NULL);
  for (v=0; v<ALTURA; v++) {
    for (u=0; u<LARGURA; u++) {
      Porig = I[v][u];
      Porig.getHPG(H,P,G);
      Pcalc = H2PxRGB(H);
      J[v][u] = Pcalc;
    }
  }
  gettimeofday(&tFin,NULL);
  t4 = deltaT(tFin,tIni)/N;

  cout << "Tempos médios individual/acumulado (em microssegundos):" << endl;
  cout << "leitura:   \t" << t1 << '\t' << t1 << endl;
  cout << "getHPG:    \t" << t2-t1 << '\t' << t2 << endl;
  cout << "H2PxRGB:   \t" << t3-t2 << '\t' << t3 << endl;
  cout << "atribuição:\t" << t4-t3 << '\t' << t4 << endl;
}

