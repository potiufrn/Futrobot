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

  void histogram(ImagemGBRG &dest){
    unsigned histoR[255], histoG[255], histoB[255];
    for(unsigned i=0; i<255; i++){
      histoR[i] = 0;
      histoG[i] = 0;
      histoB[i] = 0;
    }

    for(unsigned i = 120; i<360; i++)
      for(unsigned j = 160; j<480; j++){
        if( (i%2 == 0) && (j%2 != 0) )
          histoB[dest.getPixel(i,j)] +=1;
        else if( (i%2 != 0) && (j%2 == 0) )
          histoR[dest.getPixel(i,j)] +=1;
        else
          histoG[dest.getPixel(i,j)] +=1;
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

void segmentacao (ImagemGBRG &dest, uint8_t ref){
  for(unsigned i = 0; i<dest.nlin(); i++)
  for(unsigned j = 0; j<dest.ncol(); j++){
    if(dest(i,j) < ref) dest(i,j) = (uint8_t)0;
    else
    dest(i,j) = (uint8_t)255;
  }
};

int main(){
  TesteCam cam(1);
  ImagemRGB imrgb(0,0), segRGB(0,0);
  char key;

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

      ImagemGBRG Imgbruta(0,0);
      Imgbruta = cam.getImg();

      cam.save("CamSaveTeste.ppm");

      double start_2RGB = relogio();
      cam.toRGB(imrgb);
      double end_2RGB = relogio();

      // imrgb.save("RGBtest.ppm");
      cam.histogram(Imgbruta);
      segmentacao(Imgbruta, 255);
      Imgbruta.save("GBRG_seg.ppm");
      Imgbruta.toImageRGB(segRGB);
      segRGB.save("segmentada.ppm");
      cout << "Captura time : " << end - start << endl;
      cout << "To RGB time  : " << end_2RGB - start_2RGB << endl;

    }
  };


  return 0;
}
