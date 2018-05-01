#include "camera.h"
#include <time.h>
using namespace std;

class TesteCam:public Camera
{
public:
  TesteCam(unsigned index = 0):Camera(index){}

  inline bool capture(){ return Camera::captureimage(); }
  inline bool wait(){return Camera::waitforimage(); }
  inline void save(const char* arq){ imgBruta.save(arq); }
  inline void toirgb(ImagemRGB &dest){imgBruta.toImgRGB(dest); }
};

int main(){
  TesteCam cam(1);
  clock_t tick[2];

  ImagemRGB imrgb(0,0);
  char key;
  while(true){
    cout << "q - Quit \n ENTER - Capture "<<endl;
    cin.get(key);
    if(key == 'q'){
      cout << "Quit\n";
      return 0;
    }
    if(key == '\n'){
      tick[0] = clock();
      cam.capture();
      cam.wait();
      tick[1] = clock();
      cout << (tick[1] - tick[0])*10000/CLOCKS_PER_SEC << "*10ms\n";

      cam.save("CamSaveTeste.ppm");
      cam.toirgb(imrgb);
      imrgb.save("RGBtest.ppm");
    }
    
  };

  return 0;
}
