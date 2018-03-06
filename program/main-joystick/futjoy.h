#ifndef _FUTJOY_H_
#define _FUTJOY_H_

#include <iostream>
#include <fstream>

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

using namespace std;

#define JOY_DEV0 "/dev/input/js0"
#define JOY_DEV1 "/dev/input/js1"
#define JOY_DEV2 "/dev/input/js2"


class FutJoy {
  
 private:
  enum JS_AXIS_ORDER{
    UNKNOW,
    LIN1_ANG2,
    LIN1_ANG3,
    LIN1_ANG4
  };

  struct js_event js;
  int joy_fd;
  int *axis;
  int num_of_axis;
  int num_of_buttons;
  int x;
  int *button;
  char name_of_joystick[80];
  bool connected;
  JS_AXIS_ORDER js_axis_order;

 public:
  FutJoy();
  FutJoy(const FutJoy &a);
  ~FutJoy();
  FutJoy & operator= (const FutJoy & a);
  
  inline bool isConnected(){ return connected; };
  int initialize(const char* js_addr = JOY_DEV0);
  int shutdown();
  int readjs(int &vel_lin, int &vel_ang);
};

#endif //#ifndef _FUTJOY_H_

