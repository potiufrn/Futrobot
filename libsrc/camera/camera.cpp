#include "camera.h"
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include "../../program/system.h" 
using namespace std;


static void errno_exit (const char * s)
{
        fprintf (stderr, "%s error %d, %s\n",
                 s, errno, strerror (errno));

        exit (EXIT_FAILURE);
}

static int xioctl(int fd, int request, void *arg)
{
        int r,itt=0;

        do {
		r = ioctl (fd, request, arg);
		itt++;
	}
        while ((-1 == r) && (EINTR == errno) && (itt<100));

        return r;
}

 //Função que lê de um arquivo os parametros da câmera
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
  if(arq == NULL){
    return true;  
  }

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


Camera::Camera (CAMERA_T cam):
  tipoCam(cam),
  encerrar(false),
  ImBruta(0,0)
{
  
   
  //Mudar para 0 se pc não tem webcam instalada de fábrica
  //const char *dev="/dev/video1";

  // 1) Instance a Camera object
  //CameraUSB *c(dev, 640, 480, 30);
  width = 640; height =480; fps = 30;
  //camera = new CameraUSB(dev, width, height, 30);
  
    name="/dev/video1";

 // data=(unsigned char *)malloc(width*height*4);
//  dst = (unsigned char*)malloc(width*height*3*sizeof(char));
   
  this->Open();
  this->Init();
  this->Start();
  initialised = true;
  
  ImBruta.resize (width, height);

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

  fd=open(name, O_RDWR | O_NONBLOCK, 0);

  if(-1 == fd) {
    fprintf(stderr, "Cannot open '%s': %d, %s\n", name, errno, strerror(errno));
    exit(1);
  }

}


void Camera::Init() { // Rotina que configura os principais parâmetros da câmera.
  
  struct v4l2_capability cap;
  struct v4l2_cropcap cropcap;
  struct v4l2_crop crop;
  struct v4l2_format fmt;
  unsigned int min;

  //******************************************************************************
  //Rotina que serve para verificar se o dispositivo é compatível com V4L
  if(-1 == xioctl (fd, VIDIOC_QUERYCAP, &cap)) {
    if (EINVAL == errno) {
      fprintf(stderr, "%s is no V4L2 device\n",name);
      exit(1);
    } else {
       errno_exit("VIDIOC_QUERYCAP");
    }
  }

  if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
    fprintf(stderr, "%s is no video capture device\n", name);
    exit(1);
  }

  if(!(cap.capabilities & V4L2_CAP_STREAMING)) {
      fprintf (stderr, "%s does not support streaming i/o\n", name);
      exit(1);
  }

  CLEAR (cropcap);

  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  if(0 == xioctl (fd, VIDIOC_CROPCAP, &cropcap)) {
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect; /* reset to default */

    if(-1 == xioctl (fd, VIDIOC_S_CROP, &crop)) {
      switch (errno) {
        case EINVAL:
          /* Cropping not supported. */
          break;
        default:
          /* Errors ignored. */
          break;
        }
      }
    } else {
      /* Errors ignored. */
    }
    //********************************************************************
    //Aqui o formato da imagem é configurado
    CLEAR (fmt);

    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = width;
    fmt.fmt.pix.height      = height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;


  if(-1 == xioctl (fd, VIDIOC_S_FMT, &fmt))
    errno_exit ("VIDIOC_S_FMT");

//********************************************************************

//Aqui é configurado o formato do vídeo
struct v4l2_streamparm p;
p.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
p.parm.capture.timeperframe.numerator=1;
p.parm.capture.timeperframe.denominator=fps;
p.parm.output.timeperframe.numerator=1;
p.parm.output.timeperframe.denominator=fps;



if(-1==xioctl(fd, VIDIOC_S_PARM, &p))
  errno_exit("VIDIOC_S_PARM");

  //default values, mins and maxes
  struct v4l2_queryctrl queryctrl;

/*
  // Brightness
  memset(&queryctrl, 0, sizeof(queryctrl));
  queryctrl.id = V4L2_CID_BRIGHTNESS;
  if(-1 == xioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
    if(errno != EINVAL) {
      //perror ("VIDIOC_QUERYCTRL");
      //exit(EXIT_FAILURE);
      printf("brightness error\n");
    } else {
      printf("brightness is not supported\n");
    }
  } else if(queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
    printf ("brightness is not supported\n");
  }
  mb=queryctrl.minimum;
  Mb=queryctrl.maximum;
  db=queryctrl.default_value;
  
  //Exposure
  v4l2_control c;
  c.id = V4L2_CID_EXPOSURE_AUTO;
  c.value = V4L2_EXPOSURE_MANUAL;
  if(xioctl(fd, VIDIOC_S_CTRL, &c) == 0)
    printf("Exposure error\n");
//   memset(&queryctrl, 0, sizeof(queryctrl));
//   queryctrl.id = V4L2_CID_EXPOSURE_AUTO;
//   queryctrl.value = V4L2_EXPOSURE_MANUAL;
//   if(-1 == xioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
//     if(errno != EINVAL) {
//       //perror ("VIDIOC_QUERYCTRL");
//       //exit(EXIT_FAILURE);
//       printf("Exposure error\n");
//     } else {
//       printf("Exposure is not supported\n");
//     }
//   } else if(queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
//     printf ("Exposure is not supported\n");
//   }
//   me=queryctrl.minimum;
//   printf("me = %d",me);
//   Me=queryctrl.maximum;
//   printf("Me = %d",Me);
//   de=queryctrl.default_value;
//   printf("de = %d",de);

  //Whiteness
  memset(&queryctrl, 0, sizeof(queryctrl));
  queryctrl.id = V4L2_CID_WHITENESS;
  if(-1 == xioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
    if(errno != EINVAL) {
      //perror ("VIDIOC_QUERYCTRL");
      //exit(EXIT_FAILURE);
      printf("Whiteness error\n");
    } else {
      printf("Whiteness is not supported\n");
    }
  } else if(queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
    printf ("Whiteness is not supported\n");
  }
  mw=queryctrl.minimum;
  Mw=queryctrl.maximum;
  dw=queryctrl.default_value;
  
  
  memset(&queryctrl, 0, sizeof(queryctrl));
  queryctrl.id = V4L2_CID_CONTRAST;
  if(-1 == xioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
    if(errno != EINVAL) {
      //perror ("VIDIOC_QUERYCTRL");
      //exit(EXIT_FAILURE);
      printf("contrast error\n");
    } else {
      printf("contrast is not supported\n");
    }
  } else if(queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
    printf ("contrast is not supported\n");
  }
  mc=queryctrl.minimum;
  Mc=queryctrl.maximum;
  dc=queryctrl.default_value;


  memset(&queryctrl, 0, sizeof(queryctrl));
  queryctrl.id = V4L2_CID_SATURATION;
  if(-1 == xioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
    if(errno != EINVAL) {
      //perror ("VIDIOC_QUERYCTRL");
      //exit(EXIT_FAILURE);
      printf("saturation error\n");
    } else {
      printf("saturation is not supported\n");
    }
  } else if(queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
    printf ("saturation is not supported\n");
  }
  ms=queryctrl.minimum;
  Ms=queryctrl.maximum;
  ds=queryctrl.default_value;


  memset(&queryctrl, 0, sizeof(queryctrl));
  queryctrl.id = V4L2_CID_HUE;
  if(-1 == xioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
    if(errno != EINVAL) {
      //perror ("VIDIOC_QUERYCTRL");
      //exit(EXIT_FAILURE);
      printf("hue error\n");
    } else {
      printf("hue is not supported\n");
    }
  } else if(queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
    printf ("hue is not supported\n");
  }
  mh=queryctrl.minimum;
  Mh=queryctrl.maximum;
  dh=queryctrl.default_value;


  memset(&queryctrl, 0, sizeof(queryctrl));
  queryctrl.id = V4L2_CID_HUE_AUTO;
  if(-1 == xioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
    if(errno != EINVAL) {
      //perror ("VIDIOC_QUERYCTRL");
      //exit(EXIT_FAILURE);
      printf("hueauto error\n");
    } else {
      printf("hueauto is not supported\n");
    }
  } else if(queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
    printf ("hueauto is not supported\n");
  }
  ha=queryctrl.default_value;

  memset(&queryctrl, 0, sizeof(queryctrl));
  queryctrl.id = V4L2_CID_SHARPNESS;
  if(-1 == xioctl (fd, VIDIOC_QUERYCTRL, &queryctrl)) {
    if(errno != EINVAL) {
      //perror ("VIDIOC_QUERYCTRL");
      //exit(EXIT_FAILURE);
      printf("sharpness error\n");
    } else {
      printf("sharpness is not supported\n");
    }
  } else if(queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
    printf ("sharpness is not supported\n");
  }
  msh=queryctrl.minimum;
  Msh=queryctrl.maximum;
  dsh=queryctrl.default_value;

//TODO: TO ADD SETTINGS
//here should go custom calls to xioctl

//END TO ADD SETTINGS
*/
  /* Note VIDIOC_S_FMT may change width and height. */

  /* Buggy driver paranoia. */
  min = fmt.fmt.pix.width * 2;
  if(fmt.fmt.pix.bytesperline < min)
    fmt.fmt.pix.bytesperline = min;
  min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
  if(fmt.fmt.pix.sizeimage < min)
    fmt.fmt.pix.sizeimage = min;

  //
  init_mmap();
     

}



void Camera::Start() {

  for(unsigned int i = 0; i < NUM_BUFFERS; i++) {
    struct v4l2_buffer buf;
    CLEAR (buf);

    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = i;

    if(-1 == xioctl (fd, VIDIOC_QBUF, &buf))
      errno_exit ("VIDIOC_QBUF");
  }

  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
  if(-1 == xioctl (fd, VIDIOC_STREAMON, &type))
    errno_exit ("VIDIOC_STREAMON");
}

void Camera::init_mmap() {
  struct v4l2_requestbuffers req;

  CLEAR (req);

  req.count               = NUM_BUFFERS;
  req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory              = V4L2_MEMORY_MMAP;

  if(-1 == xioctl (fd, VIDIOC_REQBUFS, &req)) {
    if(EINVAL == errno) {
      fprintf (stderr, "%s does not support memory mapping\n", name);
      exit (1);
    } else {
      errno_exit ("VIDIOC_REQBUFS");
    }
  }

  for(unsigned int i = 0;i<req.count;i++){
    struct v4l2_buffer buf;

    CLEAR (buf);

    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = i;
    if(-1 == xioctl (fd, VIDIOC_QUERYBUF, &buf))
      errno_exit ("VIDIOC_QUERYBUF");

    meuBufferLength[i] = buf.length;
    meuBuffer[i] = (uint8_t*)mmap (NULL, 
                                 buf.length, 
                                 PROT_READ | PROT_WRITE, 
                                 MAP_SHARED, 
                                 fd, buf.m.offset);
   
  
    if(MAP_FAILED == meuBuffer[i])
      errno_exit ("mmap");

  }
}

void Camera::UnInit() {
  for(unsigned int i = 0;i<1;i++){
     if(-1 == munmap (meuBuffer[i], meuBufferLength[i]))
	errno_exit ("munmap");
  }
}

bool Camera::start_transmission(){
  //if(dc1394_video_set_transmission(camera, DC1394_ON)!=DC1394_SUCCESS)
    return true;
  //return false;

}


void Camera::run () {
  //cout<<"Estou em run"<<endl;
  
  while(!encerrar){ 
    waitforimage();
    captureimage();

 } 
  
  
}
//******************************
  //modificado por Filipe
bool Camera::waitforimage(){
    
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    struct timeval tv = {0};
    tv.tv_sec = 2;
    //*
   // double inicio = relogio();
    int r = select(fd+1, &fds, NULL, NULL, &tv);
    //double fim = relogio();
   // cout <<"_Select:" << fim - inicio << "\t"<< r <<"\n";
    if(-1 == r){
        perror("Waiting for Frame");
	return true;
    }
    //*/
   return false;
  
}



//******************************

bool Camera::captureimage() {
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
    YUV422toRGB888(width,height,meuBuffer[buf.index],(uint8_t*)ImBruta.getRawData());
    
  }  
  return false;
  
}

void Camera::YUV422toRGB888(int width, int height, uint8_t *src, uint8_t *dst)
{
  int line, column;
  uint8_t *py, *pu, *pv;
  uint8_t *tmp = dst;
  
  
  /* In this format each four bytes is two pixels. Each four bytes is two Y's, a Cb and a Cr. 
     Each Y goes to one of the pixels, and the Cb and Cr belong to both pixels. */
  py = src;
  pu = src + 1;
  pv = src + 3;
  //cout<<"Estou aqui 2"<<endl;
  #define CLIP(x) ( (x)>=0xFF ? 0xFF : ( (x) <= 0x00 ? 0x00 : (x) ) )

  for (line = 0; line < height; ++line) {
  //for (line = 0; line < 160; ++line) {
    //cout<<"line = "<<line<<endl;
    for (column = 0; column < width; ++column) {
      *tmp++ = CLIP((double)*py + 1.402*((double)*pv-128.0));
      *tmp++ = CLIP((double)*py - 0.344*((double)*pu-128.0) - 0.714*((double)*pv-128.0));      
      *tmp++ = CLIP((double)*py + 1.772*((double)*pu-128.0));
      
      // increase py every time
      py += 2;
      // increase pu,pv every second time
      if ((column & 1)==1) {
        pu += 4;
        pv += 4;
      }
    }
  }
}

bool Camera::ajusteparam (PARAMETROS_CAMERA cameraParam) {

  setBrightness(cameraParam.brightness);
  setContrast(cameraParam.exposure);
  setHue(cameraParam.hue);
  setSaturation(cameraParam.saturation);
  
  setWhiteness(cameraParam.shutter);
  setSharpness(cameraParam.gain);
  
  
  
  
//   if(dc1394_feature_set_mode(camera,
// 			     DC1394_FEATURE_BRIGHTNESS,
// 			     DC1394_FEATURE_MODE_MANUAL) != DC1394_SUCCESS){
//     fprintf(stderr,"Could not set camera brightness mode\n");
//     return true;
//   }
//   if(dc1394_feature_set_value(camera,
// 			      DC1394_FEATURE_BRIGHTNESS,
// 			      cameraParam.brightness) != DC1394_SUCCESS){
//     fprintf(stderr,"Could not set camera brightness value\n");
//     return true;
//   }
//       
//   //setting auto exposure
//   if(dc1394_feature_set_mode(camera,
// 			     DC1394_FEATURE_EXPOSURE,
// 			     DC1394_FEATURE_MODE_MANUAL) != DC1394_SUCCESS){
//     fprintf(stderr,"Could not set camera exposure mode\n");
//     return true;
//   }
//   if(dc1394_feature_set_value(camera,
// 			      DC1394_FEATURE_EXPOSURE,
// 			      cameraParam.exposure) != DC1394_SUCCESS){
//     fprintf(stderr,"Could not set camera exposure value\n");
//     return true;
//   }
//       
//   //setting white balance
//   if(dc1394_feature_set_mode(camera,
// 			     DC1394_FEATURE_WHITE_BALANCE,
// 			     DC1394_FEATURE_MODE_AUTO) != DC1394_SUCCESS){
//     fprintf(stderr,"Could not set camera white balance mode\n");
//     return true;
//   }
//       
//   //setting hue
//   if(dc1394_feature_set_mode(camera,
// 			     DC1394_FEATURE_HUE,
// 			     DC1394_FEATURE_MODE_MANUAL) != DC1394_SUCCESS){
//     fprintf(stderr,"Could not set camera hue mode\n");
//     return true;
//   }
//   if(dc1394_feature_set_value(camera,
// 			      DC1394_FEATURE_HUE,
// 			      cameraParam.hue) != DC1394_SUCCESS){
//     fprintf(stderr,"Could not set camera hue value\n");
//     return true;
//   }
//       
//   //setting saturation
//   if(dc1394_feature_set_mode(camera,
// 			     DC1394_FEATURE_SATURATION,
// 			     DC1394_FEATURE_MODE_MANUAL) != DC1394_SUCCESS){
//     fprintf(stderr,"Could not set camera saturation mode\n");
//     return true;
//   }
//   if(dc1394_feature_set_value(camera,
// 			      DC1394_FEATURE_SATURATION,
// 			      cameraParam.saturation) != DC1394_SUCCESS){
//     fprintf(stderr,"Could not set camera saturation value\n");
//     return true;
//   }
//       
//   //setting gamma
//   if(dc1394_feature_set_mode(camera,
// 			     DC1394_FEATURE_GAMMA,
// 			     DC1394_FEATURE_MODE_MANUAL) != DC1394_SUCCESS){
//     fprintf(stderr,"Could not set camera gamma mode\n");
//     return true;
//   }
//   if(dc1394_feature_set_value(camera,
// 			      DC1394_FEATURE_GAMMA,
// 			      cameraParam.gamma) != DC1394_SUCCESS){
//     fprintf(stderr,"Could not set camera gamma value\n");
//     return true;
//   }
//       
//   //setting shutter
//   if(dc1394_feature_set_mode(camera,
// 			     DC1394_FEATURE_SHUTTER,
// 			     DC1394_FEATURE_MODE_MANUAL) != DC1394_SUCCESS){
//     fprintf(stderr,"Could not set camera shutter mode\n");
//     return true;
//   }
//   if(dc1394_feature_set_value(camera,
// 			      DC1394_FEATURE_SHUTTER,
// 			      cameraParam.shutter) != DC1394_SUCCESS){
//     fprintf(stderr,"Could not set camera shutter value\n");
//     return true; 
//   }
//       
//   //setting gain
//   if(dc1394_feature_set_mode(camera,
// 			     DC1394_FEATURE_GAIN,
// 			     DC1394_FEATURE_MODE_MANUAL) != DC1394_SUCCESS){
//     fprintf(stderr,"Could not set camera gain mode\n");
//     return true;
//   }
//   if(dc1394_feature_set_value(camera,
// 			      DC1394_FEATURE_GAIN,
// 			      cameraParam.gain) != DC1394_SUCCESS){
//     fprintf(stderr,"Could not set camera gain value\n");
//     return true; 
//   }
   return false;
}

void Camera::Stop() {
  enum v4l2_buf_type type;

  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if(-1 == xioctl (fd, VIDIOC_STREAMOFF, &type))
    errno_exit ("VIDIOC_STREAMOFF");

}

int Camera::minBrightness() {
  return mb;
}


int Camera::maxBrightness() {
  return Mb;
}


int Camera::defaultBrightness() {
  return db;
}


int Camera::minContrast() {
  return mc;
}


int Camera::maxContrast() {
  return Mc;
}


int Camera::defaultContrast() {
  return dc;
}


int Camera::minSaturation() {
  return ms;
}


int Camera::maxSaturation() {
  return Ms;
}


int Camera::defaultSaturation() {
  return ds;
}


int Camera::minHue() {
  return mh;
}


int Camera::maxHue() {
  return Mh;
}


int Camera::defaultHue() {
  return dh;
}


bool Camera::isHueAuto() {
  return ha;
}


int Camera::minSharpness() {
  return msh;
}


int Camera::maxSharpness() {
  return Msh;
}


int Camera::defaultSharpness() {
  return dsh;
}

int Camera::setBrightness(int v) {
  if(v<mb || v>Mb) return -1;
  /*
  struct v4l2_control control;
  control.id = V4L2_CID_BRIGHTNESS;
  control.value = v;

  if(-1 == ioctl (fd, VIDIOC_S_CTRL, &control)) {
    perror("error setting brightness");
    return -1;
  }
  */
  return 1;
}

int Camera::setExposure(int v) {

//if(v<me || v>Me) return -1;
  struct v4l2_control control;
  /*
  control.id = V4L2_CID_EXPOSURE_AUTO;
  control.value = v;

  if(-1 == ioctl (fd, VIDIOC_S_CTRL, &control)) {
    perror("error setting Exposure");
    return -1;
  }
	*/
  return 1;

}

int Camera::setContrast(int v) {
  if(v<mc || v>Mc) return -1;
  /*
  struct v4l2_control control;
  control.id = V4L2_CID_CONTRAST;
  control.value = v;

  if(-1 == ioctl (fd, VIDIOC_S_CTRL, &control)) {
    perror("error setting contrast");
    return -1;
  }
  */
  return 1;
}

int Camera::setSaturation(int v) {
  if(v<ms || v>Ms) return -1;
  /*
  struct v4l2_control control;
  control.id = V4L2_CID_SATURATION;
  control.value = v;

  if(-1 == ioctl (fd, VIDIOC_S_CTRL, &control)) {
    perror("error setting saturation");
    return -1;
  }
  */
  return 1;
}

int Camera::setHue(int v) {
  if(v<mh || v>Mh) return -1;
  /*
  struct v4l2_control control;
  control.id = 	V4L2_CID_WHITENESS;//V4L2_CID_HUE;
  control.value = v;

  if(-1 == ioctl (fd, VIDIOC_S_CTRL, &control)) {
    perror("error setting hue");
    return -1;
  }
  */
  return 1;
}

int Camera::setWhiteness(int v) {
  if(v<mh || v>Mh) return -1;
  /*
  struct v4l2_control control;
  control.id = 	V4L2_CID_WHITENESS;//V4L2_CID_HUE_AUTO;
  control.value = v;

  if(-1 == ioctl (fd, VIDIOC_S_CTRL, &control)) {
    perror("error setting Whiteness");
    return -1;
  }
  */
  return 1;
}

int Camera::setSharpness(int v) {
  if(v<mh || v>Mh) return -1;
  /*
  struct v4l2_control control;
  control.id = V4L2_CID_SHARPNESS;
  control.value = v;

  if(-1 == ioctl (fd, VIDIOC_S_CTRL, &control)) {
    perror("error setting sharpness");
    return -1;
  }
  */
  return 1;
}


Camera::~Camera() {
  encerrar = true;
  UnInit();
  Stop();
  
  //free ((void *) mRgbFrame);
  //dc1394_video_set_transmission(camera, DC1394_OFF);
  //dc1394_capture_stop(camera); 

}

void Camera::terminar () {
  encerrar = true;
  UnInit();
  Stop();
}
