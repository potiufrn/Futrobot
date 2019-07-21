#ifndef _ACQUISITION_H_
#define _ACQUISITION_H_

#include "futdata.h"
#include "../dados_calibracao.h"
#include "../parameters.h"
#include <sys/time.h>

#ifndef _SO_SIMULADO_
#include <camera.h>
#include <imagem.h>
#include "../dados_calibracao.h"
#endif


enum  REG_COLOR {
  REG_COLOR_ORANGE =0,
  REG_COLOR_BLUE   =1,
  REG_COLOR_YELLOW =2,
  REG_COLOR_CIAN   =3,
  REG_COLOR_PINK   =4,
  REG_COLOR_GREEN  =5,
  REG_COLOR_BLACK  =6,
  REG_COLOR_WHITE  =7,
  REG_COLOR_OTHER1 =8,
  REG_COLOR_OTHER2 =9,
  REG_COLOR_OTHER3 =10,
  REG_COLOR_UNDEFINED = -1
};


struct REGION{
  Coord2 center;
  double orientation;
  bool symetric;
  REG_COLOR colorID;
  int nPixel;
};

class Acquisition :
  public virtual FutData
#ifndef _SO_SIMULADO_
  , protected Camera
#endif
{
private:
#ifndef _SO_SIMULADO_
  Homografia Homography;
  DistRadial RDistortion;
  unsigned int MinU, MaxU, MinV, MaxV;
  PARAMETROS_CALIBRACAO calibracaoParam;
#endif
  IDQUADRO id_ant;
  bool saveNextImage;
  //FUNCTIONS
#ifndef _SO_SIMULADO_

/**
 * Verifica se o pixel na posicao (u,v) pertence ao intervalo definido para a cor ColorID
 * @param  colorID Testa se pixel pode ser considerado dessa cor.
 * @param  u       indice da coluna da imagem.
 * @param  v       indice da linha da imagem.
 * @return         true caso seja da cor ColorID, false caso contrario.
 */
  bool canBePainted(REG_COLOR colorID,unsigned u, unsigned v);
  /**
   * Algoritmo otimizado de SEEDFILL/FLOODFILL
   * dado uma posicao na imagem e uma cor,
   * o algoritmo encontra todos os pixels associados a esse pixel inicial
   * semente e sobrescreve os valores desses pixels
   * pela cor especificada.
   * @param  colorID Cor que sera aplicada na regiao.
   * @param  u       indice da coluna na imagem.
   * @param  v       indice da linha na imagem.
   * @return         Retorna uma estrutura REGION.
   */
  REGION seedFill( REG_COLOR colorID, unsigned int u, unsigned int v);
  /**
   * Metodo que realiza os algoritmos de visao computacional,
   * identica a regiao, posicao central e theta, de cada robo e
   * localizacao da bola.
   * @return retorna true em caso de erro.
   */
  bool processGameState();
#endif
  /*************************************************************************************
   * Interpeta o estado atual do jogo apartir da leitura via socket                    *
   * @return true em caso de erro
   *************************************************************************************/
  bool readGameState();
  /**
   * [calculaMinhaPose description]
   * @param  regTeam     [description]
   * @param  angBusca    [description]
   * @param  angCorrecao [description]
   * @param  index       [description]
   * @param  teamPose    [description]
   * @return             [description]
   */
  bool calculaMinhaPose(REGION regTeam, double angBusca,
			double angCorrecao,int &index, POS_ROBO &teamPose);
  /**
   * [calculaMinhaPoseAproximada description]
   * @param  regTeam     [description]
   * @param  angCorrecao [description]
   * @param  index       [description]
   * @param  teamPose    [description]
   * @return             [description]
   */
  bool calculaMinhaPoseAproximada(REGION regTeam,double angCorrecao,
				  int &index, POS_ROBO &teamPose);
  /**
   * [calculaPoseAdv description]
   * @param  regTeam   [description]
   * @param  index     [description]
   * @param  teamPose  [description]
   * @param  corrX     [description]
   * @param  corrY     [description]
   * @param  corrTheta [description]
   * @return           [description]
   */
  bool calculaPoseAdv(REGION regTeam, int &index,POS_ROBO &teamPose,
			     double corrX, double corrY, double corrTheta);

public:
  /**
   * [Acquisition description]
   * @param team [description]
   * @param side [description]
   * @param mode [description]
   */
  Acquisition( TEAM team, SIDE side, GAME_MODE mode);
  ~Acquisition();
  /**
   * [configAcquisition description]
   * @param  str [description]
   * @return     [description]
   */
  bool configAcquisition(const char *str);
  /**
   * [acquisitionWait description]
   * @return [description]
   */
  bool acquisitionWait();
  /**
   * [acquisitionCapture description]
   * @return [description]
   */
  bool acquisitionCapture();
  /**
   * [acquisition description]
   * @return [description]
   */
  bool acquisition();
  /**
   * [save_image description]
   */
  void save_image();
};

#endif
