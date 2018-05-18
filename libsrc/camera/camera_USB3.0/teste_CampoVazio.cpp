// #include <time.h>
#include <math.h> // sqrt
#include "camera.h"
#include "../../../program/system.h"

// using namespace std;



class ImgAmostra{
private:
  ImagemGBRG img;
  ImgAmostra *next;

  ImgAmostra(const ImagemGBRG &I):img(I){
    next = NULL;
  }
  friend class ListImgGBRG;
};
//Ideia:
//Utilizar uma esturtura de dados do tipo FIFO
//Para armazenar imagens e realizar o calculo da Media
//Variancia e Desvio Padrao a medida que novas imagens forem adicionadas a esturtura
//Estrutura escolhida foi uma lista duplamente encadeada
class ListImgGBRG{
  private:
  ImgAmostra *first;
  ImgAmostra *last;

  ImagemGBRG imgMedia;
  float **fMedia;
  float **fVariancia;

  float varianciaMax;

  unsigned length;

  void calcVariancia(){

    if(length == 0)return;

    ImgAmostra *iter = first;


    for(unsigned l = 0; l < imgMedia.nlin(); l++)
      for(unsigned c = 0; c < imgMedia.ncol(); c++)
        fVariancia[l][c] = 0;

    varianciaMax = 0;

    while(iter != NULL){

      for(unsigned l = 0; l < imgMedia.nlin(); l++)
      for(unsigned c = 0; c < imgMedia.ncol(); c++){
        float tmp  = (iter->img(l,c) - fMedia[l][c]);
        fVariancia[l][c] = tmp*tmp;
      }

      iter = iter->next;
    }

    for(unsigned l = 0; l < imgMedia.nlin(); l++)
      for(unsigned c = 0; c < imgMedia.ncol(); c++){
        fVariancia[l][c] = fVariancia[l][c]/length;

        varianciaMax = (fVariancia[l][c] > varianciaMax)?fVariancia[l][c]:varianciaMax;
    }

  }
 public:
  ListImgGBRG():first(NULL),last(NULL),
                length(0),imgMedia(0,0),fMedia(NULL),
                fVariancia(NULL),varianciaMax(0){}
  ~ListImgGBRG(){
    if(fMedia != NULL){
      for(unsigned l = 0; l < imgMedia.nlin();l++)
        delete[] fMedia[l];
      delete[] fMedia;
    }
    if(fVariancia != NULL){
      for(unsigned l = 0; l < imgMedia.nlin();l++)
        delete[] fVariancia[l];
      delete[] fVariancia;
    }
    delete first;
    delete last;
  }
  //Acesso ultimo elemento da fila
  // ImgAmostra back()const{ return *last; }

  //add no final da fila e recalcula a media
  void push(const ImagemGBRG &I){
    //primeiro push
    if(first == NULL){
      first = new ImgAmostra(I);
      last = first;
      imgMedia = I;

      fMedia = new float*[I.nlin()];
      for(unsigned l = 0; l < I.nlin();l++)
        fMedia[l] = new float[I.ncol()];

      for(unsigned l = 0; l < imgMedia.nlin(); l++)
        for(unsigned c = 0; c < imgMedia.ncol(); c++)
          fMedia[l][c] = I(l,c);

      fVariancia = new float*[I.nlin()];
      for(unsigned l = 0; l < I.nlin();l++)
        fVariancia[l] = new float[I.ncol()];

      for(unsigned l = 0; l < imgMedia.nlin(); l++)
        for(unsigned c = 0; c < imgMedia.ncol(); c++)
          fVariancia[l][c] = I(l,c);
    }
    else{
      last->next = new ImgAmostra(I);
      last = last->next;
      //Recalculo da Imagem Media
      for(unsigned l = 0; l < imgMedia.nlin(); l++)
        for(unsigned c = 0; c < imgMedia.ncol(); c++)
        {
          fMedia[l][c] = (I(l,c) + fMedia[l][c])/2.0;
          imgMedia(l,c) = fMedia[l][c];
        }
    }
    length++;
    calcVariancia();


  }

  ImgAmostra begin()const{ return *first;}

  inline bool isEmpty() { return (length == 0)?true:false; }
  inline unsigned size()const{ return length; }

  const ImagemGBRG& getImgMedia()const{ return imgMedia; }
  const float** getImgVariancia()const{ return (const float**)fVariancia; }

  //Retorna o maior valor da Imagem de Desvios
  inline float getDesvioPadrao()const { return std::sqrt(varianciaMax); }
  //Retorna o maior valor da Imagem de Variancia
  float getVariancia()const { return varianciaMax; }

};

class TesteCam:public Camera{
public:
  ListImgGBRG list;

  TesteCam(unsigned index = 0):Camera(index),list(){this->capturando = true;}

  inline bool capture(){ return Camera::captureimage(); }
  inline bool wait(){return Camera::waitforimage(); }
  inline void save(const char* arq){ ImBruta.save(arq); }
  inline void toRGB(ImagemRGB &dest){ Camera::toRGB(dest); }

  void push(){ list.push(ImBruta); };
  inline void saveMed(const char* arq){ list.getImgMedia().save(arq); }
  inline float getVaciancia()const{ return list.getVariancia(); }
  inline float getDesvioPadrao()const{ return list.getDesvioPadrao(); }

  void copyImg(ImagemGBRG &I){ I = ImBruta; }
};


int main(){
  TesteCam cam(2);
  ImagemRGB imrgb(0,0);
  char key;

  int gain = 100;
  int ExpAbs = 292;

  struct controler ctrl;
  if(!cam.queryMinBuffer(ctrl))
    std::cout << "Comando nao reconhecido" << '\n';
  else{
    std::cout << "Min buffer"<<ctrl.min << '\n';
    std::cout << "Max Buffer"<<ctrl.max << '\n';
  }

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

  // double start = relogio();
  // for(unsigned n = 0; n < 100; n++){
  //   cam.wait();
  //   cam.capture();
  //   cam.push();
  // }
  // double end = relogio();
  // cam.save("Ultima_captura.ppm");
  // cam.saveMed("CampoMedio.ppm");
  // std::cout << "Tempo total Gasto : "<<end - start << '\n';

  while(false){
    std::cout << "q - Quit \n ENTER - Capture "<< std::endl;
    std::cout << "v - visualizar resultados" << '\n';

    std::cin.get(key);
    if(key == 'q'){
      std::cout << "Quit\n";
      break;
    }
    if(key == '\n'){
      double start = relogio();
      cam.wait();
      cam.capture();
      double end = relogio();

      cam.save("CamSaveTeste.ppm");

      double start_2RGB = relogio();
      cam.toRGB(imrgb);
      double end_2RGB = relogio();

      imrgb.save("RGBtest.ppm");

      cam.push();
      cam.saveMed("ImagemMedia.ppm");


      std::cout << "Captura time : " << end - start << std::endl;
      std::cout << "To RGB time  : " << end_2RGB - start_2RGB << std::endl;
    }
    if(key == 'v'){
      std::cout << "Variancia Maxima "<< cam.getVaciancia() << '\n';
      std::cout << "Desvio Padrao Maximo "<<cam.getDesvioPadrao() << '\n';
    }

  };
  std::cout << "Variancia Maxima "<< cam.getVaciancia() << '\n';
  std::cout << "Desvio Padrao Maximo "<<cam.getDesvioPadrao() << '\n';
  return 0;
}
