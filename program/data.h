#ifndef _DATA_H_
#define _DATA_H_

#include <imagem.h>
#include "parameters.h"//IMAGERWIDTH e HEIGHT

#define NUM_POINTS 28
#define POSITION_UNDEFINED -9999

//Se voce so quiser usar o simulador, pode descomentar a linha abaixo
//e nao sera necessario instalar as bibliotecas de acesso a camera
//firewire e de acesso ao dispositivo USB-Serial
//#define _SO_SIMULADO_

// Lados do Campo
enum SIDE {
  RIGHT_SIDE, LEFT_SIDE
};

// Cor do time
enum TEAM {
  BLUE_TEAM, YELLOW_TEAM
};

// Modo de jogo real ou simulado
enum GAME_MODE {
  REAL_MODE, SIMULATED_MODE
};

struct VELOCITY {
  double mod;
  double ang;
};

struct POS_BOLA: public Coord2 {
  inline POS_BOLA(): Coord2() {}
  inline POS_BOLA(const Coord3 &C): Coord2(C.x(), C.y()) {}
  inline bool undef() const {
    return(x() == POSITION_UNDEFINED || y() == POSITION_UNDEFINED);
  }
};

struct POS_ROBO: public Coord3 {
  inline bool undef() const {
    return(x() == POSITION_UNDEFINED || y() == POSITION_UNDEFINED);
  }
};


struct TPOS_ROBO
{
  POS_ROBO p0,p1,p2;
  inline TPOS_ROBO():
    p0(),p1(),p2() {}
  inline TPOS_ROBO(const POS_ROBO &P0, const POS_ROBO &P1, const POS_ROBO &P2):
    p0(P0),p1(P1),p2(P2) {}
  inline const POS_ROBO operator[](unsigned i) const {
    return (i==0 ? p0 : (i==1 ? p1 : p2) );
  }
  inline POS_ROBO &operator[](unsigned i) {
    return (i==0 ? p0 : (i==1 ? p1 : p2));
  }
};

struct CONFIG {
  TPOS_ROBO me;
  TPOS_ROBO op;
  POS_BOLA ball;
  POS_BOLA future_ball;
  VELOCITY vel_ball;
};

// Posições de referência
struct REFERENCES {
  TPOS_ROBO me;
};

// Ciclo de trabalho (percentual de nível alto no PWM) das tensões dos
// motores das rodas do robô (valores entre +1 e -1)
struct PWM_WHEEL {
  double left, right;
};

struct TPWM_WHEEL{
 private:
  PWM_WHEEL x0, x1, x2;
 public:
  inline const PWM_WHEEL operator[] (unsigned i) const {
    return (i==0 ? x0 : (i==1 ? x1 : x2) );
  }
  inline PWM_WHEEL &operator[] (unsigned i) {
    return (i==0 ? x0 : (i==1 ? x1 : x2));
  }
};


struct PWM_ROBOTS {
  TPWM_WHEEL me;
};

/**
 * Variaveis necessarias para a simulacao
 */
enum GAME_STATE {
  FINISH_STATE,   //0
  PAUSE_STATE,    //1
  PENALTY_STATE,  //2
  FREEKICK_STATE, //3
  GOALKICK_STATE, //4
  FREEBALL_STATE, //5
  INICIALPOSITION_STATE, //6
  CELEBRATION_STATE,     //7
  PLAY_STATE //8
};
//Variaveis necessarias para a simulacao


/**
 * O contador de quadros (para sincronizacao)
 */
typedef long unsigned IDQUADRO;

/**
 * Dado utilizado para comunicacao com o simulador
 */
struct POSICAO {
  TPOS_ROBO azul;
  TPOS_ROBO amrl;
  POS_BOLA bola;
};

//Dado utilizado para classe exportacao
struct POSITION {
  TPOS_ROBO me;
  TPOS_ROBO op;
  POS_BOLA ball;
};

//Dado utilizado para comunicacao com o simulador
struct PLACAR {
  unsigned azul,amrl;
};

//Dado utilizado para classe exportacao
struct SCORE {
  unsigned me,op;
};

//Dado utilizado para comunicacao com o simulador
struct SITUACAO  {
  IDQUADRO id;
  POSICAO pos;
};

struct SINAL_RADIO {
  IDQUADRO id;
  PWM_ROBOTS c;
};

enum FUNCTION {
  GOLEIRO, COM_BOLA, SEM_BOLA
};

enum ACTION {
  // As acoes que servem para todos os estados
  ESTACIONAR=1,
  // As acoes que servem para sem_bola e com_bola
  IR_MEIO_CAMPO=11, FORMAR_BARREIRA=12, CIRCULO_CENTRAL=13, COMEMORAR=14,
  // As acoes que servem para goleiro e com_bola
  ISOLAR_BOLA=21, LADO_AREA=22, IR_BOLA=23, POS_PENALTY1=24, POS_PENALTY2=25,
  // As acoes do goleiro
  G_DEFENDER=101, G_CENTRO_GOL=102,
  // As acoes do jogador com bola
  A_IR_MARCA=201, A_FREE_BALL=202, A_BOLA_CENTRO=203,
  A_DESCOLAR=204, A_POSICIONAR_PARA_DESCOLAR=205,
  A_POSICIONAR_FRENTE_AREA=206,
  A_CONTORNAR=208, A_CONTORNAR_POR_DENTRO=209,
  A_ALINHAR_GOL=210, A_ALINHAR_SEM_ORIENTACAO=211, A_CHUTAR_GOL=212,
  // As acoes do jogador sem bola
  D_NAO_ATRAPALHAR=301,
  // As acoes impossiveis
  IMPOSSIVEL=-1, NAO_DEFINIDO=0

};

struct ROLE_JOGADOR {
  FUNCTION funcao;
  ACTION acao;
};

struct TROLE_JOGADOR{
 private:
  ROLE_JOGADOR x0, x1, x2;
 public:
  inline const ROLE_JOGADOR operator[] (unsigned i) const {
    return (i==0 ? x0 : (i==1 ? x1 : x2) );
  }
  inline ROLE_JOGADOR &operator[] (unsigned i) {
    return (i==0 ? x0 : (i==1 ? x1 : x2));
  }
};

struct ROLES {
  TROLE_JOGADOR me;
};

struct REFS {
  TPOS_ROBO me;
};

/* //ESTRUTURA utilizada na exportacao de dados para interface */
struct PACKAGEDAT {
  /* sentido FutRobot -> interface */
  SIDE side;
  TEAM team;
  IDQUADRO id;//FALTA Este
  POSITION pos;
  VELOCITY vel_ball;
  ROLES roles;
  REFS ref;
  PWM_ROBOTS pwm;
  bool block0, block1, block2;

  //Imagem
  // PxRGB imagem[IMAGE_WIDTH][IMAGE_HEIGHT];
  // ImagemBruta *imgBruta;


  GAME_STATE game;

  /*sentido interface -> FutRobot*/
  //Vantagem
  bool adv;
  GAME_STATE ret_game;
};

#endif
