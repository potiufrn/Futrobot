#include <imagem.h>

#include "camera.h"
#include "../../../program/system.h"


using namespace std;

class TesteCam:public Camera
{
public:
  TesteCam():Camera(){}
  TesteCam(unsigned index):Camera(index){this->capturando = true;}
  inline void save(const char* arq) { ImagemRGB(ImBruta).save(arq);}
  inline bool capture(){return Camera::captureimage();}
  inline bool wait(){return Camera::waitforimage(); }
  inline bool Open(unsigned index){ Camera::Open(index); this->capturando = true; }
  inline PxRGB  atRGB(unsigned lin, unsigned col){ return ImBruta.atRGB(lin,col); }
};

int main(){
  TesteCam cam;
  unsigned numDevices = 0;
  unsigned index = 0;
  char key;

  do {
    do{
      numDevices = cam.listDevices();
      std::cout << "\nInforme um index valido da Camera : " << '\n';
      cin >> index;
    }while(index >= numDevices);
    cam.Open(index);
    cin.ignore(1,'\n');

    cam.wait();
    cam.capture();
    cam.wait();

    cam.capture();
    cam.save("ok.ppm");

    cout << "q - Quit \n ENTER - escolher"<<endl;
    cin.get(key);
  } while(key != 'q');


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

      // unsigned lin,col;
      // std::cout << "lin e col ? " << '\n';
      // cin >> lin >> col;
      // cout << cam.atRGB(lin,col) << endl;
      // cin.ignore();
      cout << "Save time  : " << end_save - start_save << endl;
    }
  };
  return 0;
}
