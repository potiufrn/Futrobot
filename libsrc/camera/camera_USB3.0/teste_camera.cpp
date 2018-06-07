#include <iostream>
#include <imagem.h>

#include "camera.h"


class TesteCam:public Camera
{
public:
  TesteCam(unsigned index = 0):Camera(index){this->capturando = true;}
  inline bool capture(){ return Camera::captureimage(); }
  inline bool wait(){return Camera::waitforimage(); }
  inline uint8_t getFormat()const{return Camera::getDataImage()[0];}
  inline bool read(const char* arq){return Camera::read(arq); }
  inline bool write(const char* arq){return Camera::write(arq);}
};

int main(){
  TesteCam cam;
  cam.wait();
  cam.capture();
  uint8_t tmp = cam.getFormat();

  std::cout << "pxFormat "<< ((tmp == 0)?"GBRG":"YUYV") << '\n';
  cam.write("camParam.cam");
  cam.read("camParam.cam");
}
