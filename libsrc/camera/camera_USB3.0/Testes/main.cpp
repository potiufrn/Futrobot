#include <iostream>
#include <camera.h>

using namespace std;

int main()
{
  Camera cam(1);
  unsigned N = 1;
  cam.capturando = true;
  //Por causa do bug na captura, estou fazendo
  //uma captura a mais do que o numero de buffers
  //para poder ter a primeira imagem
  for(unsigned i = 0; i < N; i++)
  {
    cam.waitforimage();
    cam.captureimage();
  }

  cam.imgBruta.save("CamSaveTeste.ppm");
  return 0;
}
