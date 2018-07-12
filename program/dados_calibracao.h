#ifndef _DADOS_CALIBRACAO_
#define _DADOS_CALIBRACAO_

#include <cstdio>
#include <matrix.h>
#include <imagem.h>

#include <fstream>
#include <string.h>

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
  // ImagemByte campoVazio;
  // uint8_t desvioPadrao;

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

  bool read(std::istream &I);
  std::ostream &write(std::ostream &O)const;

  // bool difData(uint8_t byte, unsigned pos);

  int getSoftColor(const float H,const float P, const float G) const;
  int getHardColor(const float H,const float P, const float G) const;
};

#endif
