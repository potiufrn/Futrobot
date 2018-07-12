#include <iostream>
#include <imagem.h>

#include <fstream>
#include "../camera.h"

using namespace std;

class TesteCam:public Camera
{
public:
  TesteCam():Camera(){this->capturando = true;}
  void save(const char* arq){ ImagemRGB(ImBruta).save(arq); }
  inline bool capture(){ return Camera::captureimage(); }
  inline bool wait(){return Camera::waitforimage(); }
};

int main(){
  TesteCam cam;
  unsigned numDevices = 0;
  unsigned index = 0;
  char key;

  ifstream I;
  ofstream O;

  do {
    O.open("out.cal");
    I.open("test.cal");
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

    if(cam.read(I))std::cerr << "ERRO: in Read" << '\n';
    cam.write(O);
    O.close();
    I.close();

    cam.save("ok.ppm");

    cout << "q - Quit \n ENTER - escolher"<<endl;
    cin.get(key);

  } while(key != 'q');
  I.close();
}
