#include "camera.h"

using namespace std;

class TesteCam:public Camera
{
public:
  TesteCam(CAMERA_T CAM):Camera(CAM){ this->capturando = true; }

  inline bool capture(){ return Camera::captureimage(); }
  inline bool wait(){return Camera::waitforimage(); }
  inline void save(const char* arq){ ImBruta.save(arq); }
};

int main()
{
  TesteCam cam(CAM_FUTROBOT);

  char key;
  while(true)
  {
    cout << "q - Quit or ENTER - Capture\n";
    cin.get(key);
    if(key == 'q'){
      cout << "Quit\n";
      return 0;
    }
    if(key == '\n'){
      cam.wait();
      cam.capture();
      cam.save("CamSaveTeste.ppm");
    }
  };

  return 0;
}
