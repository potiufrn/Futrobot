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

enum REG_COLOR {
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

/* struct WIDEREGION{ */
/*   Coord3 pose;  */
/*   REG_COLOR colorID; */
/*   int nPixel; */
/*   bool symetric; */
/* }; */

class Acquisition :
  public virtual FutData
#ifndef _SO_SIMULADO_
  , protected Camera
#endif
{
private:
#ifndef _SO_SIMULADO_
  // ImagemRGB image;
  Homografia Homography;
  DistRadial RDistortion;
  unsigned int MinU, MaxU, MinV, MaxV;
  PARAMETROS_CALIBRACAO calibracaoParam;

#endif
  IDQUADRO id_ant;
  bool saveNextImage;
  //FUNCTIONS
#ifndef _SO_SIMULADO_
  REGION seedFill( REG_COLOR colorID, unsigned int u, unsigned int v);
  bool processGameState();
#endif
  bool readGameState();
  bool calculaMinhaPose(REGION regTeam, double angBusca,
			double angCorrecao,int &index, POS_ROBO &teamPose);
  bool calculaMinhaPoseAproximada(REGION regTeam,double angCorrecao,
				  int &index, POS_ROBO &teamPose);
  bool calculaPoseAdv(REGION regTeam, int &index,POS_ROBO &teamPose,
			     double corrX, double corrY, double corrTheta);

public:
  Acquisition( TEAM team, SIDE side, GAME_MODE mode);
  ~Acquisition();
  bool configAcquisition(const char *str);
  bool acquisitionWait();
  bool acquisitionCapture();
  bool acquisition();
  void save_image();
  //  const PxRGB *exptimg () {return (ImBruta.getRawData());};
};

#endif
