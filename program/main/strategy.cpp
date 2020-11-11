#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include "strategy.h"
#include "../parameters.h"
#include "../functions.h"
#include "../system.h"

using namespace ::std;

// As constantes numericasla usados na estrategia
static const double EPSILON_L(0.005);                              //CHECAR
static const double EPSILON_ANG(0.087);                            //5graus//CHECAR
static const double LONGE_L(4 * EPSILON_L);                        //CHECAR
static const double LONGE_ANG(4 * EPSILON_ANG);                    //CHECAR
static const double DIST_CHUTE(ROBOT_RADIUS + BALL_RADIUS + 0.05); // (ROBOT_RADIUS+BALL_RADIUS-0.075-0.018) LARC 2016
static const double VEL_BOLA_LENTA(0.2);
static const double DIST_COLADA(3.0 * BALL_RADIUS);
// Distancia a partir da qual se considera que a bola estah na lateral do campo
// ou que um robo ou a bola estao suficientemente proximos da area do gol
static const double LARGURA_ZONA(DIST_CHUTE + ROBOT_EDGE);
// Pequena distancia que vai ser utilizada para dar comportamento de histerese aos predicados
static const double MARGEM_HISTERESE(0.01); // 1cm
static const unsigned COM_BOLA_DEFAULT(0);
static const unsigned SEM_BOLA_DEFAULT(2);
static const unsigned GOLEIRO_DEFAULT(1);
//static const unsigned LIMITE_CONT_PARADO(20);
static const unsigned LIMITE_CONT_PARADO(5 * FPS); //equivalente a 5 segundos
//static const unsigned LIMITE_CONT_PERDIDO(10);
static const unsigned LIMITE_CONT_PERDIDO(FPS / 3);

// Classes locais
class Repulsao
{
private:
  unsigned cell[9][10];
  POS_BOLA calculaCentro(unsigned i, unsigned j) const;

public:
  inline Repulsao() { iniciar(); }
  void iniciar();
  void incluirObstaculo(POS_BOLA obs);
  POS_BOLA calculaPosicao(const POS_BOLA &prox) const;
};

void Repulsao::iniciar()
{
  unsigned i, j;
  for (i = 0; i < 9; i++)
  {
    for (j = 0; j < 10; j++)
    {
      cell[i][j] = 0;
    }
  }
  // Areas do gol
  for (i = 2; i <= 6; i++)
  {
    // Dentro da area
    cell[i][0] += 2;
    cell[i][9] += 2;
    // Frente da area
    cell[i][1] += 2;
    cell[i][8] += 2;
    // Lado da area
    cell[1][0] += 2;
    cell[7][0] += 2;
    cell[1][9] += 2;
    cell[7][9] += 2;
    // "Quinas" da area
    cell[1][1] += 1;
    cell[7][1] += 1;
    cell[1][8] += 1;
    cell[7][8] += 1;
  }
  // Paredes
  for (j = 0; j <= 9; j++)
  {
    // Superior
    cell[0][j] += 1;
    // Inferior
    cell[8][j] += 1;
  }
  for (i = 0; i <= 8; i++)
  {
    // Esquerda
    cell[i][0] += 1;
    // Direita
    cell[i][9] += 1;
  }
}

void Repulsao::incluirObstaculo(POS_BOLA obs)
{

  if (obs.x() <= -FIELD_WIDTH / 2.0)
    obs.x() = -0.99 * FIELD_WIDTH / 2.0;
  if (obs.x() >= FIELD_WIDTH / 2.0)
    obs.x() = 0.99 * FIELD_WIDTH / 2.0;
  // if (obs.y() <= -FIELD_HEIGHT/2.0) obs.x() = -0.99*FIELD_HEIGHT/2.0;
  // if (obs.y() >= FIELD_HEIGHT/2.0) obs.x() = 0.99*FIELD_HEIGHT/2.0;
  if (obs.y() <= -FIELD_HEIGHT / 2.0)
    obs.y() = -0.99 * FIELD_HEIGHT / 2.0;
  if (obs.y() >= FIELD_HEIGHT / 2.0)
    obs.y() = 0.99 * FIELD_HEIGHT / 2.0;
  //  cout << "depois do if incluir Obstaculos\n"; cout.flush();
  unsigned iObs, jObs;
  jObs = (unsigned)trunc((obs.x() + (FIELD_WIDTH / 2.0)) / (FIELD_WIDTH / 10.0));
  if (fabs(obs.y()) < 0.05)
  {
    iObs = 4;
  }
  else if (obs.y() > 0.0)
  {
    iObs = 3 - (unsigned)trunc((obs.y() - 0.05) / 0.15);
  }
  else
  {
    iObs = 5 + (unsigned)trunc((fabs(obs.y()) - 0.05) / 0.15);
  }
  //  cout << "meio incluir Obstaculos\n"; cout.flush();
  cell[iObs][jObs] += 3;
  //  cout << "1\n"; cout.flush();
  if (iObs < 8)
    cell[iObs + 1][jObs] += 2;
  //  cout << "2\n"; cout.flush();
  if (iObs > 3)
    cell[iObs - 1][jObs] += 2;
  //  cout << "3\n"; cout.flush();
  if (jObs < 9)
    cell[iObs][jObs + 1] += 2;
  //  cout << "4\n"; cout.flush();
  if (jObs > 0)
    cell[iObs][jObs - 1] += 2;
  //  cout << "5\n"; cout.flush();
  if (iObs < 8 && jObs < 9)
    cell[iObs + 1][jObs + 1] += 1;
  //  cout << "6\n"; cout.flush();
  if (iObs > 0 && jObs < 9)
    cell[iObs - 1][jObs + 1] += 1;
  //  cout << "7\n"; cout.flush();
  if (iObs < 8 && jObs > 0)
    cell[iObs + 1][jObs - 1] += 1;
  //  cout << "8\n"; cout.flush();
  if (iObs > 0 && jObs > 0)
    cell[iObs - 1][jObs - 1] += 1;
  //  cout << "9\n"; cout.flush();
}

POS_BOLA Repulsao::calculaCentro(unsigned i, unsigned j) const
{
  POS_BOLA pos;
  int iCell = 4 - (int)i;
  pos.x() = 0.15 * j + 0.075 - FIELD_WIDTH / 2.0;
  pos.y() = 0.15 * iCell - 0.025 * sgn(iCell);
  return pos;
}

POS_BOLA Repulsao::calculaPosicao(const POS_BOLA &prox) const
{
  unsigned iPos = 4, jPos = 4;
  double dist, bestDist = 9999999.9;

  POS_BOLA centro;
  for (unsigned i = 0; i <= 8; i++)
  {
    for (unsigned j = 0; j <= 9; j++)
    {
      if (cell[i][j] == 0)
      {
        centro = calculaCentro(i, j);
        dist = hypot(centro.y() - prox.y(), centro.x() - prox.x());
        if (dist < bestDist)
        {
          iPos = i;
          jPos = j;
          bestDist = dist;
        }
      }
    }
  }
  return calculaCentro(iPos, jPos);
}

// Funcoes locais
//static void repulsao(double &dx, double &dy, double dist, double ang);
static POS_BOLA posicao_relativa(POS_BOLA pos, POS_ROBO origem);
static POS_ROBO posicao_absoluta(POS_ROBO pos, POS_ROBO origem);
static inline bool aleatorio() { return (drand48() < 0.5); }

// Variaveis locais
static Repulsao R;

Strategy::Strategy(TEAM team, SIDE side, GAME_MODE gameMode) : FutData(team, side, gameMode)
{
  //INICIALIZAR OS PARAMETROS DA CLASSE
  t = 0.0;

  // Soh um desses predicados pode ser true ao mesmo tempo
  bola_dentro_gol = bola_area_defesa = bola_frente_area = bola_lado_area =
      bola_parede_fundo = bola_quina_fundo = bola_fundo =
          bola_parede_lateral = bola_lateral = bola_quina_frente =
              bola_parede_frente = bola_frente = bola_regiao_central = false;

  for (int i = 0; i < 3; i++)
  {
    bloqueado[i] = false;
    perdido[i] = false;
    contador_parado[i] = 0;
    contador_perdido[i] = 0;
  }

  papeis.me[COM_BOLA_DEFAULT].funcao = COM_BOLA;
  papeis.me[SEM_BOLA_DEFAULT].funcao = SEM_BOLA;
  papeis.me[GOLEIRO_DEFAULT].funcao = GOLEIRO;

  sinal = (mySide() == LEFT_SIDE ? 1 : -1);

  //PREENCHIMENTO DA TABELA V. DO COM_BOLA
  //preenchimento();
  estado_penalty = 0; // Não é penalty
}

Strategy::~Strategy()
{
}

bool Strategy::strategy()
{
  int i;

  t = relogio();

  // a principio sempre deixa o controle atuar
  for (i = 0; i < 3; i++)
  {
    bypassControl[i] = false;
  }
  // analisa a posicao da bola
  analisa_bola();
  // analisa posição dos meus jogadores
  analisa_jogadores();
  // analisa posiçao dos adversários
  analisa_adversarios();
  // detecta se algum dos robos esta bloqueado
  detecta_bloqueados();

  // escolhe uma funcao (goleiro, com_bola, sem_bola) para cada jogador
  escolhe_funcoes();

  // escolhe uma acao para cada jogador
  acao_goleiro(goleiro());
  acao_com_bola(com_bola());
  // A função de cálculo da acao do defensor deve ser chamada depois
  // do atacante e do goleiro, para que o defensor possa evitá-los
  acao_sem_bola(sem_bola());

  // calcula as referencias a partir das acoes
  for (i = 0; i < 3; i++)
  {
    calcula_referencias(i);
  }

  // guarda dados atuais para o proximo ciclo
  ant = pos;

  return false;
}

void Strategy::analisa_jogadores()
{
  for (int i = 0; i < 3; i++)
  {

    // O roboh estah dentro da sua area do gol
    meu_na_area[i] =
        (sinal * pos.me[i].x() < -(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH) ||
         (sinal * pos.me[i].x() < -(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH) + MARGEM_HISTERESE && meu_na_area[i])) &&
        fabs(pos.me[i].y()) < GOAL_FIELD_HEIGHT / 2.0;

    // O roboh estah "encalhado" na regiao concava que existe atras do gol do seu time
    meu_atras_gol[i] = sinal * pos.me[i].x() < -FIELD_WIDTH / 2.0;

    // O roboh tem coordenada x dentro ou perto da area do gol
    meu_lado_area[i] =
        sinal * pos.me[i].x() < -(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH - LARGURA_ZONA);

    // O robo estah  a frente da bola
    meu_na_frente_bola[i] =
        (sinal * pos.me[i].x() - sinal * pos.ball.x() > 0.0) ||
        (sinal * pos.me[i].x() - sinal * pos.ball.x() > -MARGEM_HISTERESE && meu_na_frente_bola[i]);

    ref_desc[i] = posicao_para_descolar_bola();
    dlin_desc[i] = hypot(ref_desc[i].y() - pos.me[i].y(),
                         ref_desc[i].x() - pos.me[i].x());
    dang_desc[i] = ang_equiv2(ref_desc[i].theta() - pos.me[i].theta());
    meu_posicionado_descolar[i] =
        dlin_desc[i] < ROBOT_EDGE / 3.0 && dang_desc[i] < M_PI / 8.0;

    meu_furou[i] =
        dlin_desc[i] > ROBOT_RADIUS && dang_desc[i] < M_PI / 8.0;

    meu_frente_area[i] =
        sinal * pos.me[i].x() < -(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH - LARGURA_ZONA) &&
        fabs(pos.me[i].y()) < GOAL_FIELD_HEIGHT / 2.0;

    // Angulo do robo para a bola
    thetapos_fut[i] = arc_tang(pos.future_ball.y() - pos.me[i].y(),
                               pos.future_ball.x() - pos.me[i].x());
    // posicao onde a bola bateria se seguisse rumo ao fundo do campo de
    // acordo com a reta que vem do robo
    ypos_fut[i] = mytan(thetapos_fut[i]) *
                      (sinal * FIELD_WIDTH / 2.0 - pos.me[i].x()) +
                  pos.me[i].y();
    // posicao onde o robô com o alinhamento atual bateria no fundo do
    // campo se seguisse reto rumo ao fundo do campo
    yalin[i] = mytan(pos.me[i].theta()) *
                   (sinal * FIELD_WIDTH / 2.0 - pos.me[i].x()) +
               pos.me[i].y();
    meu_alinhado_chutar[i] =
        fabs(ypos_fut[i]) < GOAL_FIELD_HEIGHT / 2.0 &&
        fabs(yalin[i]) < GOAL_FIELD_HEIGHT / 2.0;

    meu_bem_alinhado_chutar[i] =
        fabs(ypos_fut[i]) < GOAL_HEIGHT / 2.0 &&
        fabs(yalin[i]) < GOAL_HEIGHT / 2.0;

    bola_frente_y_do_meu[i] =
        fabs(pos.me[i].y()) < fabs(pos.ball.y()) ||
        (fabs(pos.me[i].y()) < (fabs(pos.ball.y()) + MARGEM_HISTERESE) && bola_frente_y_do_meu[i]);

    meu_posicionado_isolar[i] =
        (sgn(pos.ball.y()) * pos.ball.y() < sgn(pos.ball.y()) * pos.me[i].y() ||
         (sgn(pos.ball.y()) * pos.ball.y() < sgn(pos.ball.y()) * pos.me[i].y() - MARGEM_HISTERESE && meu_posicionado_isolar[i])) &&
        (sinal * pos.me[i].x() < sinal * pos.ball.x() ||
         (sinal * pos.me[i].x() < sinal * pos.ball.x() - MARGEM_HISTERESE && meu_posicionado_isolar[i]));

    meu_alinhado_isolar[i] =
        fabs(pos.me[i].y() - pos.ball.y()) < ROBOT_EDGE;

    meu_bem_alinhado_isolar[i] =
        fabs(pos.me[i].y() - pos.ball.y()) < 2.0 * BALL_RADIUS;
  }
}

void Strategy::analisa_adversarios()
{

  for (int i = 0; i < 3; i++)
  {
    adv_na_area[i] = adv_mais_prox_bola[i] = false;
    dist_meu[i] = hypot(pos.ball.x() - pos.me[i].x(),
                        pos.ball.y() - pos.me[i].y());

    for (int j = 0; j < 3 && !adv_na_area[i] && !adv_mais_prox_bola[i]; j++)
    {
      if (pos.op[j].undef())
      {
        adv_na_area[i] = adv_mais_prox_bola[i] = true;
      }
      else
      {
        adv_na_area[i] = adv_na_area[i] ||
                         (sinal * pos.op[j].x() < -(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH) &&
                          fabs(pos.op[j].y()) < GOAL_FIELD_HEIGHT / 2.0);
        dist_adv[j] = hypot(pos.ball.x() - pos.op[j].x(),
                            pos.ball.y() - pos.op[j].y());
        adv_mais_prox_bola[i] = adv_mais_prox_bola[i] || (dist_adv[j] < dist_meu[j]);
      }
    }
  }
}

void Strategy::analisa_bola()
{

  bola_parada =
      pos.vel_ball.mod < VEL_BOLA_PARADA;

  bola_no_ataque =
      sinal * pos.ball.x() > 0.0;

  bola_area_lateral =
      fabs(pos.ball.y()) > GOAL_HEIGHT / 2.0;

  bola_dentro_gol = fabs(pos.ball.x()) > (FIELD_WIDTH / 2.0) + MARGEM_HISTERESE || //MAIS RESTRITIVO
                    (fabs(pos.ball.x()) > (FIELD_WIDTH / 2.0 - MARGEM_HISTERESE) && bola_dentro_gol);
  if (bola_dentro_gol)
  {
    bola_area_defesa = bola_frente_area = bola_lado_area =
        bola_parede_fundo = bola_quina_fundo = bola_fundo =
            bola_parede_lateral = bola_lateral = bola_quina_frente =
                bola_parede_frente = bola_frente = bola_regiao_central = false;
    return;
  }

  bola_area_defesa =
      (sinal * pos.ball.x() < -(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH) - MARGEM_HISTERESE &&
       fabs(pos.ball.y()) < GOAL_FIELD_HEIGHT / 2.0 - MARGEM_HISTERESE) || // MAIS_RESTRITIVO
      (sinal * pos.ball.x() < -(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH) + MARGEM_HISTERESE &&
       fabs(pos.ball.y()) < GOAL_FIELD_HEIGHT / 2.0 + MARGEM_HISTERESE && bola_area_defesa);
  if (bola_area_defesa)
  {
    bola_dentro_gol = bola_frente_area = bola_lado_area =
        bola_parede_fundo = bola_quina_fundo = bola_fundo =
            bola_parede_lateral = bola_lateral = bola_quina_frente =
                bola_parede_frente = bola_frente = bola_regiao_central = false;
    return;
  }

  bola_frente_area =
      (sinal * pos.ball.x() < -(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH - LARGURA_ZONA + MARGEM_HISTERESE) &&
       fabs(pos.ball.y()) < GOAL_FIELD_HEIGHT / 2.0 - MARGEM_HISTERESE) || // MAIS RESTRITIVO
      (sinal * pos.ball.x() < -(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH - LARGURA_ZONA - MARGEM_HISTERESE) &&
       fabs(pos.ball.y()) < GOAL_FIELD_HEIGHT / 2.0 + MARGEM_HISTERESE && bola_frente_area);
  if (bola_frente_area)
  {
    bola_dentro_gol = bola_area_defesa = bola_lado_area =
        bola_parede_fundo = bola_quina_fundo = bola_fundo =
            bola_parede_lateral = bola_lateral = bola_quina_frente =
                bola_parede_frente = bola_frente = bola_regiao_central = false;
    return;
  }

  bola_lado_area =
      (sinal * pos.ball.x() < -(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH) - MARGEM_HISTERESE &&
       fabs(pos.ball.y()) < (GOAL_FIELD_HEIGHT / 2.0 + ROBOT_RADIUS + BALL_RADIUS) - MARGEM_HISTERESE) || // MAIS RESTRITIVO
      (sinal * pos.ball.x() < -(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH) + MARGEM_HISTERESE &&
       fabs(pos.ball.y()) < (GOAL_FIELD_HEIGHT / 2.0 + ROBOT_RADIUS + BALL_RADIUS) + MARGEM_HISTERESE && bola_lado_area);
  if (bola_lado_area)
  {
    bola_dentro_gol = bola_area_defesa = bola_frente_area =
        bola_parede_fundo = bola_quina_fundo = bola_fundo =
            bola_parede_lateral = bola_lateral = bola_quina_frente =
                bola_parede_frente = bola_frente = bola_regiao_central = false;
    return;
  }

  bola_parede_fundo =
      sinal * pos.ball.x() < -(FIELD_WIDTH / 2.0 - DIST_COLADA) - MARGEM_HISTERESE || //MAIS RESTRITIVO
      (sinal * pos.ball.x() < -(FIELD_WIDTH / 2.0 - DIST_COLADA) + MARGEM_HISTERESE && bola_parede_fundo);
  if (bola_parede_fundo)
  {
    bola_dentro_gol = bola_area_defesa = bola_frente_area =
        bola_lado_area = bola_quina_fundo = bola_fundo =
            bola_parede_lateral = bola_lateral = bola_quina_frente =
                bola_parede_frente = bola_frente = bola_regiao_central = false;
    return;
  }

  bola_quina_fundo =
      fabs(pos.ball.y()) > (FIELD_HEIGHT / 2.0 - CORNER_DIMENSION - sqrt(2.0) * DIST_COLADA +
                            sinal * pos.ball.x() + FIELD_WIDTH / 2.0) +
                               MARGEM_HISTERESE || //MAIS RESTRITIVO;
      (fabs(pos.ball.y()) > (FIELD_HEIGHT / 2.0 - CORNER_DIMENSION - sqrt(2.0) * DIST_COLADA +
                             sinal * pos.ball.x() + FIELD_WIDTH / 2.0) -
                                MARGEM_HISTERESE &&
       bola_quina_fundo);
  if (bola_quina_fundo)
  {
    bola_dentro_gol = bola_area_defesa = bola_frente_area =
        bola_lado_area = bola_parede_fundo = bola_fundo =
            bola_parede_lateral = bola_lateral = bola_quina_frente =
                bola_parede_frente = bola_frente = bola_regiao_central = false;
    return;
  }

  bola_parede_lateral =
      fabs(pos.ball.y()) > (FIELD_HEIGHT / 2.0 - DIST_COLADA) + MARGEM_HISTERESE || // MAIS RESTRITIVO;
      (fabs(pos.ball.y()) > (FIELD_HEIGHT / 2.0 - DIST_COLADA) - MARGEM_HISTERESE && bola_parede_lateral);
  if (bola_parede_lateral)
  {
    bola_dentro_gol = bola_area_defesa = bola_frente_area =
        bola_lado_area = bola_parede_fundo = bola_quina_fundo =
            bola_fundo = bola_lateral = bola_quina_frente =
                bola_parede_frente = bola_frente = bola_regiao_central = false;
    return;
  }

  bola_quina_frente =
      fabs(pos.ball.y()) > (FIELD_HEIGHT / 2.0 - CORNER_DIMENSION - sqrt(2.0) * DIST_COLADA - sinal * pos.ball.x() + FIELD_WIDTH / 2.0) + MARGEM_HISTERESE || //MAIS RESTRITIVO;
      (fabs(pos.ball.y()) > (FIELD_HEIGHT / 2.0 - CORNER_DIMENSION - sqrt(2.0) * DIST_COLADA - sinal * pos.ball.x() + FIELD_WIDTH / 2.0) - MARGEM_HISTERESE && bola_quina_frente);
  if (bola_quina_frente)
  {
    bola_dentro_gol = bola_area_defesa = bola_frente_area =
        bola_lado_area = bola_parede_fundo = bola_quina_fundo =
            bola_fundo = bola_parede_lateral = bola_lateral =
                bola_parede_frente = bola_frente = bola_regiao_central = false;
    return;
  }

  bola_parede_frente =
      (sinal * pos.ball.x() > (FIELD_WIDTH / 2.0 - DIST_COLADA) + MARGEM_HISTERESE &&
       fabs(pos.ball.y()) > GOAL_HEIGHT / 2.0 + MARGEM_HISTERESE) || //MAIS RESTRITIVO;
      (sinal * pos.ball.x() > (FIELD_WIDTH / 2.0 - DIST_COLADA - MARGEM_HISTERESE) &&
       fabs(pos.ball.y()) > GOAL_HEIGHT / 2.0 - MARGEM_HISTERESE && bola_parede_frente);
  if (bola_parede_frente)
  {
    bola_dentro_gol = bola_area_defesa = bola_frente_area =
        bola_lado_area = bola_parede_fundo = bola_quina_fundo =
            bola_fundo = bola_parede_lateral = bola_lateral =
                bola_quina_frente = bola_frente = bola_regiao_central = false;
    return;
  }

  bola_fundo =
      sinal * pos.ball.x() < -(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH) - MARGEM_HISTERESE || // MAIS RESTRITIVO
      (sinal * pos.ball.x() < -(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH) + MARGEM_HISTERESE && bola_fundo);
  if (bola_fundo)
  {
    bola_dentro_gol = bola_area_defesa = bola_frente_area =
        bola_lado_area = bola_parede_fundo = bola_quina_fundo =
            bola_parede_lateral = bola_lateral = bola_quina_frente =
                bola_parede_frente = bola_frente = bola_regiao_central = false;
    return;
  }

  bola_frente =
      fabs(pos.ball.y()) > -(sinal * pos.ball.x()) + FIELD_WIDTH / 2.0 + GOAL_HEIGHT / 2.0 + sqrt(2.0) * MARGEM_HISTERESE || // MAIS_RESTRITIVO
      (fabs(pos.ball.y()) > -(sinal * pos.ball.x()) + FIELD_WIDTH / 2.0 + GOAL_HEIGHT / 2.0 - sqrt(2.0) * MARGEM_HISTERESE && bola_frente);
  if (bola_frente)
  {
    bola_dentro_gol = bola_area_defesa = bola_frente_area = bola_lado_area =
        bola_parede_fundo = bola_quina_fundo = bola_fundo =
            bola_parede_lateral = bola_lateral = bola_quina_frente =
                bola_parede_frente = bola_regiao_central = false;
    return;
  }

  bola_lateral =
      fabs(pos.ball.y()) > (FIELD_HEIGHT / 2.0 - LARGURA_ZONA) + MARGEM_HISTERESE || //MAIS RESTRITIVO;
      (fabs(pos.ball.y()) > (FIELD_HEIGHT / 2.0 - LARGURA_ZONA) - MARGEM_HISTERESE && bola_lateral);
  if (bola_lateral)
  {
    bola_dentro_gol = bola_area_defesa = bola_frente_area =
        bola_lado_area = bola_parede_fundo = bola_quina_fundo =
            bola_fundo = bola_parede_lateral = bola_quina_frente =
                bola_parede_frente = bola_frente = bola_regiao_central = false;
    return;
  }
  // Se chegou aqui, nao estah em nenhuma regiao "especial"
  bola_regiao_central = true;
}

void Strategy::detecta_bloqueados()
{
  int id;
  // Se o jogo nao esta em execucao normal, nao existem robos bloqueados...
  // nao trocar quando estiver na defesa, para evitar facilitar para o adversario
  if (gameState() != PLAY_STATE || !bola_no_ataque)
  {
    for (id = 0; id < 3; id++)
    {
      contador_parado[id] = 0;
      bloqueado[id] = false;
      contador_perdido[id] = 0;
      perdido[id] = false;
    }
    return;
  }

  double distlin_ref, distlin_ant, distang_ref, distang_ant;
  bool nao_andou;
  for (id = 0; id < 3; id++)
  {
    nao_andou = false;
    // Assume-se que todos os robos cuja posicao anterior ou atual eh
    // indefinida nao andaram como previsto nesta iteracao...
    if (pos.me[id].undef() || ant.me[id].undef() || ref.me[id].undef())
    {
      nao_andou = true;
    }
    else
    {
      // Para os robos com pose definida, considera-se que ele nao
      // andou se a pose atual eh similar aa anterior e a referencia
      // desejada eh diferente da pose anterior
      distlin_ant = hypot(pos.me[id].y() - ant.me[id].y(),
                          pos.me[id].x() - ant.me[id].x());
      distang_ant = fabs(ang_equiv(pos.me[id].theta() -
                                   ant.me[id].theta()));
      distlin_ref = hypot(ref.me[id].y() - ant.me[id].y(),
                          ref.me[id].x() - ant.me[id].x());
      if (ref.me[id].theta() != POSITION_UNDEFINED)
      {
        distang_ref = fabs(ang_equiv(ref.me[id].theta() -
                                     ant.me[id].theta()));
      }
      else
      {
        distang_ref = 0.0;
      }
      // FAZER: Precisa trocar este teste de bloqueado...
      if (distlin_ant <= EPSILON_L &&
          distang_ant <= EPSILON_ANG &&
          (distlin_ref > LONGE_L || distang_ref > LONGE_ANG))
      {
        nao_andou = true;
      }
    }
    if (nao_andou)
    {
      //robô está parado ou perdido.
      contador_parado[id]++;
      if (contador_parado[id] > LIMITE_CONT_PARADO)
        //o robo esta parado por mais que LIMITE_CONT_PARADO iterações
        bloqueado[id] = true;
    }
    else
    {
      contador_parado[id] = 0;
      bloqueado[id] = false;
    }
    if (pos.me[id].undef())
    {
      //robô nao foi localizado
      contador_perdido[id]++;
      if (contador_perdido[id] > LIMITE_CONT_PERDIDO)
        //o robo NAO FOI localizado por mais que LIMITE_CONT_PERDIDO iterações
        perdido[id] = true;
    }
    else
    {
      contador_perdido[id] = 0;
      perdido[id] = false;
    }
  }
}

void Strategy::escolhe_funcoes()
{
  int id_gol = goleiro(), id_com = com_bola(), id_sem = sem_bola();
  if (gameState() != PENALTY_STATE)
  {
    estado_penalty = 0;
  }

  switch (gameState())
  {
  case FINISH_STATE:
    // nao faz nada. O programa está para terminar.
    break;
  case PAUSE_STATE:
    //Nao altera o goleiro
    if (!pos.ball.undef() &&
        !perdido[id_com] && !perdido[id_sem])
    {
      // decide entre os atuais com_bola e sem_bola quem vai ser o
      // novo com_bola, com base em qual estah mais próximo da bola.
      double dist_com = hypot(pos.me[id_com].y() - pos.ball.y(),
                              pos.me[id_com].x() - pos.ball.x());
      double dist_sem = hypot(pos.me[id_sem].y() - pos.ball.y(),
                              pos.me[id_sem].x() - pos.ball.x());
      if (dist_com > dist_sem)
      {
        papeis.me[id_sem].funcao = COM_BOLA;
        papeis.me[id_com].funcao = SEM_BOLA;
      }
      // else deixa_como_estah
    }
    // else deixa_como_estah
    break;
  case PENALTY_STATE:
    if (estado_penalty == 0)
    {
      estado_penalty = 1;
    }
    else if (estado_penalty == 1)
    {
      estado_penalty = 2;
    }
    if (estado_penalty == 1)
    {
      papeis.me[GOLEIRO_DEFAULT].funcao = GOLEIRO;
      if (aleatorio())
      {
        papeis.me[COM_BOLA_DEFAULT].funcao = COM_BOLA;
        papeis.me[SEM_BOLA_DEFAULT].funcao = SEM_BOLA;
      }
      else
      {
        papeis.me[SEM_BOLA_DEFAULT].funcao = COM_BOLA;
        papeis.me[COM_BOLA_DEFAULT].funcao = SEM_BOLA;
      }
    }
    break;
  case FREEKICK_STATE:
  case GOALKICK_STATE:
  case FREEBALL_STATE:
  case INICIALPOSITION_STATE:
  case CELEBRATION_STATE:
    // nesses casos, retorna aa atribuicao de funcoes inicial
    papeis.me[GOLEIRO_DEFAULT].funcao = GOLEIRO;
    papeis.me[COM_BOLA_DEFAULT].funcao = COM_BOLA;
    papeis.me[SEM_BOLA_DEFAULT].funcao = SEM_BOLA;
    break;
  case PLAY_STATE:

    // Se quiser eliminar a atribuicao dinamica de funcoes, descomente...

    /*
    papeis.me[GOLEIRO_DEFAULT].funcao = GOLEIRO;
    papeis.me[COM_BOLA_DEFAULT].funcao = COM_BOLA;
    papeis.me[SEM_BOLA_DEFAULT].funcao = SEM_BOLA;
    break;
    */

    if (bloqueado[id_gol] || perdido[id_gol])
    {
      // Nunca muda o goleiro qdo a bola estiver no campo de defesa
      if (bola_no_ataque)
      {
        // O goleiro estah bloqueado -> escolher o seguinte
        // para o gol e colocar o bloqueado como sem_bola
        papeis.me[id_gol].funcao = SEM_BOLA;
        if (!bloqueado[id_sem] && !perdido[id_sem])
        {
          papeis.me[id_sem].funcao = GOLEIRO;
          //papeis.me[id_com].funcao = COM_BOLA;  // Tautologia...
        }
        else
        {
          // goleiro e sem_bola estao com problemas...
          papeis.me[id_com].funcao = GOLEIRO;
          papeis.me[id_sem].funcao = COM_BOLA;
        }
      }
      //else nao troca goleiro: bola no ataque
    }
    else
    {
      // Nao altera o goleiro
      if (bloqueado[id_com] || perdido[id_com])
      {
        // Inverte com_bola e sem_bola
        papeis.me[id_com].funcao = SEM_BOLA;
        papeis.me[id_sem].funcao = COM_BOLA;
      }
      else if (!bloqueado[id_sem] && !perdido[id_sem])
      {
        // Nenhum robo estah bloqueado e todos foram localizados
        // Determinação dinâmica das funcoes

        // Soh existe possibilidade de troca de papeis se o sem_bola estiver atras da bola;
        // neste caso, pode trocar de funcoes com o com_bola
        bool sem_bola_atras_da_bola = sinal * pos.me[id_sem].x() < sinal * pos.ball.x();
        bool com_bola_atras_da_bola = sinal * pos.me[id_com].x() < sinal * pos.ball.x();
        double dist_com = hypot(pos.ball.x() - pos.me[id_com].x(),
                                pos.ball.y() - pos.me[id_com].y());
        double dist_sem = hypot(pos.ball.x() - pos.me[id_sem].x(),
                                pos.ball.y() - pos.me[id_sem].y());
        if (sem_bola_atras_da_bola)
        {
          if (com_bola_atras_da_bola)
          {
            // Deixa como estah, exceto se:
            // B - o sem_bola estiver bem mais proximo (1.25) da bola; e
            // C - o com_bola nao estiver alinhado para o gol.
            // neste caso, troca de funcoes com o com_bola
            if (dist_sem < dist_com / 1.25)
            {
              // sem_bola estah bem mais proximo da bola

              // Calcula se o robo com_bola esta alinhado

              // Angulo do robo para a bola
              double thetapos_fut = arc_tang(pos.future_ball.y() - pos.me[id_com].y(),
                                             pos.future_ball.x() - pos.me[id_com].x());
              // posicao onde a bola bateria se seguisse rumo ao fundo do campo de
              // acordo com a reta que vem do robo com_bola
              double ypos_fut = mytan(thetapos_fut) *
                                    (sinal * FIELD_WIDTH / 2.0 - pos.me[id_com].x()) +
                                pos.me[id_com].y();
              bool com_bola_alinhado = (com_bola_atras_da_bola &&
                                        fabs(ypos_fut) < GOAL_HEIGHT / 2.0);

              if (!com_bola_alinhado)
              {
                //com_bola nao esta orientado
                papeis.me[id_sem].funcao = COM_BOLA;
                papeis.me[id_com].funcao = SEM_BOLA;
              }
            }
          }
          else
          {
            // sem bola atras, com bola na frente da bola
            // Nesse caso, geralmente troca, a nao ser que o com_bola esteja bem mais perto (2.0)
            if (dist_sem < 2.0 * dist_com)
            { //|| dist_sem < 4*ROBOT_RADIUS tiramos para o jogo das oitavas
              papeis.me[id_sem].funcao = COM_BOLA;
              papeis.me[id_com].funcao = SEM_BOLA;
            }
          }
        }
        // else nunca troca se o sem_bola na frente da bola
      }
    }
    break;
  }
}

void Strategy::acao_goleiro(int id)
{
  if (pos.ball.undef() || perdido[id] || bloqueado[id])
  {
    papeis.me[id].acao = ESTACIONAR;
    return;
  }

  switch (gameState())
  {
  case PAUSE_STATE:
  case FINISH_STATE:
    papeis.me[id].acao = ESTACIONAR;
    break;
  case GOALKICK_STATE:
    papeis.me[id].acao = (getAdvantage() ? LADO_AREA : G_CENTRO_GOL);
    break;
  case PENALTY_STATE:
  case FREEKICK_STATE:
  case FREEBALL_STATE:
  case INICIALPOSITION_STATE:
    papeis.me[id].acao = G_CENTRO_GOL;
    break;
  case CELEBRATION_STATE:
    papeis.me[id].acao = COMEMORAR;
    break;
  case PLAY_STATE:
    acao_goleiro_play(id);
    break;
  }
}

void Strategy::acao_goleiro_play(int id)
{
  // GOLEIRO FORA DA AREA: PERIGO!!!!!!!!
  if (!meu_na_area[id])
  {
    // Situacao perigosa!
    // Tenta voltar aa area pelos lados para evitar gol contra
    if (meu_lado_area[id])
    { //predicado goleiro esta_lado_area
      papeis.me[id].acao = G_CENTRO_GOL;
    }
    else
    {
      papeis.me[id].acao = LADO_AREA;
    }
  }
  // testar se o goleiro estah preso atras do gol
  else if (meu_atras_gol[id])
  {
    papeis.me[id].acao = G_CENTRO_GOL;
  }
  // BOLA NA AREA
  else if (bola_area_defesa)
  {
    if (bola_parada &&
        !adv_na_area[id] && !adv_mais_prox_bola[id] &&
        !meu_na_frente_bola[id])
    { // predicado para bola bola_na_minha_frente
      papeis.me[id].acao = ISOLAR_BOLA;
    }
    else if (bola_area_lateral || //predicado para bola_dentro_area_lateral
             !meu_na_frente_bola[id])
    { //bola_na_minha_frente
      papeis.me[id].acao = IR_BOLA;
    }
    else
    {
      papeis.me[id].acao = G_DEFENDER;
    }
  }
  else if (bola_lado_area)
  {
    papeis.me[id].acao = IR_BOLA;
  }
  else if (bola_no_ataque)
  {
    papeis.me[id].acao = G_CENTRO_GOL;
  }
  else
  {
    papeis.me[id].acao = G_DEFENDER;
  }
}

void Strategy::acao_com_bola(int id)
{
  if (pos.ball.undef() || perdido[id] || bloqueado[id])
  {
    //Prioridade baixa
    if (pos.ball.undef())
      papeis.me[id].acao = ESTACIONAR;

    //Prioridade media
    if (bloqueado[id])
      papeis.me[id].acao = (gameState() == PAUSE_STATE ? ESTACIONAR : IR_MEIO_CAMPO);

    //Prioridade maxima
    if (perdido[id])
      papeis.me[id].acao = ESTACIONAR;

    return;
  }

  switch (gameState())
  {
  case PAUSE_STATE:
  case FINISH_STATE:
    papeis.me[id].acao = ESTACIONAR;
    break;
  case PENALTY_STATE:
    if (estado_penalty == 1)
    {
      papeis.me[id].acao = (getAdvantage() ? (aleatorio() ? POS_PENALTY1 : POS_PENALTY2) : IR_MEIO_CAMPO);
    }
    break;
  case FREEKICK_STATE:
    papeis.me[id].acao = (getAdvantage() ? A_IR_MARCA : FORMAR_BARREIRA);
    break;
  case GOALKICK_STATE:
    papeis.me[id].acao = (getAdvantage() ? G_CENTRO_GOL : A_IR_MARCA);
    break;
  case FREEBALL_STATE: //Vantagem quando cobrada no campo adversario
    papeis.me[id].acao = A_FREE_BALL;
    break;
  case INICIALPOSITION_STATE:
    papeis.me[id].acao = (getAdvantage() ? A_BOLA_CENTRO : CIRCULO_CENTRAL);
    break;
  case CELEBRATION_STATE:
    papeis.me[id].acao = COMEMORAR;
    break;
  case PLAY_STATE:
    acao_com_bola_play(id);
    // papeis.me[id].acao = A_LEVAR_BOLA;
    break;
  }
}

void Strategy::acao_com_bola_play(int id)
{
  if (bola_dentro_gol)
  {
    if (bola_no_ataque)
    {
      // Fizemos um gol !!!
      //papeis.me[id].acao = COMEMORAR;
    }
    else
    {
      // Sofremos um gol !!!
      papeis.me[id].acao = A_BOLA_CENTRO;
    }
    return;
  }

  // Bola nas paredes
  if (bola_parede_fundo || bola_quina_fundo ||
      bola_parede_lateral || bola_quina_frente ||
      bola_parede_frente)
  {

    if (meu_posicionado_descolar[id] || (papeis.me[id].acao == A_DESCOLAR && !meu_furou[id]))
    {
      papeis.me[id].acao = A_DESCOLAR;
    }
    else
    {
      papeis.me[id].acao = A_POSICIONAR_PARA_DESCOLAR;
    }
    return;
  }

  // Bola dentro da area
  if (bola_area_defesa)
  {
    papeis.me[id].acao = LADO_AREA;
    return;
  }
  // Bola na frente da area
  if (bola_frente_area)
  {

    if (meu_frente_area[id] && (papeis.me[id].acao == ISOLAR_BOLA ||
                                !meu_na_frente_bola[id]))
    { // predicado bola bola_na_minha_frente
      papeis.me[id].acao = ISOLAR_BOLA;
    }
    else
    {
      papeis.me[id].acao = A_POSICIONAR_FRENTE_AREA;
    }
    //    }
    return;
  }

  // Bola no lado da area
  if (bola_lado_area)
  {
    if (meu_na_area[id])
    {
      if (meu_atras_gol[id])
      {
        papeis.me[id].acao = G_CENTRO_GOL;
      }
      else
      {
        papeis.me[id].acao = ISOLAR_BOLA;
      }
    }
    else
    {
      papeis.me[id].acao = LADO_AREA;
    }
    return;
  }

  // Bola nas outras regiões do campo:
  // bola_fundo, bola_lateral, bola_frente ou região normal
  if (meu_na_frente_bola[id])
  {
    if (bola_fundo || bola_lateral || bola_frente)
    {
      papeis.me[id].acao = A_CONTORNAR_POR_DENTRO;
    }
    else
    {
      papeis.me[id].acao = A_CONTORNAR;
    }
  }
  else
  {
    if (bola_no_ataque && !bola_lateral && !bola_frente)
    {

      if (meu_alinhado_chutar[id] && (papeis.me[id].acao == A_CHUTAR_GOL ||
                                      meu_bem_alinhado_chutar[id]))
      {
        papeis.me[id].acao = A_CHUTAR_GOL;
      }
      else
      {
        // papeis.me[id].acao = A_ALINHAR_GOL;
        papeis.me[id].acao = A_LEVAR_BOLA;
      }
    }
    else
    {
      if (bola_fundo)
      {
        papeis.me[id].acao = ISOLAR_BOLA;
        return;
      }
      //      bool nao_passou = fabs (pos.me[id].y()) < fabs(pos.ball.y());
      //      bool nao_bem_passado = fabs (pos.me[id].y()) < fabs(pos.ball.y())+MARGEM_HIS;
      if (bola_lateral)
      {
        if (bola_frente_y_do_meu[id])
        {
          papeis.me[id].acao = A_ALINHAR_SEM_ORIENTACAO;
        }
        else
        {
          papeis.me[id].acao = ISOLAR_BOLA;
        }
        return;
      }
      if (bola_frente)
      {

        if (meu_posicionado_isolar[id])
        {
          papeis.me[id].acao = ISOLAR_BOLA;
        }
        else
        {
          papeis.me[id].acao = A_ALINHAR_SEM_ORIENTACAO;
        }
        return;
      }
      // Usa um criterio de "alinhamento" bem mais
      // simples para priorizar o chute. Alem disso, apos "alinhado",
      // simplesmente chuta a bola para frente

      if (meu_alinhado_isolar[id] && (papeis.me[id].acao == ISOLAR_BOLA ||
                                      meu_bem_alinhado_isolar[id]))
      {
        papeis.me[id].acao = ISOLAR_BOLA;
      }
      else
      {
        papeis.me[id].acao = A_ALINHAR_SEM_ORIENTACAO;
      }
    }
  }
}

void Strategy::acao_sem_bola(int id)
{
  if (pos.ball.undef() || perdido[id] || bloqueado[id])
  {
    //Prioridade baixa
    if (pos.ball.undef())
      //ir_ultima posicao da bola;
      papeis.me[id].acao = (gameState() == PAUSE_STATE ? ESTACIONAR : D_NAO_ATRAPALHAR);

    //Prioridade media
    if (bloqueado[id])
      papeis.me[id].acao = (gameState() == PAUSE_STATE ? ESTACIONAR : IR_MEIO_CAMPO);

    //Prioridade maxima
    if (perdido[id])
      papeis.me[id].acao = ESTACIONAR;
    return;
  }

  switch (gameState())
  {
  case PAUSE_STATE:
  case FINISH_STATE:
    papeis.me[id].acao = ESTACIONAR;
    break;
  case PENALTY_STATE:
    if (estado_penalty == 1)
    {
      papeis.me[id].acao = getAdvantage() ? (papeis.me[com_bola()].acao == POS_PENALTY1 ? POS_PENALTY2 : POS_PENALTY1) : IR_MEIO_CAMPO;
    }
    break;
  case FREEKICK_STATE:
    papeis.me[id].acao = (getAdvantage() ? IR_MEIO_CAMPO : FORMAR_BARREIRA);
    break;
  case GOALKICK_STATE:
  case FREEBALL_STATE: //NAS DUAS SITUACOES AFASTAR_DO_COM_BOLA
    papeis.me[id].acao = D_NAO_ATRAPALHAR;
    break;
  case INICIALPOSITION_STATE:
    papeis.me[id].acao = CIRCULO_CENTRAL;
    break;
  case CELEBRATION_STATE:
    papeis.me[id].acao = COMEMORAR;
    break;
  case PLAY_STATE:
    papeis.me[id].acao = D_NAO_ATRAPALHAR;
    break;
  }
}

void Strategy::calcula_referencias(int id)
{
  double ang_gol_penalty;
  switch (papeis.me[id].acao)
  {
  case ESTACIONAR:
    bypassControl[id] = true;
    ref.me[id].x() = pos.me[id].x();
    ref.me[id].y() = pos.me[id].y();
    ref.me[id].theta() = POSITION_UNDEFINED; // pos.me.[id].theta();
    pwm.me[id].left = 0.0;
    pwm.me[id].right = 0.0;
    break;
  case IR_MEIO_CAMPO:
    ref.me[id].x() = (getAdvantage() ? -sinal : sinal) * ROBOT_RADIUS;
    ref.me[id].y() = (id == 0 ? 0.0 : (id == 1 ? +1.0 : -1.0) * (CIRCLE_RADIUS + ROBOT_RADIUS));
    ref.me[id].theta() = POSITION_UNDEFINED; //0.0;
    break;
  case FORMAR_BARREIRA:
    ref.me[id].x() = -sinal * (FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH - ROBOT_RADIUS);
    ref.me[id].y() = (id == com_bola() ? ARC_HEIGHT / 2.0 + ROBOT_RADIUS : -(ARC_HEIGHT / 2.0 + ROBOT_RADIUS));
    ref.me[id].theta() = M_PI_2;
    break;
  case CIRCULO_CENTRAL:
    if (id == com_bola())
    {
      ref.me[id].x() = -sinal * (CIRCLE_RADIUS + ROBOT_RADIUS);
      ref.me[id].y() = 0.0;
      ref.me[id].theta() = 0.0;
    }
    else
    {
      ref.me[id].x() = -sinal * (CIRCLE_RADIUS + ROBOT_RADIUS) * 0.707;
      ref.me[id].y() = (CIRCLE_RADIUS + ROBOT_RADIUS) * 0.707;
      ref.me[id].theta() = POSITION_UNDEFINED;
    }
    break;
  case COMEMORAR:
  { // :-)

    double tempo_volta = 3.0; // Uma volta em 5 segundos
    double ang_circulo = ang_equiv(2.0 * M_PI * id_pos / (FPS * tempo_volta));
    ang_circulo += id * 2.0 * M_PI / 3.0;
    ref.me[id].x() = 1.0 * CIRCLE_RADIUS * cos(ang_circulo);
    ref.me[id].y() = 1.0 * CIRCLE_RADIUS * sin(ang_circulo);
    ref.me[id].theta() = ang_circulo + M_PI_2;

    /*bypassControl[id] = true;
      ref.me[id].x() = pos.me[id].x();
      ref.me[id].y() = pos.me[id].y();
      ref.me[id].theta() = pos.me[id].theta();
      pwm.me[id].left = 0.0;
      pwm.me[id].right = 0.0;
	*/
  }
  break;
  case ISOLAR_BOLA:
  {
    double dist = hypot(pos.ball.y() - pos.me[id].y(),
                        pos.ball.x() - pos.me[id].x());
    double ang = arc_tang(pos.ball.y() - pos.me[id].y(),
                          pos.ball.x() - pos.me[id].x());
    ref.me[id].x() = pos.me[id].x() + (dist + DIST_CHUTE) * cos(ang);
    ref.me[id].y() = pos.me[id].y() + (dist + DIST_CHUTE) * sin(ang);
    ref.me[id].theta() = POSITION_UNDEFINED;
  }
  break;
  case IR_BOLA:
    ref.me[id].x() = pos.ball.x();
    ref.me[id].y() = pos.ball.y();
    ref.me[id].theta() = POSITION_UNDEFINED;
    //x*p[] y*p[] t*p[]
    break;
  case LADO_AREA:
    ref.me[id].x() = -sinal * (FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH - ROBOT_RADIUS);
    ref.me[id].y() = sgn(pos.me[id].y()) * //MELHORAR DEPOIS
                     (GOAL_FIELD_HEIGHT / 2.0 + ROBOT_EDGE / 2.0);
    ref.me[id].theta() = POSITION_UNDEFINED; // M_PI_2;
    break;
  case POS_PENALTY1:
    ref.me[id].x() = -sinal * ROBOT_EDGE / 2.0;
    ref.me[id].y() = CIRCLE_RADIUS - ROBOT_EDGE / 2.0;
    ref.me[id].theta() = (sinal > 0.0 ? -M_PI / 8.0 : -M_PI + M_PI / 8.0);
    break;
  case POS_PENALTY2:
    ang_gol_penalty = arc_tang(PK_Y - (GOAL_HEIGHT / 2.0 - 4.0 * BALL_RADIUS),
                               sinal * PK_X - sinal * FIELD_WIDTH / 2.0);
    ref.me[id].x() = sinal * FIELD_WIDTH / 2.0 + ((FIELD_WIDTH / 2.0 - CIRCLE_RADIUS / 2.0) * cos(ang_gol_penalty));
    ref.me[id].y() = (GOAL_HEIGHT / 2.0 - 4.0 * BALL_RADIUS) + ((FIELD_WIDTH / 2.0 - CIRCLE_RADIUS / 2.0) * sin(ang_gol_penalty));
    ref.me[id].theta() = ang_equiv(ang_gol_penalty + M_PI);
    break;
  case G_DEFENDER:
    ref.me[id].x() = -sinal * (FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH / 2.0);
    // Usar a estimativa de posição da bola qdo bola estiver rápida...
    if (pos.vel_ball.mod < VEL_BOLA_LENTA ||
        (mySide() == LEFT_SIDE && cos(pos.vel_ball.ang) >= 0.0) ||
        (mySide() == RIGHT_SIDE && cos(pos.vel_ball.ang) <= 0.0))
    {
      ref.me[id].y() = pos.ball.y();
    }
    else
    {
      ref.me[id].y() = pos.ball.y() +
                       tan(pos.vel_ball.ang) * (ref.me[id].x() - pos.ball.x());
    }
    if (fabs(ref.me[id].y()) > (GOAL_HEIGHT - ROBOT_EDGE / 2.0) / 2.0)
    {
      ref.me[id].y() = sgn(ref.me[id].y()) * (GOAL_HEIGHT - ROBOT_EDGE / 2.0) / 2.0;
    }
    ref.me[id].theta() = M_PI_2;
    break;
  case G_CENTRO_GOL:
    ref.me[id].x() = -sinal * (FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH / 2.0);
    ref.me[id].y() = 0.0;
    ref.me[id].theta() = M_PI_2;
    break;
  case A_IR_MARCA:
    ref.me[id].x() = sinal * (PK_X - 1.5 * DIST_CHUTE);
    ref.me[id].y() = PK_Y;
    // FALTA: fazer pequenos movimentos angulares para despistar
    ref.me[id].theta() = POSITION_UNDEFINED;
    break;
  case A_FREE_BALL:
    ref.me[id].x() = sgn(pos.ball.x()) * FB_X - sinal * DIST_FB;
    ref.me[id].y() = PK_Y;
    ref.me[id].theta() = 0.0;
    break;
  case A_BOLA_CENTRO:
    ref.me[id].x() = -sinal * 2.0 * ROBOT_RADIUS;
    ref.me[id].y() = 0.0;
    ref.me[id].theta() = 0.0;
    break;
  case A_DESCOLAR:
  {
    pwm.me[id] = descolar_parede(id);

    POS_ROBO ref;
    double ang = M_PI_2 / 2.0;
    if ((sinal * pos.me[id].y()) > 0.0)
    {
      ref.x() = pos.me[id].x() + (ROBOT_EDGE / 2.0) * cos(pos.me[id].theta() - ang);
      ref.y() = pos.me[id].y() + (ROBOT_EDGE / 2.0) * sin(pos.me[id].theta() - ang);
      ref.theta() = pos.me[id].theta() - ang;
    }
    else
    {
      ref.x() = pos.me[id].x() + (ROBOT_EDGE / 2.0) * cos(pos.me[id].theta() + ang);
      ref.y() = pos.me[id].y() + (ROBOT_EDGE / 2.0) * sin(pos.me[id].theta() + ang);
      ref.theta() = pos.me[id].theta() + ang;
    }
  }
  break;
  case A_POSICIONAR_PARA_DESCOLAR:
    ref.me[id] = posicao_para_descolar_bola();
    break;
  case A_POSICIONAR_FRENTE_AREA:
    ref.me[id].x() = -sinal * (FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH - ROBOT_RADIUS);
    if (pos.ball.y() > 0.0)
    {
      //tenta isolar para cima (y>0)
      ref.me[id].y() = pos.ball.y() - 2.0 * DIST_CHUTE;
    }
    else
    {
      //isola para baixo (y<0)
      ref.me[id].y() = pos.ball.y() + 2.0 * DIST_CHUTE;
    }
    ref.me[id].theta() = POSITION_UNDEFINED;
    break;
  case A_CONTORNAR_POR_DENTRO:
    ref.me[id].x() = pos.ball.x() - sinal * DIST_CHUTE;
    ref.me[id].y() = pos.ball.y() - sgn(pos.ball.y()) * (BALL_RADIUS + 1.5 * ROBOT_RADIUS);
    ref.me[id].theta() = POSITION_UNDEFINED;
    break;
  case A_CONTORNAR:
    // A referência "x" pode sair do campo,
    // mas não tem pb porque ele muda de estado antes
    ref.me[id].x() = pos.ball.x() - sinal * DIST_CHUTE;
    if (pos.ball.y() > pos.me[id].y())
    {
      //esta abaixo da bola
      ref.me[id].y() = pos.ball.y() - DIST_CHUTE;
      if (ref.me[id].y() < -(FIELD_HEIGHT / 2.0 - ROBOT_RADIUS))
      {
        ref.me[id].y() = -(FIELD_HEIGHT / 2.0 - ROBOT_RADIUS);
      }
    }
    else
    {
      //esta acima da bola
      ref.me[id].y() = pos.ball.y() + DIST_CHUTE;
      if (ref.me[id].y() > (FIELD_HEIGHT / 2.0 - ROBOT_RADIUS))
      {
        ref.me[id].y() = (FIELD_HEIGHT / 2.0 - ROBOT_RADIUS);
      }
    }
    ref.me[id].theta() = POSITION_UNDEFINED;
    if (sinal * ref.me[id].x() < -(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH) &&
        fabs(ref.me[id].y()) < GOAL_FIELD_HEIGHT / 2.0)
    {
      ref.me[id].x() = -sinal * (FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH);
    }
    break;
  case A_ALINHAR_SEM_ORIENTACAO:
  case A_ALINHAR_GOL:
    ref.me[id].theta() = arc_tang(-pos.future_ball.y(),
                                  sinal * FIELD_WIDTH / 2.0 - pos.future_ball.x());
    ref.me[id].x() = pos.future_ball.x() - DIST_CHUTE * cos(ref.me[id].theta());
    ref.me[id].y() = pos.future_ball.y() - DIST_CHUTE * sin(ref.me[id].theta());
    //ef.me[id].theta() = POSITION_UNDEFINED;
    // testa se o alinhamento fica fora dos limites laterais
    if (fabs(ref.me[id].y()) > (FIELD_HEIGHT / 2.0 - ROBOT_RADIUS))
    {
      ref.me[id].y() = sgn(ref.me[id].y()) * (FIELD_HEIGHT / 2.0 - ROBOT_RADIUS);
    }
    // testa se o alinhamento fica fora dos limites de fundo
    if (fabs(ref.me[id].x()) > (FIELD_WIDTH / 2.0 - ROBOT_RADIUS))
    {
      ref.me[id].x() = sgn(ref.me[id].x()) * (FIELD_WIDTH / 2.0 - ROBOT_RADIUS);
    }
    // testa se o alinhamento bate nas quinas
    if (fabs(ref.me[id].y()) > (FIELD_HEIGHT / 2.0 - ROBOT_RADIUS - CORNER_DIMENSION) &&
        fabs(ref.me[id].x()) > (FIELD_WIDTH / 2.0 - ROBOT_RADIUS - CORNER_DIMENSION))
    {
      ref.me[id].x() = sgn(ref.me[id].x()) *
                       (FIELD_WIDTH / 2.0 - ROBOT_RADIUS - CORNER_DIMENSION / 2.0);
      ref.me[id].y() = sgn(ref.me[id].y()) *
                       (FIELD_HEIGHT / 2.0 - ROBOT_RADIUS - CORNER_DIMENSION / 2.0);
    }
    if (papeis.me[id].acao == A_ALINHAR_SEM_ORIENTACAO)
    {
      ref.me[id].theta() = POSITION_UNDEFINED;
    }
    break;
  case A_LEVAR_BOLA:
  {
    Coord2 future_ball_tmp;
    double dt = dt_amostr*10.0;//usando a predição para 5 amostras na frente
    // posição futura da bola
    future_ball_tmp.x() = pos.ball.x() + 
                          pos.vel_ball.mod*cos(pos.vel_ball.ang)*dt;
    future_ball_tmp.y() = pos.ball.y() + 
                          pos.vel_ball.mod*sin(pos.vel_ball.ang)*dt;
    
    double ang = arc_tang(future_ball_tmp.y() - pos.me[id].y(),
                          future_ball_tmp.x() - pos.me[id].x());

    ref.me[id].theta() = arc_tang( -future_ball_tmp.y(),
                                  sinal * (FIELD_WIDTH/2.0 + GOAL_FIELD_WIDTH) - future_ball_tmp.x());
    ref.me[id].x() = future_ball_tmp.x() + (BALL_RADIUS)*cos(ang);
    ref.me[id].y() = future_ball_tmp.y() + (BALL_RADIUS)*sin(ang);
    break;
  }
  case A_CHUTAR_GOL:
  {
    double ang = arc_tang(pos.ball.y() - pos.me[id].y(),
                          pos.ball.x() - pos.me[id].x());
    ref.me[id].theta() = POSITION_UNDEFINED;
    ref.me[id].x() = sinal * FIELD_WIDTH / 2.0;
    ref.me[id].y() = mytan(ang) * (sinal * FIELD_WIDTH / 2.0 - pos.me[id].x()) + pos.me[id].y();
  }
  break;

  case D_NAO_ATRAPALHAR:
  {
    //      cout << "inicio D_NAO_ATRAPALHAR\n"; cout.flush();
    R.iniciar();
    // Incluir bola para se afastar
    if (!pos.ball.undef())
    {
      R.incluirObstaculo(pos.ball);
    }
    //      cout << "quase meio D_NAO_ATRAPALHAR\n"; cout.flush();
    // Incluir colegas para se afastar
    for (int i = 0; i < 3; i++)
    {
      if (i != id && !pos.me[i].undef())
      {
        R.incluirObstaculo(pos.me[i]);
      }
    }
    //      cout << "quase quase meio D_NAO_ATRAPALHAR\n"; cout.flush();
    // Incluir referencias dos colegas para se afastar
    for (int i = 0; i < 3; i++)
    {
      if (i != id && !ref.me[i].undef())
      {
        R.incluirObstaculo(ref.me[i]);
      }
    }
    //      cout << "meio D_NAO_ATRAPALHAR\n"; cout.flush();
    //ESCOLHA DA POSICAO PREFERENCIAL
    //POS_BOLA r = R.calculaPosicao(pos.me[id]);
    POS_BOLA r;
    if (sinal * pos.ball.x() < 0.0 ||
        sinal * pos.me[com_bola()].x() < 0.0)
    {
      //r = R.calculaPosicao(pos.me[id]); // Ficar parado
      r.x() = pos.me[com_bola()].x() + sinal * DIST_CHUTE;
      r.y() = 0.0;
      r = R.calculaPosicao(r);
    }
    else
    {
      r.x() = pos.me[com_bola()].x() - sinal * DIST_CHUTE;
      r.y() = 0.0;
      r = R.calculaPosicao(r);
    }
    ref.me[id].x() = r.x();
    ref.me[id].y() = r.y();
    ref.me[id].theta() = 0.0;
  }
  //    cout << "fim D_NAO_ATRAPALHAR\n"; cout.flush();
  break;
  case IMPOSSIVEL:
    printf("Acao em situacao Impossivel foi chamada!\n");
  case NAO_DEFINIDO:
    ref.me[id].x() = POSITION_UNDEFINED;
    ref.me[id].y() = POSITION_UNDEFINED;
    ref.me[id].theta() = POSITION_UNDEFINED;
    break;
  }
}

POS_ROBO Strategy::posicao_para_descolar_bola()
{
  // Calcula a origem do sistema de coordenadas da parede
  POS_ROBO origem = calcula_origem_parede();

  // Altera posicao da bola para o lado de cima e atacando para direita
  POS_BOLA new_pos_ball;
  new_pos_ball.x() = sinal * pos.ball.x(); // sempre sinal>0
  new_pos_ball.y() = fabs(pos.ball.y());   // sempre ybol>0
  //cout << "new_pos_ball: ";
  //cout << new_pos_ball.x() << ',' << new_pos_ball.y() << endl;

  // Calcula posição relativa da nova bola em relação à parede
  new_pos_ball = posicao_relativa(new_pos_ball, origem);
  //cout << "new_pos_ball2: ";
  //cout << new_pos_ball.x() << ',' << new_pos_ball.y() << endl;

  // Calcula a referência relativa do robô
  POS_ROBO new_ref;
  const double SOBRA = 1.1 * (ROBOT_RADIUS - ROBOT_EDGE / 2.0);
  //  const double dist_robo_parede = SOBRA+(ROBOT_EDGE);
  const double dist_robo_bola_y = BALL_RADIUS + ROBOT_EDGE - 0.02;
  const double dist_robo_bola_x = ROBOT_EDGE - SOBRA;

  new_ref.x() = new_pos_ball.x() + dist_robo_bola_x;
  new_ref.y() = new_pos_ball.y() - dist_robo_bola_y;
  //if (new_pos_ball.x()<SOBRA+(ROBOT_EDGE/2.0)) {

  // }
  new_ref.theta() = M_PI;
  //cout << "new_ref: ";
  //cout << new_ref.x() << ',' << new_ref.y() << ',' << new_ref.theta() << endl;

  // Transforma a referência relativa para absoluta
  POS_ROBO ref = posicao_absoluta(new_ref, origem);
  //cout << "ref: ";
  //cout << ref.x() << ',' << ref.y() << ',' << ref.theta() << endl;

  // Destransforma para os dois lados e dois sentidos de ataque
  ref.x() = sinal * ref.x();
  ref.y() = sgn(pos.ball.y()) * ref.y();
  if (sinal < 0.0)
    ref.theta() = ang_equiv(M_PI - ref.theta());
  if (pos.ball.y() < 0.0)
    ref.theta() = -ref.theta();
  //cout << "ref: ";
  //cout << ref.x() << ',' << ref.y() << ',' << ref.theta() << endl;

  return ref;
}

POS_ROBO Strategy::calcula_origem_parede()
{
  POS_ROBO origem;

  if (bola_parede_fundo)
  {
    origem.x() = -(FIELD_WIDTH / 2.0);
    origem.y() = (GOAL_FIELD_HEIGHT / 2.0);
    origem.theta() = 0.0;
  }
  else if (bola_quina_fundo)
  {
    origem.x() = -(FIELD_WIDTH / 2.0);
    origem.y() = (FIELD_HEIGHT / 2.0 - CORNER_DIMENSION);
    origem.theta() = -M_PI_4;
  }
  else if (bola_parede_lateral)
  {
    origem.x() = -(FIELD_WIDTH / 2.0 - CORNER_DIMENSION);
    origem.y() = (FIELD_HEIGHT / 2.0);
    origem.theta() = -M_PI_2;
  }
  else if (bola_quina_frente)
  {
    origem.x() = (FIELD_WIDTH / 2.0 - CORNER_DIMENSION);
    origem.y() = (FIELD_HEIGHT / 2.0);
    origem.theta() = -3.0 * M_PI_4;
  }
  else if (bola_parede_frente)
  {
    origem.x() = (FIELD_WIDTH / 2.0);
    origem.y() = (GOAL_HEIGHT / 2.0 - CORNER_DIMENSION);
    origem.theta() = -M_PI;
  }
  else
  {
    origem.x() = 0.0;
    origem.y() = 0.0;
    origem.theta() = 0.0;
  }
  return origem;
}

static POS_BOLA posicao_relativa(POS_BOLA pos, POS_ROBO origem)
{
  POS_BOLA new_pos_t, new_pos_rot;
  new_pos_t.x() = pos.x() - origem.x();
  new_pos_t.y() = pos.y() - origem.y();
  new_pos_rot.x() = cos(origem.theta()) * new_pos_t.x() + sin(origem.theta()) * new_pos_t.y();
  new_pos_rot.y() = -sin(origem.theta()) * new_pos_t.x() + cos(origem.theta()) * new_pos_t.y();
  return new_pos_rot;
}

static POS_ROBO posicao_absoluta(POS_ROBO pos, POS_ROBO origem)
{
  POS_ROBO new_pos_t, new_pos_rot;
  new_pos_rot.x() = cos(origem.theta()) * pos.x() - sin(origem.theta()) * pos.y();
  new_pos_rot.y() = sin(origem.theta()) * pos.x() + cos(origem.theta()) * pos.y();
  new_pos_rot.theta() = pos.theta() + origem.theta();
  new_pos_t.x() = new_pos_rot.x() + origem.x();
  new_pos_t.y() = new_pos_rot.y() + origem.y();
  new_pos_t.theta() = new_pos_rot.theta();
  return new_pos_t;
}

/*
  static void repulsao(double &dx, double &dy, double dist, double ang)
  {
  if (dist < 0.0) {
  dist = -dist;
  ang += M_PI;
  }
  if (dist > 3*DIST_CHUTE) return;
  double dist_repulsao = 3*DIST_CHUTE-dist;
  dx += dist_repulsao*cos(ang);
  dy += dist_repulsao*sin(ang);
  }
*/

PWM_WHEEL Strategy::descolar_parede(int id)
{
  // Calcula a origem do sistema de coordenadas da parede
  PWM_WHEEL ret;
  POS_ROBO origem = calcula_origem_parede();
  //cout << "Origem: ";
  //cout << origem.x() << ',' << origem.y() << ',' << origem.theta() << endl;

  // Altera posicao da bola para o lado de cima e atacando para direita
  //cout << "pos_ball: ";
  //cout << pos.ball.x() << ',' << pos.ball.y() << endl;
  /*  POS_BOLA new_pos_ball;
      new_pos_ball.x() = sinal*pos.ball.x(); // sempre sinal>0
      new_pos_ball.y() = fabs(pos.ball.y()); // sempre ybol>0
      //cout << "new_pos_ball: 0";
      //cout << new_pos_ball.x() << ',' << new_pos_ball.y() << endl;

      // Calcula posição relativa da nova bola em relação à parede
      new_pos_ball = posicao_relativa(new_pos_ball, origem);*/
  double ang_robo = (1.0 - sinal) * M_PI_2 + sinal * pos.me[id].theta();
  double ang_relativo = ang_equiv(sgn(pos.ball.y()) * ang_robo - origem.theta());

  //cout << new_pos_ball.x() << ',' << new_pos_ball.y() << endl;
  bypassControl[id] = true;
  //  if (ang_relativo > M_PI_4 || ang_relativo < -M_PI_2) {
  if (fabs(ang_relativo) > M_PI_2)
  {
    //  cout << "FRENTE\n";
    ret.left = 1.0;
    ret.right = 0.0;
  }
  else
  {
    //   cout << "DE COSTAS\n";
    ret.left = 0.0;
    ret.right = -1.0;
  }
  // SWAP ENTRE RODAS  caso jogando sentido contrario
  if (sinal * sgn(pos.ball.y()) < 0.0)
  {
    //  cout << "SWAP\n";
    double temp = ret.left;
    ret.left = ret.right;
    ret.right = temp;
  }
  return ret;
}
