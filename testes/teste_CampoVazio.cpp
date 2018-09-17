#include <cmath>
#include <imagem.h>
#include <camera.h>

#include "../program/system.h"


using namespace std;

#define MAX 100.0

class TesteCam:public Camera
{
public:
  ImagemByte media;
  unsigned varMax;
  unsigned *dp;

  TesteCam():Camera(),media(),varMax(0),dp(NULL){}
  TesteCam(unsigned index):Camera(index),varMax(0),media(){this->capturando = true;}
  ~TesteCam(){
    if(dp != NULL)delete[] dp;
  }

  inline void save(const char* arq) { ImagemRGB(ImBruta).save(arq);}
  inline bool capture(){return Camera::captureimage();}
  inline bool wait(){return Camera::waitforimage(); }
  inline bool Open(unsigned index){ Camera::Open(index); this->capturando = true; }

  void campoMedio(){
    unsigned length = 0;
    unsigned *sum2;
    unsigned *sum1;
    unsigned *var;

    wait();
    capture();
    length= ImBruta.getLength();
    sum1  = new unsigned[length];
    sum2  = new unsigned[length];
    var   = new unsigned[length];
    dp    = new unsigned[length];
    media = ImBruta;

    for(unsigned pos = 0; pos < length; pos++){
      sum1[pos] = 0;
      sum2[pos] = 0;
    }

    uint8_t byte;

    for(unsigned i = 0; i < MAX; i++){
      wait();
      capture();
      for(unsigned pos = 0; pos < length; pos++){
        byte = ImBruta.atByte(pos);
        sum1[pos] += byte;
        sum2[pos] += byte*byte;
      }
    }

    for(unsigned pos = 0; pos < length; pos++){
      var[pos] = (sum2[pos] - (1/MAX)*sum1[pos]*sum1[pos])/(MAX-1);
      media.atByte(pos) = sum1[pos]/MAX;
      varMax = (var[pos] > varMax || pos == 0)?var[pos]:varMax;
      dp[pos] = sqrt(var[pos]);
    }

    delete[] sum1;
    delete[] sum2;
    delete[] var;
    ImagemRGB(media).save("media.ppm");
  }
  void pintar(const char* arq){
    unsigned qtdDif = 0;
    unsigned width = ImBruta.getWidth();
    unsigned height = ImBruta.getHeight();
    ImagemByte tmp = ImBruta;
    ImagemRGB  rgb = tmp;

    unsigned sup;
    unsigned inf;

    #define POS(i,j) 2*((i)*width + (j))

    for(unsigned i = 0; i < height; i++)
      for(unsigned j = 0; j < width; j++){
        // sup = media.atByte(i,j) + 2*sqrt(varMax);
        // inf = media.atByte(i,j) - 2*sqrt(varMax);
        sup = media.atByte(i,j) + 2*sqrt(varMax);
        inf = media.atByte(i,j) - 2*sqrt(varMax);
        sup = (sup > 255)?255:sup;
        inf = (inf > 255)?0:inf;

        if(tmp.atByte(i,j) > sup || tmp.atByte(i,j) < inf){
          rgb[i][j] = PxRGB(0,0,255);
          qtdDif++;
        }

      }

    std::cout << "DP: "<<sqrt(varMax) << '\n';
    rgb.save("imgPintada.ppm");
    std::cout << "Qtd Dif: "<<qtdDif << '\n';
  }

};

int main(){
  TesteCam cam;

  unsigned numDevices = 0;
  unsigned index = 0;
  char key;

  do{
    numDevices = cam.listDevices();
    std::cout << "\nInforme um index valido da Camera : " << '\n';
    cin >> index;
  }while(index >= numDevices);
  cin.ignore(1,'\n');

  cam.Open(index);

  std::cout << "Calculando..." << '\n';
  cam.campoMedio();

  cam.save("imgNormal.ppm");

  cam.wait();
  do{
    std::cout <<"Q- Exit or Enter- Capture..." << '\n';
    cin.get(key);
    if(key == 'q' || key == 'Q')break;
    if(key != '\n')continue;
    cam.capture();
    cam.wait();
    cam.pintar("Pintado.ppm");
    cam.save("Captura.ppm");
  }while(key == '\n');


  return 0;
}
