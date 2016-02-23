#include <iostream>
#include <cmath>
#include <stdio.h>
#include "strategy.h"
#include "../parameters.h"
#include "../functions.h"
#include "../system.h"


using namespace::std;

#define EPSILON_L 0.01 //CHECAR
#define DELTA_L (2*EPSILON_L) //CHECAR
#define LONGE_L (4*EPSILON_L) //CHECAR
 
#define EPSILON_ANG 0.087 //5graus//CHECAR
#define LONGE_ANG (4*EPSILON_ANG) //CHECAR
#define DELTA_ANG (2*EPSILON_ANG) //CHECAR
#define DIST_BOLA 0.2

#define INTERV_TESTE_TRAV 2

static const double LIMITE_CONT_PARADO = 75;
static const double DIST_CHUTE = 0.10;
static const double CHUTE_GOLEIRO = 0.15;
static const double VEL_BOLA_LENTA = 0.2;


/*
Tabela::Tabela (unsigned int nent, unsigned int nsai):
  nentradas (nent),
  nsaidas (nsaidas),
  intervazio (1)
{}
  
voidTabela::
*/

Strategy::Strategy(TEAM team, SIDE side, GAME_MODE gameMode):
  FutData(team,side,gameMode)
{
  //INICIALIZAR OS PARAMETROS DA CLASSE
  t = 0.0;

  for(int i=0; i < 3; i++){
    bloqueado[i] = false;
    contador_parado[i] = 0;
  }
  setint (zagueiro, "xxxxxxxxxx", NAO_DEFINIDO, true);
  setint (atacante, "xxxxxxxxx", NAO_DEFINIDO, true);

  zagueiro = 2;
  atacante = 1;
  goleiro = 0;

  sinal = (mySide() == LEFT_SIDE ? 1 : -1);

  //PREENCHIMENTO DA TABELA V. DO ATACANTE
  preenchimento();

}

Strategy::~Strategy(){

}

//########################################################################
//########################################################################

bool Strategy::full_table (int id) {
  unsigned int i = 0;
  if (id == zagueiro) {
  while (resp_zagueiro[i] != NAO_DEFINIDO && i < TSIZE_ZAG) i++;
  return (i == TSIZE_ZAG && resp_zagueiro[TSIZE_ZAG-1] != NAO_DEFINIDO);
  }
  if (id == atacante) {
    while (resp_atacante[i] != NAO_DEFINIDO && i < TSIZE_ATA) i++;
    return (i == TSIZE_ATA && resp_atacante[TSIZE_ATA-1] != NAO_DEFINIDO);
  }
  return true;
}

bool Strategy::setint(int id, const char * binario, PAPEL_JOGADOR resp, bool silence) {
  bool ret = true;
  if (id == zagueiro) {
    for (int i=0; i < TSIZE_ZAG; i++) {
      if ((binario[0] == 'x' || (bool (i & 512) == ('1' == binario[0]))) &&
	  (binario[1] == 'x' || (bool (i & 256) == ('1' == binario[1]))) &&
	  (binario[2] == 'x' || (bool (i & 128) == ('1' == binario[2]))) &&
	  (binario[3] == 'x' || (bool (i & 64) == ('1' == binario[3]))) &&
	  (binario[4] == 'x' || (bool (i & 32) == ('1' == binario[4]))) &&
	  (binario[5] == 'x' || (bool (i & 16) == ('1' == binario[5]))) &&
	  (binario[6] == 'x' || (bool (i & 8) == ('1' == binario[6]))) &&
	  (binario[7] == 'x' || (bool (i & 4) == ('1' == binario[7]))) &&
	  (binario[8] == 'x' || (bool (i & 2) == ('1' == binario[8]))) &&
	  (binario[9] == 'x' || (bool (i & 1) == ('1' == binario[9])))) {
	if (resp_zagueiro[i] != NAO_DEFINIDO && !silence) {
	  printf ("[strategy:]Warning: overwrite of role %d in strategy!\n", i);
	  printf ("     old value: %i,  new value: %i\n", resp_zagueiro[i], resp);
	  ret = false;
	}
	resp_zagueiro[i] = resp;
      }
            
    }

  }
  if (id == atacante) {
    for (int i=0; i < TSIZE_ATA; i++) {
      if ((binario[0] == 'x' || (bool (i & 256) == ('1' == binario[0]))) &&
	  (binario[1] == 'x' || (bool (i & 128) == ('1' == binario[1]))) &&
	  (binario[2] == 'x' || (bool (i & 64) == ('1' == binario[2]))) &&
	  (binario[3] == 'x' || (bool (i & 32) == ('1' == binario[3]))) &&
	  (binario[4] == 'x' || (bool (i & 16) == ('1' == binario[4]))) &&
	  (binario[5] == 'x' || (bool (i & 8) == ('1' == binario[5]))) &&
	  (binario[6] == 'x' || (bool (i & 4) == ('1' == binario[6]))) &&
	  (binario[7] == 'x' || (bool (i & 2) == ('1' == binario[7]))) &&
	  (binario[8] == 'x' || (bool (i & 1) == ('1' == binario[8])))) {
	if (resp_atacante[i] != NAO_DEFINIDO && !silence) {
	  printf ("[strategy:]Warning: overwrite of role %d in strategy!\n", i);
	  printf ("     old value: %i,  new value: %i\n", resp_atacante[i], resp);
	  ret = false;
	}
	resp_atacante[i] = resp;
      }
    }
  }
  return ret;
}


void Strategy::preenchimento() {
  // for (unsigned int i=0; i <256; i++) {
  //   if (i & 64) {
  //     resp_atacante[i] = POSICIONAR;
  //     if (!(i & 16) && (i & 2)) resp_atacante[i] = IR_BOLA;
  //     if (!(i & 16) && !(i & 2)) resp_atacante[i] = DESVIAR;
  //     if ((i & 16) && !(i & 4)) resp_atacante[i] = CONTORNAR;
  //     if (!(i & 128) && !(i & 32) && (i & 16) && !(i & 8) && (i & 4)) resp_atacante[i] = ALINHAR;
  //     if (!(i & 128) && (i & 32) && (i & 16) && !(i & 8) && (i & 4) && (i & 1)) resp_atacante[i] = ALINHAR_PAREDE;
  //     if (!(i & 128) && (i & 32) && (i & 16) && !(i & 8) && (i & 4) && !(i & 1)) resp_atacante[i] = DESCOLAR_PAREDE;
  //   }
  //   else {
  //     resp_atacante[i]=POSICIONAR;
  //   }

  // }//      abcdefghij

  setint (atacante, "1xxxxxxxx", POSICIONAR, false);
  setint (atacante, "0111xxxxx", CHUTAR_GOL, false);
  setint (atacante, "0110xxxxx", GIRAR, false);

  // setint (atacante, "010xxxxx1", IR_CENTRO, false);
  setint (atacante, "010x10x0x", IR_BOLA, false);
 


  setint (atacante, "010x11x0x", RETIRAR_BOLA_PAREDE, false);
  setint (atacante, "010xxx01x", IR_BOLA, false);
  //  setint (atacante, "010xx101x", POSICIONAR_RETIRADA, false); 
  setint (atacante, "010xxx11x", TIRAR_BOLA, false);
  setint (atacante, "010x0xx0x", ALINHAR, false);
  
  setint (atacante, "00xxxxxxx", CONTORNAR_ZAG, false);
  // setint ("xxxxxxxxx1", SAIR_DA_AREA);
  // setint ("00xxxxxxx0", DEFENDER);
  // setint ("10xxxxxxx0", IMPOSSIVEL);
  // setint ("11xxxxxxx0", POSICIONAR_ZAGA);
  // setint ("010xxx1xx0", LEVAR_BOLA);
  // setint ("011x0x1xx0", IR_BOLA_ZAG);
  // setint ("011x1x1x00", CONTORNAR_ZAG);
  // setint ("011x1x1x10", RETIRAR_BOLA_PAREDE);
  // setint ("0100xx00x0", POSICIONAR_RETIRADA);
  // setint ("0101xx00x0", LEVAR_BOLA);
  // setint ("011x0x00x0", POSICIONAR_RETIRADA);
  // setint ("011x1x00x0", RETIRAR_BOLA_PAREDE);
  // setint ("01xxxx01x0", CONTORNAR_ZAG);
  
             //      abcdefghij
  setint (zagueiro, "xxxxxxxxx1", SAIR_DA_AREA, false);
  setint (zagueiro, "00xxxxxxx0", DEFENDER, false);
  setint (zagueiro, "10xxxxxxx0", IMPOSSIVEL, false);
  setint (zagueiro, "11xxxxxxx0", POSICIONAR_ZAGA, false);
  setint (zagueiro, "010xxxx0x0", TIRAR_BOLA_MEIO, false);
 
  setint (zagueiro, "0100xx11x0", GIRAR, false);
  setint (zagueiro, "0101xx11x0", LEVAR_BOLA, false);
  
  setint (zagueiro, "01100x11x0", GIRAR, false);
  setint (zagueiro, "01110x11x0", IR_BOLA_ZAG, false);
  
  setint (zagueiro, "011x1x11x0", RETIRAR_BOLA_PAREDE, false);
 
  setint (zagueiro, "010xxx0100", IR_BOLA_ZAG, false);
  setint (zagueiro, "010xxx0110", CONTORNAR_ZAG, false);
  
  setint (zagueiro, "011xxx0000", IR_BOLA_ZAG, false);
  setint (zagueiro, "011xx00010", POSICIONAR_RETIRADA, false);
  setint (zagueiro, "011xx10010", TIRAR_BOLA, false);
  
  setint (zagueiro, "011xxx0100", IR_BOLA_ZAG, false);
  setint (zagueiro, "011xxx0110", CONTORNAR_ZAG, false);

  setint (zagueiro, "011xx010x0", POSICIONAR_RETIRADA, false);
  setint (zagueiro, "011xx110x0", TIRAR_BOLA, false);

  if (!full_table(zagueiro))
      printf ("[strategy]: Warning, table of player zagueiro is not full!\n");
  if (!full_table(atacante))
      printf ("[strategy]: Warning, table of player atacante is not full!\n");
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
      int jog_ativo = atacante;
      int jog_parado = zagueiro;
      if (sinal*pos.ball.x() < -0.05) {
	jog_ativo = zagueiro;
	jog_parado = atacante;
      }
      //cout << "atacante: " << atacante << endl << "zagueiro: " << zagueiro << endl; 
      if((bloqueado[jog_ativo] &&
	  hypot(pos.ball.x()-pos.me[jog_ativo].x(),pos.ball.y()-pos.me[jog_ativo].y()) > ROBOT_RAY*2)){// ||
	 //	 ( hypot(pos.ball.x()-pos.me[atacante].x(),pos.ball.y()-pos.me[atacante].y()) > 
	 //	 hypot(pos.ball.x()-pos.me[zagueiro].x(),pos.ball.y()-pos.me[zagueiro].y()) &&
	 //	   (pos.me[jog_parado].x() != POSITION_UNDEFINED &&
	 //	    pos.me[jog_parado].y() != POSITION_UNDEFINED))){
	int aux = atacante;
	atacante = zagueiro;
	zagueiro = aux;
	//		cout << "TROCOU BLOQUEADO!!" << endl;
      }
      /* TROCA */
      // int *  jogador_ativo = &atacante;
      // int * jogador_parado = &zagueiro;
      // if (sinal*pos.ball.x()< -0.05) {
      // 	jogador_ativo = &zagueiro;
      // 	jogador_parado = &atacante;
      // }
      // if((bloqueado[(*jogador_ativo)] &&
	
      // 	  hypot(pos.ball.x()-pos.me[(*jogador_ativo)].x(),pos.ball.y()-pos.me[(*jogador_ativo)].y()) > ROBOT_RAY*2) ||
      // 	 hypot(pos.ball.x()-pos.me[(*jogador_ativo)].x(),pos.ball.y()-pos.me[(*jogador_ativo)].y()) >
      // 	 hypot(pos.ball.x()-pos.me[(*jogador_parado)].x(),pos.ball.y()-pos.me[(*jogador_parado)].y())){

	

      // 	int * aux =  jogador_ativo;
      // 	jogador_ativo = jogador_parado;
      // 	jogador_parado = aux; 
	
      // 		cout << "TROCOU BLOQUEADO!!" << endl;
      // }

      // Determinação estática dos papéis
      //zagueiro = 0;
      //atacante = 1;
      //goleiro = 2;

      //      goleiro_floripa(goleiro);
      goleiro_brasilia(goleiro);
      novo_atacante (atacante);
      // A função de cálculo da referência do defensor deve ser chamada depois
      // do atacante e do goleiro, para que o defensor possa evitá-los
      defensor_floripa(zagueiro,atacante,goleiro);
      //   cout << papeis.me[zagueiro] << " ";

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
      case POSICIONAR:
	{
	  if (pos.ball.y()> 0) {
	    ref.me[i].x() = sinal*0.2;
	    ref.me[i].y() = FIELD_HEIGHT/2.0-sinal*0.25;
	    ref.me[i].theta() = -0.707;
	  } else {
	    ref.me[i].x() = sinal*0.2;
	    ref.me[i].y() = -FIELD_HEIGHT/2.0+sinal*0.25;
	    ref.me[i].theta() = 0.707;
	  }
	  break;
	}
      case IR_BOLA:
	ref.me[i].x() = pos.ball.x();
	ref.me[i].y() = pos.ball.y();
	break;
      case IR_CENTRO:
	ref.me[i].x() = 0.0;
	ref.me[i].y() = 0.0;
      case DESVIAR:
	{
	  double dist = hypot (ref.me[i].x() -pos.me[i].x(), ref.me[i].y() -pos.me[i].y())/2.0;
	  double ang = arc_tang (ref.me[i].y()-pos.me[i].y(), ref.me[i].x()-pos.me[i].x());
	  ref.me[i].x() = dist*cos(ang) + pos.me[i].x();
	  ref.me[i].y() = dist*sin(ang) + pos.me[i].y();
	  break;
	}

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
      case NOVO_ALIN: 
	{
	  double a, b, c;
	  double y = pos.me[i].y();
	  double x = pos.me[i].x();
	  a = pos.ball.y();
	  b = (sinal*FIELD_WIDTH/2.0) -pos.ball.x();
	  c = -(sinal*FIELD_WIDTH/2.0)*pos.ball.y();
	  double dist = fabs (a*x+b*y+c)/hypot (a, b);
	  double ang = atan2 (pos.ball.y(), pos.ball.x()-(sinal*FIELD_WIDTH/2.0));
	  double ang_robola = atan2 (y-pos.ball.y(), x-pos.ball.x());
	  if (ang-M_PI_2 < ang_robola && ang_robola < ang+M_PI_2) {
	    double nv = 0;
	    if (dist -0.05> 0) nv = dist-0.05;
	    ref.me[i].x() = ((nv)+DIST_CHUTE)*cos(ang)+pos.ball.x();
	    ref.me[i].y() = ((nv)+DIST_CHUTE)*sin(ang)+pos.ball.y();
	    if (fabs (ref.me[i].y()) > FIELD_HEIGHT/2.0-ROBOT_RAY)
	      ref.me[i].y() = sgn (ref.me[i].y())*(FIELD_HEIGHT/2.0-ROBOT_RAY);
	  } else {
	    ref.me[i].x() = (FIELD_HEIGHT/2.0)*cos(ang)+pos.ball.x();
	    ref.me[i].y() = (FIELD_HEIGHT/2.0)*sin(ang)+pos.ball.y();
	  }

	  ref.me[i].theta() = ang;
	}
	break;

      case ALINHAR:
      case ALINHAR_PAREDE:
	//	double pballx, pbally;
	//pballx = pos.ball.x()+ (pos.vel_ball.mod*cos(pos.vel_ball.ang));
	//	pbally = pos.ball.y()+ (pos.vel_ball.mod*sin(pos.vel_ball.ang));

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
      // case DESCOLAR_FUNDO_ATA:	
      // 	// BOLA NA LATERAL DA AREA E NO FUNDO DO CAMPO
      // 	if((fabs (pos.ball.y()) > GOAL_FIELD_HEIGHT/2.0) &&
      // 	   (fabs (pos.ball.y())  < GOAL_FIELD_HEIGHT/2.0+3*ROBOT_RAY) &&
      // 	   (fabs (pos.ball.x()) > FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH)) {
      // 	  ref.me[i].x() = pos.ball.x()+sinal*(ROBOT_EDGE/2.0+BALL_RAY); //2*ROBOT_RAY;
      // 	  ref.me[i].y() = pos.ball.y(); //sgn(pos.ball.y())*(GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY); 
      // 	  if (fabs(pos.ball.y()) < GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY) 
      // 	    ref.me[i].y() = sgn(pos.ball.y())*(GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY);
      // 	  ref.me[i].theta() = (sinal+1)*M_PI_2;
      // 	  //  cout << "BOLA NA LATERAL\n";
      // 	}

      // 	//BOLA NO FUNDO E NA LATERAL DO CAMPO E 
      // 	if((fabs (pos.ball.y()) > GOAL_FIELD_HEIGHT/2.0+3*ROBOT_RAY) &&
      // 	   (fabs (pos.ball.x()) > FIELD_WIDTH/2.0-(GOAL_FIELD_WIDTH+ROBOT_RAY))) {
      // 	  ref.me[i].x() = pos.ball.x();
      // 	  if (fabs(ref.me[i].x()) > FIELD_WIDTH/2.0-ROBOT_RAY) 
      // 	    ref.me[i].x() = sinal*(-FIELD_WIDTH/2.0+ROBOT_RAY);
      // 	  ref.me[i].y() = pos.ball.y() -sgn(pos.ball.y())*(ROBOT_EDGE/2.0+BALL_RAY);
      // 	  ref.me[i].theta() = sgn(pos.ball.y())*M_PI_2;
      // 	  // cout << "BOLA NO FUNDO\n";
      // 	}
      // 	break;	

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
	bypassControl[i] = true;
	if (pos.me[i].y() > 0) {
	  pwm.me[i].left = 1.0;
	  pwm.me[i].right = 0.0;
	} else {
	  pwm.me[i].left = 0.0;
	  pwm.me[i].right = 1.0;
	}
	
	/*ref.me[i].theta() = arc_tang(pos.ball.y()-pos.me[i].y(),
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
	  ref.me[i].y() -= sgn(ref.me[i].y())*lado;*/
       
	break;

	//Papéis do defensor
      case POSICIONAR_ZAGA:
	{
	  if (pos.ball.y()>= 0) {
	     ref.me[i].x() = sinal*(-FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+3*ROBOT_RAY);
	     ref.me[i].y() = GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY;
	     ref.me[i].theta() = atan2(pos.me[i].y()-pos.ball.y(),pos.me[i].x()-pos.ball.x());
	   } else {                             
	     ref.me[i].x() = sinal*(-FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+3*ROBOT_RAY);
	     ref.me[i].y() = -GOAL_FIELD_HEIGHT/2.0-ROBOT_RAY;
	     ref.me[i].theta() = atan2(pos.me[i].y()-pos.ball.y(),pos.me[i].x()-pos.ball.x());;
	   }  
	}
	break;
      case DEFENDER:
	{
	  double ang = arc_tang (pos.future_ball.y(), fabs(sinal*FIELD_WIDTH/2.0+pos.future_ball.x())); 
	  ref.me[i].x() = sinal*(-FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+2*ROBOT_RAY);
	  ref.me[i].y() = (GOAL_FIELD_WIDTH+5*ROBOT_RAY)*sin(ang);
	  ref.me[i].theta() = M_PI_2;
	}
	break;
      case TOCAR_BOLA:
	{
	  ref.me[i].x() = -0.05;
	  ref.me[i].y() = (pos.me[i].x()-ref.me[i].x())*sin(pos.me[i].theta());
	  double limite = FIELD_HEIGHT/2.0-ROBOT_RAY;
	  if (ref.me[i].y() > limite)
	    ref.me[i].y() = limite;
	  if (ref.me[i].y() <  -limite)
	    ref.me[i].y() = -limite;
	  ref.me[i].theta() = pos.me[i].theta();
	}
	break;
      case TIRAR_BOLA:
	{double ang = M_PI_2/2.0;
	  if ((sinal*pos.me[i].y()) > 0){
	    ref.me[i].x() = pos.me[i].x();
	    ref.me[i].y() = pos.me[i].y();
	    ref.me[i].theta() = pos.me[i].theta()-ang;
	  } else {
	    ref.me[i].x() = pos.me[i].x();
	    ref.me[i].y() = pos.me[i].y();
	    ref.me[i].theta() = pos.me[i].theta()+ang;
	  }
	}
	break;
      case IR_BOLA_ZAG:
	{double angb = atan2 (pos.ball.y()-pos.me[i].y(), pos.ball.y()-pos.ball.x());
	  ref.me[i].x() = pos.ball.x() -sinal*ROBOT_RAY;
	  ref.me[i].y() = pos.ball.y(); //+ ROBOT_RAY*sin(angb); //-sgn(pos.ball.y())*ROBOT_RAY;
	  ref.me[i].theta() = angb;
	  // TRATAMENTO DAS PAREDES
	  if (sinal*ref.me[i].x() < -FIELD_WIDTH/2.0+ROBOT_RAY) {
	    ref.me[i].x() = sinal*(-FIELD_WIDTH/2.0+ROBOT_RAY);
	    ref.me[i].theta() = M_PI_2;
	  }
	  if (fabs(ref.me[i].y()) > FIELD_HEIGHT/2.0-ROBOT_RAY)
	    ref.me[i].y() = pos.ball.y()-sgn(pos.ball.y())*ROBOT_RAY;

	  // TRATA REFERENCIAS NA QUINA
	  double d = fabs(ref.me[i].x())+fabs(ref.me[i].y()) - (BOUND_SIDE_WALL_CORNER + BOUND_BACK_WALL_CORNER);
	  if (d > 0) {
	    ref.me[i].x() = ref.me[i].x()+sinal*0.7071*d;
	    ref.me[i].y() = ref.me[i].y()-sgn(ref.me[i].y())*0.7071*d;
	  }

	  // TRATAMENTO DE REFERENCIAS DENTRO DA NOSSA AREA
	  if (fabs (ref.me[i].y()) <= GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY && 
	      sinal*ref.me[i].x() <= -FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+ROBOT_RAY) {
	    double a, b;
	    //teste de proximidade da refer. com linhas da area (lateral e frente)
	    //se tiver mais perto de uma das linhas projeta fora a nova posicao
	    //se parecida as distancias, referencia fica na quina:
	    // (GOAL_FIELD_WIDTH+ROBOT_RAY, GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY
	    a = (GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY)-fabs (ref.me[i].y());
	    b = fabs (ref.me[i].x())-(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH-ROBOT_RAY);
	    if (fabs(a-b) < 0.02) {
	      ref.me[i].y() = sgn(ref.me[i].y())*(GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY);
	      ref.me[i].x() = sinal*(-FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+ROBOT_RAY);
	    } else {
	      if (a<b) {
		ref.me[i].y() = sgn(ref.me[i].y())*(GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY);
	      } else {
		ref.me[i].x() = sinal*(-FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+ROBOT_RAY);
	      }
	    }
	  }


	  //TRATAMENTO DE TRAJETOS DENTRO DA AREA (proximas 45 linhas)
	  double angrob, angref;
	  angrob = atan2 (sgn(pos.me[i].y())*(GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY)-pos.me[i].y(),
			  -(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH-ROBOT_RAY)-sinal*pos.me[i].x());
	  angref = atan2 (ref.me[i].y()-sgn(pos.me[i].y())*(GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY),
			  sinal*ref.me[i].x()-(-(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH-ROBOT_RAY)));

	  if ((sinal*pos.me[i].x() < -FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+ROBOT_RAY &&
	       fabs(pos.me[i].y())>GOAL_FIELD_HEIGHT/2.0) ||
	      sinal*ref.me[i].x() < -FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+ROBOT_RAY) {
	    if(sinal*pos.me[i].x() < -FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+ROBOT_RAY &&
	       fabs(pos.me[i].y())>GOAL_FIELD_HEIGHT/2.0) {
	     
	      if (pos.me[i].y() > 0 ) {
		if (angref < angrob && ref.me[i].y() < GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY) {
		  ref.me[i].x() = sinal*(-FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+2*ROBOT_RAY);
		  ref.me[i].y() = (GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY);
		  ref.me[i].theta() = -M_PI_2;
		}
	      } else {
		if (angref > angrob && ref.me[i].y() > -GOAL_FIELD_HEIGHT/2.0-ROBOT_RAY) {
		  ref.me[i].x() = sinal*(-FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+2*ROBOT_RAY);
		  ref.me[i].y() = -(GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY);
		  ref.me[i].theta() = M_PI_2;
		}
	      }
	    } else {//sinal*ref.me[i].x() < -FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+ROBOT_RAY
	      if (ref.me[i].y() > 0) {
		if (angref > angrob && pos.me[i].y() < GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY ) {
		  ref.me[i].y() = (GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY);
		  ref.me[i].x() = sinal*(-FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+2*ROBOT_RAY)+
		    sinal*fabs(ref.me[i].y()-pos.me[i].y())/2.0;
		  ref.me[i].theta() = M_PI;
		}
	      } else {
		if (angref < angrob && pos.me[i].y() > -(GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY)) {
		  ref.me[i].y() = -(GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY);
		  ref.me[i].x() = sinal*(-FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+2*ROBOT_RAY)+
		    sinal*fabs(ref.me[i].y()-pos.me[i].y())/2.0;
		  ref.me[i].theta() = M_PI;
		}
	      }
	    }
	  }     
	}
	break;
	
      case SAIR_DA_AREA:
	{  double a, b;
	    //teste de proximidade da refer. com linhas da area (lateral e frente)
	    //se tiver mais perto de uma das linhas projeta fora a nova posicao
	    //se parecida a distancia referencia fica na quina:
	    // (GOAL_FIELD_WIDTH+ROBOT_RAY, GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY
	    a = (GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY)-fabs (pos.me[i].y());
	    b = fabs (sinal*pos.me[i].x()+FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH-ROBOT_RAY);
	    if (a<b) {
	      ref.me[i].y() = sgn(ref.me[i].y())*(GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY+0.05);
	      ref.me[i].x() = pos.me[i].x();
	      ref.me[i].theta() = pos.me[i].theta();
	    } else {
	      ref.me[i].x() = sinal*(-FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+ROBOT_RAY+0.05);
	      ref.me[i].y() = pos.me[i].y();
	      ref.me[i].theta() = pos.me[i].theta();
	    }
	    
	}
	  
	break;
   
      case CONTORNAR_ZAG:
	{
	  // ETAPA 3
	  if// (sinal*(pos.me[i].x()+sinal*ROBOT_RAY) < sinal*pos.ball.x()&&
	    (fabs(pos.me[i].y()-pos.ball.y()) > ROBOT_RAY+2*BALL_RAY) {
	    ref.me[i].x() = pos.ball.x()-sinal*2*ROBOT_RAY;
	    ref.me[i].y() = pos.ball.y();
	    ref.me[i].theta() = 0;
	    //cout << "3 etapa\n";
	  }

	  if (sinal*(pos.ball.x()-sinal*(ROBOT_RAY+BALL_RAY)) < sinal*pos.me[i].x()){// &&
	    //	      fabs(pos.ball.y()-pos.me[i].y()) >= ROBOT_RAY+2*BALL_RAY) {
	    ref.me[i].y() = pos.me[i].y(); //pos.ball.y()-sgn(pos.ball.y())*2*ROBOT_RAY;
	    ref.me[i].x() = pos.ball.x()-sinal*0.12;
	    ref.me[i].theta() = M_PI_2;
	    //  cout << "2 etapa\n";
	  }
	  
	  //ETAPA 1
	  int inverte = sgn(pos.ball.y());
	  // QUANDO A BOLA ESTA DO COLADO (Lateralmente) NA NOSSA invertemos 
	  // o sentido do contorno;
	  if ((sinal*pos.ball.x() < -FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH) &&
	      (fabs(pos.ball.y()) < GOAL_FIELD_HEIGHT/2.0+2*ROBOT_EDGE)) inverte *= -1;
	  
	  if ((pos.ball.y()-inverte*(ROBOT_RAY+2*BALL_RAY))*inverte < pos.me[i].y()*inverte &&
	      sinal*pos.me[i].x()>sinal*pos.ball.x()-sinal*BALL_RAY) {
	    ref.me[i].y() = pos.ball.y()-inverte*(BALL_RAY+2*ROBOT_RAY);
	    ref.me[i].x() = pos.me[i].x();
	    ref.me[i].theta() = 0;
	    // cout << "1 etapa\n";
	  }
	}
	break;

      case TIRAR_BOLA_MEIO:
	if //(pos.ball.y()+BALL_RAY > pos.me[i].y()+ROBOT_RAY &&
	  //pos.ball.y()-BALL_RAY < pos.me[i].y()-ROBOT_RAY) {
	  (fabs (pos.ball.y()-pos.me[i].y()) < ROBOT_RAY+BALL_RAY &&
	   (fabs (pos.ball.x() -pos.me[i].x()) > 0.04)) {
	  if (pos.ball.y() > 0) {
	    ref.me[i].y() = pos.me[i].y()-2*ROBOT_EDGE;
	  } else {
	    ref.me[i].y() = pos.me[i].y()+2*ROBOT_EDGE;
	  }
	  ref.me[i].x() = pos.me[i].x();
	  ref.me[i].theta() = 0;
	}
	if (//(fabs (pos.ball.x() -pos.me[i].x())  BALL_RAY) &&
	    //	       ((M_PI/2.1>  pos.me[i].theta() || pos.me[i].theta() > (M_PI-M_PI/2.1)) ||
	    //		(-M_PI/2.1 > pos.me[i].theta() || pos.me[i].theta() > (-M_PI+M_PI/2.1)))) && 
	    (fabs(pos.ball.y()-pos.me[i].y()) >= ROBOT_RAY+BALL_RAY+0.04)){ 
	  // ((M_PI/2.3 >  pos.me[i].theta() || pos.me[i].theta() > (M_PI-M_PI/2.3)) ||
	  // (-M_PI/2.3 > pos.me[i].theta() || pos.me[i].theta() > (-M_PI+M_PI/3)))){
	    
	  ref.me[i].y() = pos.me[i].y();
	  ref.me[i].x() = pos.ball.x();
	  if (sinal*ref.me[i].x() < -FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+ROBOT_RAY)
	    ref.me[i].x() = sinal*(-FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+ROBOT_RAY);
	  
	  ref.me[i].theta() = sgn(pos.ball.y()-pos.me[i].y())*M_PI_2;
	}
	if ((fabs (pos.ball.x() -pos.me[i].x()) < 2*BALL_RAY) &&
	    (fabs(pos.me[i].y() -pos.ball.y()) > ROBOT_RAY+BALL_RAY) && 
	     ((M_PI/2.2< pos.me[i].theta() && pos.me[i].theta() < (M_PI-M_PI/2.2)) ||
	      (-M_PI/2.2 > pos.me[i].theta() && pos.me[i].theta() > (-M_PI+M_PI/2.2)))) {
	      
	  ref.me[i].x() =  pos.ball.x()+(pos.ball.x()-pos.me[i].x());
	  // sinal*(-FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+ROBOT_RAY); // pos.ball.x();
	  // ref.me[i].y() = sgn(-pos.me[i].y()+pos.ball.y())*GOAL_FIELD_HEIGHT/2.0;
			     ref.me[i].y() = sgn(-pos.me[i].y()+pos.ball.y())*FIELD_HEIGHT/2.0;
	  ref.me[i].theta() =  sgn(pos.ball.y()-pos.me[i].y())*M_PI_2;
	  
	}

	break;
      
      case LEVAR_BOLA:
	{
	  double ang =   atan2 (pos.ball.y()-pos.me[i].y(), pos.ball.x()-pos.me[i].x());
	  ref.me[i].x() = pos.ball.x()+DIST_BOLA/2.0*cos(ang);
	  ref.me[i].y() = pos.ball.y()+DIST_BOLA/2.0*sin(ang);
	  ref.me[i].theta() = ang;
	}
	break;
      
      case RETIRAR_BOLA_PAREDE: 
	{double ang = M_PI_2/2;
	  if ((sinal*pos.me[i].y()) > 0){
	    ref.me[i].x() = pos.me[i].x()+5*ROBOT_RAY*cos (pos.me[i].theta()-ang);
	    ref.me[i].y() = pos.me[i].y()+5*ROBOT_RAY*sin (pos.me[i].theta()-ang);
	    ref.me[i].theta() = pos.me[i].theta()-ang;
	  } else {
	    ref.me[i].x() = pos.me[i].x()+5*ROBOT_RAY*cos (pos.me[i].theta()+ang);
	    ref.me[i].y() = pos.me[i].y()+5*ROBOT_RAY*sin (pos.me[i].theta()+ang);
	    ref.me[i].theta() = pos.me[i].theta()+ang;
	  }
	}
	break;

      case POSICIONAR_RETIRADA:	
	// BOLA NA LATERAL DA AREA E NO FUNDO DO CAMPO
 	if((fabs (pos.ball.y()) > GOAL_FIELD_HEIGHT/2.0) &&
 	   (fabs (pos.ball.y())  < GOAL_FIELD_HEIGHT/2.0+3*ROBOT_RAY) &&
 	   (fabs (pos.ball.x()) > FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH)) {
 	  ref.me[i].x() = pos.ball.x()+sinal*(ROBOT_EDGE/2.0+BALL_RAY); //2*ROBOT_RAY;
 	  ref.me[i].y() = pos.ball.y(); //sgn(pos.ball.y())*(GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY); 
 	  if (fabs(pos.ball.y()) < GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY) 
	    ref.me[i].y() = sgn(pos.ball.y())*(GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY);
	  ref.me[i].theta() = (sinal+1)*M_PI_2;
	  //  cout << "BOLA NA LATERAL\n";
 	}

	//BOLA NO FUNDO E NA LATERAL DO CAMPO E 
	if((fabs (pos.ball.y()) > GOAL_FIELD_HEIGHT/2.0+3*ROBOT_RAY) &&
	   (fabs (pos.ball.x()) > FIELD_WIDTH/2.0-(GOAL_FIELD_WIDTH+ROBOT_RAY))) {
	  ref.me[i].x() = pos.ball.x();
	  if (fabs(ref.me[i].x()) > FIELD_WIDTH/2.0-ROBOT_RAY) 
	    ref.me[i].x() = sinal*(-FIELD_WIDTH/2.0+ROBOT_RAY);
	  ref.me[i].y() = pos.ball.y() -sgn(pos.ball.y())*(ROBOT_EDGE/2.0+BALL_RAY);
	  ref.me[i].theta() = sgn(pos.ball.y())*M_PI_2;
	  // cout << "BOLA NO FUNDO\n";
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
   
      case IMPOSSIVEL:
	printf ("ERRO: PAPEL \"IMPOSSIVEL\" foi chamado, nunca deveria entra nese caso!\n");
	break;
	
      default:
	printf ("Erro: nenhum papel foi atribuido!\n");
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



void Strategy::atacante_floripa(int id)
{

  /*   
  float distx =  FIELD_HEIGHT/2.0 -sinal*pos.ball.x();
  float disty =  sinal*pos.ball.y();
  float distb =  hypot (distx, disty);
    

  // PREDICADOS

  bool bola_na_area_adv = distb <= GOAL_FIELD_HEIGHT;  
  bool bola_no_ataque = (sinal * pos.ball.x()) > -0.05;
  bool bola_na_parede = (fabs(pos.ball.y())>(FIELD_HEIGHT/2.0 -ROBOT_RAY)) ||
			 (fabs(pos.ball.x())>(FIELD_WIDTH/2.0-ROBOT_RAY)) ||
			 (fabs(pos.ball.y()) + fabs(pos.ball.x()) > BOUND_SIDE_WALL_CORNER + BOUND_BACK_WALL_CORNER);
  bool bola_comigo =  hypot (pos.me[id].x()-pos.ball.x(), pos.me[id].y() -pos.ball.y()) < DIST_BOLA;
  bool bola_com_adv = (hypot (pos.op[0].x()-pos.ball.x(), pos.op[0].y() -pos.ball.y()) < DIST_BOLA) ||
    (hypot (pos.op[1].x()-pos.ball.x(), pos.op[1].y() -pos.ball.y()) < DIST_BOLA) ||
    (hypot (pos.op[2].x()-pos.ball.x(), pos.op[2].y() -pos.ball.y()) < DIST_BOLA);
  bool atras_da_bola = (sinal*pos.me[id].x()) < (sinal*pos.ball.x());
  bool trajeto_livre= true;

  
  
  bool pode_posicionar = ((fabs(ref.me[id].y())>(FIELD_HEIGHT/2.0 -ROBOT_RAY)) ||
			  (fabs(ref.me[id].x())>(FIELD_WIDTH/2.0-ROBOT_RAY)) ||
			  (fabs(ref.me[id].y()) + fabs(ref.me[id].x()) > BOUND_SIDE_WALL_CORNER + BOUND_BACK_WALL_CORNER))&&
                          (hypot (ref.me[id].x() -pos.op[0].x(), ref.me[id].y() -pos.op[0].y()) > 2.0*ROBOT_RAY) && 
                          (hypot (ref.me[id].x() -pos.op[0].x(), ref.me[id].y() -pos.op[0].y()) > 2.0*ROBOT_RAY) &&
			   (hypot (ref.me[id].x() -pos.op[0].x(), ref.me[id].y() -pos.op[0].y()) > 2.0*ROBOT_RAY);
			   


 RPAPEL pos_papeis =
   plano_atacante (bola_na_area_adv, bola_no_ataque, bola_na_parede, bola_comigo, bola_com_adv, 
		   atras_da_bola, trajeto_livre, pode_posicionar);
  */

  //TEMPORARIO
//  if (pos_papeis.estacionar) {papeis.me[id]= ESTACIONAR;/* cout << "estacionar\n";*/ return;}
//  if (pos_papeis.contornar) {papeis.me[id]= CONTORNAR; /*cout << "contornar\n";*/ return;}
//  if (pos_papeis.alinhar) {papeis.me[id]= ALINHAR; /*cout << "alinhar\n";*/ return;}
//  if (pos_papeis.girar) {papeis.me[id]= GIRAR; /*cout << "girar\n";*/ return;}
//  if (pos_papeis.chutar_gol) {papeis.me[id]= CHUTAR_GOL; /*cout << "chutar\n";*/  return;}
//  if (pos_papeis.alinhar_parede) {papeis.me[id]= ALINHAR_PAREDE; /*cout << "alinha_p\n";*/ return;}
//  if (pos_papeis.descolar_parede) {papeis.me[id]= DESCOLAR_PAREDE; /*cout << "descolar_p\n";*/ return;}
//  if (pos_papeis.descolar_quina) {papeis.me[id]= DESCOLAR_QUINA; /*cout << "descolar_q\n";*/ return;}
//  if (pos_papeis.goleiro) {papeis.me[id]= GOLEIRO; /*cout << "goleiro\n";*/ return;}
//  if (pos_papeis.tirar_do_gol) {papeis.me[id]= TIRAR_DO_GOL;/* cout << "tirar_do_gol\n";*/  return;}
//  if (pos_papeis.posicionar) {papeis.me[id]= POSCIONAR; return;}
//  if (pos_papeis.ir_bola) {papeis.me[id]= IR_BOLA; return;}
//  if (pos_papeis.desviar) {papeis.me[id]= DESVIAR; return;}
  papeis.me[id]=POSICIONAR;

}

void Strategy::defensor_floripa(int id, int atacante, int goleiro)
{//VARIAVEIS PARA CALCULO:

  // usado no alinhado
 bool regiao_per =
   (sinal*pos.me[id].x() > -FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH &&
    sinal*pos.me[id].x() < -FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+2*ROBOT_RAY &&
    fabs (pos.me[id].y()) < GOAL_FIELD_HEIGHT/2.0);

  double dist_bola = 
    hypot (pos.me[id].x()-pos.ball.x(), pos.me[id].y()-pos.ball.y());
  double ang_bola =
    atan2 (pos.ball.y()-pos.me[id].y(), pos.ball.x()-pos.me[id].x());
  double px, py, erro; 
  px = dist_bola*cos (pos.me[id].theta()) + pos.me[id].x();
  py = dist_bola*sin (pos.me[id].theta()) + pos.me[id].y();
  erro = hypot (px -pos.ball.x(), py -pos.ball.y());

  // usado no caminho_livre
  //  param eq de reta:
  double a, b, c;
  double x,y;
  y = pos.me[id].y()+DIST_BOLA*sin(pos.me[id].theta());
  x = pos.me[id].x()+DIST_BOLA*cos(pos.me[id].theta());
  a = pos.me[id].y() -y;   
  b = x -pos.me[id].x();
  c = pos.me[id].x()*y-x*pos.me[id].y();
  // double dist_trajeto_adv0 = fabs (a*pos.op[0].x()+b*pos.op[0].y()+c)/hypot(a, b);
  // double dist_trajeto_adv1 = fabs (a*pos.op[1].x()+b*pos.op[1].y()+c)/hypot(a, b);
  // double dist_trajeto_adv2 = fabs (a*pos.op[2].x()+b*pos.op[2].y()+c)/hypot(a, b);
  
  //  double dist_pos_ref = DIST_BOLA; //hypot (pos.me[id].x()-ref.me[id].x(), pos.me[id].y()-ref.me[id].y());

  // double dist_pos_adv0 = hypot (pos.me[id].x()-pos.op[0].x(), pos.me[id].y()-pos.op[0].y());
  // double dist_pos_adv1 = hypot (pos.me[id].x()-pos.op[1].x(), pos.me[id].y()-pos.op[1].y());
  // double dist_pos_adv2 = hypot (pos.me[id].x()-pos.op[2].x(), pos.me[id].y()-pos.op[2].y());
  // double dist_ref_adv0 = hypot (ref.me[id].x()-pos.op[0].x(), ref.me[id].y()-pos.op[0].y());
  // double dist_ref_adv1 = hypot (ref.me[id].x()-pos.op[1].x(), ref.me[id].y()-pos.op[1].y());
  // double dist_ref_adv2 = hypot (ref.me[id].x()-pos.op[2].x(), ref.me[id].y()-pos.op[2].y());
  
  // bool no_intervalo_adv0 = dist_pos_adv0 < dist_pos_ref+2*ROBOT_RAY && dist_ref_adv0 < dist_pos_ref+2*ROBOT_RAY;
  // bool no_intervalo_adv1 = dist_pos_adv1 < dist_pos_ref+2*ROBOT_RAY && dist_ref_adv1 < dist_pos_ref+2*ROBOT_RAY;
  // bool no_intervalo_adv2 = dist_pos_adv2 < dist_pos_ref+2*ROBOT_RAY && dist_ref_adv2 < dist_pos_ref+2*ROBOT_RAY;  

 
  

  //PREDICADOS
  bool bola_nossa_area = 
    (pos.ball.x()*sinal <= -FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+BALL_RAY) &&
    (fabs (pos.ball.y()) <= GOAL_FIELD_HEIGHT/2.0+BALL_RAY);
  bool bola_na_zaga =
    pos.ball.x()*sinal <= -0.05;
  bool bola_na_parede =
    (fabs(pos.ball.y())>(FIELD_HEIGHT/2.0 -2*ROBOT_RAY)) ||
    (fabs(pos.ball.x())>(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH)) ||
    (fabs(pos.ball.y())  + fabs(pos.ball.x()) > BOUND_SIDE_WALL_CORNER + BOUND_BACK_WALL_CORNER);
  bool atras_da_bola =
    pos.me[id].x()*sinal < pos.ball.x()*sinal;
  bool alinhado =
    ((regiao_per && fabs (pos.me[id].theta()-ang_bola) < DELTA_ANG) ||
     !regiao_per) ||
    //HISTERESE
    (((regiao_per && fabs (pos.me[id].theta()-ang_bola) < 2*DELTA_ANG) ||
      !regiao_per) && papeis.me[id] == GIRAR);

 
    
  bool bola_comigo =
    hypot (pos.me[id].x()-pos.ball.x(), pos.me[id].y()-pos.ball.y()) <= ROBOT_RAY*2 ;
  bool colado_na_bola =
    hypot (pos.me[id].x()-pos.ball.x(), pos.me[id].y()-pos.ball.y()) <= ROBOT_RAY*1.5;
  bool dentro_area = 
    ((pos.me[id].x()*sinal <= -FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+ROBOT_RAY) &&
     (fabs (pos.me[id].y()) <= GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY)) ||
    //HISTERESE
    ((pos.me[id].x()*sinal <= -FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+ROBOT_EDGE) &&
     (fabs (pos.me[id].y()) <= GOAL_FIELD_HEIGHT/2.0+ROBOT_EDGE) && papeis.me[id] == SAIR_DA_AREA);
  //   (dist_trajeto_adv0 > 2*ROBOT_RAY || !no_intervalo_adv0) && 
    //    (dist_trajeto_adv1 > 2*ROBOT_RAY || !no_intervalo_adv1) &&
    //    (dist_trajeto_adv2 > 2*ROBOT_RAY || !no_intervalo_adv2);
  //regiao critica
  bool pode_ficar_atras = 
  ((sinal*pos.ball.x() > -FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH && fabs (pos.ball.y()) > GOAL_FIELD_HEIGHT/2.0) ||
     (sinal*pos.ball.x() > -FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+3*ROBOT_RAY && fabs (pos.ball.y()) < GOAL_FIELD_HEIGHT/2.0));
  // bool bola_vindo = //false;
  //      pos.me[id].y()+ROBOT_RAY < (fabs (pos.me[id].x()-pos.ball.x())*sin (pos.vel_ball.ang)+pos.ball.y()) &&
  //       pos.me[id].y()-ROBOT_RAY > (fabs (pos.me[id].x()-pos.ball.x())*sin (pos.vel_ball.ang)+pos.ball.y()) &&
  //       sinal*pos.ball.x() > sinal*pos.future_ball.x() &&
  //       pos.vel_ball.mod > VEL_BOLA_LENTA && atras_da_bola; 
    // hypot (pos.me[id].y()-pos.ball.y(), pos.me[id].x()-pos.ball.x()) < DIST_BOLA;
    //(pos.me[id].y()-pos.ball.y()) < DIST_BOLA;
  bool alinhadoy =
    fabs(pos.me[id].y()-pos.ball.y()) < DIST_BOLA;

  // bool posicao_padrao = hypot (sinal*pos.me[id].x()-FIELD_WIDTH/2.0+GOAL_FIELD_WIDTH+ROBOT_RAY, 
  // 	   pos.me[id].y()-(sgn(pos.me[id].y())*(GOAL_FIELD_HEIGHT/2.0+ROBOT_RAY))) < DELTA_L;
  
  unsigned int indice =
    (bola_nossa_area*512)+(bola_na_zaga*256)+(bola_na_parede*128)+(alinhado*64)+(bola_comigo*32)+
    (colado_na_bola*16)+(atras_da_bola*8)+(pode_ficar_atras*4)+(alinhadoy*2)+dentro_area;
    papeis.me[id] = resp_zagueiro[indice];
    //     cout << "resp_zagueiro[" << indice << "]:" <<  resp_zagueiro[indice] << endl;
  // cout << "C.L.=" << caminho_livre << endl; 
    //papeis.me[id] = NOVO_ALIN; //CONTORNAR_ZAG;

}

/*
PAPEL_JOGADOR Strategy::plano_zagueiro (bool bola_na_area, bool bola_na_zaga, bool bola_na_parede, bool alinhado,  
				 bool bola_comigo, bool bola_com_adv, bool caminho_livre, bool atras_da_bola, 
				 bool trajeto_livre, bool pode_posicionar) {
  unsigned int indice =
    (bola_na_area*512)+(bola_na_zaga*256)+(bola_na_parede*128)+(alinhado*64)+(bola_comigo*32)+
    (bola_com_adv*16)+(caminho_livre*8)+(atras_da_bola*4)+(trajeto_livre*2)+pode_posicionar;
  PAPEL_JOGADOR r = resp_zagueiro[indice];
  RPAPEL ret;
  if (r == ESTACIONAR) {
*/  

void Strategy::novo_atacante(int id) {
  if (t >= INTERV_TESTE_TRAV+tata) 
    tata = t;

  //variaveis para calculos
  

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
  
  // double thetaref = arc_tang(-pos.ball.y(),
  // 			     sinal*FIELD_WIDTH/2.0-pos.ball.x());
  // double xref = pos.ball.x()-DIST_CHUTE*cos(thetaref);
  // double yref = pos.ball.y()-DIST_CHUTE*sin(thetaref);

  // double dist_ref = hypot(xref-pos.me[id].x(),yref-pos.me[id].y());

  // if ((PWM.me[i].left() > PWM_MINIMO || PWM.me[i].right() > PWM_MINIMO) &&
  //   (LONGE_L > hypot
  //PREDICADOS
  bool n_bola_no_ataque =
    sinal*pos.ball.x() < -0.05; 
    // (sinal*pos.ball.x() < -0.05 && papeis.me[id] == POSICIONAR);
  // cout << "bola_regiao_perigosa: " << bola_regiao_perigosa << endl;
  bool atras_da_bola =
    (sinal*pos.ball.x()-sinal*pos.me[id].x()>0) ||
    (sinal*pos.ball.x()-sinal*pos.me[id].x()>-0.05 &&
     papeis.me[id]!=CONTORNAR);
  //cout << "atras_da_bola: " << atras_da_bola << endl;
  bool consegue_alinhar =
    fabs(ypos_fut)< 0.85*GOAL_HEIGHT/2.0  &&
	  ( dist < 2*DIST_CHUTE  ||
	    papeis.me[id]==CHUTAR_GOL ||
	    papeis.me[id]==GIRAR );
  //cout << "consegue_alinhar: " << consegue_alinhar << endl;
  bool consegue_chutar =
    (fabs(yalin)<0.85*GOAL_HEIGHT/2.0 ) ||
    (fabs(yalin)<GOAL_FIELD_HEIGHT/2.0 && papeis.me[id]==CHUTAR_GOL );
  //cout << "consegue_chutar: " << consegue_chutar << endl;
  bool bola_na_parede =
    (fabs (pos.ball.y()) >  FIELD_HEIGHT/2.0-2*ROBOT_RAY);
  //  bool fora_area_permitida =
  //  (fabs(yref)>(FIELD_HEIGHT/2.0-ROBOT_RAY)) ||
  //   (fabs(xref)>(FIELD_WIDTH/2.0-ROBOT_RAY)) ||
  //   (fabs(xref) + fabs (yref) > BOUND_SIDE_WALL_CORNER + BOUND_BACK_WALL_CORNER);
  //cout << "fora_area_permitida: " << fora_area_permitida << endl;
  bool bola_comigo =
    hypot (pos.me[id].x()-pos.ball.x(), pos.me[id].y()-pos.ball.y()) <= ROBOT_RAY*2;
  bool colado_na_bola = 
    hypot (pos.me[id].x()-pos.ball.x(), pos.me[id].y()-pos.ball.y()) <= 1.5*ROBOT_RAY;
  bool bola_no_fundo = 
    fabs (pos.ball.x()) > FIELD_WIDTH/2.0-CORNER_DIMENSION &&
    //                                            melhora condicao de giro
    fabs (pos.ball.y()) > GOAL_FIELD_HEIGHT/2.0 -ROBOT_RAY;  
  bool travado =
    (fabs(pwm.me[id].left) >  0.2 || fabs(pwm.me[id].right) > 0.2) &&
    (DELTA_L > hypot (pos.me[id].x()-pos_antiga_ata.x(), pos.me[id].y()-pos_antiga_ata.y()) &&
     DELTA_ANG > fabs (pos.me[id].theta()-pos_antiga_ata.theta())) &&
    (bola_na_parede || bola_no_fundo) &&
    // PERTO DA PAREDE
    ((fabs(pos.me[id].y())>(FIELD_HEIGHT/2.0 -2*ROBOT_RAY)) ||
     (fabs(pos.me[id].x())>(FIELD_WIDTH/2.0-2*ROBOT_RAY)));
     //  (fabs(pos.me[id].y())  + fabs(pos.me[id].x()) > BOUND_SIDE_WALL_CORNER + BOUND_BACK_WALL_CORNER));

  unsigned int indice = 
    n_bola_no_ataque*256+atras_da_bola*128+consegue_alinhar*64+consegue_chutar*32+
    bola_na_parede*16+bola_comigo*8+colado_na_bola*4+bola_no_fundo*2+travado;
  papeis.me[id] = resp_atacante[indice]; 

  //    cout << "resp_atacante[" << n_bola_no_ataque << atras_da_bola << consegue_alinhar << consegue_chutar << bola_na_parede  << bola_comigo << colado_na_bola << bola_no_fundo << travado << "]:" <<  resp_atacante[indice] << endl;
   
   if (t == tata) {
     pos_antiga_ata.X = pos.me[id].x();
     pos_antiga_ata.Y = pos.me[id].y();
     pos_antiga_ata.T = pos.me[id].theta();
   }


}


void Strategy::atacante_tradi (int id) {
  //  void Strategy::atacante_floripa(int id)
 //variaveis para calculos
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
    sinal*pos.ball.x() < -0.05;
  // cout << "bola_regiao_perigosa: " << bola_regiao_perigosa << endl;
  bool atras_da_bola =
    (sinal*pos.ball.x()-sinal*pos.me[id].x()>0) ||
    (sinal*pos.ball.x()-sinal*pos.me[id].x()>-0.05 &&
     papeis.me[id]!=CONTORNAR);
  //cout << "atras_da_bola: " << atras_da_bola << endl;
  bool consegue_alinhar =
    fabs(ypos_fut)< 0.85*GOAL_HEIGHT/2.0  &&
	  ( dist < 2*DIST_CHUTE  ||
	    papeis.me[id]==CHUTAR_GOL ||
	    papeis.me[id]==GIRAR );
  //cout << "consegue_alinhar: " << consegue_alinhar << endl;
  bool consegue_chutar =
    (fabs(yalin)<0.85*GOAL_HEIGHT/2.0 ) ||
    (fabs(yalin)<GOAL_FIELD_HEIGHT/2.0 && papeis.me[id]==CHUTAR_GOL );
  //cout << "consegue_chutar: " << consegue_chutar << endl;
  bool fora_area_permitida =
    (fabs(yref)>(FIELD_HEIGHT/2.0-ROBOT_RAY)) ||
    (fabs(xref)>(FIELD_WIDTH/2.0-ROBOT_RAY)) ||
    (fabs(xref) + fabs (yref) > BOUND_SIDE_WALL_CORNER + BOUND_BACK_WALL_CORNER);
  //cout << "fora_area_permitida: " << fora_area_permitida << endl;
  bool precisa_descolar =
    dist_ref<DIST_CHUTE ||
    (dist_ref<1.5*DIST_CHUTE && (papeis.me[id]==DESCOLAR_PAREDE ||
			     papeis.me[id]==DESCOLAR_QUINA));
  //cout << "precisa_descolar: " << precisa_descolar;
  bool precisa_descolar_quina =
    (fabs(xref)>BOUND_BACK_WALL_CORNER &&
     fabs(yref)>BOUND_SIDE_WALL_CORNER &&
     fabs(xref)+fabs(yref)>BOUND_SIDE_WALL_CORNER + BOUND_BACK_WALL_CORNER) ||
    (fabs(xref)>1.15*BOUND_BACK_WALL_CORNER &&
     fabs(yref)>1.15*BOUND_SIDE_WALL_CORNER &&
     fabs(xref)+fabs(yref)>1.15*(BOUND_SIDE_WALL_CORNER+BOUND_BACK_WALL_CORNER) &&
     papeis.me[id]== DESCOLAR_QUINA);
  //cout << "precisa_descolar_quina: " << precisa_descolar_quina << endl;
  // unsigned int indice = 
  // (bola_regiao_perigosa*64) + (atras_da_bola*32) + (consegue_alinhar*16) + (consegue_chutar*8) +
  // (fora_area_permitida*4) + (precisa_descolar*2) + precisa_descolar_quina;
  
 // papeis.me[id] = resp_atacante[indice];

  if (bola_regiao_perigosa) {
    papeis.me[id] = POSICIONAR;
  }
  else {
    if (atras_da_bola) {
      if (consegue_alinhar) {
  	if (consegue_chutar) {
  	  papeis.me[id]=CHUTAR_GOL;
  	}
  	else {
  	  papeis.me[id]=GIRAR;
  	}
      }
      else {
  	if (fora_area_permitida) {
  	  if (precisa_descolar) {
  	    if (precisa_descolar_quina) {
  	      papeis.me[id]=DESCOLAR_QUINA;
  	    }
  	    else {
  	      papeis.me[id]=DESCOLAR_PAREDE;
  	    }
  	  } else {
  	    papeis.me[id]=ALINHAR_PAREDE;
  	  }
  	} 
  	else {
  	  papeis.me[id]=ALINHAR;
  	}
      }
    } 
    else {
      papeis.me[id]=CONTORNAR;
    }

  }
}


RPAPEL Strategy::plano_atacante (bool bola_na_area_adv, bool bola_no_ataque, bool bola_na_parede, bool bola_comigo, 
				 bool bola_com_adv, bool atras_da_bola, bool trajeto_livre, bool pode_posicionar) {
  unsigned int indice = 
    (bola_na_area_adv*128) + (bola_no_ataque*64) + (bola_na_parede*32) + (bola_comigo*16) +
    (bola_com_adv*8) + (atras_da_bola*4) + (trajeto_livre*2) + pode_posicionar; 
  
  PAPEL_JOGADOR papel_at = resp_atacante[indice];
  // cout << papel_at <<  "indice: " <<  indice << endl;
  RPAPEL ret;
  if (papel_at == ESTACIONAR) {ret.estacionar= true;} else {ret.estacionar= false;}
  if (papel_at == CONTORNAR) {ret.contornar= true;}  else {ret.contornar= false;}
  if (papel_at == ALINHAR) {ret.alinhar= true;} else {ret.alinhar= false;}
  if (papel_at == GIRAR) {ret.girar= true;} else {ret.girar= false;}
  if (papel_at == CHUTAR_GOL) {ret.chutar_gol= true;} else {ret.chutar_gol= false;}
  if (papel_at == ALINHAR_PAREDE) {ret.alinhar_parede= true;} else {ret.alinhar_parede= false;}
  if (papel_at == DESCOLAR_PAREDE) {ret.descolar_parede= true;} else {ret.descolar_parede= false;}
  if (papel_at == DESCOLAR_QUINA) {ret.descolar_quina= true;} else {ret.descolar_quina= false;}
 
  if (papel_at == GOLEIRO) {ret.goleiro= true;} else {ret.goleiro= false;}
  if (papel_at == TIRAR_DO_GOL) {ret.tirar_do_gol= true;} else {ret.tirar_do_gol= false;}
  if (papel_at == POSICIONAR) {ret.posicionar = true;} else {ret.posicionar= false;}
  if (papel_at == IR_BOLA) {ret.ir_bola = true;} else {ret.ir_bola= false;}
  if (papel_at == DESVIAR) {ret.desviar = true;} else {ret.desviar= false;}
  return ret;
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
