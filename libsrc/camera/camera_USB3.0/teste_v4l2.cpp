#include <time.h>
#include <cstdlib> //exit()

#include <errno.h>

#include "camera.h"
#include "../../../program/system.h"

using namespace std;

class TesteCam:public Camera
{
public:
  TesteCam(unsigned index = 0):Camera(index){this->capturando = true;}

  inline bool capture(){ return Camera::captureimage(); }
  inline bool wait(){return Camera::waitforimage(); }
  inline void save(const char* arq){ ImBruta.save(arq); }
  inline void toirgb(ImagemRGB &dest){ Camera::toRGB(dest); }

  PxRGB getRGB(unsigned lin,unsigned col) { return ImBruta.getRGB(lin,col); } ;

};

//V4L2_CID_MIN_BUFFERS_FOR_CAPTURE

int main(){
  //TesteCam cam(1);
  struct v4l2_queryctrl queryctrl;
  int fd = open("/dev/video1", O_RDWR | O_NONBLOCK , 0);  
  memset(&queryctrl, 0, sizeof(queryctrl));

  queryctrl.id = V4L2_CTRL_FLAG_NEXT_CTRL;
  while (0 == ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
      if (!(queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)) {
          std::cout << "Control "<<queryctrl.name << '\n';
          std::cout << "Min - Max "<<queryctrl.minimum << ' ' << queryctrl.maximum << '\n';
          if (queryctrl.type == V4L2_CTRL_TYPE_MENU)
              std::cout << "Menu" << '\n';
          std::cout << "id "<<queryctrl.id << '\n';
      }
      queryctrl.id |= V4L2_CTRL_FLAG_NEXT_CTRL;
  }
  if (errno != EINVAL) {
      perror("VIDIOC_QUERYCTRL");
      exit(EXIT_FAILURE);
  }

  return 0;
}
