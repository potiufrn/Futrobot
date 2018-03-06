#include <iostream>
#include <cmath>
#include "strategy.h"
#include "../parameters.h"
#include "../functions.h"
#include "../system.h"


using namespace::std;

#define EPSILON_L 0.01 //CHECAR
#define DELTA_L (2*EPSILON_L) //CHECAR
#define LONGE_L (4*EPSILON_L) //CHECAR
 
#define EPSILON_ANG 0.087 //5graus //CHECAR
#define LONGE_ANG (4*EPSILON_ANG) //CHECAR
#define DELTA_ANG (2*EPSILON_ANG) //CHECAR



static const double LIMITE_CONT_PARADO = 75;
static const double DIST_CHUTE = 0.10;
static const double CHUTE_GOLEIRO = 0.15;
static const double VEL_BOLA_LENTA = 0.2;



Strategy::Strategy(TEAM team, SIDE side, GAME_MODE gameMode):
  FutData(team,side,gameMode)
{
  //INICIALIZAR OS PARAMETROS DA CLASSE
  t = 0.0;

  for(int i=0; i < 3; i++){
    bloqueado[i] = false;
    contador_parado[i] = 0;
  }
  
  zagueiro = 2;
  atacante = 1;
  goleiro = 0;

  sinal = (mySide() == LEFT_SIDE ? 1 : -1);

  //PREENCHIMENTO DA TABELA V. DO ATACANTE
  preenchimento();

}

Strategy::~Strategy(){

}

// bool Strategy::strategy()
// {
//   //IMPLEMENTAR A ESTRATEGIA AQUI
  
//   static double ultimo_t = 0.0, sinal=1.0;

//   if (myTeam() == YELLOW_TEAM) {
//     // Trajetória linear
//     /*
//     ref.me[0].x() = (FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH)*sin(0.3*t);
//     ref.me[0].y() = 0.0;
//     ref.me[0].theta() = 0.0;
//     // Mudanças abruptas
//     //ref.me[0].x() = 0.4;
//     //ref.me[0].y() = sinal*0.5;
//     //ref.me[0].theta() = M_PI_4*sinal;
//     ref.me[2].x() = 0.0;
//     ref.me[2].y() = 0.0;
//     ref.me[2].theta() = 0.0;    
//     // Círculo
//     ref.me[1].x() = CIRCLE_RAY*cos(t);
//     ref.me[1].y() = CIRCLE_RAY*sin(t);
//     ref.me[1].theta() = ang_equiv2(t-M_PI_2);
//     */
//     // Círculo
// //     ref.me[0].x() = sinal*(1.5/2 - 0.15);
// //     ref.me[0].y() = -0.70/2;
// //     ref.me[0].theta() = 0.0;
// //     ref.me[1].x() = (FIELD_WIDTH/2.0 - 2*ROBOT_RAY)*cos(t);
// //     ref.me[1].y() = FIELD_HEIGHT/2.0 - ROBOT_RAY;
// //     ref.me[1].theta() = 0.0;
// //     ref.me[2].x() = sinal*(1.5/2 - 0.15);
// //     ref.me[2].y() = 0.70/2;
// //     ref.me[2].theta() = 0.0;

//     double RAIO_GIRO = 1.5;
//     ref.me[0].x() = RAIO_GIRO*CIRCLE_RAY*cos(t);
//     ref.me[0].y() = RAIO_GIRO*CIRCLE_RAY*sin(t);
//     ref.me[0].theta() = ang_equiv2(t-M_PI_2);
//     ref.me[1].x() = RAIO_GIRO*CIRCLE_RAY*cos(t + (2*M_PI)/3);
//     ref.me[1].y() = RAIO_GIRO*CIRCLE_RAY*sin(t + (2*M_PI)/3);
//     ref.me[1].theta() = ang_equiv2((t + (2*M_PI)/3)-M_PI_2); 
//     ref.me[2].x() = RAIO_GIRO*CIRCLE_RAY*cos(t + (4*M_PI)/3);
//     ref.me[2].y() = RAIO_GIRO*CIRCLE_RAY*sin(t + (4*M_PI)/3);
//     ref.me[2].theta() = ang_equiv2((t + (4*M_PI)/3)-M_PI_2);

// //     ref.me[0].x() = 2.0*CIRCLE_RAY*cos(t);
// //     ref.me[0].y() = 2.0*CIRCLE_RAY*sin(t);
// //     ref.me[0].theta() = ang_equiv2(t-M_PI_2);
// //     ref.me[1].x() = 2.0*CIRCLE_RAY*cos(t);
// //     ref.me[1].y() = 2.0*CIRCLE_RAY*sin(t);
// //     ref.me[1].theta() = ang_equiv2(t-M_PI_2); 
// //     ref.me[2].x() = 2.0*CIRCLE_RAY*cos(t);
// //     ref.me[2].y() = 2.0*CIRCLE_RAY*sin(t);
// //     ref.me[2].theta() = ang_equiv2(t-M_PI_2);
//  }
//   else {
//     /*    // Trajetória linear
//     ref.me[0].x() = -(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH);
//     ref.me[0].y() = (GOAL_FIELD_HEIGHT/2.0)*sin(t/2);
//     ref.me[0].theta() = M_PI_2;
//     // Chutar bola para o gol
//     if (fabs(pos.ball.x())<0.4 &&
// 	pos.ball.x()>pos.me[1].x()) {
//       ref.me[1].x() = pos.ball.x();
//       ref.me[1].y() = pos.ball.y();
//       ref.me[1].theta() = arc_tang(-pos.ball.y(),FIELD_WIDTH/2.0-pos.ball.x());
//     }
//     else {
//       ref.me[1].x() = -0.4;
//       ref.me[1].y() = pos.ball.y();
//       ref.me[1].theta() = 0.0;
//     }
//     // Círculo
//     ref.me[2].x() = CIRCLE_RAY*cos(t);
//     ref.me[2].y() = CIRCLE_RAY*sin(t);
//     ref.me[2].theta() = ang_equiv2(t-M_PI_2);
//     */
    
//     ref.me[1].x() = CIRCLE_RAY*cos(1*t);
//     ref.me[1].y() = CIRCLE_RAY*sin(1*t);
//     ref.me[1].theta() = ang_equiv2(1*t-M_PI_2);
//     ref.me[1].theta() = ang_equiv2(0.25*t-M_PI_2);
//     ref.me[0].x() = 1.5*CIRCLE_RAY*cos(1*t);
//     ref.me[0].y() = 1.5*CIRCLE_RAY*sin(1*t);
//     ref.me[0].theta() = ang_equiv2(1*t-M_PI_2); 
//     ref.me[2].x() = 2.0*CIRCLE_RAY*cos(1*t);
//     ref.me[2].y() = 2.0*CIRCLE_RAY*sin(1*t);
//     ref.me[2].theta() = ang_equiv2(1*t-M_PI_2);
    
// //     ref.me[0].x() = sinal*(1.5/2 - 0.15);
// //     ref.me[0].y() = -0.70/2;
// //     ref.me[0].theta() = 0.0;
// //     ref.me[1].x() = (FIELD_WIDTH/2.0 - 2*ROBOT_RAY)*cos(t);
// //     ref.me[1].y() = FIELD_HEIGHT/2.0 - ROBOT_RAY;
// //     ref.me[1].theta() = 0.0;
// //     ref.me[2].x() = sinal*(1.5/2 - 0.15);
// //     ref.me[2].y() = 0.70/2;
// //     ref.me[2].theta() = 0.0;
    
//   }
//   if (t-ultimo_t > 8.0) {
//     ultimo_t = t;
//     sinal = -sinal;
//   }
  
//   t+=dt_amostr; 

//   return false;

// }



//########################################################################
//########################################################################

void Strategy::preenchimento() {
  for (unsigned int i=0; i <256; i++) {
    if (i & 64) {
      resp_atacante[i]=ESTACIONAR;
    }
    else {
      if (i & (unsigned int)32) {
	if (i & (unsigned int)16) {
	  if (i & (unsigned int) 8) {
	    resp_atacante[i]=CHUTAR_GOL;
	  }
	  else {
	    resp_atacante[i]=GIRAR;
	  }
	}
	else {
	  if (i & (unsigned int)4) {
	    if (i & (unsigned int)2) {
	      if (i & (unsigned int)1) {
		resp_atacante[i]=DESCOLAR_QUINA;
	      }
	      else {
		resp_atacante[i]=DESCOLAR_PAREDE;
	      }
	    } else {
	      resp_atacante[i]=ALINHAR_PAREDE;
	    }
	  } 
	  else {
	    resp_atacante[i]=ALINHAR;
	  }
	}
      } 
      else {
	resp_atacante[i]=CONTORNAR;
      }
      
    }
    //cout << "resp_atacante[" << i << "]=" << resp_atacante[i] << endl;
  }
}  




bool Strategy::strategy()
{
  int i;

  t = relogio();
  double distlin_ref, distlin_ant, distang_ref, distang_ant;

  //a principio sempre deixa o controle atuar
  for(int i=0; i < 3; i++)
    bypassControl[i] = false;
  sinal = (mySide() == LEFT_SIDE ? 1 : -1);
  //switch baseado no estado do jogo.
  switch(gameState()){
  case FINISH_STATE:
    //nao faz nada. O programa está para terminar.
    return false;
    break;
  case PAUSE_STATE:
    for(int ind = 0; ind < 3; ind++){
      contador_parado[ind] = 0;
      bloqueado[ind] = false;
    }

    //enquanto parado, decide quem vai ser o atacante. Ele vai ser o
    //mais próximo da bola.
    {
    double dist_zag = hypot( pos.me[zagueiro].y()-pos.ball.y(),
			     pos.me[zagueiro].x()-pos.ball.x());

    double dist_atk = hypot( pos.me[atacante].y()-pos.ball.y(),
			     pos.me[atacante].x()-pos.ball.x());

    //    cout << "dist_zag = "<< dist_zag << endl;
    //    cout << "dist_atk = "<< dist_atk << endl;


    if(dist_zag < dist_atk){
      int aux_id = zagueiro;
      zagueiro = atacante;
      atacante = aux_id;
      //      cout << "TROCOU!" << endl;
    }
    }

    //mantem os robôs aonde eles estão.
    //desabilita controle nesta iteracao
    for(i=0; i<3;i++){
      bypassControl[i] = true;
      ref.me[i].x() = pos.me[i].x();
      ref.me[i].y() = pos.me[i].y();
      ref.me[i].theta() = pos.me[i].theta();
      pwm.me[i].left = 0.0;
      pwm.me[i].right = 0.0;
    }
    break;
  case PENALTY_STATE:
    //manda os robôs para a posição de defender/bater penalti
    if(getAdvantage()){ 
      //vai bater um penalti
      ref.me[goleiro].x() = sinal*(-(FIELD_WIDTH/2-ROBOT_EDGE/2));
      ref.me[goleiro].y() = 0.0;
      ref.me[goleiro].theta() = M_PI_2;
      ref.me[atacante].x() = sinal*(PK_WIDTH-ROBOT_EDGE);
      ref.me[atacante].y() = 0.0;
      ref.me[atacante].theta() = M_PI_2;
      ref.me[zagueiro].x() = sinal*(-ROBOT_EDGE);
      ref.me[zagueiro].y() = -CIRCLE_RAY/2;
      ref.me[zagueiro].theta() = 0.0;
    }else{ 
      //vai defender um penalti
      ref.me[goleiro].x() = sinal*(-(FIELD_WIDTH/2-ROBOT_EDGE/2));
      ref.me[goleiro].y() = 0.0;
      ref.me[goleiro].theta() = M_PI_2;
      ref.me[atacante].x() = sinal*(ROBOT_EDGE);
      ref.me[atacante].y() = CIRCLE_RAY/2;
      ref.me[atacante].theta() = 0.0;
      ref.me[zagueiro].x() = sinal*(ROBOT_EDGE);
      ref.me[zagueiro].y() = -CIRCLE_RAY/2;
      ref.me[zagueiro].theta() = 0.0;
    }
    break;
  case FREEKICK_STATE:
    //manda os robôs para a posição de defender/bater tiro livre
    if(getAdvantage()){ 
      //vai bater um tiro livre
      ref.me[goleiro].x() = sinal*(-(FIELD_WIDTH/2-ROBOT_EDGE/2));
      ref.me[goleiro].y() = 0.0;
      ref.me[goleiro].theta() = M_PI_2;
      ref.me[atacante].x() = sinal*(PK_WIDTH-ROBOT_EDGE);
      ref.me[atacante].y() = 0.0;
      ref.me[atacante].theta() = M_PI_2;
      ref.me[zagueiro].x() = sinal*(-ROBOT_EDGE);
      ref.me[zagueiro].y() = -CIRCLE_RAY/2;
      ref.me[zagueiro].theta() = 0.0;
    }else{
      //vai defender um tiro livre
      ref.me[goleiro].x() = sinal*(-(FIELD_WIDTH/2-ROBOT_EDGE/2));
      ref.me[goleiro].y() = 0.0;
      ref.me[goleiro].theta() = M_PI_2;
      ref.me[atacante].x() = sinal*(-(FIELD_WIDTH/2
				      -GOAL_FIELD_WIDTH-ROBOT_EDGE/2));
      ref.me[atacante].y() = ARC_HEIGHT/2+ROBOT_EDGE;
      ref.me[atacante].theta() = M_PI_2;
      ref.me[zagueiro].x() = sinal*(-(FIELD_WIDTH/2
				      -GOAL_FIELD_WIDTH-ROBOT_EDGE/2));
      ref.me[zagueiro].y() = -(ARC_HEIGHT/2+ROBOT_EDGE);
      ref.me[zagueiro].theta() = M_PI_2;
      
      
    }
    break;
  case GOALKICK_STATE:
    //manda os robôs para a posição de defender/bater tiro de meta
    if(getAdvantage()){ 
      //vai bater um tiro de meta
      ref.me[atacante].x() = sinal*(-(FIELD_WIDTH/2 - ROBOT_EDGE));
      if(fabs(pos.ball.y()) <= GOAL_FIELD_HEIGHT/2){
	ref.me[atacante].y() = pos.ball.y();
      }else{
	ref.me[atacante].y() = 0.0;
      }
      ref.me[atacante].theta() = 0.0;
      ref.me[goleiro].x() = sinal*(-(FIELD_WIDTH/2-GOAL_FIELD_WIDTH/2));
      ref.me[goleiro].y() = GOAL_FIELD_HEIGHT/2+ROBOT_EDGE;
      ref.me[goleiro].theta() = M_PI_2;
      ref.me[zagueiro].x() = sinal*(-(FK_WIDTH));
      ref.me[zagueiro].y() = -FK_HEIGHT;
      ref.me[zagueiro].theta() = 0.0;
           
    }else{
      //vai esperar o adversario bater o tiro de meta
      ref.me[goleiro].x() = sinal*(-(FIELD_WIDTH/2-ROBOT_EDGE/2));
      ref.me[goleiro].y() = 0.0;
      ref.me[goleiro].theta() = M_PI_2;
      ref.me[atacante].x() = sinal*(FK_WIDTH/2);
      ref.me[atacante].y() = FK_HEIGHT/2;
      ref.me[atacante].theta() = M_PI_2;
      ref.me[zagueiro].x() = sinal*(FK_WIDTH/2);
      ref.me[zagueiro].y() = -FK_HEIGHT/2;
      ref.me[zagueiro].theta() = 0.0;
    }
    break;
  case FREEBALL_STATE:
    //TODO: FALTA FAZER;
    break;
  case INICIALPOSITION_STATE:
    if(getAdvantage()){ 
      //vai iniciar com a bola
      ref.me[goleiro].x() =sinal*(-(FIELD_WIDTH/2-ROBOT_EDGE/2));
      ref.me[goleiro].y() = 0.0;
      ref.me[goleiro].theta() = M_PI_2;
      ref.me[atacante].x() = sinal*(-(ROBOT_EDGE));
      ref.me[atacante].y() = 0.0;
      ref.me[atacante].theta() = 0.0;
      ref.me[zagueiro].x() = sinal*(-FK_WIDTH/2);
      ref.me[zagueiro].y() = -FK_HEIGHT/2;
      ref.me[zagueiro].theta() = 0.0;
    }else{
      //vai esperar o time adversario iniciar
      ref.me[goleiro].x() = sinal*(-(FIELD_WIDTH/2-ROBOT_EDGE/2));
      ref.me[goleiro].y() = 0.0;
      ref.me[goleiro].theta() = M_PI_2;
      ref.me[atacante].x() = sinal*(-(CIRCLE_RAY+ROBOT_EDGE));
      ref.me[atacante].y() = 0.0;
      ref.me[atacante].theta() = 0.0;
      ref.me[zagueiro].x() = sinal*(-FK_WIDTH/2);
      ref.me[zagueiro].y() = -FK_HEIGHT/2;
      ref.me[zagueiro].theta() = 0.0;
      
    }
    break;
  case PLAY_STATE:
    //detecta se o robo esta bloqueado
    for(int ind = 0; ind < 3; ind++){
      distlin_ant = hypot( pos.me[ind].y()-ant.me[ind].y(),
			   pos.me[ind].x()-ant.me[ind].x() );
      distang_ant = fabs(ang_equiv(pos.me[ind].theta() - ant.me[ind].theta()));

      distlin_ref = hypot( ref.me[ind].y()-ant.me[ind].y(),
			   ref.me[ind].x()-ant.me[ind].x() );
      distang_ref = fabs(ang_equiv(ref.me[ind].theta() - ant.me[ind].theta()));
      if((distlin_ant <= EPSILON_L &&
	  distang_ant <= EPSILON_ANG &&
	  (distlin_ref > LONGE_L || distang_ref > LONGE_ANG)
	  ) ||
	 (pos.me[ind].x() == POSITION_UNDEFINED)){

        //robô está parado ou perdido.
        contador_parado[ind]++;
        if(contador_parado[ind] > LIMITE_CONT_PARADO)
          //o robo esta parado por mais que LIMITE_CONT_PARADO iterações
          bloqueado[ind] = true;
      }else{
        contador_parado[ind] = 0;
        bloqueado[ind] = false;
      }
    }

    if( pos.ball.x() != POSITION_UNDEFINED && 
	pos.ball.y() != POSITION_UNDEFINED ){


      // Determinação dinâmica dos papéis
      /*
	double dist0 = hypot(pos.ball.x()-pos.me[0].x(),pos.ball.y()-pos.me[0].y()),
	dist1 = hypot(pos.ball.x()-pos.me[1].x(),pos.ball.y()-pos.me[1].y());
	if( dist0<dist1) {
	atacante = 0;
	zagueiro = 1;
	}
	else {
	atacante = 1;
	zagueiro = 0;
	}
	if ( sinal*pos.ball.x() < 0 ) {
	if (sinal*pos.me[0].x() < sinal*pos.ball.x() &&
	sinal*pos.me[1].x() >= sinal*pos.ball.x()) {
        atacante = 0;
        zagueiro = 1;
	}
	else if (sinal*pos.me[1].x() < sinal*pos.ball.x() &&
	sinal*pos.me[0].x() >= sinal*pos.ball.x()) {
        atacante = 1;
        zagueiro = 0;
	}
	}
      */
      
      /* TROCA */
      if(bloqueado[atacante] &&
	
	 hypot(pos.ball.x()-pos.me[atacante].x(),pos.ball.y()-pos.me[atacante].y()) > ROBOT_RAY*2){
	int aux = atacante;
	atacante = zagueiro;
	zagueiro = aux;
	//	cout << "TROCOU BLOQUEADO!!" << endl;
      }

      // Determinação estática dos papéis
      //zagueiro = 0;
      //atacante = 1;
      //goleiro = 2;

      //      goleiro_floripa(goleiro);
      goleiro_brasilia(goleiro);
      atacante_floripa(atacante);
      // A função de cálculo da referência do defensor deve ser chamada depois
      // do atacante e do goleiro, para que o defensor possa evitá-los
      defensor_floripa(zagueiro,atacante,goleiro);

    }
    else {
      // Quando perdeu a bola, deixa as referências como está...
      for (i=0; i<3; i++) {
	papeis.me[i] = ESTACIONAR;
      }
    }
    for (i=0; i<3; i++) {
      //cout << "Papel do atacante:"<< papeis.me[atacante] << endl;
      switch (papeis.me[i]) {

	// Os robôs assumem este papel quando não sabem o que fazer
	// ou em caso de perigo de gol contra.

      case ESTACIONAR:
	bypassControl[i] = true;
	ref.me[i].x() = pos.me[i].x();
	ref.me[i].y() = pos.me[i].y();
	ref.me[i].theta() = pos.me[i].theta();
	pwm.me[i].left = 0.0;
	pwm.me[i].right = 0.0;
	break;

	// Papeis do atacante

      case CONTORNAR:
	//Se esta muito acima ou muito abaixo da bola, vai para a
	//posição da bola
	if (fabs(pos.me[i].y()-pos.ball.y()) > DIST_CHUTE ) {
	  ref.me[i].x() = pos.ball.x() - sinal*DIST_CHUTE;
	  ref.me[i].y() = pos.me[i].y();
	}
	else {
	  if (pos.ball.y() > pos.me[i].y()) {
	    //esta abaixo da bola
	    ref.me[i].x() = pos.ball.x() - sinal*DIST_CHUTE;
	    ref.me[i].y() = pos.ball.y()-DIST_CHUTE;
	    if (ref.me[i].y() < -FIELD_HEIGHT/2.0) ref.me[i].y() = 0.0;
	  }
	  else {
	    //esta acima da bola
	    ref.me[i].x() = pos.ball.x() - sinal*DIST_CHUTE;
	    ref.me[i].y() = pos.ball.y()+DIST_CHUTE;
	    if (ref.me[i].y() > FIELD_HEIGHT/2.0) ref.me[i].y() = 0.0;
	  }
	}
	// A referência "x" pode sair do campo,
	// mas não tem pb porque ele muda de estado antes
	ref.me[i].theta() = 0;
	if (sinal*ref.me[i].x() < -(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH) &&
	    fabs(ref.me[i].y()) < GOAL_FIELD_HEIGHT/2.0) {
	  ref.me[i].x() = -sinal*(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH);
	}
	break;

      case ALINHAR:
      case ALINHAR_PAREDE:
	ref.me[i].theta() = arc_tang(-pos.future_ball.y(),
				     sinal*FIELD_WIDTH/2.0-pos.future_ball.x());
	ref.me[i].x() = pos.future_ball.x()-DIST_CHUTE*cos(ref.me[i].theta());
	ref.me[i].y() = pos.future_ball.y()-DIST_CHUTE*sin(ref.me[i].theta());
	if (fabs(ref.me[i].y())>(FIELD_HEIGHT/2.0-ROBOT_RAY)) {//testa se o alinhamento fica fora dos limites laterais
	  ref.me[i].y()=sgn(ref.me[i].y())*(FIELD_HEIGHT/2.0-ROBOT_RAY);
	}
	if (fabs(ref.me[i].x())>(FIELD_WIDTH/2.0-ROBOT_RAY)) {// testa se o alinhamento fica fora dos limites de fundo
	  ref.me[i].x()=sgn(ref.me[i].x())*(FIELD_WIDTH/2.0-ROBOT_RAY);
	  if (sinal*ref.me[i].x() < 0) {
	    ref.me[i].y()=pos.future_ball.y() - sgn(pos.future_ball.y())*ROBOT_RAY;
	  }
	}
	if (fabs(ref.me[i].x()) > BOUND_BACK_WALL_CORNER &&
	    fabs(ref.me[i].y()) > BOUND_SIDE_WALL_CORNER &&
	    fabs(ref.me[i].x())+fabs(ref.me[i].y()) > BOUND_SIDE_WALL_CORNER + BOUND_BACK_WALL_CORNER) {
	  // A referencia extrapolou os limites de quina
	  // Projetando a posicao dentro dos limites da quina 
	  // a partir da posicao fora 
	  double lado = fabs(ref.me[i].y()) -(fabs (ref.me[i].y()) -fabs(ref.me[i].x()) + (FIELD_HEIGHT+FIELD_WIDTH)/2.0-CORNER_DIMENSION)/2.0 ;
	  ref.me[i].x() -= sgn(ref.me[i].x())*lado;
	  ref.me[i].y() -= sgn(ref.me[i].y())*lado;
	}  
	break;
      case GIRAR:
	ref.me[i].theta() = arc_tang(pos.ball.y()-pos.me[i].y(),
				     pos.ball.x()-pos.me[i].x());
	ref.me[i].x() = pos.me[i].x();
	ref.me[i].y() = pos.me[i].y();
	break;

      case CHUTAR_GOL:
	ref.me[i].theta() = arc_tang(pos.ball.y()-pos.me[i].y(),
				     pos.ball.x()-pos.me[i].x());
	ref.me[i].x() = sinal*FIELD_WIDTH/2.0;
	ref.me[i].y() = mytan(ref.me[i].theta())*
	  (sinal*FIELD_WIDTH/2.0 - pos.me[i].x()) + pos.me[i].y();
	break;
 
      case DESCOLAR_QUINA:
	ref.me[i].theta() = arc_tang(pos.ball.y()-pos.me[i].y(),
				     pos.ball.x()-pos.me[i].x());
	if (fabs(ang_equiv2(ref.me[i].theta())) > 22.5 &&
	    fabs(ang_equiv2(ref.me[i].theta())) < 67.5) {
	  //em quandrante impar o angulo desejado é o contrario do quad. par
	  ref.me[i].theta() = sgn(ref.me[i].x())*sgn(ref.me[i].y())*M_PI_4;
	} else {
	  ref.me[i].theta() = -sgn(ref.me[i].x())*sgn(ref.me[i].y())*M_PI_4;
	}
	ref.me[i].x()=pos.ball.x();
	ref.me[i].y()=pos.ball.y();
	if (fabs(ref.me[i].y())>(FIELD_HEIGHT/2.0-ROBOT_RAY/2.0)) {
	  ref.me[i].y()=sgn(ref.me[i].y())*(FIELD_HEIGHT/2.0-ROBOT_RAY/2.0);
	}
	if (fabs(ref.me[i].x())>(FIELD_WIDTH/2.0-ROBOT_RAY/2.0)) {
	  ref.me[i].x()=sgn(ref.me[i].x())*(FIELD_WIDTH/2.0-ROBOT_RAY/2.0);
	}
	if (fabs(ref.me[i].x())+fabs(ref.me[i].y()) > (BOUND_SIDE_WALL_CORNER + BOUND_BACK_WALL_CORNER)) {
	   double lado = 
	     fabs(ref.me[i].y()) -(fabs (ref.me[i].y()) -fabs(ref.me[i].x()) + (FIELD_HEIGHT+FIELD_WIDTH)/2.0-CORNER_DIMENSION)/2.0 ;
	  ref.me[i].x() -= sgn(ref.me[i].x())*lado;
	  ref.me[i].y() -= sgn(ref.me[i].y())*lado;
	}
	break;

      case DESCOLAR_PAREDE:
	ref.me[i].theta() = arc_tang(pos.ball.y()-pos.me[i].y(),
				     pos.ball.x()-pos.me[i].x());
	if (fabs(ang_equiv2(ref.me[i].theta())) >= M_PI_4) { 
	  ref.me[i].theta() = M_PI_2;
	}
	else{
	  ref.me[i].theta() = 0.0;
	}
	ref.me[i].x()=pos.ball.x();
	ref.me[i].y()=pos.ball.y();
	if (fabs(ref.me[i].y())>(FIELD_HEIGHT/2.0-ROBOT_RAY/2.0)) {
	  ref.me[i].y()=sgn(ref.me[i].y())*(FIELD_HEIGHT/2.0-ROBOT_RAY/2.0);
	}
	if (fabs(ref.me[i].x())>(FIELD_WIDTH/2.0-ROBOT_RAY/2.0)) {
	  ref.me[i].x()=sgn(ref.me[i].x())*(FIELD_WIDTH/2.0-ROBOT_RAY/2.0);
	}
	if (fabs(ref.me[i].x())+fabs(ref.me[i].y()) > (BOUND_SIDE_WALL_CORNER + BOUND_BACK_WALL_CORNER)) {
	   double lado = fabs(ref.me[i].y()) -(fabs (ref.me[i].y()) -fabs(ref.me[i].x()) + (FIELD_HEIGHT+FIELD_WIDTH)/2.0-CORNER_DIMENSION)/2.0 ;
	  ref.me[i].x() -= sgn(ref.me[i].x())*lado;
	  ref.me[i].y() -= sgn(ref.me[i].y())*lado;
	}
	break;

	//Papéis do defensor
      case ESPERA_UP:
	ref.me[i].x() = -sinal*FK_WIDTH;
	ref.me[i].y() = FK_HEIGHT;
	ref.me[i].theta() = M_PI_2;
	break;
      case ESPERA_DOWN:
	ref.me[i].x() = -sinal*FK_WIDTH;
	ref.me[i].y() = -FK_HEIGHT;
	ref.me[i].theta() = M_PI_2;
	break;
      case ZAGUEIRO:
	{
	  double theta = arc_tang(pos.ball.y()-0,
				  pos.ball.x()-sinal*(-FIELD_WIDTH/2.0));
	  //ref.me[i].x() = pos.me[i].x();
	  ref.me[i].x() = -sinal*FK_WIDTH;
	  /*
	    if ( fabs(ref.me[i].x()) > 0.55 ) {
	    ref.me[i].x() = (ref.me[i].x()>0 ? 0.55 : -0.55);
	    }
	  */
	  ref.me[i].y() = tan(theta)*(ref.me[i].x()-sinal*(-FIELD_WIDTH/2.0));
	  if( ref.me[i].y() > (FK_HEIGHT))
	    ref.me[i].y() = (FK_HEIGHT);
	  if( ref.me[i].y() < -(FK_HEIGHT))
	    ref.me[i].y() = -(FK_HEIGHT);
	  ref.me[i].theta() = M_PI_2;
	}
	break;

	// Papéis do goleiro

      case GOLEIRO:
	ref.me[i].x() = -sinal*(FIELD_WIDTH/2.0 - GOAL_FIELD_WIDTH/2.5);
	if (sinal*pos.ball.x() > 0) {
	  ref.me[i].y() = 0.0;
	  ref.me[i].theta() = M_PI_2;
	}
	else {
	  // Usar a estimativa de posição da bola qdo bola estiver rápida...
	  if ( pos.vel_ball.mod < VEL_BOLA_LENTA ||
	       (mySide()==LEFT_SIDE && cos(pos.vel_ball.ang)>=0.0) ||
	       (mySide()==RIGHT_SIDE && cos(pos.vel_ball.ang)<=0.0) ) {
	    ref.me[i].y() = pos.ball.y();
	  }
	  else {
	    ref.me[i].y() = pos.ball.y()+
	      tan(pos.vel_ball.ang)*(ref.me[i].x()-pos.ball.x());
	  }
	  if( fabs(ref.me[i].y()) > (GOAL_HEIGHT-ROBOT_EDGE/2.0)/2.0) {
	    ref.me[i].y() = sgn(ref.me[i].y())*(GOAL_HEIGHT-ROBOT_EDGE/2.0)/2.0;
	  }
	}
	ref.me[i].theta() = M_PI_2;
	break;
      case TIRAR_DO_GOL:
	double ang_bola_goleiro;
	ang_bola_goleiro = atan2(pos.ball.y()-pos.me[i].y(),
				 pos.ball.x()-pos.me[i].x());
	ref.me[i].x() = pos.ball.x()+CHUTE_GOLEIRO*cos(ang_bola_goleiro);
	ref.me[i].y() = pos.ball.y()+CHUTE_GOLEIRO*sin(ang_bola_goleiro);
	break;
   
      default:
	break;
      }
    }
    break; //PLAY_STATE
  }
    
  
  ant.me[0].x() = pos.me[0].x(); ant.me[0].y() = pos.me[0].y();
  ant.me[0].theta() = pos.me[0].theta();
  
  ant.me[1].x() = pos.me[1].x();  ant.me[1].y() = pos.me[1].y();
  ant.me[1].theta() = pos.me[1].theta();
  
  ant.me[2].x() = pos.me[2].x();  ant.me[2].y() = pos.me[2].y();
  ant.me[2].theta() = pos.me[2].theta();
  
  ant.op[0].x() = pos.op[0].x();  ant.op[0].y() = pos.op[0].y();
  ant.op[0].theta() = pos.op[0].theta();
  
  ant.op[1].x() =  pos.op[1].x();  ant.op[1].y() =  pos.op[1].y();
  ant.op[1].theta() =  pos.op[1].theta();
  
  ant.op[2].x() =  pos.op[2].x();  ant.op[2].y() =  pos.op[2].y();
  ant.op[2].theta() =  pos.op[2].theta();
  
  ant.ball.x() = pos.ball.x();  ant.ball.y() = pos.ball.y();
  ant.future_ball = pos.future_ball;
  ant.vel_ball =  pos.vel_ball;
  

  return false;

}

void Strategy::goleiro_floripa(int id)
{
  papeis.me[id] = GOLEIRO;
}

void Strategy::goleiro_brasilia(int id)
{
  //PREDICADOS
  bool esta_na_area =
    sinal*pos.me[id].x() < -(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH) &&
    fabs(pos.me[id].y()) < GOAL_FIELD_HEIGHT/2.0;
  bool bola_na_area = 
    sinal*pos.ball.x() < -(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH) &&
    fabs(pos.ball.y()) < GOAL_FIELD_HEIGHT/2.0;
  bool bola_parada = 
    pos.vel_ball.mod < VEL_BOLA_PARADA; 
    
  if (esta_na_area && bola_na_area && bola_parada) {
    papeis.me[id] = TIRAR_DO_GOL;
  }
  else {
    papeis.me[id] = GOLEIRO;
  }

//   if(//se o goleiro esta na area...
//      sinal*pos.me[id].x() < -(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH) &&
//      fabs(pos.me[id].y()) < GOAL_FIELD_HEIGHT/2.0 &&
//      //e a bola esta dentro da area...
//      sinal*pos.ball.x() < -(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH) &&
//      fabs(pos.ball.y()) < GOAL_FIELD_HEIGHT/2.0 &&
//      //e esta parada...
//      pos.vel_ball.mod < VEL_BOLA_PARADA){
//     papeis.me[id] = TIRAR_DO_GOL;
//   }else{
//     papeis.me[id] = GOLEIRO;
//   }
}

void Strategy::atacante_floripa(int id)
{ //variaveis para calculos
  double dist = hypot(pos.ball.x()-pos.me[id].x(),
		      pos.ball.y()-pos.me[id].y());
  double thetapos_fut = arc_tang(pos.future_ball.y()-pos.me[id].y(),
				 pos.future_ball.x()-pos.me[id].x());
  double ypos_fut = mytan(thetapos_fut)*
    (sinal*FIELD_WIDTH/2.0-pos.me[id].x()) + pos.me[id].y();
  // yalin onde o robô com o alinhamento atual bateria no fundo do
  // campo se seguisse reto fundo do campo
  double yalin = mytan(pos.me[id].theta())*
    (sinal*FIELD_WIDTH/2.0-pos.me[id].x()) + pos.me[id].y();
  
  double thetaref = arc_tang(-pos.ball.y(),
			     sinal*FIELD_WIDTH/2.0-pos.ball.x());
  double xref = pos.ball.x()-DIST_CHUTE*cos(thetaref);
  double yref = pos.ball.y()-DIST_CHUTE*sin(thetaref);

  double dist_ref = hypot(xref-pos.me[id].x(),yref-pos.me[id].y());

  //PREDICADOS
  bool bola_regiao_perigosa =
    (sinal*pos.ball.x() < -(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH) &&
     fabs(pos.ball.y()) < GOAL_FIELD_HEIGHT/2.0);
  bool atras_da_bola =
    (sinal*pos.ball.x()-sinal*pos.me[id].x()>0) ||
    (sinal*pos.ball.x()-sinal*pos.me[id].x()>-0.05 &&
     papeis.me[id]!=CONTORNAR);

  bool consegue_alinhar =
    fabs(ypos_fut)< 0.85*GOAL_HEIGHT/2.0  &&
	  ( dist < 2*DIST_CHUTE  ||
	    papeis.me[id]==CHUTAR_GOL ||
	    papeis.me[id]==GIRAR );
  bool consegue_chutar =
    (fabs(yalin)<0.85*GOAL_HEIGHT/2.0 ) ||
    (fabs(yalin)<GOAL_FIELD_HEIGHT/2.0 && papeis.me[id]==CHUTAR_GOL );
  
  bool fora_area_permitida =
    fabs(yref)>(FIELD_HEIGHT/2.0-ROBOT_RAY) ||
    fabs(xref)>(FIELD_WIDTH/2.0-ROBOT_RAY) ||
    fabs(xref) + fabs (yref) > BOUND_SIDE_WALL_CORNER + BOUND_BACK_WALL_CORNER;

  bool precisa_descolar =
    dist_ref<DIST_CHUTE ||
    (dist_ref<1.5*DIST_CHUTE && (papeis.me[id]==DESCOLAR_PAREDE ||
			     papeis.me[id]==DESCOLAR_QUINA));
  bool precisa_descolar_quina =
    (fabs(xref)>BOUND_BACK_WALL_CORNER &&
     fabs(yref)>BOUND_SIDE_WALL_CORNER &&
     fabs(xref)+fabs(yref)>BOUND_SIDE_WALL_CORNER + BOUND_BACK_WALL_CORNER) ||
    (fabs(xref)>1.15*BOUND_BACK_WALL_CORNER &&
     fabs(yref)>1.15*BOUND_SIDE_WALL_CORNER &&
     fabs(xref)+fabs(yref)>1.15*(BOUND_SIDE_WALL_CORNER+BOUND_BACK_WALL_CORNER) &&
     papeis.me[id]== DESCOLAR_QUINA);
  
  RPAPEL pos_papeis =
    plano_atacante (bola_regiao_perigosa, atras_da_bola, consegue_alinhar, 
		    consegue_chutar, fora_area_permitida, precisa_descolar, 
		    precisa_descolar_quina);
  //TEMPORARIO
  if (pos_papeis.estacionar) {papeis.me[id]= ESTACIONAR; return;}
  if (pos_papeis.contornar) {papeis.me[id]= CONTORNAR; return;}
  if (pos_papeis.alinhar) {papeis.me[id]= ALINHAR; return;}
  if (pos_papeis.girar) {papeis.me[id]= GIRAR; return;}
  if (pos_papeis.chutar_gol) {papeis.me[id]= CHUTAR_GOL; return;}
  if (pos_papeis.alinhar_parede) {papeis.me[id]= ALINHAR_PAREDE; return;}
  if (pos_papeis.descolar_parede) {papeis.me[id]= DESCOLAR_PAREDE; return;}
  if (pos_papeis.descolar_quina) {papeis.me[id]= DESCOLAR_QUINA; return;}
  if (pos_papeis.espera_up) {papeis.me[id]= ESPERA_UP; return;}
  if (pos_papeis.espera_down) {papeis.me[id]= ESPERA_DOWN; return;}
  if (pos_papeis.zagueiro) {papeis.me[id]= ZAGUEIRO; return;}
  if (pos_papeis.goleiro) {papeis.me[id]= GOLEIRO; return;}
  if (pos_papeis.tirar_do_gol) {papeis.me[id]= TIRAR_DO_GOL; return;}
  
  // if (bola_regiao_perigosa) {
  //   papeis.me[id] =ESTACIONAR;
  // }
  // else {
  //   if (atras_da_bola) {
  //     if (consegue_alinhar) {
  // 	if (consegue_chutar) {
  // 	  papeis.me[id]=CHUTAR_GOL;
  // 	}
  // 	else {
  // 	  papeis.me[id]=GIRAR;
  // 	}
  //     }
  //     else {
  // 	if (fora_area_permitida) {
  // 	  if (precisa_descolar) {
  // 	    if (precisa_descolar_quina) {
  // 	      papeis.me[id]=DESCOLAR_QUINA;
  // 	    }
  // 	    else {
  // 	      papeis.me[id]=DESCOLAR_PAREDE;
  // 	    }
  // 	  } else {
  // 	    papeis.me[id]=ALINHAR_PAREDE;
  // 	  }
  // 	} 
  // 	else {
  // 	  papeis.me[id]=ALINHAR;
  // 	}
  //     }
  //   } 
  //   else {
  //     papeis.me[id]=CONTORNAR;
  //   }

  // }
        


//   if (sinal*pos.ball.x() < -(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH) &&
//       fabs(pos.ball.y()) < GOAL_FIELD_HEIGHT/2.0) {
//     // A bola está muito próxima do nosso gol
//     papeis.me[id] = ESTACIONAR;
//   }
//   else {
//     // Para evitar chaveamentos, há uma "histerese" nos ifs...
//     if( ( sinal*pos.ball.x()-sinal*pos.me[id].x()>0 ) ||
// 	( sinal*pos.ball.x()-sinal*pos.me[id].x()>-0.05 &&
// 	  papeis.me[id]!=CONTORNAR) ){
//       // O atacante está atrás da bola
//       // dist e thetapos são a distância e o ângulo do robô para a bola
//       double dist = hypot(pos.ball.x()-pos.me[id].x(),
// 			  pos.ball.y()-pos.me[id].y());
// //       double thetapos = arc_tang(pos.ball.y()-pos.me[id].y(),
// // 				 pos.ball.x()-pos.me[id].x());

// //       double dist_fut = hypot(pos.future_ball.x()-pos.me[id].x(),
// // 			  pos.future_ball.y()-pos.me[id].y());
//       double thetapos_fut = arc_tang(pos.future_ball.y()-pos.me[id].y(),
// 				 pos.future_ball.x()-pos.me[id].x());

//       // ypos é a posicao onde a reta que une o robô à bola bate no
//       // fundo do campo
// //       double ypos = mytan(thetapos)*(sinal*FIELD_WIDTH/2.0-pos.me[id].x()) +
// // 	pos.me[id].y();
//       double ypos_fut = mytan(thetapos_fut)*
// 	(sinal*FIELD_WIDTH/2.0-pos.me[id].x()) + pos.me[id].y();
//       // yalin onde o robô com o alinhamento atual bateria no fundo do
//       // campo se seguisse reto fundo do campo
//       double yalin = mytan(pos.me[id].theta())*
// 	(sinal*FIELD_WIDTH/2.0-	pos.me[id].x()) + pos.me[id].y();
//       if( fabs(ypos_fut)< 0.85*GOAL_HEIGHT/2.0  &&
// 	  ( dist < 2*DIST_CHUTE  ||
// 	    papeis.me[id]==CHUTAR_GOL ||
// 	    papeis.me[id]==GIRAR ) ) {
// 	// Está posicionado na zona de chute para o gol,
// 	// levando em conta as posições da bola e do robô
// 	if ( ( fabs(yalin)<0.85*GOAL_HEIGHT/2.0 ) ||
// 	     ( fabs(yalin)<GOAL_FIELD_HEIGHT/2.0 && 
// 	       papeis.me[id]==CHUTAR_GOL ) ) {
// 	  // Está apontado para o gol
// 	  papeis.me[id] = CHUTAR_GOL;
// 	}
// 	else {
// 	  // Está posicionado mas nao alinhado
// 	  papeis.me[id] = GIRAR;
// 	}
//       }
//       else {
// 	double thetaref = arc_tang(-pos.ball.y(),
// 				   sinal*FIELD_WIDTH/2.0-pos.ball.x());
// 	double xref = pos.ball.x()-DIST_CHUTE*cos(thetaref);
// 	double yref = pos.ball.y()-DIST_CHUTE*sin(thetaref);
// 	if( fabs(yref)>(FIELD_HEIGHT/2.0-ROBOT_RAY) ||
// 	    fabs(xref)>(FIELD_WIDTH/2.0-ROBOT_RAY) ||
// 	    fabs(xref) + fabs (yref) > BOUND_SIDE_WALL_CORNER + BOUND_BACK_WALL_CORNER) {
// 	  // Nao dá para fazer o alinhamento normal: resta decidir se
// 	  // vai alinhar na parede ou tentar descolar a bola
// 	  double dist = hypot(xref-pos.me[id].x(),yref-pos.me[id].y());
// 	  if (dist<DIST_CHUTE ||
// 	      (dist<1.5*DIST_CHUTE && 
// 	       (papeis.me[id]==DESCOLAR_PAREDE ||
// 		papeis.me[id]==DESCOLAR_QUINA))) {
// 	    if ((fabs(xref)>BOUND_BACK_WALL_CORNER &&
// 		 fabs(yref)>BOUND_SIDE_WALL_CORNER &&
// 		 fabs(xref)+fabs(yref)>BOUND_SIDE_WALL_CORNER + BOUND_BACK_WALL_CORNER) ||
// 		(fabs(xref)>1.15*BOUND_BACK_WALL_CORNER &&
// 		 fabs(yref)>1.15*BOUND_SIDE_WALL_CORNER &&
// 		 fabs(xref)+fabs(yref)>1.15*(BOUND_SIDE_WALL_CORNER+BOUND_BACK_WALL_CORNER) &&
// 		 papeis.me[id]== DESCOLAR_QUINA)) {
// 	      papeis.me[id] = DESCOLAR_QUINA;
// 	    } 
// 	    else {
// 	      papeis.me[id] = DESCOLAR_PAREDE;
// 	    }
// 	  }
// 	  else {
// 	    papeis.me[id] = ALINHAR_PAREDE;
// 	  }
// 	}
// 	else {
// 	  papeis.me[id] = ALINHAR;
// 	}
//       }
//     }
//     else {
//       papeis.me[id] = CONTORNAR;
//     }
//   }
}

void Strategy::defensor_floripa(int id, int atacante, int goleiro)
{//PREDICADOS
  bool atacante_no_outro_lado = 
    sgn (pos.me[id].x()*pos.me[atacante].x()) == -1.0;
  bool em_laterais_distintas =
    sgn (pos.me[id].y()*pos.me[atacante].y()) == -1.0;
 
  if (atacante_no_outro_lado) {
    papeis.me[id] = (pos.me[atacante].y()>0 ? ESPERA_DOWN : ESPERA_UP);
  }
  else {
    if (em_laterais_distintas) {
      papeis.me[id] = (pos.me[id].y()>0 ? ESPERA_UP : ESPERA_DOWN);
    } 
    else {
      papeis.me[id] = ESTACIONAR;
    }
  }

//   //    if(sinal*pos.me[atacante].x() > 0){
//   if(pos.me[atacante].x()*pos.me[id].x() == -1.0){
//     // O atacante e o defensor estao de lados opostos
//     papeis.me[id] = (pos.me[atacante].y()>0 ? ESPERA_DOWN : ESPERA_UP);
//   }
//   else{
//     // Testa se o atacante e o defensor estao na mesma metade do lado
//     if(pos.me[atacante].y()*pos.me[id].y() == -1.0){
//       // Atacante e defensor estao em metade do lado opostos
//       papeis.me[id] = (pos.me[id].y()>0 ? ESPERA_UP : ESPERA_DOWN);
//     }else{
//       //Atacante e defensor estao na mesma metade do lado
//       papeis.me[id] = ESTACIONAR;
//     }
//   }
}


RPAPEL Strategy::plano_atacante (bool bola_perigosa, bool atras_bola, bool pode_ali, bool pode_chutar, bool fora_r_permit, bool precisa_descolar, bool precisa_desc_quina) {
  unsigned int indice = 
    (bola_perigosa*64) + (atras_bola*32) + (pode_ali*16) + (pode_chutar*8) +
    (fora_r_permit*4) + (precisa_descolar*2) + precisa_desc_quina;
  
  PAPEL_JOGADOR papel_at = resp_atacante[indice];
  RPAPEL ret;
  if (papel_at == ESTACIONAR) {ret.estacionar = true;} else {ret.estacionar=false;}
  if (papel_at == CONTORNAR) {ret.contornar = true;} else {ret.contornar=false;}
  if (papel_at == ALINHAR) {ret.alinhar = true;} else {ret.alinhar=false;}
  if (papel_at == GIRAR) {ret.girar = true;} else {ret.girar=false;}
  if (papel_at == CHUTAR_GOL) {ret.chutar_gol = true;} else {ret.chutar_gol=false;}
  if (papel_at == ALINHAR_PAREDE) {ret.alinhar_parede = true;} else {ret.alinhar_parede=false;}
  if (papel_at == DESCOLAR_PAREDE) {ret.descolar_parede = true;} else {ret.descolar_parede=false;}
  if (papel_at == DESCOLAR_QUINA) {ret.descolar_quina = true;} else {ret.descolar_quina=false;}
  if (papel_at == ESPERA_UP) {ret.espera_up = true;} else {ret.espera_up=false;}
  if (papel_at == ESPERA_DOWN) {ret.espera_down = true;} else {ret.espera_down=false;}
  if (papel_at == ZAGUEIRO) {ret.zagueiro = true;} else {ret.zagueiro=false;}
  if (papel_at == GOLEIRO) {ret.goleiro = true;} else {ret.goleiro=false;}
  if (papel_at == TIRAR_DO_GOL) {ret.tirar_do_gol = true;} else {ret.tirar_do_gol=false;}
  return ret;
}
