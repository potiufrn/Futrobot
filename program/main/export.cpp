#include "export.h"
#include <stdio.h>



Semaphore::Semaphore (key_t key, unsigned int rsc, unsigned int perm) {
  //ajustando valores do union do semaforo para oper. do semaforo
  semdo.sem_num = 0;
  semdo.sem_flg = SEM_UNDO;

  semid = semget (key, 1, IPC_CREAT| perm/*|IPC_EXCL|*/);
  if (semid < 0) {
    //nao conseguiu criar semaforo
    printf ("[Export]:Nao foi possivel criar semaforo!\n");
  }
  //definindo valor inicial do semaforo
  semdat.val = rsc;
  semctl (semid, 0, SETVAL, semdat);
}

int Semaphore::P() {
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

Sharedmem::Sharedmem(key_t key, size_t bytes, bool rdonly, unsigned int perm):
  ptmem(NULL),
  smeid(0),
  readonly(rdonly),
  ready(false)
{
  smeid = shmget (key, bytes, perm|IPC_CREAT);
  if (smeid <= 0) {
    //nao conseguiu criar memoria
    printf("[Export]:Nao foi possivel criar mem. comp.!\n");
  }
}

void Sharedmem::attach() {
   if (readonly) {
     ptmem = shmat (smeid, (void *)0, SHM_RDONLY);
  } else {
     ptmem = shmat (smeid, (void *)0, SHM_RND);
  }
   ready = true;
}


Sharedmem::~Sharedmem(){
  shmdt (ptmem);
  shmctl (smeid, IPC_RMID, 0);
}

Export::Export (TEAM team, SIDE side, GAME_MODE mode):
  FutData (team,side, mode),
  smeid(0),
  pt (NULL),
  count (0)
{
  //SEMAFORO
  semdo.sem_num = 0;
  semdo.sem_flg = SEM_UNDO;

  semid = semget ((myTeam()==YELLOW_TEAM)? KEY_SEM_YELLOW : KEY_SEM_BLUE, 1, IPC_CREAT| 0666);
  if (semid < 0) {
    //nao conseguiu criar semaforo
    printf ("[Export]:Nao foi possivel criar semaforo!\n");
  }
  //definindo valor inicial do semaforo
  semdat.val = 1;
  semctl (semid, 0, SETVAL, semdat);

  //MEMORIA
  smeid = shmget ((myTeam()==YELLOW_TEAM)? KEY_MEM_YELLOW : KEY_MEM_BLUE, sizeof(PACKAGEDAT), 0666|IPC_CREAT);
  if (smeid <= 0) {
    //nao conseguiu criar memoria
    printf("[Export]:Nao foi possivel criar mem. comp.!\n");
  }

  //Attach  nao pode ser feito agora porque o
  //futrobot roda em outra thread
  //pt = (PACKAGEDAT *) shmat (smeid, (void *) 0, SHM_RND);

}

Export::~Export (){
  //sem.~Semaphore();
   semctl (semid, 0, IPC_RMID);

  // sme.~Sharedmem();
   shmdt (pt);
   shmctl (smeid, IPC_RMID, 0);
}

// bool Export::exxport(const PxRGB *data)
bool Export::exxport()
{

  //INICIO REGIAO CRITICA
  semdo.sem_op = -1;
  semop (semid, &semdo, 1);

  //////////////////////
  //LEITURA NA MEMORIA//
  //////////////////////

   setAdvantage(pt->adv);
   //ret_game contem o comando da interface
   //
   if (pt->game != pt->ret_game) {
     setGameState (pt->ret_game);
   }

   //////////////////////
   //ESCRITA NA MEMORIA//
   //////////////////////
   pt->side = mySide();
   pt->team = myTeam();

   //POSITION NA MAO
   pt->pos.me[0].x() = pos.me[0].x();
   pt->pos.me[0].y() = pos.me[0].y();
   pt->pos.me[0].theta() = pos.me[0].theta();
   pt->pos.me[1].x() = pos.me[1].x();
   pt->pos.me[1].y() = pos.me[1].y();
   pt->pos.me[1].theta() = pos.me[1].theta();

   pt->pos.me[2].x() = pos.me[2].x();
   pt->pos.me[2].y() = pos.me[2].y();
   pt->pos.me[2].theta() = pos.me[2].theta();

   pt->pos.op[0].x() = pos.op[0].x();
   pt->pos.op[0].y() = pos.op[0].y();
   pt->pos.op[0].theta() = pos.op[0].theta();

   pt->pos.op[1].x() = pos.op[1].x();
   pt->pos.op[1].y() = pos.op[1].y();
   pt->pos.op[1].theta() = pos.op[1].theta();

   pt->pos.op[2].x() = pos.op[2].x();
   pt->pos.op[2].y() = pos.op[2].y();
   pt->pos.op[2].theta() = pos.op[2].theta();

   pt->pos.ball.x() = pos.ball.x();
   pt->pos.ball.y() = pos.ball.y();
   pt->vel_ball = pos.vel_ball;

   //PAPEIS:TEM LEVAR PAPEIS PARA INTERFACE
   pt->roles = papeis;

  // //REFS
   pt->ref.me[0].x()=ref.me[0].x();
   pt->ref.me[0].y()=ref.me[0].y();
   pt->ref.me[0].theta()=ref.me[0].theta();

   pt->ref.me[1].x()=ref.me[1].x();
   pt->ref.me[1].y()=ref.me[1].y();
   pt->ref.me[1].theta()=ref.me[1].theta();

   pt->ref.me[2].x()=ref.me[2].x();
   pt->ref.me[2].y()=ref.me[2].y();
   pt->ref.me[2].theta()=ref.me[2].theta();

  //PWMs
   pt->pwm.me[0].left = pwm.me[0].left;
   pt->pwm.me[0].right =pwm.me[0].right;

   pt->pwm.me[1].left = pwm.me[1].left;
   pt->pwm.me[1].right =pwm.me[1].right;

   pt->pwm.me[2].left = pwm.me[2].left;
   pt->pwm.me[2].right =pwm.me[2].right;

  //BLOQUEADOS
   pt->block0 = bloqueado[0];
   pt->block1 = bloqueado[1];
   pt->block2 = bloqueado[2];

  // //GAMES STATES
   pt->game = gameState();
   pt->ret_game = gameState();

  // memcpy((void*) pt->imagem, (void *) data, sizeof(PxRGB)*640*480);  

  //IDENTIFICACAO DO QUADRO
     pt->id = count;

  //FIM REGIAO CRITICA
  semdo.sem_op = 1;
  semop (semid, &semdo, 1);
  count= count+1;
  //printf (pos robo 0 x: %f y: %f \n", pos.me[0].x(), pos.me[0].y());
  return false;
}

 void Export::initialize() {
   //Attach
  pt = (PACKAGEDAT *) shmat (smeid, (void *) 0, SHM_RND);

  //VALORES INICIAIS DA MEMORIA
  if (pt == NULL) {
    printf ( "ERROR: pointer points to NULL!!\n");
  } else {
    pt->ret_game = PAUSE_STATE;
    pt->game = PLAY_STATE;
    pt->adv = true;
  }
}
