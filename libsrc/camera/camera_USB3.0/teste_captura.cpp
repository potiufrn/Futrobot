#include <imagem.h>

#include "camera.h"
#include "../../../program/system.h"


using namespace std;

class TesteCam:public Camera
{
  Imagem ImBruta;
public:
  TesteCam(unsigned index = 0):Camera(index),ImBruta(){this->capturando = true;}
  inline bool capture(){
    bool retr = Camera::captureimage();
    uint8_t pxFormat = Camera::getPxFormat();
    unsigned length = Camera::getDataSize();
    ImBruta.loadFromData(Camera::getDataImage(),length, pxFormat,Camera::getWidth(),Camera::getHeight());
    return retr;
  }
  inline bool wait(){return Camera::waitforimage(); }
  inline void save(const char* arq){ ImBruta.save(arq); }
  // inline void toRGB(ImagemRGB &dest){ Camera::toRGB(dest); }
};

int main(){
  unsigned index = 0;
  char key;

  std::cout << "Informe o index da Camera : " << '\n';
  cin >> index;
  TesteCam cam(index);
  cin.ignore(1,'\n');

  // int gain = 77;
  // int ExpAbs = 292;
  //
  // struct controler qGain;
  // struct controler qExpAbs;
  //
  // if(!cam.queryGain(qGain) ) std::cout << "Camera nao possui o controles" << '\n';
  // else std::cout << "Gain min - max " <<qGain.min << ' ' <<qGain.max << '\n';
  // if ( !cam.setGain(gain) ) std::cout << "falha na alteracao dos controles" << '\n';
  //
  // if(!cam.queryExposureAbs(qExpAbs) ) std::cout << "Camera nao possui o controles" << '\n';
  // else  std::cout << "Exposure(Absolute) min  - max " <<qExpAbs.min << ' ' <<qExpAbs.max << '\n';
  // if ( !cam.setExposureAbs( ExpAbs ) ) std::cout << "falha na alteracao do controle" << '\n';

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

      double start_save = relogio();
      cam.save("CamSaveTeste.ppm");
      double end_save = relogio();

      cout << "Captura time : " << end - start << endl;
      cout << "Save time  : " << end_save - start_save << endl;
    }

  };
  return 0;
}
