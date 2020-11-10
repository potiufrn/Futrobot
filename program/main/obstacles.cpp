#include <iostream>
#include <cmath>
#include "obstacles.h"
#include "../functions.h"
#include "../parameters.h"

bool fusao_obst(double &x1, double &y1, double &r1,
                double x2, double y2, double r2);

double desvio_obst(double x_rob, double y_rob, double x_ref, double y_ref,
                   double x_obs, double y_obs, double raio);

Obstacles::Obstacles(TEAM team, SIDE side, GAME_MODE gameMode) : FutData(team, side, gameMode)
{
}

Obstacles::~Obstacles()
{
}

bool Obstacles::obstacles()
{
  //desviar de obstaculos apenas no estado de play e no modo simulado
  if(gameMode() == SIMULATED_MODE && gameState() != PLAY_STATE)
    return false;

  bool ignora_meu[3], ignora_adv[3];
  double dist, xc, yc, raio, gama, delta_gama;
  int i, j, k;

  // Calcula o efeito dos obstáculos sobre o i-ésimo robô do meu time
  for (i = 0; i < 3; i++)
  {
    delta_gama = 0.0;
    for (j = 0; j < 3; j++)
    {
      // A princípio, todos os robôs do meu time são obstáculos
      ignora_meu[j] = false;
      // Todos os robôs do outro time são obstáculos, exceto o
      // adversário que está com a ball
      dist = hypot(pos.ball.y() - pos.op[j].y(), pos.ball.x() - pos.op[j].x());
      ignora_adv[j] = (dist <= ROBOT_RADIUS + BALL_RADIUS);
    }
    // O robô não é obstáculo para si mesmo
    ignora_meu[i] = true;
    // Calcula o desvio dos outros robôs do meu time
    for (j = 0; j < 3; j++)
      if (!ignora_meu[j])
      {
        // Inicialmente, o j-ésimo robô é representado por um obstáculo
        // posicionado na mesma posição e com o mesmo raio que o robô
        xc = pos.me[j].x();
        yc = pos.me[j].y();
        raio = ROBOT_RADIUS;
        // Testa se o j-ésimo robô do meu time deve ser fundido com o
        // k-ésimo robô do mesmo time
        for (k = j + 1; k < 3; k++)
          if (!ignora_meu[k])
          {
            ignora_meu[k] = fusao_obst(xc, yc, raio,
                                       pos.me[k].x(), pos.me[k].y(), ROBOT_RADIUS);
          }
        // Testa se o j-ésimo robô do meu time deve ser fundido com o
        // k-ésimo robô do outro time
        for (k = 0; k < 3; k++)
          if (!ignora_adv[k])
          {
            ignora_adv[k] = fusao_obst(xc, yc, raio,
                                       pos.op[k].x(), pos.op[k].y(), ROBOT_RADIUS);
          }
        // Desvia do j-ésimo robô do meu time
        gama = desvio_obst(pos.me[i].x(), pos.me[i].y(),
                           ref.me[i].x(), ref.me[i].y(),
                           xc, yc, raio);
        if (fabs(gama) > fabs(delta_gama))
          delta_gama = gama;
      }
    // Calcula o desvio dos robôs do outro time
    for (j = 0; j < 3; j++)
      if (!ignora_adv[j])
      {
        // Inicialmente, o j-ésimo robô é representado por um obstáculo
        // posicionado na mesma posição e com o mesmo raio que o robô
        xc = pos.op[j].x();
        yc = pos.op[j].y();
        raio = ROBOT_RADIUS;
        // Testa se o j-ésimo robô do outro time deve ser fundido com o
        // k-ésimo robô do mesmo time
        for (k = j + 1; k < 3; k++)
          if (!ignora_adv[k])
          {
            ignora_adv[k] = fusao_obst(xc, yc, raio,
                                       pos.op[k].x(), pos.op[k].y(), ROBOT_RADIUS);
          }
        // Desvia do j-ésimo robô do outro time
        gama = desvio_obst(pos.me[i].x(), pos.me[i].y(),
                           ref.me[i].x(), ref.me[i].y(),
                           xc, yc, raio);
        if (fabs(gama) > fabs(delta_gama))
          delta_gama = gama;
      }
    // Desvia a referência do i-ésimo robô
    gama = arc_tang(ref.me[i].y() - pos.me[i].y(), ref.me[i].x() - pos.me[i].x());
    dist = hypot(ref.me[i].y() - pos.me[i].y(), ref.me[i].x() - pos.me[i].x());
    gama += delta_gama;
    ref.me[i].x() = pos.me[i].x() + dist * cos(gama);
    ref.me[i].y() = pos.me[i].y() + dist * sin(gama);
    ref.me[i].theta() += delta_gama;
  }
  return false;
}

// Caso a distância entre dois obstáculos não permita a passagem de um
// robô entre eles, os dois obstáculos são fundidos no primeiro.
// Retorna true caso a fusão tenha ocorrido

bool fusao_obst(double &x1, double &y1, double &r1,
                double x2, double y2, double r2)
{
  double dist = hypot(y2 - y1, x2 - x1);
  if (dist - r1 - r2 > 2 * ROBOT_RADIUS)
  {
    return false;
  }
  double theta = arc_tang(y2 - y1, x2 - x1);
  double novo_raio = (dist + r1 + r2) / 2.0;
  x1 += (novo_raio - r1) * cos(theta);
  y1 += (novo_raio - r1) * sin(theta);
  r1 = novo_raio;
  return true;
}

// Calcula de quanto deve ser girada a referência do robô, dada a
// posição do robô, a referência atual e a posição e o raio do obstáculo

double desvio_obst(double x_rob, double y_rob, double x_ref, double y_ref,
                   double x_obs, double y_obs, double raio)
{
  raio = fabs(raio) + ROBOT_RADIUS; // Obstáculo engrandecido
  double delta_x = x_obs - x_rob, delta_y = y_obs - y_rob;
  double dist = hypot(delta_x, delta_y);
  if (dist - raio >= ZONA_INFL_MAX)
  {
    // O robô está fora da zona de influência do obstáculo
    return 0.0;
  }
  double coef;
  if (dist - raio <= ZONA_INFL_MIN)
  {
    // O robô está dentro da zona de influência total do obstáculo
    coef = 1.0;
  }
  else
  {
    // O robô está dentro da zona de influência parcial do obstáculo
    coef = 1.0 - (dist - raio - ZONA_INFL_MIN) / (ZONA_INFL_MAX - ZONA_INFL_MIN);
  }

  double ang_faixa, t1, t2;
  // Calcula a faixa angular [t1,t2] que está "eclipsada" pelo obstáculo
  if (dist >= raio)
  {
    ang_faixa = asin(raio / dist);
  }
  else
  {
    dist = 2 * raio - dist;
    ang_faixa = M_PI - asin(raio / dist);
  }
  t2 = ang_equiv(arc_tang(delta_y, delta_x) + ang_faixa);
  t1 = ang_equiv(arc_tang(delta_y, delta_x) - ang_faixa);

  // Calcula o ângulo do robô para a referência. Se estiver na faixa
  // "eclipsada", retorna o ângulo de giro.
  double theta = arc_tang(y_ref - y_rob, x_ref - x_rob);
  if (t2 < t1)
  {
    if (theta >= 0)
      t2 += 2 * M_PI;
    else
      t1 -= 2 * M_PI;
  }
  if (theta >= t2 || theta <= t1)
  {
    return 0.0;
  }
  double gama;
  if (t2 - theta < theta - t1)
  {
    gama = t2 - theta;
  }
  else
  {
    gama = t1 - theta;
  }
  return coef * gama;
}

void Obstacles::desvio_area(int i)
{
  //TRATAMENTO DE TRAJETOS DENTRO DA AREA (proximas 45 linhas)
  int sinal = (mySide() == LEFT_SIDE ? 1 : -1);
  double angrob, angref;
  angrob = atan2(sgn(pos.me[i].y()) * (GOAL_FIELD_HEIGHT / 2.0 + ROBOT_RADIUS) - pos.me[i].y(),
                 -(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH - ROBOT_RADIUS) - sinal * pos.me[i].x());
  angref = atan2(ref.me[i].y() - sgn(pos.me[i].y()) * (GOAL_FIELD_HEIGHT / 2.0 + ROBOT_RADIUS),
                 sinal * ref.me[i].x() - (-(FIELD_WIDTH / 2.0 - GOAL_FIELD_WIDTH - ROBOT_RADIUS)));

  if ((sinal * pos.me[i].x() < -FIELD_WIDTH / 2.0 + GOAL_FIELD_WIDTH + ROBOT_RADIUS &&
       fabs(pos.me[i].y()) > GOAL_FIELD_HEIGHT / 2.0) ||
      sinal * ref.me[i].x() < -FIELD_WIDTH / 2.0 + GOAL_FIELD_WIDTH + ROBOT_RADIUS)
  {
    if (sinal * pos.me[i].x() < -FIELD_WIDTH / 2.0 + GOAL_FIELD_WIDTH + ROBOT_RADIUS &&
        fabs(pos.me[i].y()) > GOAL_FIELD_HEIGHT / 2.0)
    {

      if (pos.me[i].y() > 0)
      {
        if (angref < angrob && ref.me[i].y() < GOAL_FIELD_HEIGHT / 2.0 + ROBOT_RADIUS)
        {
          ref.me[i].x() = sinal * (-FIELD_WIDTH / 2.0 + GOAL_FIELD_WIDTH + 2 * ROBOT_RADIUS);
          ref.me[i].y() = (GOAL_FIELD_HEIGHT / 2.0 + ROBOT_RADIUS);
          ref.me[i].theta() = POSITION_UNDEFINED;
        }
      }
      else
      {
        if (angref > angrob && ref.me[i].y() > -GOAL_FIELD_HEIGHT / 2.0 - ROBOT_RADIUS)
        {
          ref.me[i].x() = sinal * (-FIELD_WIDTH / 2.0 + GOAL_FIELD_WIDTH + 2 * ROBOT_RADIUS);
          ref.me[i].y() = -(GOAL_FIELD_HEIGHT / 2.0 + ROBOT_RADIUS);
          ref.me[i].theta() = POSITION_UNDEFINED;
        }
      }
    }
    else
    { //sinal*ref.me[i].x() < -FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+ROBOT_RADIUS
      if (ref.me[i].y() > 0)
      {
        if (angref > angrob && pos.me[i].y() < GOAL_FIELD_HEIGHT / 2.0 + ROBOT_RADIUS)
        {
          ref.me[i].y() = (GOAL_FIELD_HEIGHT / 2.0 + ROBOT_RADIUS);
          ref.me[i].x() = sinal * (-FIELD_WIDTH / 2.0 + GOAL_FIELD_WIDTH + 2 * ROBOT_RADIUS) +
                          sinal * fabs(ref.me[i].y() - pos.me[i].y()) / 2.0;
          ref.me[i].theta() = POSITION_UNDEFINED;
        }
      }
      else
      {
        if (angref < angrob && pos.me[i].y() > -(GOAL_FIELD_HEIGHT / 2.0 + ROBOT_RADIUS))
        {
          ref.me[i].y() = -(GOAL_FIELD_HEIGHT / 2.0 + ROBOT_RADIUS);
          ref.me[i].x() = sinal * (-FIELD_WIDTH / 2.0 + GOAL_FIELD_WIDTH + 2 * ROBOT_RADIUS) +
                          sinal * fabs(ref.me[i].y() - pos.me[i].y()) / 2.0;
          ref.me[i].theta() = POSITION_UNDEFINED;
        }
      }
    }
  }
}
