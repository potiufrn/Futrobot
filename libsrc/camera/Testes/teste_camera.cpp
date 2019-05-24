#include <iostream>
#include <imagem.h>

#include <fstream>
#include "../camera.h"

using namespace std;

class TesteCam:public Camera
{
public:
  TesteCam():Camera(){this->capturando = true;}
  void save(const char* arq){ ImagemRGB(ImBruta).save(arq); }

  inline bool capture(){ return Camera::captureimage(); }
  inline bool wait(){return Camera::waitforimage(); }
};

int main(){
  TesteCam cam;
  unsigned numDevices = 0;
  unsigned index = 0;
  char key;

  ifstream I;
  ofstream O;
  do{
    numDevices = cam.listDevices();
    std::cout << "\nInforme um index valido da Camera : " << '\n';
    cin >> index;
  }while(index >= numDevices);

  cam.Open(index);
  cin.ignore(1,'\n');

  cam.wait();

  do{
    std::cout << "Q-Exit or Enter-Capture" << '\n';
    cin.get(key);
    if(key == 'q' || key == 'Q')return 0;
    cam.capture();
    cam.wait();
  }while(true);

  return 0;
}
