#include <time.h>
#include "camera.h"
#include "../../../program/system.h"

using namespace std;

class TesteCam:public Camera
{
public:
  TesteCam():Camera(){ this->capturando = true; }

  inline bool capture(){ return Camera::captureimage(); }
  inline bool wait(){return Camera::waitforimage(); }
  inline void save(const char* arq){ ImBruta.save(arq); }
  PxRGB getPixel(unsigned lin,unsigned col){ return ImBruta[lin][col];}
};

int main()
{
  TesteCam cam;
  char key;

  double start;
  double end;
  double max;
  unsigned indice;

  start = relogio();
  cam.wait();
  cam.capture();
  end = relogio();
  std::cout << "Primeira cap " << end - start << '\n';

  for(unsigned i = 0; i < 100; i++){
    start = relogio();
    cam.wait();
    cam.capture();
    end = relogio();

    double delta = end - start;
    if(i == 0)max = delta;
    if(delta > max){
      max = delta;
      indice = i;
    }
    // max = (delta > max)?delta:max;
  }
  std::cout << "Maior tempo de captura "<< max << "  cap_num: "<<indice<< '\n';

  // while(true){
  //   cout << "q - Quit or ENTER - Capture\n";
  //   cin.get(key);
  //   if(key == 'q'){
  //     cout << "Quit\n";
  //     break;
  //   }
  //   if(key == '\n'){
  //     double on = relogio();
  //     cam.wait();
  //     cam.capture();
  //     double end = relogio();
  //     cout << "Tempo de captura " << (end - on) << endl;
  //
  //     cam.save("CamSaveTeste.ppm");
  //   }
  // };
  return 0;
}
