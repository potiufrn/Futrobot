#include <iostream>
#include <imagem.h>

#include <fstream>
#include "../camera.h"
#include "system.h"

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

  // ifstream I;
  // ofstream O;
  // do{
  //   numDevices = cam.listDevices();
  //   std::cout << "\nInforme um index valido da Camera : " << '\n';
  //   cin >> index;
  // }while(index >= numDevices);

  cam.Open(0);
  // cin.ignore(1,'\n');
  // cam.wait();

  double td_start,td_end;
  while(true){
    // std::cout << "Q-Exit or Enter-Capture" << '\n';
    // if(key == 'q' || key == 'Q')return 0;
    td_start = relogio();
    cam.wait();
    cam.capture();
    td_end = relogio();
    std::cout << "Delta_T:" << td_end - td_start << '\n';
    // cin.get(key);
  };

  return 0;
}
