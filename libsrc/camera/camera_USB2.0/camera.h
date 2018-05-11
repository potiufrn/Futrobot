#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>          /* for videodev2.h */

#include <linux/videodev2.h>

#define CLEAR(x) memset (&(x), 0, sizeof (x))


#include <stdio.h>
#include <pthread.h>
#include <imagem.h>
#include <stdlib.h>

#ifndef CAMERA_H_
#define CAMERA_H_
//Nao altere o numero de buffers
#define NUM_BUFFERS 1
//#define NUM_BUFFERS 30
//#define NUM_BUFFERS 10
#define ISO_SPEED DC1394_ISO_SPEED_400
#define FRAME_RATE_FUT DC1394_FRAMERATE_30
#define FRAME_RATE_MIX DC1394_FRAMERATE_15
//#define DEBAYER_METHOD DC1394_BAYER_METHOD_NEAREST
#define DEBAYER_METHOD DC1394_BAYER_METHOD_BILINEAR
#define VIDEO_MODE_FUT DC1394_VIDEO_MODE_640x480_MONO8
#define VIDEO_MODE_MIX DC1394_VIDEO_MODE_1280x960_MONO8
#define BAYER_FILTER DC1394_COLOR_FILTER_GBRG

#define IMAGE_FILE_NAME "ImageRGB.ppm"


struct PARAMETROS_CAMERA {
  int brightness,hue,saturation,contrast, whiteness,sharpness, exposure,gamma,shutter,gain;
  bool read(const char * arquivo);
  bool write(const char * arquivo) const;
};

struct buffer {
        void *                  start;
        size_t                  length;
};


class Camera {
private:

  unsigned int width, height, fps;


 FILE* imagefile;

  void Open();
  void Close();

  void Init();
  void UnInit();

  void Start();
  void Stop();

  void init_mmap();

  void YUV422toRGB888(int width_, int height_, uint8_t *src_, uint8_t *dst_);

  bool initialised;

   int fd;
   const char *name;  //dev_name


  // unsigned char *data;

  //unsigned char* src;
  //unsigned char* dst;

  //buffer *buffers;
  typedef uint8_t* ptr_uint8;
  ptr_uint8 meuBuffer[NUM_BUFFERS];
  size_t meuBufferLength[NUM_BUFFERS];
  //int n_buffers;

  int mb, Mb, db,me,Me,de,mw,Mw,dw, mc, Mc, dc, ms, Ms, ds, mh, Mh, dh, msh, Msh, dsh;
  bool ha;
protected:
  Camera();
   ~Camera();

   bool ajusteparam(PARAMETROS_CAMERA cameraparam);
   bool encerrar;
   bool capturando;
   ImagemRGB ImBruta;

   uint8_t* getFrame(){ return meuBuffer[0]; }

   bool waitforimage();
   bool captureimage();
   // bool capturetcimage(); // <- True color

   inline unsigned int Width() {return width;};
   inline unsigned int Height() {return height;};
   bool start_transmission();

 public:

   void run();
   void terminar();
   void printvideoformats();

   void StopCam();

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
};

#endif
