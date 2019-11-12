#include "common.h"

#include <time.h>
#include <sys/time.h>
#include <string.h>

void bits2float(const uint8_t *bitstream, float*f, uint32_t num_float)
{
  memcpy((float*)bitstream, f, num_float*sizeof(float));
}

void decodeFloat(const uint8_t *data, float *fa, float *fb)
{
  uint16_t ref[2];
  uint8_t  sense[2]; // 0 -> back, 1 -> front

  sense[LEFT]  = (data[1] & 0x80) >> 7;
  sense[RIGHT] = (data[3] & 0x80) >> 7;
  ref[LEFT]    = ((data[1] << 8) | data[2]) & 0x7FFF;
  ref[RIGHT]   = ((data[3] << 8) | data[4]) & 0x7FFF;

  *fa  = (ref[LEFT]/32768.0)*(2.0*sense[LEFT]-1.0);
  *fb  = (ref[RIGHT]/32768.0)*(2.0*sense[RIGHT]-1.0);
}

double get_time_sec()
{
  static struct timeval tbase={-1,-1};
  struct timeval t;

  gettimeofday(&t,NULL);
  if (tbase.tv_sec==-1 && tbase.tv_usec==-1)
    {
      tbase = t;
    }
  return ( t.tv_sec-tbase.tv_sec + (t.tv_usec-tbase.tv_usec)/1000000.0 );
}
