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



#define CLEAR(x) memset (&(x), 0, sizeof (x))

//Default
#define WIDTH 640
#define HEIGHT 480
#define FPS 30



#define NUM_BUFFERS 1


// class Controler{
//
// public:
//   inline int   getMin()     { return (enable)?queryCtrl.minimum:-1; }
//   inline int   getMax()     { return (enable)?queryCtrl.maximum:-1; }
//   inline int   getDefault() { return (enable)?queryCtrl.default_value:-1; }
//   inline __u8* getName()    { return (enable)?queryCtrl.name:"0"; }
//
//   virtual int  get()const = 0;
//   virtual bool set(int v) = 0;
//
//   virtual bool write(std::ostream &O) = 0;
//   virtual bool read(std::istream &I) = 0;
//
// private:
//   bool enable;
//   v4l2_queryctrl queryCtrl;//Todas as info. sobre o controle
//   v4l2_control control; // estado atual do controle
//
//   struct controler *nextCtrl;
//
//
//   virtual Controler(const int &fd) = 0;
//   virtual ~Controler();
//
//   friend class ListaCtrls;
// };

// class ListCtrl{
//
// }

struct controler{
  bool enable;
  __u8 name[32];
  int min,max;
  int default_value;
};


struct PARAMETROS_CAMERA {
  struct controler ctrls[];
  bool read(const char *arquivo);
  bool write(const char *arquivo) const;
};

struct buffer{
  uint8_t *bytes;
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

  bool setControl(__u32 id, int v);
  int getControl(__u32 id)const;
  struct controler queryControl(__u32 id)const;

protected:
  Camera(unsigned index = 0);
  Camera(const char* device);
   ~Camera();

   //Falta fazer
   //bool ajusteparam(PARAMETROS_CAMERA cameraparam);

   bool capturando;
   bool inicializado;
   bool encerrar;

   ImagemGBRG ImBruta;

   //Estes metodos retornam true em caso de saida indesejada
   //e false caso tudo ocorreu como esperado
   bool captureimage();
   bool waitforimage();

   inline unsigned getWidth()const {return width;};
   inline unsigned getHeight()const {return height;};

   bool ajusteparam(PARAMETROS_CAMERA cameraparam);//falta fazer

   inline void toRGB(ImagemRGB &imgRGB) { ImBruta.toImageRGB(imgRGB); }


 public:
   void run();
   void terminar();

   //equivalente a v4l2-ctl --list-formats-ext
   //char* printVideoFormats()const; //falta fazer

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

   bool queryMinBuffer(struct controler &ctrl)const;
};
