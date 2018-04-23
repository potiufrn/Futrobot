#include <linux/videodev2.h> //struct
#include <fcntl.h>  // O_RDWR
#include <unistd.h> //close() , select
#include <sys/ioctl.h> //ioctl
#include <sys/mman.h> // mmap
//#include <sys/select.h> //select
#include <stdint.h> //unt8_t
#include <string.h>//memset
#include <fstream>

#include <sys/time.h> //timevalue
#include <sys/select.h> // select
#include <sys/types.h>

#include <imagem.h>

using namespace std;

#define CLEAR(x) memset (&(x), 0, sizeof (x))

//Default
/*
#define WIDTH 752
#define HEIGHT 480
#define fps 30
*/

#define NUM_BUFFERS 1

struct PARAMETROS_CAMERA {
  int brightness,hue,saturation,contrast, whiteness,sharpness, exposure,gamma,shutter,gain;
  bool read(const char * arquivo);
  bool write(const char * arquivo) const;
};

struct buffer{
  uint8_t * bytes;
  unsigned length;
};

class Camera {
 private:
  unsigned int width, height, fps;

  void Open();
  void Close();

  void Init();
  void UnInit();

  void Start();
  void Stop();

  void init_mmap();

   int fd;
   const char *name;  //dev_name

  struct buffer meuBuffer[NUM_BUFFERS];

public:
  Camera(unsigned index);
   ~Camera();

   //Falta fazer
   //bool ajusteparam(PARAMETROS_CAMERA cameraparam);

   bool capturando;
   bool inicializado;
   bool encerrar;

   //Imagem *imgBruta; //Quando imagem for uma classe a abstrata
   ImagemGBRG imgBruta;

   //Estes metodos retornam true em caso de saida indesejada
   //e false caso tudo ocorreu como esperado
   bool waitforimage();
   bool captureimage();

   inline unsigned getWidth()const {return width;};
   inline unsigned getHeight()const {return height;};

 //public:
   void run();
   void terminar();
   //equivalente a v4l2-ctl --list-formats-ext
   void printvideoformats(); //falta fazer
   //Falta fazer esses metodos abaixo
   /*
   int minBrightness();
   int maxBrightness();
   int defaultBrightness();
   int minContrast();
   int maxContrast();
   int defaultContrast();
   int minSaturation();
   int maxSaturation();
   int defaultSaturation();
   int minHue();
   int maxHue();
   int defaultHue();
   bool isHueAuto();
   int minSharpness();
   int maxSharpness();
   int defaultSharpness();

   int setBrightness(int v);
   int setExposure(int v);
   int setContrast(int v);
   int setSaturation(int v);
   int setHue(int v);
   int setHueAuto(bool v);
   int setSharpness(int v);
   int setWhiteness(int v);
   */
};
