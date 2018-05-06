#include "camera.h"
#include <cstdlib> //exit()

static void errno_exit (const char* s)
{
        std::cerr << s << endl;
        exit (EXIT_FAILURE);
}


static int xioctl(int fd, int request, void *arg)
{
  int r,itt=0;
  do {
		r = ioctl (fd, request, arg);
		itt++;
	}while ((-1 == r) && (itt<100));
  return r;
}

bool PARAMETROS_CAMERA::read (const char * arquivo) {
  FILE * arq=fopen(arquivo, "r");
  if (arq==NULL) {
    return true;
  }

  unsigned bri, exp, hue, sat, gam, shu, gai;
  if(fscanf(arq,"Brightness: %u\n",&bri) != 1) return true;
  if(fscanf(arq,"Auto exposure: %u\n",&exp) != 1) return true;
  if(fscanf(arq,"Hue: %u\n",&hue) != 1) return true;
  if(fscanf(arq,"Saturation: %u\n",&sat) != 1) return true;
  if(fscanf(arq,"Gamma: %u\n",&gam) != 1) return true;
  if(fscanf(arq,"Shutter: %u\n",&shu) != 1) return true;
  if(fscanf(arq,"Gain: %u\n",&gai) != 1) return true;

  fclose(arq);

  brightness = bri;
  exposure = exp;
  hue = hue;
  saturation = sat;
  gamma = gam;
  shutter = shu;
  gain = gai;

  return false;
}

bool PARAMETROS_CAMERA::write(const char* arquivo) const{
  FILE *arq=fopen(arquivo,"w");
  if(arq == NULL)
    return true;

  fprintf(arq,"Brightness: %u\n",brightness);
  fprintf(arq,"Auto exposure: %u\n",exposure);
  fprintf(arq,"Hue: %u\n",hue);
  fprintf(arq,"Saturation: %u\n",saturation);
  fprintf(arq,"Gamma: %u\n",gamma);
  fprintf(arq,"Shutter: %u\n",shutter);
  fprintf(arq,"Gain: %u\n",gain);

  fclose(arq);

  return false;
}


Camera::Camera (unsigned index):
  encerrar(false),
  capturando(false),
  inicializado(false),
  imgBruta(0,0)
{

  //Mudar para 0 se pc não tem webcam instalada de fábrica
  width = 640; height = 480; fps = 30;

  if(index > 9)errno_exit("Camera: device invalid index");

  string dev =  string("/dev/video") + string(new (char)(index+48));
  name = (char*)dev.c_str();
  cout << name << endl;

  this->Open();
  this->Init();
  this->Start();
  inicializado = true;

  imgBruta.resize(width, height);
}

void Camera::Open() { // Rotina que serve para abrir dispositivo.

  fd = open(name, O_RDWR | O_NONBLOCK , 0);

  if(-1 == fd)errno_exit("No Open");

}
void Camera::Close()
{
  close(fd);
}

void Camera::Init(){
  //formatar o dispotivo
  struct v4l2_format format;
  format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  format.fmt.pix.pixelformat = V4L2_PIX_FMT_SGBRG8; //Depende da camera
  format.fmt.pix.width = this->width;
  format.fmt.pix.height = this->height;

  //aplicar
  if( -1 == xioctl(fd, VIDIOC_S_FMT,&format) )
    errno_exit("Format");
  //Configuracoes da camera

  //configuracoes de captura
  struct v4l2_streamparm p;

  p.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
  p.parm.capture.timeperframe.numerator=1;
  p.parm.capture.timeperframe.denominator=fps;
  p.parm.output.timeperframe.numerator=1;
  p.parm.output.timeperframe.denominator=fps;

  if(-1==xioctl(fd, VIDIOC_S_PARM, &p))
    errno_exit("Stream Param");

  init_mmap();
}

void Camera::init_mmap(){
  //Informando ao dispositivo sobre os buffers utilizados
  //Alocando buffer
  struct v4l2_requestbuffers bufrequest;
  CLEAR(bufrequest);
  bufrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  //como serao alocados e como o dispositivo deve lidar com ele (o buffer)
  bufrequest.memory = V4L2_MEMORY_MMAP;
  //numero de buffers
  bufrequest.count = NUM_BUFFERS;
  //aplicar
  if(-1 == xioctl(fd,VIDIOC_REQBUFS, &bufrequest))
    errno_exit("Request Buffer");
  //limpando a memoria
  //Obs.: o memset(ptr,value,size_t) => preencher um espaco de memoria apartir de ptr com tamanho size_t
  //com o valor value;
  //memset(&buffer,0,sizeof(buffer));
  for(int i = 0; i < NUM_BUFFERS; i++)
  {
    //Alocando os buffer de consulta
    struct v4l2_buffer buffer;
    CLEAR(buffer);
    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer.memory = V4L2_MEMORY_MMAP;
    buffer.index = i;
    //aplicar
    if( -1 == xioctl(fd,VIDIOC_QUERYBUF,&buffer) )
      errno_exit("Query buffer");
    //Mapeando o buffer em na memoria virtual da aplicação
    meuBuffer[i].bytes  = (uint8_t*)mmap(NULL,buffer.length,PROT_READ | PROT_WRITE,MAP_SHARED,fd,buffer.m.offset);
    meuBuffer[i].length = buffer.length;
    if(meuBuffer[i].bytes == MAP_FAILED)
      errno_exit("Mmap");
  }
}

void Camera::UnInit() {
  for(unsigned int i = 0;i<1;i++){
     if(-1 == munmap (meuBuffer[i].bytes, meuBuffer[i].length))
	errno_exit ("munmap");
  }
}

void Camera::Start()
{
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if(-1 == xioctl (fd, VIDIOC_STREAMON, &type))
  errno_exit ("VIDIOC_STREAMON");

  for(unsigned int i = 0; i < NUM_BUFFERS; i++) {
    struct v4l2_buffer buf;
    CLEAR (buf);

    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = i;

    if(-1 == xioctl (fd, VIDIOC_QBUF, &buf))
      errno_exit ("VIDIOC_QBUF");
  }
}

void Camera::Stop()
{
  enum v4l2_buf_type type;
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if(-1 == xioctl (fd, VIDIOC_STREAMOFF, &type))
    errno_exit("Stop");
  capturando = false;
}

bool Camera::captureimage(){

  if (capturando) {

   struct v4l2_buffer buf;
   CLEAR(buf);
   buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
   buf.memory = V4L2_MEMORY_MMAP;
   if(-1 == xioctl (fd, VIDIOC_DQBUF, &buf)) {
     perror("Retrieving Frame");
     return true;
    }

   if(-1 == xioctl (fd, VIDIOC_QBUF, &buf)) {
       perror("Requesting new Frame");
       return true; //errno_exit ("VIDIOC_QBUF");
     }
    memcpy((uint8_t*)imgBruta.getRawData(),meuBuffer[buf.index].bytes,meuBuffer[buf.index].length);
  }
 return false;
}

bool Camera::waitforimage()
{

  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  struct timeval tv = {0};
  tv.tv_sec = 2;

  int r = select(fd+1, &fds, NULL, NULL, &tv);

  if(-1 == r){
    perror("Waiting for Frame");
    return true;
  }

  return false;
}


void Camera::run()
{
  while(!encerrar){
    waitforimage();
    captureimage();
  }
}

void Camera::terminar () {
  encerrar = true;
  UnInit();
  Stop();
}

Camera::~Camera(){
  terminar();
}
