#ifndef _MODELO_H_
#define _MODELO_H_

#include "../data.h"
#include "../parameters.h"

// O passo de integração (s)
#define PASSO_INTEGRACAO 0.0001

// Parametros do modelo dinamico

// Velocidade angular maxima dos motores (em rad/s = PI*rpm/30)
#define VEL_MOTOR 75
// Raio das duas rodas (em metros)
#define RAIO_RODA 0.015
// Massa do robo (em kg)
#define MASSA_ROBO 0.6
// Para efeito de calculo do momento de inercia, supõe-se que a massa
// do robo esta distribuida em um circulo em torno do ponto central.
// O raio deste circulo pode variar de 0 (0%) a LADO_ROBO/2 (100%). A
// constante AFAST_MASSA, que varia de 0.0 a 1.0, da este percentual.
// O momento de inercia e diretamente proporcional a AFAST_MASSA.
#define AFAST_MASSA 0.9
// Constante de tempo do movimento linear do robo (em seg)
#define CONST_TEMPO_LIN 2.0
// Massa da bola (em kg)
#define MASSA_BOLA 0.4
// Constante de tempo da bola (em seg)
#define CONST_TEMPO_BOLA 0.8

enum MOTOR{
  MOTOR_ESQUERDO,
  MOTOR_DIREITO
};

struct VERTICES {
  Coord2 centro;
  Coord2 vert[4];
};

struct CORRECAO {
  Coord2 colisao, erro;
  double plano;
};

// O vetor de estados do robo é composto por:
// v (velocidade linear do robô)
// w (velocidade angular do robô)
// x (coordenada x)
// y (coordenada y)
// theta (orientação)
// Os sinais de entrada são:
// alphad (velocidade angular normalizada [-1 a 1] p/ roda direita)
// alphae (velocidade angular normalizada [-1 a 1] p/ roda esquerda)
struct EST_ROBO: public Coord3
{
  double v,w;
  double alphad,alphae; 
};

// O vetor de estados da bola é composto por:
// v (velocidade linear da bola)
// x (coordenada x)
// y (coordenada y)
// theta (direção do movimento)

struct EST_BOLA: public Coord3
{
  double v;
};

typedef struct
{
  EST_ROBO azul[3];
  EST_ROBO amrl[3];
  EST_BOLA bola;
} ESTADO;

class Modelo {
 private:
  //As constantes do modelo
  // Velocidades linear e angular máxima do robô (em m/s e rad/s)
  double VLinMax,VAngMax;
  // Massa e momento de inércia do robô (em kg e kg.m2)
  double MassaRobo,MomentoRobo;
  // Constantes de tempo dos movimentos linear e angular (em seg)
  double TLin,TAng;
  // Massa da bola (em kg)
  double MassaBola;
  // Constante de tempo do movimento da bola (em seg)
  double TBola;
  //Dados privados
  EST_ROBO azul[3]; 
  EST_ROBO amrl[3];
  EST_BOLA bola;
  double tempo_simul;
  //Funções auxiliares
  void colisao_bola_paredes(const CORRECAO &correcao);
  void colisao_bola_robo(TEAM team, int id, const CORRECAO &correcao);
  void colisao_robo_paredes(TEAM team, int id, const CORRECAO &correcao);
  void colisao_robo_robo(TEAM team1, int id1, TEAM team2, int id2,
			 const CORRECAO &correcao);
 public:
  Modelo();               // Construtor
  //  ~Modelo();          // Destrutor
  void simular(double dt);// Simula um passo de amostragem
  void avancar_sem_simular(double dt);
  void posicao_inicial(); // Reseta os robôs para a posição inicial
  void atualizar_controle(TEAM,int,MOTOR,double); //(team,robo,motor,tensãoe)
  void le_posicao(POSICAO&);
  inline double le_tempo() {return tempo_simul;}
  void set_posicao(const POSICAO&,double,double);  // Recebe um tipo POSICAO
  void set_posicao_robo(TEAM,int,double,double,double); // (x,y,theta)
  void set_posicao_bola(double,double);  // (x,y)
  void set_tempo(double);  // (tempo)
};

#endif
