#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "futdata.h"

using namespace std;

//################definicoes do Controle############
// Distância na qual se considera que o robô atingiu o alvo
#define EPSILON_L 0.01
// Distância na qual se considera que o robô não mais está no alvo
#define DELTA_L (2*EPSILON_L)


// Distância a partir da qual o controle de orientação começa a atuar
#define DIST_ORIENT 0.15
// Valor mínimo do PWM abaixo do qual o robô não anda...
//Equipe Jerimum
//#define PWM_MINIMO 0.10
//Equipe Poti
// #define PWM_MINIMO 0.08


// valor abaixo do qual o sinal enviado do pwm eh zero
//Equipe Poti
#define PWM_ZERO (1.0/255.0)

// Ordem e atraso do modelo do robô identificado
#define ORDEM 2
#define ATRASO 3

class PID {
private:
  double K, Ti, Td, N;
  double e_ant, I_ant, I_ant2, D_ant;
public:
  // A relação com as constantes tradicionais do PID é a seguinte:
  // Kp=k, Ki=k/ti, Kd=k*td
  // Para eliminar a parte integral (Ki=0), ti=infinito
  // Para eliminar a parte derivativa (Kd=0), td=0.0
  // O número n é o limitador de altas freqüencias da parte derivativa.
  // Normalmente, 3<=n<=20. Para eliminar a filtragem, n=infinito
  inline PID(double k=85.0, double ti=0.44/*1E+6*/, double td=0.11/*0.0*/, double n=1E+6) {
    fixa_constantes(k,ti,td,n);
  }
  void fixa_constantes(double k, double ti, double td, double n=1E+6);
  void anti_windup();
  void reset();
  double controle(double erro, double h);
};

// PREDITOR DE SMITH

// Modelo identificado
//         (a*z + b)
// G(z) = ------------ * z^(-atraso)
//        (z-1)(z + c)

// Modelo em Equação a Diferença
// y(k) = -(c-1)*y(k-1) + c*y(k-2) + a*u(k-1-atraso) + b*u(k-2-atraso);

class PreditorSmith
{
  private:
    bool enable;
    double a, b, c;
    double predicao_old;
    double  y[ORDEM + 1],  u[ORDEM + 1];            // Sem atraso
    double ya[ORDEM + 1], ua[ORDEM + 1 + ATRASO];   // Com atraso

  public:
    inline PreditorSmith(double a=0.0, double b=0.0, double c=0.0){
        fixa_coeficientes(a, b, c);
        reset();
        enabled(true);
    };

    double predicao(double u);
    double getPredicao();
    void fixa_coeficientes(double a, double b, double c);
    void reset();
    void enabled(bool en);

    ~PreditorSmith();
};

class Control : public virtual FutData
{
private:
  bool controle_orientacao;
  PID lin[3],ang[3];
  PreditorSmith pslin[3], psang[3];
  bool chegou[3];
  int sentidoGiro[3];
public:
  Control(TEAM team, SIDE side, GAME_MODE gameMode);
  ~Control();
  bool control();
  bool stop_control();
};

#endif
