#include "camera.h"
#include <cstdlib> //exit()
#include <errno.h> // EINVAL
#include <sys/stat.h> //S_ISCHR

static void errno_exit (const char* s)
{
        std::cerr << s << '\n';
        exit (EXIT_FAILURE);
}


static int xioctl(int fd, int request, void *arg)
{
  int r,itt=0;
  do {
		r = ioctl (fd, request, arg);
		itt++;
	}while ((-1 == r) && (itt<100)  && (EINTR == errno));
  return r;
}

bool PARAMETROS_CAMERA::read (const char * arquivo) {
  return true;
}

bool PARAMETROS_CAMERA::write(const char* arquivo) const{
  return true;
}


Camera::Camera (const char* device):
  encerrar(false),
  capturando(false),
  inicializado(false),
  ImBruta(0,0),
  name(device)
{
  //Mudar para 0 se pc não tem webcam instalada de fábrica
  width = WIDTH;
  height = HEIGHT;
  fps = FPS;

  this->Open();
  this->Init();
  this->Start();
  inicializado = true;

  ImBruta.resize(width, height);
}

Camera::Camera (unsigned index):
  encerrar(false),
  capturando(false),
  inicializado(false),
  ImBruta(0,0)
{

  //Mudar para 0 se pc não tem webcam instalada de fábrica
  width = WIDTH;
  height = HEIGHT;
  fps = FPS;

  if(index > 9)errno_exit("Camera: device invalid index");

  std::string dev =  std::string("/dev/video") + std::string(new (char)(index+48));
  name = (char*)dev.c_str();


  this->Open();
  this->Init();
  this->Start();
  inicializado = true;

  ImBruta.resize(width, height);
}

void Camera::Open() { // Rotina que serve para abrir dispositivo.
  struct stat st;
  if(-1==stat(name, &st)) {
    fprintf(stderr, "Cannot identify '%s' : %d, %s\n", name, errno, strerror(errno));
    exit(1);
  }

  if(!S_ISCHR(st.st_mode)) {
    fprintf(stderr, "%s is no device\n", name);
    exit(1);
  }

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
    memcpy((uint8_t*)ImBruta.getRawData(),meuBuffer[buf.index].bytes,meuBuffer[buf.index].length);
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

bool Camera::ajusteparam(PARAMETROS_CAMERA cameraparam){
  //falta fazer
  return true;
}

bool Camera::setControl(__u32 id,int v){

  struct v4l2_control control;
  struct controler ctrl = queryControl(id);
  CLEAR(control);

  control.id = id;
  control.value = v;
  if(!ctrl.enable)
    return false;

  if(-1 == xioctl(fd,VIDIOC_S_CTRL, &control))
    errno_exit("Camera-setControl: ERRO\n");
  return true;
}

int Camera::getControl(__u32 id)const{
  struct v4l2_control control;
  CLEAR(control);
  struct controler ctrl = queryControl(id);
  if(!ctrl.enable)
    return 0;

  control.id = id;
  if(-1 == xioctl(fd,VIDIOC_G_CTRL, &control))
    errno_exit("Camera-getControl: ERRO\n");
  return control.value;
}

struct controler Camera::queryControl(__u32 id)const{
    struct v4l2_queryctrl queryctrl;
    struct controler ctrl;
    CLEAR(queryctrl);
    ctrl.enable = false;

    queryctrl.id = id;
    if(-1 == xioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)){
      std::cerr << "Camera-Erro: Camera nao reconhece o controle" << '\n';
      return ctrl;
    }

    //Testa se o dispositivo nao possui o controler
    if ( queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
      return ctrl;

    strcpy((char*)ctrl.name, (const char*)queryctrl.name);

    ctrl.min = queryctrl.minimum;
    ctrl.max = queryctrl.maximum;
    ctrl.default_value = queryctrl.default_value;

    ctrl.enable = true;
    return ctrl;
}

bool Camera::setBrightness(int v){
  return setControl(V4L2_CID_BRIGHTNESS,v);
}

bool Camera::setConstrast(int v){
  return setControl(V4L2_CID_CONTRAST,v);
}

bool Camera::setSaturation(int v){
  return setControl(V4L2_CID_SATURATION,v);
}

bool Camera::setSharpness(int v){
  return setControl(V4L2_CID_SHARPNESS,v);
}

bool Camera::setGain(int v){
  return setControl(V4L2_CID_GAIN,v);
}

bool Camera::setExposure(int v){
  return setControl(V4L2_CID_EXPOSURE,v);
}

bool Camera::setExposureAbs(int v){
  return setControl(V4L2_CID_EXPOSURE_ABSOLUTE,v);
}

bool Camera::setHue(int v){
  return setControl(V4L2_CID_HUE,v);
}

bool Camera::setGamma(int v){
  return setControl(V4L2_CID_GAMMA,v);
}

int Camera::getExposureAbs()const{
  return getControl(V4L2_CID_EXPOSURE_ABSOLUTE);
}

int Camera::getBrightness()const{
  return getControl(V4L2_CID_BRIGHTNESS);
}

int  Camera::getContrast()const{
  return getControl(V4L2_CID_CONTRAST);
}
int Camera::getSaturation()const{
  return getControl(V4L2_CID_SATURATION);
}
int  Camera::getHue()const{
  return getControl(V4L2_CID_HUE);
}
int  Camera::getSharpness()const{
  return getControl(V4L2_CID_SHARPNESS);
}
int  Camera::getGain()const{
  return getControl(V4L2_CID_GAIN);
}
int  Camera::getExposure()const{
  return getControl(V4L2_CID_EXPOSURE);
}

int  Camera::getGamma()const{
  return getControl(V4L2_CID_GAMMA);
}
bool Camera::queryBrightness(struct controler &ctrl)const{
  //V4L2_CID_BRIGHTNESS
  ctrl = queryControl(V4L2_CID_BRIGHTNESS);
  //testa se possui o controler
  if(!ctrl.enable)
  return false;
  return true;
}
bool Camera::queryContrast(struct controler &ctrl)const{
  //V4L2_CID_CONTRAST
  ctrl = queryControl(V4L2_CID_CONTRAST);
  //testa se possui o controler
  if(!ctrl.enable)
    return false;
  return true;
}



bool Camera::querySaturation(struct controler &ctrl)const{
  //V4L2_CID_SATURATION
  ctrl = queryControl(V4L2_CID_SATURATION);
  //testa se possui o controler
  if(!ctrl.enable)
    return false;
  return true;
}

bool Camera::queryHue(struct controler &ctrl)const{
  //V4L2_CID_HUE
  ctrl = queryControl(V4L2_CID_HUE);
  //testa se possui o controler
  if(!ctrl.enable)
    return false;
  return true;
}

bool Camera::querySharpness(struct controler &ctrl)const{
  //V4L2_CID_SHARPNESS
  ctrl = queryControl(V4L2_CID_SHARPNESS);
  //testa se possui o controler
  if(!ctrl.enable)
    return false;
  return true;
}

bool Camera::queryGain(struct controler &ctrl)const{
  //V4L2_CID_GAIN
  ctrl = queryControl(V4L2_CID_GAIN);
  //testa se possui o controler
  if(!ctrl.enable)
    return false;
  return true;

}

bool Camera::queryExposure(struct controler &ctrl)const{
  //V4L2_CID_EXPOSURE
  ctrl = queryControl(V4L2_CID_EXPOSURE);
  //testa se possui o controler
  if(!ctrl.enable)
    return false;
  return true;
}
bool Camera::queryExposureAbs(struct controler &ctrl)const{
  ctrl = queryControl(V4L2_CID_EXPOSURE_ABSOLUTE);
  //testa se possui o controler
  if(!ctrl.enable)
    return false;
  return true;
}

//V4L2_CID_GAMMA
bool Camera::queryGamma(struct controler &ctrl)const{
  ctrl = queryControl(V4L2_CID_GAMMA);
  //testa se possui o controler
  if(!ctrl.enable)
    return false;
  return true;
}
bool Camera::queryMinBuffer(struct controler &ctrl)const{
  ctrl = queryControl(V4L2_CID_MIN_BUFFERS_FOR_CAPTURE);
  if(!ctrl.enable)
    return false;
  return true;
}
