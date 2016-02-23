#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <sys/sem.h>
#include <sys/time.h>
#include "system.h"

static struct termios stored;

// Lê o tempo atual em segundos
double relogio(void)
{
  static struct timeval tbase={-1,-1};
  struct timeval t;

  gettimeofday(&t,NULL);
  if (tbase.tv_sec==-1 && tbase.tv_usec==-1)
    {
      tbase = t;
    }
  return( t.tv_sec-tbase.tv_sec + (t.tv_usec-tbase.tv_usec)/1000000.0 );
}

// Decrementar um semáforo
int entrar_regiao_critica(int sem)
{
  struct sembuf x;
  x.sem_num = 0;
  x.sem_op = -1;
  x.sem_flg = 0;
  if (semop(sem,&x,1)==-1)
    {
      return(-1);
    }
  return(0);
}

// Incrementar um semáforo
int sair_regiao_critica(int sem)
{
  struct sembuf x;
  x.sem_num = 0;
  x.sem_op = +1;
  x.sem_flg = 0;
  if (semop(sem,&x,1)==-1)
    {
      return(-1);
    }
  return(0);
}

// Colocar o terminal no modo de ler teclas sem <ENTER> depois
void set_keypress(void)
{
  struct termios newterm;
  
  tcgetattr(0,&stored);
  
  memcpy(&newterm,&stored,sizeof(struct termios));
  
  /* Disable canonical mode, and set buffer size to 1 byte */
  newterm.c_lflag &= (~ICANON);
  newterm.c_cc[VTIME] = 10;
  newterm.c_cc[VMIN] = 0;
  
  tcsetattr(0,TCSANOW,&newterm);
  return;
}

// Restaurar o terminal ao seu modo normal
void reset_keypress(void)
{
  tcsetattr(0,TCSANOW,&stored);
  return;
}

int pega_caracter(void) {
  return getchar();
};

