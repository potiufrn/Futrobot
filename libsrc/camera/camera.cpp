#include "camera.h"
#include <cstdlib> //exit()
#include <errno.h> // EINVAL
#include <sys/stat.h> //S_ISCHR

#include "../../program/data.h"



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

bool Camera::read(const char * arquivo) {
  FILE * arq=fopen(arquivo, "r");
  if (arq==NULL) {
    return true;
  }

  unsigned bri, exp, hue, sat, gam, expAbs, gai, cont, sharp;
  if(fscanf(arq,"Brightness: %u\n",&bri) != 1) return true;
  if(fscanf(arq,"Exposure: %u\n",&exp) != 1) return true;
  if(fscanf(arq,"Exposure Absolute: %u\n",&expAbs) != 1) return true;
  if(fscanf(arq,"Hue: %u\n",&hue) != 1) return true;
  if(fscanf(arq,"Saturation: %u\n",&sat) != 1) return true;
  if(fscanf(arq,"Gamma: %u\n",&gam) != 1) return true;
  if(fscanf(arq,"Gain: %u\n",&gai) != 1) return true;
  if(fscanf(arq,"Contrast: %u\n",&cont) != 1) return true;
  if(fscanf(arq,"Sharpness: %u\n",&sharp) != 1) return true;

  fclose(arq);

  setBrightness(bri);
  setExposure(exp);
  setExposureAbs(expAbs);
  setHue(hue);
  setSaturation(sat);
  setGamma(gam);
  setGain(gai);
  setConstrast(cont);
  setSharpness(sharp);

  return false;
}
bool Camera::write(const char * arquivo) const{
  FILE *arq=fopen(arquivo,"w");
  if(arq == NULL){
    return true;
  }
  fprintf(arq,"Brightness: %d\n",getBrightness());
  fprintf(arq,"Exposure: %d\n",getExposure());
  fprintf(arq,"Exposure Absolute: %d\n",getExposureAbs());
  fprintf(arq,"Hue: %d\n",getHue());
  fprintf(arq,"Saturation: %d\n",getSaturation());
  fprintf(arq,"Gamma: %d\n",getGamma());
  fprintf(arq,"Gain: %d\n",getGain());
  fprintf(arq,"Contrast: %d\n",getContrast());
  fprintf(arq,"Sharpness: %d\n",getSharpness());
  fclose(arq);

  return false;
}
std::ostream& Camera::write(std::ostream &O) const{
  O << "Brightness: "<< getBrightness()<<'\n';
  O << "Exposure: "  << getExposure()<<'\n';
  O << "Exposure Absolute: "<< getExposureAbs()<<'\n';
  O << "Hue: "<< getHue()<<'\n';
  O << "Saturation: "<< getSaturation()<<'\n';
  O << "Gamma: "<< getGamma()<<'\n';
  O << "Gain: "<<getGain() << '\n';
  O << "Contrast: "<< getContrast()<<'\n';
  O << "Sharpness: "<< getSharpness();
  return O;
}
bool Camera::read(std::istream &I){
  std::string str;
  int bri, exp, expAbs, hue, sat, gamma, cont, sharp, gain;

  getline(I,str,':');
  if(str !="Brightness")return true;
  I >> bri;

  getline(I,str,':');
  if(str !="\nExposure")return true;
  I >> exp;

  getline(I,str,':');
  if(str !="\nExposure Absolute")return true;
  I >> expAbs;

  getline(I,str,':');
  if(str !="\nHue")return true;
  I >> hue;

  getline(I,str,':');
  if(str !="\nSaturation")return true;
  I >> sat;

  getline(I,str,':');
  if(str !="\nGamma")return true;
  I >> gamma;

  getline(I,str,':');
  if(str !="\nGain")return true;
  I >> gain;

  getline(I,str,':');
  if(str !="\nContrast")return true;
  I >> cont;

  getline(I,str,':');
  if(str !="\nSharpness")return true;
  I >> sharp;

  setBrightness(bri);
  setExposure(exp);
  setExposureAbs(expAbs);
  setHue(hue);
  setSaturation(sat);
  setGamma(gamma);
  setConstrast(cont);
  setSharpness(sharp);
  setGain(gain);
  return false;
}
Camera::Camera():
  encerrar(true),
  capturando(false),
  isOpen(false),
  ImBruta(WIDTH,HEIGHT)
{
  width = WIDTH;
  height = HEIGHT;
  fps = FPS;
}
Camera::Camera (unsigned index):
  encerrar(false),
  capturando(false),
  isOpen(false),
  ImBruta(WIDTH,HEIGHT)
{
  width = WIDTH;
  height = HEIGHT;
  fps = FPS;
  this->Open(index);
}
bool Camera::Open(unsigned index) { // Rotina que serve para abrir dispositivo.
  if(this->isOpen)this->Close();

  if(index > 9){
    std::cerr << "Camera Warning: index invalido" << '\n';
    return false;
  }

  std::string dev =  std::string("/dev/video") + std::string(new (char)(index+48),1);
  const char* name = (char*)dev.c_str();

  struct stat st;
  if(-1==stat(name, &st)) {
    fprintf(stderr, "Cannot identify '%s' : %d, %s\n", name, errno, strerror(errno));
    return false;
  }

  if(!S_ISCHR(st.st_mode)){
    fprintf(stderr, "%s is no device\n", name);
    return false;
  }
  fd = open(name, O_RDWR | O_NONBLOCK , 0);
  // fd = open(name, O_RDWR, 0);
  if(-1 == fd){
    std::cerr << "Camera Warning: No open" << '\n';
    return false;
  }

  this->isOpen = true;
  this->Init();
  this->Start();
  this->capturando = true;
  this->encerrar = false;
  return true;
}
void Camera::Close(){
  this->isOpen     = false;
  this->Stop();
  this->pxFormat   = UNDEF;
  this->encerrar   = true;
  this->capturando = false;
  close(fd);
  this->UnInit();
}
unsigned Camera::listDevices(bool printed)const{
  const char* name;
  std::string dev;
  int fd_temp;
  unsigned contDevices = 0;

  if(printed){
    std::cout << "\tIndex\tNome" << '\n';
  }

  for(unsigned i = 0; i < 9; i++){
    dev =  std::string("/dev/video") + std::string(new (char)(i+48),1);
    name = (const char*)dev.c_str();

    fd_temp = open(name,O_RDWR | O_NONBLOCK , 0);
    if(fd_temp == -1)continue;
    contDevices++;
    struct v4l2_capability cap;
    CLEAR(cap);
    ioctl(fd_temp,VIDIOC_QUERYCAP,&cap);
    if(printed)
      std::cout <<'\t'<<i<<'\t'<<cap.card << '\n';
    close(fd_temp);
  }
  return contDevices;
}
void Camera::Init(){
  //formatar o dispotivo
  struct v4l2_fmtdesc fmtdesc;
  CLEAR(fmtdesc);
  fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  bool format_invalido = true;
  while (ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc) == 0)
  {
      if(fmtdesc.pixelformat == V4L2_PIX_FMT_SGBRG8){
        pxFormat = GBRG;
        // std::cout << "Suporta a GBRG" << '\n';
        format_invalido = false;
        break;
      }
      if(fmtdesc.pixelformat == V4L2_PIX_FMT_YUYV){
        pxFormat = YUYV;
        // std::cout << "Suporta a YUYV" << '\n';
        format_invalido = false;
        break;
      }
      fmtdesc.index++;
  }
  if(format_invalido)errno_exit("Camera ERRO: Dispositivo sem formato de imagem conhecido");
  struct v4l2_format format;
  CLEAR(format);

  format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  format.fmt.pix.width  = this->width;
  format.fmt.pix.height = this->height;
  if(pxFormat == GBRG)
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_SGBRG8;
  else
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
  format.fmt.pix.field  = V4L2_FIELD_INTERLACED;

  //aplica as configuracoes de formato
  //Primeiro tenta-se setar o formato de Pixel GBRG, caso
  //nao seja suportado pela camera, tenta-se o formato YUYV
  //que eh padrao para as cameras USB

  if( ioctl(fd, VIDIOC_S_FMT,&format) == -1)
    errno_exit("Camera ERRO: init Failed");

  //configuracoes de captura
  struct v4l2_streamparm p;

  p.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
  p.parm.capture.timeperframe.numerator=1;
  p.parm.capture.timeperframe.denominator=fps;
  p.parm.output.timeperframe.numerator=1;
  p.parm.output.timeperframe.denominator=fps;

  if(-1 == xioctl(fd, VIDIOC_S_PARM, &p))
    errno_exit("Camera ERRO: Parametros de stream");

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
    errno_exit("Camera ERRO: Request Buffer");
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
      errno_exit("Camera ERRO: Query buffer");
    //Mapeando o buffer em na memoria virtual da aplicação
    meuBuffer[i].bytes  = (uint8_t*)mmap(NULL,buffer.length,PROT_READ | PROT_WRITE,MAP_SHARED,fd,buffer.m.offset);
    meuBuffer[i].length = buffer.length;
    if(meuBuffer[i].bytes == MAP_FAILED)
      errno_exit("Camera ERRO: Mmap");
  }
}
void Camera::UnInit() {
  for(unsigned int i = 0;i<1;i++){
     if(-1 == munmap (meuBuffer[i].bytes, meuBuffer[i].length))
	errno_exit ("Camera ERRO: munmap");
  }
}
void Camera::Start(){
  if(!this->isOpen)return;
  this->encerrar = false;
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if(-1 == xioctl (fd, VIDIOC_STREAMON, &type))
  errno_exit ("Camera ERRO: VIDIOC_STREAMON");

  for(unsigned int i = 0; i < NUM_BUFFERS; i++){
    struct v4l2_buffer buf;
    CLEAR (buf);

    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = i;

    if(-1 == xioctl (fd, VIDIOC_QBUF, &buf))
      errno_exit ("Camera ERRO: VIDIOC_QBUF");
  }
}
void Camera::Stop(){
  if(!isOpen)return;
  enum v4l2_buf_type type;
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if(-1 == xioctl (fd, VIDIOC_STREAMOFF, &type))
    errno_exit("Camera ERRO: Stop");
  capturando = false;
}
bool Camera::captureimage(){

  if (capturando){
   struct v4l2_buffer buf;
   CLEAR(buf);
   buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
   buf.memory = V4L2_MEMORY_MMAP;
   if(-1 == xioctl (fd, VIDIOC_DQBUF, &buf)) {
     perror("Camera WARNING: Retrieving Frame");
     return true;
    }

   if(-1 == xioctl (fd, VIDIOC_QBUF, &buf)) {
       perror("Camera WARNING: Requesting new Frame");
       return true; //errno_exit ("VIDIOC_QBUF");
     }
    ImBruta.loadFromData(meuBuffer[buf.index].bytes, meuBuffer[buf.index].length, pxFormat, width, height);
  }
 return false;
}
bool Camera::waitforimage(){
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  struct timeval tv = {0};
  tv.tv_sec = 2;

  int r = select(fd+1, &fds, NULL, NULL, &tv);

  if(-1 == r){
    perror("Camera WARNING: Waiting for Frame");
    return true;
  }

  return false;
}
void Camera::run(){
  while(!encerrar){
    waitforimage();
    captureimage();
  }
}
void Camera::terminar() {
  encerrar = true;
  UnInit();
  Stop();
}
Camera::~Camera(){
  terminar();
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
    //V4L2_CID_AUTOBRIGHTNESS = false(0) (desabilitar auto Brilho)
    //V4L2_CID_HUE_AUTO = false(0) ; desabilita auto HUE
    //V4L2_CID_AUTOGAIN = false(0); ...
    //V4L2_CID_AUTO_WHITE_BALANC = false(0)
    // std::cerr << "Camera WARNING: setControl, controle desabilitado" << '\n';
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
    errno_exit("Camera ERRO: getControl\n");
  return control.value;
}
struct controler Camera::queryControl(__u32 id)const{

    struct v4l2_queryctrl queryctrl;
    struct controler ctrl;
    CLEAR(queryctrl);
    ctrl.enable = false;

    queryctrl.id = id;
    if(-1 == xioctl(fd, VIDIOC_QUERYCTRL, &queryctrl))
      return ctrl;

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
  // std::cout << "Brightness" << '\n';
  return setControl(V4L2_CID_BRIGHTNESS,v);
}
bool Camera::setConstrast(int v){
  // std::cout << "Contrast" << '\n';
  return setControl(V4L2_CID_CONTRAST,v);
}
bool Camera::setSaturation(int v){
  // std::cout << "Saturation" << '\n';
  return setControl(V4L2_CID_SATURATION,v);
}
bool Camera::setSharpness(int v){
  // std::cout << "Sharpness" << '\n';
  return setControl(V4L2_CID_SHARPNESS,v);
}
bool Camera::setGain(int v){
  // std::cout << "Gain" << '\n';
  return setControl(V4L2_CID_GAIN,v);
}
bool Camera::setExposure(int v){
  // std::cout << "Exposure" << '\n';
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
