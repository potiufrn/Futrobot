#include <linux/videodev2.h> //struct v4l2 ...
#include <fcntl.h>  // O_RDWR
#include <unistd.h> //close()
#include <sys/ioctl.h> //ioctl
#include <sys/mman.h> // mmap
#include <stdint.h> //unt8_t
#include <string.h>//memset, memcpy
#include <fstream> //

// #include <sys/time.h> //timevalue
#include <sys/select.h> //select
#include <sys/types.h>

#include <iostream> //cerr
#include <imagem.h>

#define CLEAR(x) memset (&(x), 0, sizeof (x))

//Default
#define WIDTH 640
#define HEIGHT 480
#define FPS 30

//WARNING numero minimo
#define NUM_BUFFERS 2

//Estrutura para auxiliar o controle dos controladores do dispositivo
//como brilho, ganho, exposicao...

struct controler{
  bool enable;//flag que indica se o controle esta habilitado
  __u8 name[32];//nome do controle
  int min,max;//valores maximos e minimos
  int default_value;
};

struct buffer{
  uint8_t *bytes;
  unsigned length;
};

//Classe para realizar o controle de dispositivos de imagem, com formatos YUV:422 ou GBRG 8bits,
//foi utilizado a API Video for Linux 2 (V4L2), essa API possui uma documentação completa e pode ser
//encontrada em https://linuxtv.org/downloads/v4l-dvb-apis/uapi/v4l/v4l2.html.
//A classe foi pensada para ser usada por heranca.
//Para habilitar a captura faz-se necessario alterar o valor da flag "capturando", para true,
//a classe tambem possui outra flag de controle, a "encerrar", que habilita ou desabilita
//a rotina de captura (metodo "run").
//
class Camera {
 private:
  unsigned int width, height, fps;
  // auxiliar para decodigicar a informacao em ImagemBruta
  // GBRG ou YUYV
  PIXEL_FORMAT pxFormat;

  void Start();
  void Stop();
  void Init();
  void UnInit();
  void init_mmap();

  bool isOpen;
  int fd;
  struct buffer meuBuffer[NUM_BUFFERS];//buffer que contem os dados da imagem.
  //funcoes auxiliares para controlar os parametros da camera
  bool setControl(__u32 id, int v);
  int  getControl(__u32 id)const;
  struct controler queryControl(__u32 id)const;

protected:
  Camera();
  Camera(unsigned index);
   ~Camera();

   ImagemBruta ImBruta;
   //flags para controlar o modo stream e habilitar ou desabilitar a captura
   bool capturando;
   bool encerrar;

   //WARNING O metodo captureimage(), faz um novo "pedido de imagem",ao passo que o waitforimage()
   //aguarda para poder retirar uma imagem pronta(que ja se encontra no buffer) e a deixa acessivel
   //para usuario (que estara disponivel na ImBruta).
   bool captureimage();
   bool waitforimage();

   inline unsigned getWidth()const {return width;};
   inline unsigned getHeight()const {return height;};

 public:
   void run();
   void terminar();
   //metodo que pode ser chamado a qualquer momento, abre um dispositovo
   //pelo seu index. Cada camera conectada ao computador
   //possui um endereco em /dev/videoX, onde este "X" eh o index.
   //para visualizar a lista de todos os dispositivos conectados
   //basta chamar o metodo listDevices().
   bool Open(unsigned index);
   void Close();

   //retorna o numero de dispositivos conectados
   //e imprimi na tela o nome e o index de cada dispositivo
   //caso nao queira que seja impresso no terminal a lista,
   //passe "false" como parametro.
   unsigned listDevices(bool printed = true)const;
   //equivalente a v4l2-ctl --list-formats-ext
   //char* printVideoFormats()const; //falta fazer

   //old vers
   bool write(const char * arquivo) const;
   bool read(const char * arquivo);

   std::ostream& write(std::ostream &O) const;
   bool read(std::istream &I);
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
