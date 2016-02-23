#ifndef _DADOS_CALIBRACAO_
#define _DADOS_CALIBRACAO_

#include <cstdio>
#include <matrix.h>
#include <imagem.h>

#define FATOR_CONVERSAO 100000.0

struct RETA {
  unsigned p1;
  unsigned p2;
};

struct limites {
  int max,min;
};

struct limitesHPG {
  limites H,P,G;    
};

struct PARAMETROS_CALIBRACAO {
  unsigned nPontosNotaveis;
  unsigned nCores;
  Coord2 *pontosImagem;
  Coord2 *pontosReais;
  int limiarPInf;
  int limiarPSup;
  limitesHPG *limHPG;
  PARAMETROS_CALIBRACAO();
  ~PARAMETROS_CALIBRACAO();
  bool read(const char* arquivo);
  bool write(const char* arquivo) const;
  int getSoftColor(const PxRGB &p) const;
  int getHardColor(const PxRGB &p) const;

};

#endif
