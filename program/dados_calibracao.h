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

enum{
  IS_FIELD = 0,
  IS_OBJECT = 1,
  IS_UNDEF = -1
};

namespace cores{
  //associado diretamente aos indices do vetor de cores.
  enum{
    LARANJA = 0,
    AZUL = 1,
    AMARELO = 2,
    CIANO = 3,
    ROSA = 4,
    VERDE = 5,
    PRETO = 6,
    BRANCO = 7
  };
}

struct PARAMETROS_CALIBRACAO{
  //WARNING Apenas enquanto nao cria a classe/struct pixel_bruto
  #define CONST ((campoVazio.getPxFormat() == GBRG)?1:2) //supondo apenas os formatos GBRG e YUV422
  #define POS(i,j) CONST*((i)*campoVazio.getWidth() + (j))


  //Vetor que armazena o desvio padrao de cada byte da imagem
  //o calculo eh realizado no calibradorprocessor.
  // uint8_t *desvioPadrao;
  ImagemBruta desvioPadrao;
  //imagem de dados brutos que contem a imagem media
  //do campo sem objetos(apenas campo).
  ImagemBruta campoVazio;
  //variaveis para controlar os intervalores(quantidade de desvios) em que
  //os bytes(pixel_bruto) podem ser considerados
  //campo e objeto, respectivamente.
  //Esses valores sao ajustados no calibrador(via sliders)
  float const_Field;
  float const_Object;
  // float numDesvios; //Numeros de desvios padroes para se considerar campo


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

  bool isObject(unsigned i, unsigned j, uint8_t byte);
  bool isField(unsigned i, unsigned j, uint8_t byte);
  int isDiff(unsigned i, unsigned j, uint8_t byte);

  // bool isObject(unsigned i, unsigned j, PxBruto px);
  // bool isField(unsigned i, unsigned j, PxBruto px);
  // int isDiff(unsigned i, unsigned j, PxBruto px);

  bool isColor(const float H,const float P, const float G, int color) const;
  int getSoftColor(const float H,const float P, const float G) const;
  int getHardColor(const float H,const float P, const float G) const;
};

#endif
