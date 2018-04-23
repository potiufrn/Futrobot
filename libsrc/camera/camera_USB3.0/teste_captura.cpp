#include <camera.h>

using namespace std;

int main(){
  Camera cam(1);
  unsigned N = 2;
  cam.capturando = true;

  for(unsigned i = 0; i < N; i++)
  {
    cam.waitforimage();
    cam.captureimage();
  }

  cam.imgBruta.save("CamSaveTeste.ppm");
  return 0;
}
