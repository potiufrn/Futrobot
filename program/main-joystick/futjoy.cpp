#include "futjoy.h"
#include <errno.h>
#include <cstring>

//-----------------------------------------------------------------------------
FutJoy::FutJoy(){
  
  axis=NULL;
  button=NULL;

  num_of_axis=0;
  num_of_buttons=0;
  
  js_axis_order = UNKNOW;

  connected = false;
}  
//-----------------------------------------------------------------------------
FutJoy::~FutJoy(){
}

FutJoy::FutJoy(const FutJoy &a)
{
  *this = a;
}

FutJoy& FutJoy::operator= (const FutJoy & a)
{
  if(this != &a){
    
    js = a.js;
    joy_fd = a.joy_fd;
    x = a.x;
    
    if(num_of_axis != a.num_of_axis){
      delete axis;
      axis = new int[a.num_of_axis];
      num_of_axis = a.num_of_axis;
    }
  
    if(num_of_buttons != a.num_of_buttons){
      delete button;
      button = new int[a.num_of_buttons];
      num_of_buttons = a.num_of_buttons;
    }
    for(int i=0;i<num_of_axis;i++){
      axis[i] = a.axis[i];
    }
    for(int i=0;i<num_of_buttons;i++){
      button[i] = a.button[i];
    }
    for(int i=0;i<80;i++){
      name_of_joystick[i] = a.name_of_joystick[i];
    }
  }
  return (*this);
}


//-----------------------------------------------------------------------------
int 
FutJoy::initialize(const char* js_addr){

  if( ( joy_fd = open( js_addr , O_RDONLY)) == -1 ){
    //printf( "Couldn't open joystick\n" );
    return 1;
  }
  
  ioctl( joy_fd, JSIOCGAXES, &num_of_axis );
  ioctl( joy_fd, JSIOCGBUTTONS, &num_of_buttons );
  ioctl( joy_fd, JSIOCGNAME(80), &name_of_joystick );
  
  if(strcmp(name_of_joystick,"USB Gamepad ") == 0){
    js_axis_order = LIN1_ANG4;
  } else if(strcmp(name_of_joystick,"DragonRise Inc.   Generic   USB  Joystick  ") == 0){
    js_axis_order = LIN1_ANG3;
  } else{
    js_axis_order = UNKNOW;
  }

  axis = new int[num_of_axis];
  button = new int[num_of_buttons];

  printf("Joystick detected:\n%s\n%d axis, %d buttons\n"
	 , name_of_joystick
	 , num_of_axis
	 , num_of_buttons );
  
  fcntl( joy_fd, F_SETFL, O_NONBLOCK );	/* use non-blocking mode */
  
  connected = true;
  return 0;
}
//-----------------------------------------------------------------------------
int 
FutJoy::shutdown(){
  
  delete axis;
  delete button;
  
  close( joy_fd );

  connected = false;
  return 0;
}
//-----------------------------------------------------------------------------
int 
FutJoy::readjs(int &vel_lin, int &vel_ang)
{
  
  while (read(joy_fd, &js, sizeof(struct js_event)) > 0) {
    /* see what to do with the event */
    switch (js.type & ~JS_EVENT_INIT)
      {
      case JS_EVENT_AXIS:
	axis   [ js.number ] = js.value;
	break;
      case JS_EVENT_BUTTON:
	button [ js.number ] = js.value;
	break;
      }
  }
  /* EAGAIN is returned when the queue is empty */
  if (errno != EAGAIN) {
    printf( "Error reading event from joystick!\n" );
    shutdown();
    return 1;
  }
  switch(js_axis_order){
  case LIN1_ANG2:
    vel_lin = -axis[1];
    vel_ang = -axis[2];
    break;
  case LIN1_ANG3:
    vel_lin = -axis[1];
    vel_ang = -axis[3];
    break;
  case LIN1_ANG4:
    vel_lin = -axis[1];
    vel_ang = -axis[4];
    break;
  case UNKNOW:
  default:
    vel_lin = -axis[1];
    vel_ang = -axis[0];
    break;
  }
  
  return 0;
}  

