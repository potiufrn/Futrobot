#include <imagem.h>

#include "camera.h"
#include "../../../program/system.h"


using namespace std;

class TesteCam:public Camera
{
public:
  TesteCam():Camera(){}
  TesteCam(unsigned index):Camera(index){this->capturando = true;}
  inline void save(const char* arq) { ImagemRGB(ImBruta).save(arq);}
  inline bool capture(){return Camera::captureimage();}
  inline bool wait(){return Camera::waitforimage(); }
  inline bool Open(unsigned index){ Camera::Open(index); this->capturando = true; }
  inline PxRGB  atRGB(unsigned lin, unsigned col){ return ImBruta.atRGB(lin,col); }

  void write_byte(const char* arq){ ImBruta.write(arq); }
  void copy(ImagemBruta &I){ I = ImBruta; }
};

int main(){
  ImagemBruta tmp;

  tmp.read(".campoVazio");
  ImagemRGB(tmp).save("campoVazio.ppm");

  return 0;
}
