#include <iostream>

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

#include <string>
//Bibliotecas para testes e medicoes
//#include <ctime>

using namespace std;

#define CLEAR(x) memset (&(x), 0, sizeof (x))

#define WIDTH 752
#define HEIGHT 480
#define fps 30

#define NUM_BUFFERS 10

static int xioctl(int fd, int request, void *arg)
{
    int r,itt=0;

    do {
		r = ioctl (fd, request, arg);
		itt++;
	}while ((-1 == r) && (itt<100));

  return r;
}


void save(char*arq);
void config();

void open();
void init();
void start();

void wait();
void capture();

void stop();
void delBuffer();
//Variaveis globais
char* camera = "/dev/video1";
uint8_t *ptr_buffer[NUM_BUFFERS];
int fd;
uint8_t* imgGBRG;

int main()
{

  //Abrir o dispositivo
  open();
  //Configura o dispositivo
  init();
  //Liga o dispositivo no modo stream
  //Enfileira os buffers
  start();
  //config();
  //esperando resposta do dispositivo estar pronto:
  /*
  char* nomesArq[] = {"etc/1.ppm","etc/2.ppm", "etc/3.ppm","etc/4.ppm","etc/5.ppm","etc/6.ppm","etc/7.ppm",
                      "etc/8.ppm","etc/9.ppm","etc/10.ppm","etc/11.ppm","etc/12.ppm","etc/13.ppm","etc/14.ppm",
                      "etc/15.ppm","etc/16.ppm","etc/17.ppm","etc/18.ppm","etc/19.ppm","etc/20.ppm","etc/21.ppm"};
  */

  int begin;
  int end;
  double tempo;
  double max = 0;
  double medio = 0;

  for(unsigned testes = 0; testes < 5; testes ++)
  {
    for(unsigned i = 0;  i < 100; i++)
    {
      begin = clock();
      wait();
      capture();
      end = clock();
      tempo = (double)(end - begin)*1000/(CLOCKS_PER_SEC);
      medio = (medio+tempo)/2;
      if(tempo > max){
        max = tempo;
      }
      //save( nomesArq[i] );
    }
  }

  ofstream registrador("10_buffer.txt");

  registrador << NUM_BUFFERS <<" buffer \n";
  registrador << "Maior tempo gasto " << max<< endl;
  registrador <<"Tempo medio de captura "<< medio << endl;
  registrador.close();

  stop();
  delBuffer();
  return 0;
}

void delBuffer() {
  for(unsigned int i = 0;i < NUM_BUFFERS;i++){
     if(-1 == munmap (ptr_buffer[i], WIDTH*HEIGHT))
        cerr << "Erro no munmap " << endl;
  }
}

void stop()
{
  enum v4l2_buf_type type;
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if(-1 == xioctl (fd, VIDIOC_STREAMOFF, &type))
    cerr << "StreamOFF"<<endl;
  //Fechando o dispositivo
  close(fd);
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

  imgGBRG = ptr_buffer[buffer.index];
  //cout << "Peguei do buffer " << buffer.index << endl;
}

void init()
{
  //formatar o dispotivo
  struct v4l2_format format;
  format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  format.fmt.pix.pixelformat = V4L2_PIX_FMT_SGBRG8;
  format.fmt.pix.width = WIDTH;
  format.fmt.pix.height = HEIGHT;

  //aplicar
  if( -1 == xioctl(fd, VIDIOC_S_FMT,&format) ){
    cerr << "Erro na configuracao da camera " << endl;
    return;
  }
  //Configuracoes da camera

  //configuracoes de captura
  struct v4l2_streamparm p;

  p.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
  p.parm.capture.timeperframe.numerator=1;
  p.parm.capture.timeperframe.denominator=fps;
  p.parm.output.timeperframe.numerator=1;
  p.parm.output.timeperframe.denominator=fps;

  if(-1==xioctl(fd, VIDIOC_S_PARM, &p))
    cerr << "VIDIOC_S_PARM" << endl;

  //Informando ao dispositivo sobre os buffers utilizados
  //Alocando buffer
  struct v4l2_requestbuffers bufrequest;
  bufrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  //como serao alocados e como o dispositivo deve lidar com ele (o buffer)
  bufrequest.memory = V4L2_MEMORY_MMAP;
  //numero de buffers
  bufrequest.count = NUM_BUFFERS;
  //aplicar
  if(-1 == xioctl(fd,VIDIOC_REQBUFS, &bufrequest)){
    cerr << "Request Buffer " << endl;
    return;
  }

  //Alocando os buffer de consulta
  struct v4l2_buffer buffer;
  //limpando a memoria
  //Obs.: o memset(ptr,value,size_t) => preencher um espaco de memoria apartir de ptr com tamanho size_t
  //com o valor value;
  //memset(&buffer,0,sizeof(buffer));
  for(int i = 0; i < NUM_BUFFERS; i++)
  {
    CLEAR(buffer);
    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer.memory = V4L2_MEMORY_MMAP;
    buffer.index = i;
    //aplicar
    if( -1 == xioctl(fd,VIDIOC_QUERYBUF,&buffer) ){
      cerr << "Erro No QueryBuf" << endl;
      return;
    }
    //Mapeando o buffer em na memoria virtual da aplicação
    ptr_buffer[i] = (uint8_t*)mmap(NULL,buffer.length,PROT_READ | PROT_WRITE,MAP_SHARED,fd,buffer.m.offset);
    if(ptr_buffer[i] == MAP_FAILED)
    {
      cerr << "Erro no mapeamento de memoria " << endl;
      return;
    }
  }
}

void start()
{
  struct v4l2_buffer buffer;
  for(unsigned int i = 0; i < NUM_BUFFERS; i++) {
    CLEAR (buffer);
    buffer.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer.memory      = V4L2_MEMORY_MMAP;
    buffer.index       = i;

    if(-1 == xioctl (fd, VIDIOC_QBUF, &buffer))
      cerr << "VIDIOC_QBUF" << endl;
  }

  CLEAR(buffer);
  buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buffer.memory = V4L2_MEMORY_MMAP;

  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if(-1 == xioctl (fd, VIDIOC_STREAMON, &type))
    cerr << "VIDIOC_STREAMON" << endl;
}

void open()
{
  fd = open(camera, O_RDWR | O_NONBLOCK, 0);
  if(fd == -1)
  {
    cerr << "Erro no OPEN " << endl;
  }
}

void wait()
{

  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  struct timeval tv = {0};
  tv.tv_sec = 1;

  //double time0 = clock();
  int r = select(fd+1, &fds, NULL, NULL, &tv);
  //cout << "Tempo decorrido " << clock() - time0 << endl;
  if(r == 0 )
    cerr << "Sem atividade" << endl;
  if(-1 == r){
      cerr << "Waiting for Frame" << endl;
  }


}

void config()
{
 int GAIN = 30;
 int EXPO_ABSOLUTE = 400;


  //GAIN
  struct v4l2_control control;
  control.id = V4L2_CID_GAIN;
  control.value = GAIN;
  if(-1 == xioctl(fd,VIDIOC_S_CTRL,&control))
  {
    cerr << "Config GAIN " << endl;
  }

  CLEAR(control);
  control.id = V4L2_CID_EXPOSURE_ABSOLUTE;
  control.value = EXPO_ABSOLUTE;
  if(-1 == xioctl(fd,VIDIOC_S_CTRL,&control))
  {
    cerr << "Config EXPO_ABSOLUTE " << endl;
  }
}

void save(char *nome)
{
  ofstream file;
  file.open(nome);

  if(file.is_open() == false)
  {
    cerr << "Erro no arquivo " << endl;
    return;
  }
  //cabecalho
  file << "P6\n";
  file << WIDTH << " " << HEIGHT << '\n';
  file << 255 << '\n';
  //fim cabecalho
  if(imgGBRG == NULL)cout << "Ponteiro NULL" << endl;
  int offset = 0;
  for(unsigned lin = 0; lin < HEIGHT; lin ++)for(unsigned col = 0; col < WIDTH; col ++)
  {
    if( (lin%2 == 0) && (col%2 != 0) )//azul
      file << (unsigned char)0 << (unsigned char)0 << *(imgGBRG+offset);
    else if( (lin%2 != 0) && (col%2 == 0) )//vermelho
      file << *(imgGBRG+offset) << (unsigned char)0 << (unsigned char)0;
    else//verde
      file << (unsigned char)0 << *(imgGBRG + offset) << (unsigned char)0;

    //cout << "offset " << offset << endl;
    offset++;
  }

  file.close();
}
