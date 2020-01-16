#include <iostream>
#include <imagem.h>

#include <fstream>
#include <system_tools.h> //clock(), sleep(), sleep_us()
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

  cam.Open(0);

  double td_start,td_end;
  double t_sum = 0;
  double t_min, t_max;
  double dt;

  cam.wait();
  cam.capture();

  cam.wait();
  cam.capture();

  tools::sleep(0.020);
  td_start = tools::clock();
  cam.wait();
  cam.capture();
  dt = tools::clock() - td_start;

  std::cout << "1s:" << dt << '\n';

  // for(uint i = 0; i < 10; i++){
  //
  //   td_start = tools::clock();
  //   cam.wait();
  //   td_end = tools::clock();
  //   cam.capture();
  //
  //   dt = td_end - td_start;
  //   t_sum += dt;
  //
  //   if(i==0){
  //     t_min = dt;
  //     t_max = dt;
  //     continue;
  //   }
  //
  //   t_min = (t_min > dt)?dt:t_min;
  //   t_max = (t_max < dt)?dt:t_max;
  // }

  // std::cout << "Tempo Medio:" << t_sum/10.0 - t_max/10.0<< '\n';
  // std::cout << "Tempo Minimo:" << t_min << '\n';
  // std::cout << "Tempo Maximo:" << t_max << '\n';

  return 0;
}
