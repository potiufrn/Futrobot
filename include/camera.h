#include <linux/videodev2.h> //struct v4l2 ...
#include <fcntl.h>  // O_RDWR
#include <unistd.h> //close()
#include <sys/ioctl.h> //ioctl
#include <sys/mman.h> // mmap
#include <stdint.h> //unt8_t
#include <string.h>//memset, memcpy
#include <fstream>

#include <sys/time.h> //timevalue
#include <sys/select.h> //select
#include <sys/types.h>

#include <iostream>

#define CLEAR(x) memset (&(x), 0, sizeof (x))

//Default
#define WIDTH 640
#define HEIGHT 480
#define FPS 30

//WARNING o numero de buffer pode ser alterado
//mas nao se sabe ate o momento a influencia desta alteracao
//no desempenho da captura
#define NUM_BUFFERS 1

struct controler{
  bool enable;
  __u8 name[32];
  int min,max;
  int default_value;
};

struct buffer{
  uint8_t *bytes;
  unsigned length;
};


class Camera {
 private:
  unsigned int width, height, fps;

  //TODO utilidade da propriedade pixel_type
  // servira como auxiliar para decodigicar a informacao
  // vinda da camera (imgData)
  // 0 - GBRG
  // 1 - YUYV
  uint8_t pxFormat;
  uint8_t *imgData;

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

  bool setControl(__u32 id, int v);
  int  getControl(__u32 id)const;
  struct controler queryControl(__u32 id)const;

protected:
  Camera(unsigned index = 0);
  Camera(const char* device);
  // Camera();
   ~Camera();

   bool capturando;
   bool inicializado;
   bool encerrar;

   inline uint8_t getPxFormat()const{ return pxFormat; }
   inline const uint8_t* getDataImage()const{ return imgData; }
   const unsigned getDataSize()const{ return meuBuffer[0].length; }
   // ImagemGBRG ImBruta;
   //Estes metodos retornam true em caso de saida indesejada
   //e false caso tudo ocorreu como esperado
   bool captureimage();
   bool waitforimage();

   inline unsigned getWidth()const {return width;};
   inline unsigned getHeight()const {return height;};

   inline unsigned int Width() {return width;};
   inline unsigned int Height() {return height;};

 public:
   void run();
   void terminar();

   // void getDevices();
   // bool open(unsigned index);
   // bool isOpen(){ return inicializado; }
   // void close();

   //equivalente a v4l2-ctl --list-formats-ext
   //char* printVideoFormats()const; //falta fazer

   bool write(const char * arquivo) const;
   bool read(const char * arquivo);
   //Falta fazer esses metodos abaixo
   //Os metodos abaixo Retornam false caso o controler nao exista (get)
   //para o dispositivo ou ocorra falha na setagem dos parados
   bool queryBrightness(struct controler &ctrl)const;
   int  getBrightness()const;
   bool setBrightness(int v);

   bool queryContrast(struct controler &ctrl)const;
   int  getContrast()const;
   bool setConstrast(int v);

   bool querySaturation(struct controler &ctrl)const;
   int  getSaturation()const;
   bool setSaturation(int v);

   //V4L2_CID_GAMMA
   bool queryGamma(struct controler &ctrl)const;
   int  getGamma()const;
   bool setGamma(int v);

   bool queryHue(struct controler &ctrl)const;
   int  getHue()const;
   bool setHue(int v);

   bool querySharpness(struct controler &ctrl)const;
   int  getSharpness()const;
   bool setSharpness(int v);

   bool queryGain(struct controler &ctrl)const;
   int  getGain()const;
   bool setGain(int v);

   bool queryExposureAbs(struct controler &ctrl)const;
   int  getExposureAbs()const;
   bool setExposureAbs(int v);

   bool queryExposure(struct controler &ctrl)const;
   int  getExposure()const;
   bool setExposure(int v);

   //WARNING este metodo nao funcionou como esperado
   bool queryMinBuffer(struct controler &ctrl)const;
};
