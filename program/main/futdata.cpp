#include <math.h>
#include <sys/time.h>

#include "futdata.h"
#include "../comunicacao.h"

FutData::FutData(TEAM team, SIDE side, GAME_MODE mode)
{
  dt_amostr = 1.0 / FPS;
  start_data(team, side, mode);
  // est_jogo = JOGAR;
  game_state = PLAY_STATE;
  advantage = true;
  for (int i = 0; i < 3; i++)
  {
    bloqueado[i] = false;
    bypassControl[i] = false;
  }
}

void FutData::start_data(TEAM team, SIDE side, GAME_MODE gameMode)
{
  my_team = team;
  my_side = side;
  game_mode = gameMode;

  ant.me[0].x() = pos.me[0].x() = -0.6;
  ant.me[0].y() = pos.me[0].y() = 0.5;
  ant.me[0].theta() = pos.me[0].theta() = 0.0;

  ant.me[1].x() = pos.me[1].x() = 0.0;
  ant.me[1].y() = pos.me[1].y() = 0.0;
  ant.me[1].theta() = pos.me[1].theta() = M_PI_2;

  ant.me[2].x() = pos.me[2].x() = -0.6;
  ant.me[2].y() = pos.me[2].y() = -0.5;
  ant.me[2].theta() = pos.me[2].theta() = M_PI_2;

  ant.op[0].x() = pos.op[0].x() = 0.6;
  ant.op[0].y() = pos.op[0].y() = 0.5;
  ant.op[0].theta() = pos.op[0].theta() = M_PI;

  ant.op[1].x() = pos.op[1].x() = 0.6;
  ant.op[1].y() = pos.op[1].y() = 0.0;
  ant.op[1].theta() = pos.op[1].theta() = M_PI_2;

  ant.op[2].x() = pos.op[2].x() = 0.6;
  ant.op[2].y() = pos.op[2].y() = -0.5;
  ant.op[2].theta() = pos.op[2].theta() = M_PI;

  ant.ball.x() = pos.ball.x() = 0.0;
  ant.ball.y() = pos.ball.y() = 0.0;

  memset(ant.future_ball, 0, sizeof(POS_BOLA)*3);

  pos.vel_ball.mod = 0.0;
  pos.vel_ball.ang = 0.0;

  for (int i = 0; i < 3; i++)
  {
    ref.me[i].x() = pos.me[i].x();
    ref.me[i].y() = pos.me[i].y();
    ref.me[i].theta() = pos.me[i].theta();
    pwm.me[i].left = 0.0;
    pwm.me[i].right = 0.0;
  }
}
