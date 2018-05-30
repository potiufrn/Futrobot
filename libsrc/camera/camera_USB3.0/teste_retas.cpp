#include "camera.h"
#include "../../../program/system.h"
#include <fstream>

using namespace std;

class TesteCam:public Camera
{
public:
  TesteCam(unsigned index = 0):Camera(index){this->capturando = true;}

  inline bool capture(){ return Camera::captureimage(); }
  inline bool wait(){return Camera::waitforimage(); }
  inline void save(const char* arq){ ImBruta.save(arq); }
  inline void toRGB(ImagemRGB &dest){ Camera::toRGB(dest); }
  inline ImagemGBRG getImg()const{return ImBruta;}

  void histogram(ImagemRGB &dest){
    unsigned histoR[255], histoG[255], histoB[255];
    for(unsigned i=0; i<255; i++){
      histoR[i] = 0;
      histoG[i] = 0;
      histoB[i] = 0;
    }

    for(unsigned i = 120; i< 360; i++)
      for(unsigned j = 160; j < 480; j++){
        if( (i%2 == 0) && (j%2 != 0) )
          histoB[dest[i][j].r] +=1;
        else if( (i%2 != 0) && (j%2 == 0) )
          histoR[dest[i][j].r] +=1;
        else
          histoG[dest[i][j].r] +=1;
      }
    ofstream histograma;
    histograma.open("grafico.txt");
    for(unsigned i=0; i<255; i++){
      histograma << histoR[i]<< " ";
    }
    histograma <<"\n";
    for(unsigned i=0; i<255; i++){
      histograma << histoG[i] << " ";
    }
    histograma <<"\n";
    for(unsigned i=0; i<255; i++){
      histograma << histoB[i] << " ";
    }
    histograma <<"\n";
    histograma.close();
  }
};

void segmentacao (ImagemRGB &dest, uint8_t ref){
  for(unsigned i = 0; i<dest.nlin(); i++)
  for(unsigned j = 0; j<dest.ncol(); j++){
    if(dest[i][j].r <= ref){
      dest[i][j].r = (uint8_t)0;
      dest[i][j].g = (uint8_t)0;
      dest[i][j].b = (uint8_t)0;
    }
    else{
    dest[i][j].r = (uint8_t)255;
    dest[i][j].g = (uint8_t)255;
    dest[i][j].b = (uint8_t)255;
    }
  }
};

void detlinhas(ImagemRGB &dest){
  unsigned coef[240][320];
  for(unsigned i = 0; i<240; i++)
  for(unsigned j = 0; j<320; j++){
    coef[i][j] = 0;
  }
  for(unsigned i = 120; i<360; i++)
  for(unsigned j = 160; j<480; j++){
    if(dest[i][j].r >= (uint8_t)80)
      coef[i-120][j-160] += 1;
  }
  ofstream f;
  f.open("coef.txt");
  for(unsigned i = 0; i<240; i++){
    for(unsigned j = 0; j<320; j++)
      f << coef[i][j] << " ";
    f<<"\n";
  }
  f.close();
};


int main(){
  TesteCam cam(1);
  ImagemRGB imrgb(0,0), segRGB(0,0);
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

      ImagemGBRG Imgbruta(0,0);
      Imgbruta = cam.getImg();

      //cam.save("CamSaveTeste.ppm");

      double start_2RGB = relogio();
      cam.toRGB(imrgb);
      double end_2RGB = relogio();
      //Imgbruta.toGray();
      imrgb.save("RGBtest.ppm");
      imrgb.toGray();
      imrgb.save("RGBgray.ppm");
      cam.histogram(imrgb);
      segmentacao(imrgb, 80);
      imrgb.save("RGB_seg.ppm");
      detlinhas(imrgb);
      cout << "Captura time : " << end - start << endl;
      cout << "To RGB time  : " << end_2RGB - start_2RGB << endl;

    }
  };


  return 0;
}
