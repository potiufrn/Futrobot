/*
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
*/
#include <iostream>

#include <camera.h>

using namespace std;

int main()
{

  Camera cam(1);
  unsigned N = 2;
  cam.capturando = true;

  for(unsigned i = 0; i < N; i++)
  {
    cam.waitforimage();
    cam.captureimage();
  }

  cam.imgBruta.save("CamSaveTeste.ppm");
  return 0;
}
