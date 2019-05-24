#include <cmath>
#include <imagem.h>
#include <camera.h>
#include <fstream>
#include <stdlib.h>

#include "../program/system.h"


using namespace std;

#define MAX 100.0
#define PxBranco PxRGB(255,255,255)
#define PxPreto PxRGB(0,0,0)
#define PxVermelho PxRGB(255,0,0)

class TesteCam:public Camera
{
public:
  unsigned qtdUndef;
  unsigned qtdField;
  unsigned qtdObject;

  ImagemBruta media;
  uint8_t *dp;
  float const_isObject;
  float const_isField;

  TesteCam():Camera(),media(),dp(NULL){
    const_isObject = 3.0; //se > x desvios, eh objeto
    const_isField  = 1.0; //se < x desvios, eh campo
    qtdUndef = 0;
    qtdField = 0;
    qtdObject= 0;
  }
  TesteCam(unsigned index):Camera(index),media(){this->capturando = true;}
  ~TesteCam(){
    if(dp != NULL)delete[] dp;
  }

  inline void save(const char* arq) { ImagemRGB(ImBruta).save(arq);}
  inline bool capture(){return Camera::captureimage();}
  inline bool wait(){return Camera::waitforimage(); }
  inline bool Open(unsigned index){ Camera::Open(index); this->capturando = true; }

  #define CONST (ImBruta.getPxFormat() == GBRG)?1:2 //supondo apenas formatos sendo GBRG ou YUV422
  #define POS(i,j) CONST*((i)*ImBruta.getWidth() + (j))

  void campoMedio(){
    unsigned length = 0;
    unsigned width;
    unsigned height;
    uint32_t *sum2;
    uint32_t *sum1;

    wait();
    capture();
    length= ImBruta.getLength();
    width = ImBruta.getWidth();

    sum1  = new uint32_t[length];
    sum2  = new uint32_t[length];
    dp    = new uint8_t[length];
    media = ImBruta;//copia

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

    // #define VAR(pos) (double)( (sum2[pos] - (1/MAX)*sum1[pos]*sum1[pos])/(MAX-1) )
    double var;
    for(unsigned pos = 0; pos < length; pos++){
      media.atByte(pos) = round(sum1[pos]/MAX);
      var = (sum2[pos] - (1.0/MAX)*sum1[pos]*sum1[pos])/(MAX-1.0);
      dp[pos] = round(sqrt(var));
    }

    delete[] sum1;
    delete[] sum2;
    ImagemRGB(media).save("media.ppm");
  }
  void save_deviations(){
    ofstream O("Matrix_of_Deviations");
    for(unsigned i = 0; i < ImBruta.getLength(); i++)
      O << (int)dp[i] << ' ';
    O.close();
  }

  //TODO alterar
  bool isField(unsigned i, unsigned j, uint8_t byte){
    #define IN_RANGE(inf, sup, value) ((value) <= (sup) && (value) >= (inf))?true:false
    uint8_t supLimit,infLimit;
    infLimit = media.getByte(i,j) - const_isField*dp[POS(i,j)];
    supLimit = media.getByte(i,j) + const_isField*dp[POS(i,j)];

    return IN_RANGE(infLimit, supLimit, byte);
  }
  bool isObject(unsigned i, unsigned j, uint8_t byte){
    #define OUT_RANGE(inf, sup, value) ((value) > (sup) || (value) < (inf))?true:false
    uint8_t supLimit,infLimit;
    infLimit = media.getByte(i,j) - const_isObject*dp[i*ImBruta.getWidth() + j];
    supLimit = media.getByte(i,j) + const_isObject*dp[i*ImBruta.getWidth() + j];

    return OUT_RANGE(infLimit, supLimit, byte);
  }

  int isDiff(unsigned i,unsigned j,uint8_t byte){
    unsigned width = ImBruta.getWidth();
    unsigned height = ImBruta.getHeight();
    if(i >= height || j >= width)
      return 0;
    //eh campo
    if(isField(i,j,byte))
      return 0;
    //eh objeto
    if(isObject(i,j,byte))
      return 1;
    //incerteza
    return -1;
  }
  void pintar(const char* arq,bool hard = true){
    qtdUndef = 0;
    qtdField = 0;
    qtdObject= 0;

    #define IS_VALID(i,j) ( (i)<ImBruta.getHeight() && (j)<ImBruta.getWidth())?true:false
    unsigned qtdDiff;
    unsigned width = ImBruta.getWidth();
    unsigned height = ImBruta.getHeight();
    ImagemRGB  rgb = ImBruta;

    int r;
    for(unsigned i = 0; i < height; i++)
      for(unsigned j = 0; j < width; j++){
        qtdDiff = 0;
        r = isDiff(i,j, ImBruta.getByte(i,j));
        if(r == 0){
          qtdField++;
          rgb[i][j] = PxPreto;
        }
        else if( r == 1){
          rgb[i][j] = PxBranco;
          qtdObject++;
        }
        else{
          qtdUndef++;
          if(!hard)
            rgb[i][j] = PxVermelho;
          else{
            if(IS_VALID(i-1,j-1))
              qtdDiff += (isDiff(i-1,j-1,ImBruta.getByte(i-1,j-1)) == 1)?1:0;
            if(IS_VALID(i-1,j+1))
              qtdDiff += (isDiff(i-1,j+1,ImBruta.getByte(i-1,j+1)) == 1)?1:0;
            if(IS_VALID(i+1,j+1))
              qtdDiff += (isDiff(i+1,j+1,ImBruta.getByte(i+1,j+1)) == 1)?1:0;
            if(IS_VALID(i+1,j-1))
              qtdDiff += (isDiff(i+1,j-1,ImBruta.getByte(i+1,j-1)) == 1)?1:0;

            if(qtdDiff > 2)
              rgb[i][j] = PxBranco;
            else
              rgb[i][j] = PxPreto;
          }
        }
      }

    rgb.save(arq);
  }
};

int main(){
  TesteCam cam;

  unsigned numDevices = 0;
  unsigned index = 0;
  unsigned opcao;
  double start = 0,end = 0;

  do{
    numDevices = cam.listDevices();
    std::cout << "\nInforme um index valido da Camera : " << '\n';
    cin >> index;
  }while(index >= numDevices);
  cin.ignore(1,'\n');

  cam.Open(index);
  std::cout << "Calculando..." << '\n';
  cam.campoMedio();
  std::cout << "Calculo finalizado!" << '\n';

  cam.wait();

  while(true){
    system("clear");
    //menu
    std::cout << "Menu" << '\n';
    std::cout << "1- Calcular Imagem Media" << '\n';
    std::cout << "2- Capturar e pintar imagem" << '\n';
    std::cout << "3- Informacoes" << '\n';
    std::cout << "4- Alterar Constantes" << '\n';
    std::cout << "5- Salvar Informacoes" << '\n';
    std::cout << "6- Mudar dispositivo" << '\n';
    std::cout << "7- Salvar Matriz de Desvios" << '\n';
    std::cout << "0- Sair" << '\n';

    std::cin >> opcao;
    std::cin.ignore(1,'\n');

    switch (opcao) {
      case 1:
        std::cout << "Calculando..." << '\n';
        cam.campoMedio();
        std::cout << "Calculo finalizado!" << '\n';

        std::cout << "press enter to continue" << '\n';
        std::cin.get();
      break;
      case 2:
        start = relogio();
        cam.capture();
        cam.wait();
        end = relogio();

        cam.pintar("printed_hard.ppm");
        cam.pintar("printed_soft.ppm",false);

        cam.save("captura.ppm");
      break;
      case 3:
        system("clear");
        std::cout << "Ultimo tempo de captura: "<< end-start << '\n';
        // std::cout << "Desvio Padrao: "<< (int)cam.desvioPadrao << '\n';
        std::cout << "Constante(Objeto): "<<cam.const_isObject << '\n';
        std::cout << "Constante(Campo): "<<cam.const_isField << '\n';
        std::cout << "Quantidade: Undef Field Object: "\
                  <<(double)(100.0*cam.qtdUndef/(640.0*480.0))\
                  << '\t' << (double)(100.0*cam.qtdField/(640.0*480.0))\
                  << '\t' << (double)(100.0*cam.qtdObject/(640.0*480.0)) << '\n';
        std::cout << "press enter to continue" << '\n';
        std::cin.get();
      break;
      case 4:
        system("clear");
        std::cout << "Nova constante(Objeto) e  constante(Campo)" << '\n';
        std::cin >> cam.const_isObject >> cam.const_isField;
        std::cin.ignore(1,'\n');
      break;
      case 5:
        std::cerr << "falta fazer..." << '\n';
        std::cout << "press enter to continue" << '\n';
        std::cin.get();
      break;
      case 6:
        system("clear");
        do{
          numDevices = cam.listDevices();
          std::cout << "\nInforme um index valido da Camera : " << '\n';
          cin >> index;
        }while(index >= numDevices);
        cin.ignore(1,'\n');
        cam.Open(index);
      break;
      case 7:
        system("clear");
        cam.save_deviations();
        std::cout << "Operacao concluida, arquivo gerado na pasta local" << '\n';

        std::cout << "press enter to continue" << '\n';
        std::cin.get();
      break;
      case 0:
        return 0;
      default:
        std::cerr << "Opcao Invalida!" << '\n';
      break;
    }

  }

  return 0;
}
