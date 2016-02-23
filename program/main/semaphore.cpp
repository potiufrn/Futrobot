#include "export.h"
#include <stdio.h>

//using namespace::std;

Semaphore::Semaphore (key_t key, unsigned int rsc) {
  //ajustando valores do union do semaforo para oper. do semaforo
  semdo.sem_num = 0;
  semdo.sem_flg = SEM_UNDO;
  
  semid = semget (key, 1, IPC_CREAT|0600/*|IPC_EXCL|*/);
  if (semid < 0) {
    //nao conseguiu criar semaforo
    printf ("[Export]:Nao foi possivel criar semaforo!\n");
  }
  //definindo valor inicial do semaforo
  semdat.val = rsc;
  semctl (semid, 0, SETVAL, semdat); 
}

int Semaphore:: P() {
  semdo.sem_op = -1;
  return (semop (semid, &semdo, 1));
}

int Semaphore::V() {
  semdo.sem_op = 1;
  return (semop (semid, &semdo, 1));
}

Semaphore::~Semaphore (){
  //Destruir semaforo
  semctl (semid, 0, IPC_RMID);
}
