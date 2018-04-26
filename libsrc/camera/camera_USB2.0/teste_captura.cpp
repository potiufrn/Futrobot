#include "camera.h"

using namespace std;

int main()
{
  Camera cam(CAM_FUTROBOT);
  cam.capturando = true;

  char key;
  while(true)
  {
    cout << "q- quit or ENTER- print\n";
    cin.get(key);
    if(key == 'q'){
      cout << "Quit\n";
      return 0;
    }
    if(key == '\n'){
      
      cam.waitforimage();
      cam.captureimage();
      cam.ImBruta.save("CamSaveTeste.ppm");
    }
  }

  return 0;
}
