#ifndef _CALIBRATORPROCESSOR_H_
#define _CALIBRATORPROCESSOR_H_

#include <cstdlib>
#include <stdio.h>
#include <imagem.h>
#include <camera.h>
#include "../dados_calibracao.h"


#define LARGURA_EXIBICAO 640
#define ALTURA_EXIBICAO 480
#define NUM_LIMITES 6
#define H_MIN_VALUE -180
#define H_MAX_VALUE 180
#define PG_MIN_VALUE 0
#define PG_MAX_VALUE 100


enum MODOS_PROCESSAMENTO{
  CALIBRATOR_IMAGEM_REAL,
  CALIBRATOR_LIMITES_P_E_PONTOS,
  CALIBRATOR_LIMITES_P,
  CALIBRATOR_PONTOS,
  CALIBRATOR_COR_ATUAL,
  CALIBRATOR_COR_ETIQUETADA,
  CALIBRATOR_COR_ETIQUETADA_SOFT,
  CALIBRATOR_IMAGEM_ERROS
};


class CalibratorProcessor:
public Camera
{
 private:
  ImagemRGB ImProcessada;
  //as variaveis que controlam o modo de funcionamento da classe
  MODOS_PROCESSAMENTO modo;
  unsigned nRetas, LarguraCaptura, AlturaCaptura;

  //as variaveis que armazenam dados vindos da interface
  Coord2 *pontosImagemIniciais;

  PARAMETROS_CALIBRACAO calibracaoParam;
  RETA *retas;

  char** nomeCor;
  PxRGB *cores;
  PxRGB *coresInversas;

  int corAtual;
  unsigned offset_u,offset_v;
  bool true_color;
  bool campoVazio_capturado;
  // bool calculating; //flag sinalizando que o Calibrator esta realizando um calculo de campo medio
 public:
  CalibratorProcessor();
  CalibratorProcessor(const char* arquivo);
  ~CalibratorProcessor();
  bool readFile(const char* arquivo);

  inline void setMode(MODOS_PROCESSAMENTO m){modo = m;}
  inline void setColor(int cor){corAtual = cor;}
  inline int getColor(){ return corAtual; }
  bool fileOpen(const char* text);
  bool fileSave(const char* text);
  bool loadCameraParam(const char* arquivo);
  bool saveCameraParam(const char* arquivo);
  inline int getPinf(){return calibracaoParam.limiarPInf;}
  inline int getPsup(){return calibracaoParam.limiarPSup;}
  inline float getConstObject(){return calibracaoParam.const_Object;}
  inline float getConstField(){return calibracaoParam.const_Field;}
  //WARNING este metodo foi alterado temporariamente
  inline void setPinf(int Pinf){
    calibracaoParam.limiarPInf = Pinf;
    // setMinP(calibracaoParam.limiarPInf/100.0);
  }
  //WARNING este metodo foi alterado temporariamente
  inline void setPsup(int Psup){
    calibracaoParam.limiarPSup = Psup;
    // setMaxP(calibracaoParam.limiarPSup/100.0);
  }

  inline void setConstObject(float value){ calibracaoParam.const_Object = value/10.0; }
  inline void setConstField(float value){ calibracaoParam.const_Field = value/10.0; }

  inline limitesHPG getLimHPG(int i){return calibracaoParam.limHPG[i];}
  inline void setHmin(int i, int valor){calibracaoParam.limHPG[i].H.min = valor;};
  inline void setHmax(int i, int valor){calibracaoParam.limHPG[i].H.max = valor;};
  inline void setPmin(int i, int valor){calibracaoParam.limHPG[i].P.min = valor;};
  inline void setPmax(int i, int valor){calibracaoParam.limHPG[i].P.max = valor;};
  inline void setGmin(int i, int valor){calibracaoParam.limHPG[i].G.min = valor;};
  inline void setGmax(int i, int valor){calibracaoParam.limHPG[i].G.max = valor;};

  //O controle shutter esta sendo usado como se fosse
  //Exposure Absolute
  inline void setShutter(int valor){Camera::setExposureAbs(valor); };

  void resetHPG();
  void resetPixelsNotaveis();
  void calImgMedia(unsigned num);
  inline bool campoVazioCapturado(){return campoVazio_capturado;}

  // inline bool isCalculating(){ return calculating; }

  bool loadImage(const char* arq);
  void saveImage(const char* arq);
  bool processImage();
  inline const void *getPNMdata(){return ImProcessada.getPNMData(); };
  inline int getPNMsize(){ return ImProcessada.getPNMSize();};
  inline int getImWidth(){ return ImProcessada.getWidth();};
  inline int getImHeight(){return ImProcessada.getHeight();};
  inline int getCamWidth(){ return Camera::getWidth();};
  inline int getCamHeight(){return Camera::getHeight();};
  void getPxValor(int x, int y, int &R, int &G1, int &B, int &H, int &P, int &G2);
  int pontoSelecionado(int x,int y);
  void moverPonto(int ponto,int x,int y);
  inline bool posicaoValida(unsigned int x, unsigned int y){
    return (x < ImProcessada.getWidth() && y < ImProcessada.getHeight());
  }
  inline void modoCaptura(bool cap){capturando = cap;}
  inline char* getNomeCor(unsigned i){ return nomeCor[i]; }
  inline unsigned getNumCores(){ return calibracaoParam.nCores; };
  inline void setOffsetU(unsigned u){ offset_u = u;}
  inline void setOffsetV(unsigned v){ offset_v = v;}
};

#endif
