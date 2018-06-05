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
  //Imagem campoVazio;

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
  int getSoftColor(const float H,const float P, const float G) const;
  int getHardColor(const float H,const float P, const float G) const;
};

#endif
