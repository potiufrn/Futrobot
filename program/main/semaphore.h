#ifndef _EXPORT_H_
#define _EXPORT_H_


#include "futdata.h"
#include <imagem.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

//utilizar o tipo PAPEL
#include "strategy.h"

//utilizar CONFIG REFERENCES...
#include "../data.h"

#define KEY_SEM 752478
#define KEY_MEM 138912

union semnum {
  int val;
  struct semid_ds * buf;
  unsigned short * array;
};
/*
struct sembuf {
  unsigned short sem_num;
  short sem_op;
  short sem_flg;
};
*/  

class Semaphore {
  int semid;
  sembuf semdo;
  semnum semdat;
 public:
  Semaphore (key_t key, unsigned int rsc);
  int P();
  int V();
  ~Semaphore();

};

public virtual FutData{
 private:
  Semaphore sem;
  // Semaphore * sem_exp;
  Imagem * img_exp;
  PACKAGEDAT * pac_exp;

  bool config;

  Imagem * src_img;
  PAPEIS * src_pap;
  double * tempor[9];
  PACKAGEDAT pac;
 protected:
  bool getpointers (Imagem * ptimg, PAPEIS * ptpap);
  bool getempt(double *a, double *b, double *c, double *d, double *e,
		       double *f, double *g, double *h, double *i);
 public:
  bool configexport (PACKAGEDAT * p, Imagem * i);
  Export(TEAM team, SIDE side, GAME_MODE mode);
  ~Export();
  bool exxport();
  /// bool metodo() {return true;};

};

#endif
