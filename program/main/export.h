#ifndef _EXPORT_H_
#define _EXPORT_H_


#include "futdata.h"
#include <imagem.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>


//utilizar CONFIG REFERENCES...
#include "../data.h"
#include "../comunicacao.h"


// Nao foi possivel incluir definicoes de acquisition.cpp
//entao foi definido aqui
//#define IMAGE_WIDTH 640 
//#define IMAGE_HEIGHT 480



/* struct PACKAGEDAT { */
/*   // PxRGB matimg[IMAGE_HEIGHT][IMAGE_WIDTH]; */
/*   CONFIG posicao; */
/*   REFERENCES refer; */
/*   PWM_ROBOTS pwm; */
/*   PAPEIS pap[3]; */
/* }; */


//tipos necessarios para operacao com semaforo 

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
  Semaphore (key_t key, unsigned int rsc, unsigned int perm);
  int P();
  int V();
  ~Semaphore();

};

class Sharedmem {
 private:
  void * ptmem;
  void * atadr;
  int smeid;
  bool readonly;
  bool ready;
  

 public:
  Sharedmem (key_t key, size_t bytes, bool rdonly, unsigned int perm);
  inline void * pttomem () {
    if (!ready)
      printf("[Sharedmem]: Warning pointer without previous attachment!\n");
    return ptmem;
  };

  void attach();

  ~Sharedmem();

};



/*********************************************************/
/*Classe para exportacao de imagem e dados para interface*/
/*********************************************************/            
class Export:
public virtual FutData{
 private:
  //Semaphore sem;
  int semid;
  sembuf semdo;
  semnum semdat;

  //memoria compartilhada
  int smeid;
  PACKAGEDAT * pt;

  //para identificacao do quador
  long unsigned count;
 public:
  Export(TEAM team, SIDE side, GAME_MODE mode);
  ~Export();
  bool exxport(const PxRGB * data);
  // bool metodo() {return true;};
  void initialize(); 
};


#endif
