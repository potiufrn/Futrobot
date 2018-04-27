#include "camera.h"

using namespace std;

class TesteCam:public Camera
{
public:
  TesteCam(unsigned index = 0):Camera(index){}

  inline bool capture()const{ return Camera::captureimage(); }
  inline bool wait()const {return Camera::waitforimage(); }
  inline void save(const char* arq){ imgBruta.save(arq); }
}

int main(){
  TesteCam cam(1);

  char key;
  while(true){
    cout << "q - Quit \n ENTER - Capture "<<endl;
    cin.get(key);
    if(key == 'q'){
      cout << "Quit\n";
      return 0;
    }
    if(key == '\n'){
      cam.capture();
      cam.wait();
      cam.save("CamSaveTeste.ppm");
    }
  };

  return 0;
}
