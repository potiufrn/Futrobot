#include <time.h>
#include <cstdlib> //exit()
#include <errno.h>

#include "camera.h"
#include "../../../program/system.h"

using namespace std;
//
// class TesteCam:public Camera
// {
// public:
//   TesteCam(unsigned index = 0):Camera(index){this->capturando = true;}
//
//   inline bool capture(){ return Camera::captureimage(); }
//   inline bool wait(){return Camera::waitforimage(); }
//   inline void save(const char* arq){ ImBruta.save(arq); }
//   inline void toirgb(ImagemRGB &dest){ Camera::toRGB(dest); }
//
//   PxRGB getRGB(unsigned lin,unsigned col) { return ImBruta.getRGB(lin,col); } ;
//
// };

//V4L2_CID_MIN_BUFFERS_FOR_CAPTURE

#define CLEAR(A)  memset(&A, 0, sizeof(A))

int main(){
  //TesteCam cam(1);
  struct v4l2_queryctrl queryctrl;
  struct v4l2_capability cap;
  int fd = open("/dev/video1", O_RDWR | O_NONBLOCK , 0);

  #define GBRG V4L2_PIX_FMT_SGBRG8
  #define YUV422 V4L2_PIX_FMT_YUYV

  if (fd != -1)
   {
       struct v4l2_fmtdesc fmtdesc;
       CLEAR(fmtdesc);
       fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
       while (ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc) == 0)
       {
           printf("%s\n", fmtdesc.description);
           if(fmtdesc.pixelformat == GBRG)
             std::cout << "Suporta a GBRG" << '\n';
           if(fmtdesc.pixelformat == YUV422)
             std::cout << "Suporta a YUYV" << '\n';
           fmtdesc.index++;
       }
   }

  close(fd);
  return 0;
}
