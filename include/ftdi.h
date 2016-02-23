
#ifndef _FTDI_H_
#define _FTDI_H_

using namespace std;

#include <iostream>
#include <cstdlib>
#include <cstring>
#include<unistd.h>
/*
// FTDI BAUD RATES

#define FT_BAUD_300             300
#define FT_BAUD_600             600
#define FT_BAUD_1200            1200
#define FT_BAUD_2400            2400
#define FT_BAUD_4800            4800
#define FT_BAUD_9600            9600
#define FT_BAUD_14400           14400
#define FT_BAUD_19200           19200
#define FT_BAUD_38400           38400
#define FT_BAUD_57600           57600
#define FT_BAUD_115200          115200
#define FT_BAUD_230400          230400
#define FT_BAUD_460800          460800
#define FT_BAUD_921600          921600
*/


      #include <sys/types.h>
      #include <sys/stat.h>
      #include <fcntl.h>
      #include <termios.h>
      #include <stdio.h>
        



//      #define BAUDRATE B38400
//      #define BAUDRATE B9600
      #define BAUDRATE B57600
      #define MODEMDEVICE "/dev/ttyUSB0"
      /* #define _POSIX_SOURCE 1 /\* POSIX compliant source *\/ */
      #define FALSE 0
      #define TRUE 1
        


class FTDI {

 private:

  bool isopen;
  int bytes_read;
  int bytes_write;
  
  int fd, c, res;
  struct termios oldtio,newtio;

 public:

  FTDI();
  ~FTDI();
  int ftdi_open();
  int ftdi_close();
  int  ftdi_read(unsigned char *buffer, size_t size);
  int  ftdi_write(unsigned char *buffer, size_t size);
};

#endif
