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

double PID::getIant()
{
  return I_ant;
}

double PID::getIant2()
{
  return I_ant2;
}

double PreditorSmith::predicao()
{
  if(enable)
  {
     y[ORDEM] =  -(c-1.0)*y[ORDEM-1] +  c*y[ORDEM-2] +  a*u[ORDEM-1] +  b*u[ORDEM-2];
    ya[ORDEM] = -(c-1.0)*ya[ORDEM-1] + c*ya[ORDEM-2] + a*ua[ORDEM-1] + b*ua[ORDEM-2];
    return y[ORDEM] - ya[ORDEM];
  }
  
  return 0.0;
}

void PreditorSmith::update(double uatual)
{
  if (enable)
  {
     u[ORDEM]        = uatual;
    ua[ORDEM+ATRASO] = uatual;

    for(int k=0; k<ORDEM; k++)
    {
       y[k] =  y[k+1];
      ya[k] = ya[k+1];
       u[k] =  u[k+1];
    }

    for(int k=0; k<ORDEM+ATRASO; k++)
    {
      ua[k] = ua[k+1];
    }
  }    
}

void PreditorSmith::fixa_coeficientes(double a, double b, double c)
{
  this->a = a;
  this->b = b;
  this->c = c;
}

void PreditorSmith::reset()
{
  for(int i=0; i<ORDEM+1; i++)
  {
     y[i] = 0.0;
     u[i] = 0.0;
    ya[i] = 0.0;
  }

  for(int i=0; i<ORDEM+1+ATRASO; i++)
  {
    ua[i] = 0.0;
  }
}

void PreditorSmith::enabled(bool en)
{
  this->enable = en;
}

PreditorSmith::~PreditorSmith()
{
  //dtor
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


  //////////////////////////////////////////////////////////////////////

  // FPS 100 P

  // double klin = 1.09841901711222; // sem sobressinal
  // double klin = 2.30600025753142; // com sobressinal de 5%
  // double tilin = 10000000.0;
  // double tdlin = 0.0;

  // double kang = 0.288533627498953; // sobressinal de 7% robo 2
  // double tiang = 10000000.0;
  // double tdang = 0.0;

  // FPS 100 PI
  

  // (11) new
  double klin = 1.33864384817273; // com sobressinal
  double tilin = klin/0.669321924169537; // Kp/Ki;
  double tdlin = 0.0;

  // P angular
  // double kang = 0.288533627498953*2.0;
  // double tiang = 10000000.0;
  // double tdang = 0.0;

  // (11) PI dp:0.46
  double kang = 0.551719640142742;
  double tiang = kang/0.27585982007137;
  double tdang = 0.0; 
  

  for (int i = 0; i < 3; i++)
  {
    chegou[i] = false;
    sentidoGiro[i] = 0;
    betaf_ant[i] = 0.0;
    thetaf_ant[i] = 0.0;
    lin[i].fixa_constantes(klin, tilin, tdlin, 20);
    ang[i].fixa_constantes(kang, tiang, tdang, 20);
  }

  //########### CONSTANTES DO PREDITOR DE SMITH ####################
  //################################################################

  double alin[3], blin[3], clin[3];
  double aang[3], bang[3], cang[3];

  // Modelo do robô 0
  alin[0] =  0.001282995298595;
  blin[0] =  0.001241502193117;
  clin[0] = -0.906075092085835;
  aang[0] =  0.029411735761022;
  bang[0] =  0.028122032375883;
  cang[0] = -0.874118501219467;

  // Modelo do robô 1
  alin[1] =  0.001144525406520;
  blin[1] =  0.001112066407911;
  clin[1] = -0.917305051274910;
  aang[1] =  0.047005776297916;
  bang[1] =  0.043285288215461;
  cang[1] = -0.780761741669052;

  // Modelo do robô 2
  alin[2] =  0.001282141887944;
  blin[2] =  0.001236786551202;
  clin[2] = -0.897577458324244;
  aang[2] =  0.042508332868416;
  bang[2] =  0.039642769919082;
  cang[2] = -0.811036158599964;

  for (int i = 0; i < 3; i++)
  {
    pslin[i].fixa_coeficientes(alin[i],blin[i],clin[i]);
    psang[i].fixa_coeficientes(aang[i],bang[i],cang[i]);

    pslin[i].enabled(true);
    psang[i].enabled(true);
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
  double distancia = 0.0, beta = 0.0, beta2 = 0.0, gama = 0.0, xref = 0.0, yref = 0.0,
         erro_ang = 0.0, erro_ang2 = 0.0, erro_lin = 0.0, alpha_lin = 0.0, alpha_ang = 0.0,
         betaf = 0.0,
         erro_ang_inicial = 0.0, erro_lin_inicial = 0.0;
  double betaaux = 0.0,  thetaaux = 0.0;

  double D11 = 1.0;
  double D12 = 0.0;
  double D21 = 0.0;
  double D22 = 1.0;

  for (int i = 0; i < 3; i++)
  {
    if (bypassControl[i])
    {
      //verificar se deve fazer anti_windup ou zerar o erro integrativo
      ang[i].reset();
      lin[i].reset();
      chegou[i] = false;
      sentidoGiro[i] = 0;

      pslin[i].reset();
      psang[i].reset();
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

        pslin[i].reset();
        psang[i].reset();
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

            pslin[i].reset();
            psang[i].reset();            
          }
        }
        else
        {
          chegou[i] = (distancia < DELTA_L);
          if (!chegou[i])
          {
            ang[i].reset();
            lin[i].reset();

            pslin[i].reset();
            psang[i].reset();            
          }
        }

        if (!chegou[i])
        {
          beta = arc_tang(ref.me[i].y() - pos.me[i].y(),
                          ref.me[i].x() - pos.me[i].x());

          // Correção do ângulo beta
          betaaux = betaf_ant[i] + ang_equiv(beta - betaf_ant[i]);

          // Filtro passa-baixa para estabilizar beta corrigido
          betaf = ang_equiv(LAMBDAF*betaaux + (1-LAMBDAF)*betaf_ant[i]);

          betaf_ant[i] = betaf;

          // Cálculo da referência "xref,yref" para o controle de posição de
          // forma a garantir o controle de orientação
          if (controle_orientacao && (ref.me[i].theta() != POSITION_UNDEFINED))
          {
            gama = coef_orient(distancia) * ang_equiv2(betaf - ref.me[i].theta());
            xref = pos.me[i].x() + distancia * cos(betaf + gama);
            yref = pos.me[i].y() + distancia * sin(betaf + gama);
            beta2 = arc_tang(yref - pos.me[i].y(), xref - pos.me[i].x());
          }
          else
          {
            xref = ref.me[i].x();
            yref = ref.me[i].y();
            beta2 = betaf;
          }
          erro_ang_inicial = ang_equiv(beta2 - pos.me[i].theta());
          erro_ang = ang_equiv(beta2 - pos.me[i].theta() - psang[i].predicao());

          erro_lin_inicial = distancia * cos(erro_ang);
          erro_lin = distancia * cos(erro_ang) - pslin[i].predicao();

          thetaf_ant[i] = pos.me[i].theta();
        }
        else
        {
          // Erros nulos se próximo da referência
          //erro_ang = 0.0;
          if (ref.me[i].theta() == POSITION_UNDEFINED)
          {
            // ang[i].reset();
            erro_ang_inicial = 0.0;
            erro_ang = 0.0;
          }
          else
          {
            // Correção do ângulo theta do robô
            thetaaux = thetaf_ant[i] + ang_equiv(pos.me[i].theta() - thetaf_ant[i]);

            // Filtro passa-baixa para estabilizar theta corrigido
            thetaf_ant[i] = ang_equiv(LAMBDAF2*thetaaux + (1-LAMBDAF2)*thetaf_ant[i]);

            erro_ang_inicial = ang_equiv(ref.me[i].theta() - thetaf_ant[i]);
            erro_ang = ang_equiv(ref.me[i].theta() - thetaf_ant[i] - psang[i].predicao());
          }

          // lin[i].reset();     
          erro_lin_inicial = 0.0;     
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

        // alpha_lin = 0.0;

        // Saturação
        if (alpha_lin > 1 - fabs(alpha_ang))
        {
          alpha_lin = 1 - fabs(alpha_ang);
          lin[i].anti_windup();
        }
        if (alpha_lin < -(1 - fabs(alpha_ang)))
        {
          alpha_lin = -(1 - fabs(alpha_ang));
          lin[i].anti_windup();
        }
      }
    
      // Armazena o log da predição atual
      logControle[i].pslin = pslin[i].predicao();
      logControle[i].psang = psang[i].predicao();     

      // Atualização do Preditor de Smith
      pslin[i].update(alpha_lin);
      psang[i].update(alpha_ang);

      // Cálculo dos percentuais dos motores das rodas. Os valores
      // "alpha_ang" e "alpha_lin" são puramente teóricas, pois o que se
      // controla na prática são os percentuais dos motores direito e
      // esquerdo.

      // Constantes para a correção de assimetrias
      if (i==0){
        D11 =  1.008222145102893;
        D12 = -0.049602174507967;
        D21 = -0.167124704818218;
        D22 =  1.008222145102893;
      }
      else if (i==1){
        D11 =  0.999808713274133;
        D12 =  0.004378663213346;
        D21 = -0.043677744995903;
        D22 =  0.999808713274133;        
      }
      else if (i==2){
        D11 =  1.012892845513687;
        D12 = -0.059115513002525;
        D21 = -0.220907682532830;
        D22 =  1.012892845513687;
      }      

      pwm.me[i].right = D11*(alpha_lin + alpha_ang) + D12*(alpha_lin - alpha_ang);
      // pwm.me[i].right = alpha_lin + alpha_ang;

      if (fabs(pwm.me[i].right) < PWM_ZERO)
      {
        pwm.me[i].right = 0.0;
      }
      else
      {
        pwm.me[i].right = sature(pwm.me[i].right, -1.0, 1.0);
      }
      
      
      pwm.me[i].left = D21*(alpha_lin + alpha_ang) + D22*(alpha_lin - alpha_ang);
      // pwm.me[i].left = alpha_lin - alpha_ang;

      if (fabs(pwm.me[i].left) < PWM_ZERO)
      {
        pwm.me[i].left = 0.0;
      }
      else
      {
        pwm.me[i].left = sature(pwm.me[i].left, -1.0, 1.0);
      }
    }


    // --------- LOG DO CONTROLE ---------

    logControle[i].chegou = chegou[i];
    logControle[i].distancia = distancia;

    logControle[i].erro_lin = erro_lin;
    logControle[i].erro_ang = erro_ang;
    logControle[i].erro_ang2 = erro_ang2;

    logControle[i].beta = beta;
    logControle[i].beta2 = beta2;
    logControle[i].gama = gama;

    logControle[i].xref = xref;
    logControle[i].yref = yref;

    logControle[i].lin_I_ant = lin[i].getIant();
    logControle[i].lin_I_ant2 = lin[i].getIant2();
    logControle[i].ang_I_ant = ang[i].getIant();
    logControle[i].ang_I_ant2 = ang[i].getIant2();

    logControle[i].alpha_lin = alpha_lin;
    logControle[i].alpha_ang = alpha_ang;

    // logControle[i].pslin = pslin[i].predicao();
    // logControle[i].psang = psang[i].predicao();

    logControle[i].beta_ant = 0.0; //beta_ant[i];
    logControle[i].betaf = betaf;
    logControle[i].betaf_ant = betaf_ant[i];

    logControle[i].erro_lin_inicial = erro_lin_inicial;
    logControle[i].erro_ang_inicial = erro_ang_inicial;
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