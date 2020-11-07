#include <iostream>
#include "control.h"

#include "../parameters.h"
#include "../functions.h"

using namespace std;

void PID::fixa_constantes(double k, double ti, double td, double n)
{
  K = k;
  Ti = ti;
  Td = td;
  N = n;
  e_ant = I_ant = I_ant2 = D_ant = 0.0;
}

double PID::controle(double e, double h)
{
  double P = K * e;
  double I = I_ant + (K * h / Ti) * e;
  double D = (Td / (Td + N * h)) * D_ant + (K * Td * N / (Td + N * h)) * (e - e_ant);
  //  double D = (K*Td/h)*(e-e_ant);
  double u = P + D + I;
  I_ant2 = I_ant;
  I_ant = I;
  e_ant = e;
  D_ant = D;
  return u;
}

void PID::anti_windup()
{
  // Quando o robô saia de uma situação de bloqueio passava muito tempo girando
  // Isso deve ser porque o anti_windup não estava sendo efetivo
  // Vamos modificar para ao invés de parar de crescer o efeito integral, vamos zerar
  // I_ant = I_ant2;         // Suspende a Integração
  I_ant = I_ant2 = 0.0; // Zera a parte Integral
}

void PID::reset()
{
  I_ant = I_ant2 = 0.0;
  e_ant = 0.0;
  D_ant = 0.0;
}

Control::Control(TEAM team, SIDE side, GAME_MODE gameMode) : FutData(team, side, gameMode)
{
  // Quer ou não o controle de orientação
  //  controle_orientacao = false;
  controle_orientacao = true;

  /*
  double Totlin=1.0, Totang=0.5;

  double klin = 1.5;
  double tilin = 20.0;
  double tdlin = 0.0;

  double kang = 0.1;
  double tiang = 50.0;
  double tdang = 0.0;

  // Os sistemas (linear e angular) são descritos pela função de
  // transferência em malha aberta G(s)=V/(s(Ts+1)), onde V é a
  // velocidade máxima e T é constante de tempo.

  const double RAIO_RODA=0.017;
  const double VEL_ANG_RODA=1.0/RAIO_RODA;
  const double Vlin=VEL_ANG_RODA*RAIO_RODA;
  const double Vang=VEL_ANG_RODA*RAIO_RODA/(ROBOT_EDGE/2.0);
  const double CONST_TEMPO_LIN = 0.5;
  const double Tlin=CONST_TEMPO_LIN;
  const double AFAST_MASSA=0.5;
  const double Tang=pow2(AFAST_MASSA)*CONST_TEMPO_LIN;

  // O ganho do controlador proporcional (P) que torna o sistema mais
  // rápido e não introduz sobressinal é k=1/(4VT).

  klin=1/(4*Vlin*Tlin); tilin=1E+10; tdlin=0.0;
  kang=1/(4*Vang*Tang); tiang=1E+10; tdang=0.0;
  cout << "Sugestao 1: klin=" << klin << " tilin=" << tilin << " tdlin=" << tdlin
       << " kang=" << kang << " tiang=" << tiang << " tdang=" << tdang << endl;

  // Para o controlador proporcional-derivativo (PD), de forma a não
  // se ter sobressinal e se alterar a constante de tempo de T para
  // Tot, deve-se ter:

  Totlin=1.0; Totang=0.5;
  klin=1/(Vlin*Totlin); tilin=1E+10; tdlin=Tlin;
  kang=1/(Vang*Totang); tiang=1E+10; tdang=Tang;
  cout << "Sugestao 2: klin=" << klin << " tilin=" << tilin << " tdlin=" << tdlin
       << " kang=" << kang << " tiang=" << tiang << " tdang=" << tdang << endl;

  // Para o controlador proporcional-derivativo (PD), de forma a
  // se ter sobressinal de 5% (ksi=0,707) e se alterar a constante de
  // tempo de T para Tot, deve-se ter:

  Totlin=0.3333; Totang=0.1;
  klin=2*Tlin/(pow2(Totlin)*Vlin);
  tilin=5; tdlin=Totlin*(2*Tlin-Totlin)/(2*Tlin);
  kang=2*Tang/(pow2(Totang)*Vang);
  tiang=5; tdang=Totang*(2*Tang-Totang)/(2*Tang);
  cout << "Sugestao 3: klin=" << klin << " tilin=" << tilin << " tdlin=" << tdlin
       << " kang=" << kang << " tiang=" << tiang << " tdang=" << tdang << endl;

  // Se não quiser fazer nada disso, fixe as constantes na mâo!
  */

  //############## CONSTANTES DO CONTROLADOR #######################
  //################################################################

  // double klin = 0.6402565834*1.4;
  // double tilin= 10000000.0;//10000;
  // double tdlin = 0.0;//0.0;

  // double kang = 0.07489797434;
  // double tiang = 10000000.0;//4.0
  // double tdang = 0.0;//0.0;

  //LARC2019

  // double klin = 1.0;
  // double tilin= 10000000.0;//10000;
  // double tdlin = 0.0;//0.0;
  //
  // double kang = 0.08;
  // double tiang = 10000000.0;//4.0
  // double tdang = 0.0;//0.0;

  // FPS 100
  double klin = 0.75;
  double tilin = 10000000.0; //10000;
  double tdlin = 0.0;        //0.0;

  double kang = 0.17;
  double tiang = 10000000.0; //4.0
  double tdang = 0.0;        //0.0;

  //################################################################
  //################################################################

  for (int i = 0; i < 3; i++)
  {
    chegou[i] = false;
    sentidoGiro[i] = 0;
    lin[i].fixa_constantes(klin, tilin, tdlin, 20);
    ang[i].fixa_constantes(kang, tiang, tdang, 20);
  }
}

Control::~Control()
{
}

// Esta função calcula o "percentual de levada em conta" do controle
// angular (de orientação). Quando o robô está muito distante da sua
// referência final, não é necessário ainda se preocupar com a
// orientação final de chegada, e esta função retorna 0.0. Este valor
// vai crescendo até se tornar 1.0 para distâncias pequenas.

inline double coef_orient(double d)
{
  return (d < DIST_ORIENT ? 1.0 : (d > 2 * DIST_ORIENT ? 0.0 : (2 * DIST_ORIENT - d) / DIST_ORIENT));
}

bool Control::control()
{
  double distancia, beta, beta2, gama, xref, yref,
      erro_ang, erro_ang2, erro_lin, alpha_lin, alpha_ang;
  //bool controle_orientacao;

  for (int i = 0; i < 3; i++)
  {
    if (bypassControl[i])
    {
      //verificar se deve fazer anti_windup ou zerar o erro integrativo
      ang[i].reset();
      lin[i].reset();
      chegou[i] = false;
      sentidoGiro[i] = 0;
    }
    else
    {
      //se posicao indefinida, o jogador fica parado
      if (pos.me[i].undef())
      {
        ang[i].reset();
        lin[i].reset();
        alpha_ang = 0.0;
        alpha_lin = 0.0;
        chegou[i] = false;
        sentidoGiro[i] = 0;
      }
      else
      {
        // TESTA SE ESTA PROXIMO DA REFERENCIA
        distancia = hypot(ref.me[i].y() - pos.me[i].y(),
                          ref.me[i].x() - pos.me[i].x());
        if (!chegou[i])
        {
          chegou[i] = (distancia < EPSILON_L);
          if (chegou[i])
          {
            ang[i].reset();
            lin[i].reset();
          }
        }
        else
        {
          chegou[i] = (distancia < DELTA_L);
          if (!chegou[i])
          {
            ang[i].reset();
            lin[i].reset();
          }
        }
        if (!chegou[i])
        {
          beta = arc_tang(ref.me[i].y() - pos.me[i].y(),
                          ref.me[i].x() - pos.me[i].x());
          // Cálculo da referência "xref,yref" para o controle de posição de
          // forma a garantir o controle de orientação
          if (controle_orientacao && ref.me[i].theta() != POSITION_UNDEFINED)
          {
            gama = coef_orient(distancia) * ang_equiv2(beta - ref.me[i].theta());
            xref = pos.me[i].x() + distancia * cos(beta + gama);
            yref = pos.me[i].y() + distancia * sin(beta + gama);
            beta2 = arc_tang(yref - pos.me[i].y(), xref - pos.me[i].x());
          }
          else
          {
            xref = ref.me[i].x();
            yref = ref.me[i].y();
            beta2 = beta;
          }
          erro_ang = ang_equiv(beta2 - pos.me[i].theta());
          erro_lin = distancia * cos(erro_ang);
          //erro_lin = 0.0;
        }
        else
        {
          // Erros nulos se próximo da referência
          //erro_ang = 0.0;
          if (ref.me[i].theta() == POSITION_UNDEFINED)
          {
            //	    ang[i].reset();
            erro_ang = 0.0;
          }
          else
          {
            erro_ang = ang_equiv(ref.me[i].theta() - pos.me[i].theta());
          }
          //lin[i].reset();
          erro_lin = 0.0;
          //erro_lin = distancia*cos(erro_ang);
        }
        // Calcula o sentido mais curto para girar
        // Na medida do possível, mantém a mesma direção de giro anterior
        erro_ang2 = ang_equiv2(erro_ang);

        if (fabs(erro_ang2) > M_PI_4 && sentidoGiro[i] * erro_ang2 < 0.0)
        {
          if (sentidoGiro[i] > 0)
          {
            erro_ang2 += M_PI;
          }
          else
          {
            erro_ang2 -= M_PI;
          }
        }

        sentidoGiro[i] = sgn(erro_ang2);
        // Gera sinal de controle para o movimento angular
        //      alpha_ang = ang[i].controle(erro_ang2, T_AMOSTR);
        alpha_ang = ang[i].controle(erro_ang2, dt_amostr);
        // Saturação
        //realiza o anti_windup caso o robo esteja bloqueado
        if (bloqueado[i])
        {
          ang[i].anti_windup();
          lin[i].anti_windup();
        }

        if (alpha_ang > 1.0)
        {
          alpha_ang = 1.0;
          ang[i].anti_windup();
        }
        if (alpha_ang < -1.0)
        {
          alpha_ang = -1.0;
          ang[i].anti_windup();
        }
        // Gera sinal de controle para o movimento linear
        //      alpha_lin = lin[i].controle(erro_lin, T_AMOSTR);
        alpha_lin = lin[i].controle(erro_lin, dt_amostr);

        //alpha_lin = 0.0;

        // Saturação
        if (alpha_lin > 1 - fabs(alpha_ang))
        {
          alpha_lin = 1 - fabs(alpha_ang);
          lin[i].anti_windup(); // Podia dispensar, já que é PD
        }
        if (alpha_lin < -(1 - fabs(alpha_ang)))
        {
          alpha_lin = -(1 - fabs(alpha_ang));
          lin[i].anti_windup(); // Podia dispensar, já que é PD
        }
      }

      // Cálculo dos percentuais dos motores das rodas. Os valores
      // "alpha_ang" e "alpha_lin" são puramente teóricas, pois o que se
      // controla na prática são os percentuais dos motores direito e
      // esquerdo.

      pwm.me[i].right = alpha_lin + alpha_ang;
      if (fabs(pwm.me[i].right) < PWM_ZERO)
        pwm.me[i].right = 0.0;
      else if (pwm.me[i].right > 0.0)
        pwm.me[i].right = pwm.me[i].right;
      else
        pwm.me[i].right = pwm.me[i].right;

      pwm.me[i].left = alpha_lin - alpha_ang;
      if (fabs(pwm.me[i].left) < PWM_ZERO)
        pwm.me[i].left = 0.0;
      else if (pwm.me[i].left > 0.0)
        pwm.me[i].left = pwm.me[i].left;
      else
        pwm.me[i].left = pwm.me[i].left;
    }
  }
  return false;
}

bool Control::stop_control()
{
  for (int i = 0; i < 3; i++)
  {
    chegou[i] = false;
    lin[i].reset();
    ang[i].reset();
    pwm.me[i].right = 0.0;
    pwm.me[i].left = 0.0;
  }
  return false;
}
