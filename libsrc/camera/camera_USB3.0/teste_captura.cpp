#include <time.h>

#include "camera.h"
#include "../../../program/system.h"

using namespace std;

class TesteCam:public Camera
{
public:
  TesteCam(unsigned index = 0):Camera(index){this->capturando = true;}

  inline bool capture(){ return Camera::captureimage(); }
  inline bool wait(){return Camera::waitforimage(); }
  inline void save(const char* arq){ imgBruta.save(arq); }
  inline void toirgb(ImagemRGB &dest){imgBruta.toImgRGB(dest); }

  PxRGB getPixel(unsigned lin,unsigned col) { return imgBruta.getRGB(lin,col); } ;
  PxRGB getGBRG(unsigned lin, unsigned col) {
    uint8_t u_byte = imgBruta.getPixel(lin,col);
    if( (lin%2 == 0) && (col%2 != 0) )//azul
      return PxRGB(0,0,u_byte);
    else if( (lin%2 != 0) && (col%2 == 0) )//vermelho
      return PxRGB(u_byte,0,0);
    else//verde
      return PxRGB(0,u_byte,0);
  }
};

int main(){
  TesteCam cam(3);
  ImagemRGB imrgb(0,0);
  char key;

  while(true){
    cout << "q - Quit \n ENTER - Capture "<<endl;
    cin.get(key);
    if(key == 'q'){
      cout << "Quit\n";
      break;
    }
    if(key == '\n'){
      double start = relogio();
      cam.wait();
      cam.capture();
      double end = relogio();
      cout << "Captura time : " << end - start << endl;
      cam.save("CamSaveTeste.ppm");
      cam.toirgb(imrgb);
      imrgb.save("RGBtest.ppm");
    }

    // unsigned i = 0;
    // unsigned j = 0;
    // cout << "Original " << cam.getGBRG(i,j) << endl;
    // cout << "Pixel convertido " << cam.getPixel(i,j) << endl;
    //
    // cout << cam.getGBRG(i,j) << ' ' << cam.getGBRG(i,j+1) << '\n'
    //       << cam.getGBRG(i+1,j) << ' ' << cam.getGBRG(i+1,j+1) << endl;
  };

  return 0;
}
