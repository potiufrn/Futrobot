#include <math.h> // sqrt
#include <imagem.h>
#include "camera.h"
#include "../../../program/system.h"


class TesteCam:public Camera
{
  Imagem ImBruta;
  Imagem imgMedia;

  //Numero de amostras
  unsigned N;
  uint16_t *variancia;
  double varianciaMax;
public:

  TesteCam(unsigned index = 0):Camera(index),ImBruta(),imgMedia(),N(0){this->capturando = true;}
  inline bool capture(){
    bool retr = Camera::captureimage();
    uint8_t pxFormat = Camera::getPxFormat();
    unsigned length = Camera::getDataSize();
    ImBruta.loadFromData(Camera::getDataImage(),length, pxFormat,Camera::getWidth(),Camera::getHeight());
    return retr;
  }
  inline bool wait(){return Camera::waitforimage(); }
  inline void save(const char* arq){ ImBruta.save(arq); }

  inline void  saveMed(const char* arq){ imgMedia.save(arq); }
  inline float getVaciancia()const{ return varianciaMax; }
  inline float getDesvioPadrao()const{ return sqrt(varianciaMax); }

  inline void toRGB(ImagemRGB &dest){ Camera::toRGB(dest); }
};


int main(){
  TesteCam cam(0);
  // ImagemRGB imrgb(0,0);
  char key;

  double start = relogio();
  for(unsigned n = 0; n < 100; n++){
    cam.wait();
    cam.capture();
    cam.push();
  }
  double end = relogio();

  cam.save("Ultima_captura.ppm");
  cam.saveMed("CampoMedio.ppm");
  std::cout << "Tempo total Gasto : "<<end - start << '\n';

  std::cout << "Variancia Maxima "<< cam.getVaciancia() << '\n';
  std::cout << "Desvio Padrao Maximo "<<cam.getDesvioPadrao() << '\n';


  return 0;
}
