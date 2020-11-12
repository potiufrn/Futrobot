#include "localization.h"
#include "../parameters.h"
#include "../functions.h"
#include <iostream>

using namespace std;

Localization::Localization( TEAM team, SIDE side, GAME_MODE gameMode): 
  FutData( team,side,gameMode)
{
  //INICIALIZAR PARAMETROS DA CLASSE
  
}

Localization::~Localization()
{
  
}

bool Localization::localization()
{
  static double raio, theta;
  static double vbola,thetabola;
  static double dt, dist;

  if(gameMode() == SIMULATED_MODE)
  {
    dist = hypot(pos.me[com_bola()].y() - pos.ball.y(), pos.me[com_bola()].x() - pos.ball.x());
    dt = 1.0*dist;//usado na LARC 2020 (jogos simulados a 60FPS)
    // calcula velocidade e orientação da bola
    vbola = hypot(pos.ball.y()-ant.ball.y(), pos.ball.x()-ant.ball.x())/dt;
    thetabola = arc_tang(pos.ball.y()-ant.ball.y(), pos.ball.x()-ant.ball.x());
    
    pos.vel_ball.mod = (pos.vel_ball.mod + vbola)/2.0;
    pos.vel_ball.ang = (pos.vel_ball.ang + thetabola)/2.0;
    
    // posição futura da bola
    pos.future_ball.x() = pos.ball.x() + 
                          pos.vel_ball.mod*cos(pos.vel_ball.ang)*dt;
    pos.future_ball.y() = pos.ball.y() + 
                          pos.vel_ball.mod*sin(pos.vel_ball.ang)*dt;
    return false;
  }

  //**********************************************************
  //******* CORRECAO DA PARALAXE *****************************
  //**********************************************************
  for(int i=0; i < 3; i++){
  //correção de paralaxe para os nossos robos
    if(pos.me[i].y() != POSITION_UNDEFINED && pos.me[i].x() != POSITION_UNDEFINED){
      raio = hypot(pos.me[i].y(),pos.me[i].x());
      theta = atan2(pos.me[i].y(),pos.me[i].x());
      raio *= (CAMERA_HEIGHT - ROBOT_HEIGHT)/CAMERA_HEIGHT;
      pos.me[i].x() = raio*cos(theta);
      pos.me[i].y() = raio*sin(theta);
    }
  //correção de paralaxe para os robos adversarios
    if(pos.op[i].y() != POSITION_UNDEFINED && pos.op[i].x() != POSITION_UNDEFINED){
      raio = hypot(pos.op[i].y(),pos.op[i].x());
      theta = atan2(pos.op[i].y(),pos.op[i].x());
      raio *= (CAMERA_HEIGHT - ROBOT_HEIGHT)/CAMERA_HEIGHT;
      pos.op[i].x() = raio*cos(theta);
      pos.op[i].y() = raio*sin(theta);
    }
  }
  //correcao de paralaxe da bola
  if(pos.ball.y() != POSITION_UNDEFINED && pos.ball.x() != POSITION_UNDEFINED){
    raio = hypot(pos.ball.y(),pos.ball.x());
    theta = atan2(pos.ball.y(),pos.ball.x());
    raio *= (CAMERA_HEIGHT - BALL_HEIGHT)/CAMERA_HEIGHT;
    pos.ball.x() = raio*cos(theta);
    pos.ball.y() = raio*sin(theta);
  }

  //**********************************************************
  //********** CALCULO DA VELOCIDADE DA BOLA *****************
  //**********************************************************

  if (pos.ball.x() != POSITION_UNDEFINED && pos.ball.y() != POSITION_UNDEFINED) {
    vbola = hypot(pos.ball.y()-ant.ball.y(), pos.ball.x()-ant.ball.x())/dt_amostr;
    thetabola = arc_tang(pos.ball.y()-ant.ball.y(), pos.ball.x()-ant.ball.x());
    if (vbola > VEL_BOLA_PARADA) {
      pos.vel_ball.mod = (pos.vel_ball.mod + vbola)/2.0;
      pos.vel_ball.ang = (pos.vel_ball.ang + thetabola)/2.0;
    }
    else {
      pos.vel_ball.mod = pos.vel_ball.mod / 2.0; 
      pos.vel_ball.ang  = 0.0;
    }
  }
  else{
    pos.ball = ant.ball;
    pos.vel_ball.mod = 0.0;
    pos.vel_ball.ang = 0.0;
  }
  if(isnan(pos.ball.x()) || isnan(pos.ball.y())){
    cerr<<"POS ATUAL DA BOLA EH NAN!!! "
	<<__FILE__<<" "<<__LINE__ <<endl;
  }
  if(isnan(pos.vel_ball.mod) || isnan(pos.vel_ball.ang)){
    cerr<<"VELOCIDADE DA BOLA EH NAN!!! "
	<<__FILE__<<" "<<__LINE__ <<endl;
  }

  pos.future_ball.x() = pos.ball.x() + 
                        pos.vel_ball.mod*cos(pos.vel_ball.ang)*TEMPO_BOLA_FUTURA;
  pos.future_ball.y() = pos.ball.y() + 
                        pos.vel_ball.mod*sin(pos.vel_ball.ang)*TEMPO_BOLA_FUTURA;


  if(isnan(pos.future_ball.x()) || isnan(pos.future_ball.y())){
    cerr<<"POS_FUT DA BOLA EH NAN!!! " <<__FILE__<<" "<<__LINE__ << endl;
    cerr<<"\t"<< pos.ball.x() << " " << pos.ball.y() << " "
	      << pos.vel_ball.mod << " " << pos.vel_ball.ang << " "
	      << pos.future_ball.x() << " " << pos.future_ball.y() << endl;
  }

  return false;
}
