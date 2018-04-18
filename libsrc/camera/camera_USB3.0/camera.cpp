

/*
static void errno_exit (const char * s)
{
        fprintf (stderr, "%s error %d, %s\n",
                 s, errno, strerror (errno));

        exit (EXIT_FAILURE);
}
*/

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


Camera::Camera ():
  encerrar(false),
  ImBruta(0,0)
{

  //Mudar para 0 se pc não tem webcam instalada de fábrica
  width = 640; height = 480; fps = 30;

  name="/dev/video1";

 // data=(unsigned char *)malloc(width*height*4);
 // dst = (unsigned char*)malloc(width*height*3*sizeof(char));

  this->Open();
  this->Init();
  this->Start();
  initialised = true;

  ImBruta.resize (width, height);
}


void Camera::Open() { // Rotina que serve para abrir dispositivo.

  fd = open(name, O_RDWR | O_NONBLOCK, 0);

  if(-1 == fd) {
    fprintf(stderr, "Cannot open '%s': %d, %s\n", name, errno, strerror(errno));
    exit(1);
  }

}

void capture(){
  struct v4l2_buffer buffer;
  CLEAR(buffer);

  buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buffer.memory = V4L2_MEMORY_MMAP;
  // Recuperando o buffer (tirando da fila)

  if(-1 == xioctl(fd, VIDIOC_DQBUF, &buffer))
    cerr << "Recuperando o buffer" << endl;

  //Pedindo um novo frame (Colocando na fila)
  if(-1 == xioctl (fd, VIDIOC_QBUF, &buffer)) {
    cerr << "Pedido de novo frame" << endl;
  }

  imBruta = ptr_buffer[buffer.index];
}
