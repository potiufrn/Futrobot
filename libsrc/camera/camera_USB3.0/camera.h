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

struct buffer {
        uint8_t*                start;
        size_t                  length;
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

  buffer meuBuffer[NUM_BUFFERS];

 protected:
  Camera();
   ~Camera();

   bool ajusteparam(PARAMETROS_CAMERA cameraparam);
   bool capturando;

   //Imagem *imgBruta; //Quando imagem for uma classe a abstrata
   ImagemGBRG imgBruta;

   bool waitforimage();
   bool captureimage();

   inline unsigned getWidth()const {return width;};
   inline unsigned getHeight()const {return height;};

 public:
   void run();
   void terminar();
   //equivalente a v4l2-ctl --list-formats-ext
   void printvideoformats(); //falta fazer

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

  bool setBrightness(int v);
  bool setExposure(int v);
  bool setContrast(int v);
  bool setSaturation(int v);
  bool setHue(int v);
  bool setHueAuto(bool v);
  bool setSharpness(int v);
  bool setWhiteness(int v);
};
