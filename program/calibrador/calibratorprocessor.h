#ifndef _CALIBRATORPROCESSOR_H_
#define _CALIBRATORPROCESSOR_H_

#include <cstdlib>
#include <stdio.h>
#include <imagem.h>
#include <camera.h>
#include "../dados_calibracao.h"

#include <iostream> //TODO: tirar isso

using namespace std;

//#define N_PONTOS_NOTAVEIS 28
//#define NUM_RETAS 31
//#define LARGURA_CAPTURA 640
//#define ALTURA_CAPTURA 480
//#define METADE_LARGURA_CAPTURA (LARGURA_CAPTURA/2)
//#define METADE_ALTURA_CAPTURA (ALTURA_CAPTURA/2)
//#define NUM_CORES 11
//#define NUM_CORES_ADV 3
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
  //Camera camera; 
  //ImagemRGB *ImBruta;
  //ImagemRGB ImBruta; Esta na classe camera
  ImagemRGB ImProcessada;
  //as variaveis que controlam o modo de funcionamento da classe
  MODOS_PROCESSAMENTO modo;
  // bool capturando;
  //bool novocamparam;
  //unsigned nPontosNotaveis;
  unsigned nRetas, LarguraCaptura, AlturaCaptura;
  //unsigned nCores;
    
  //as variaveis que armazenam dados vindos da interface
  //Coord2 pixelsNotaveis[N_PONTOS_NOTAVEIS];
  //Coord2 pontosNotaveis[N_PONTOS_NOTAVEIS];
  
  //Coord2 *pixelsNotaveis;
  Coord2 *pontosImagemIniciais;
  //Coord2 *pontosNotaveis;
  //RETA retas[NUM_RETAS];
  PARAMETROS_CAMERA cameraParam;    
  PARAMETROS_CALIBRACAO calibracaoParam;
  RETA *retas;
  //limitesHPG *limHPG;
  char** nomeCor;
  PxRGB *cores;
  PxRGB *coresInversas;
  //int limiarPinf, limiarPsup;
  int corAtual;
  unsigned offset_u,offset_v;
  bool true_color;
    
    
 public:
  CalibratorProcessor();
  CalibratorProcessor(const char* arquivo);
  ~CalibratorProcessor();
  bool readFile(const char* arquivo);
  //void run();	
  inline void setMode(MODOS_PROCESSAMENTO m){modo = m;}
  inline void setColor(int cor){corAtual = cor;}
  int getColor();
  bool fileOpen(const char* text);
  bool fileSave(const char* text);
  bool loadCameraParam(const char* arquivo);
  bool saveCameraParam(const char* arquivo);    
  void setParameters();
  inline int getPinf(){return calibracaoParam.limiarPInf;}
  inline int getPsup(){return calibracaoParam.limiarPSup;}
  inline void setPinf(int Pinf){
    calibracaoParam.limiarPInf = Pinf;
    setMinP(calibracaoParam.limiarPInf/100.0);
  }
  inline void setPsup(int Psup){
    calibracaoParam.limiarPSup = Psup;
    setMaxP(calibracaoParam.limiarPSup/100.0);	
  }
  inline limitesHPG getLimHPG(int i){return calibracaoParam.limHPG[i];}
  inline void setHmin(int i, int valor){calibracaoParam.limHPG[i].H.min = valor;};
  inline void setHmax(int i, int valor){calibracaoParam.limHPG[i].H.max = valor;};
  inline void setPmin(int i, int valor){calibracaoParam.limHPG[i].P.min = valor;};
  inline void setPmax(int i, int valor){calibracaoParam.limHPG[i].P.max = valor;};
  inline void setGmin(int i, int valor){calibracaoParam.limHPG[i].G.min = valor;};
  inline void setGmax(int i, int valor){calibracaoParam.limHPG[i].G.max = valor;};
  inline PARAMETROS_CAMERA getCameraParam(){return cameraParam;}    
  inline void setBrightness(int valor){cameraParam.brightness = valor;};
  inline void setExposure(int valor){cameraParam.exposure = valor;};
  inline void setHue(int valor){cameraParam.hue = valor;};
  inline void setSaturation(int valor){cameraParam.saturation = valor;};
  inline void setGamma(int valor){cameraParam.gamma = valor;};
  inline void setShutter(int valor){cameraParam.shutter = valor;};    
  inline void setGain(int valor){cameraParam.gain = valor;};
  void resetHPG();
  void resetPixelsNotaveis();
  void resetCameraParam();
  bool loadImage(const char* arq);
  void saveImage(const char* arq);  
  bool processImage();
  inline const void *getPNMdata(){return ImProcessada.getPNMData();};
  inline int getPNMsize(){return ImProcessada.getPNMSize();};
  inline int getImWidth(){return ImProcessada.ncol();};
  inline int getImHeight(){return ImProcessada.nlin();};
  inline int getCamWidth(){return ImBruta.ncol();};
  inline int getCamHeight(){return ImBruta.nlin();};
  void getPxValor(int x, int y, int &R, int &G1, int &B, int &H, int &P, int &G2);
  int pontoSelecionado(int x,int y);
  void moverPonto(int ponto,int x,int y);
  inline bool posicaoValida(unsigned int x, unsigned int y){
    return (x < ImProcessada.ncol() && y < ImProcessada.nlin());
  }
  inline void modoCaptura(bool cap){capturando = cap;}
  inline char* getNomeCor(unsigned i){ return nomeCor[i]; }
  inline unsigned getNumCores(){ return calibracaoParam.nCores; };
  inline void setOffsetU(unsigned u){ offset_u = u;}
  inline void setOffsetV(unsigned v){ offset_v = v;}

};

#endif
