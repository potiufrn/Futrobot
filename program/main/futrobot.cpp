#include <iostream>
#include <fstream>
#include "futrobot.h"
#include <sys/time.h>

using namespace std;


inline void* management2( void *x )
{
  ((Futrobot*)x)->management();
  cout << "Saiu do management2." << endl;
  pthread_exit(NULL);
}

Futrobot::Futrobot(TEAM team, SIDE side, GAME_MODE gameMode)
  :FutData(team,side,gameMode),
   Acquisition(team,side,gameMode),
   Localization(team,side,gameMode),
   Strategy(team,side,gameMode),
   Obstacles(team,side,gameMode),
   Control(team,side,gameMode),
   Transmission(team,side,gameMode),
   Export(team,side,gameMode)
{
  t_start = t_end_acq = t_end_loc = t_end_str = t_end_obs =
    t_end_con = t_end_tra = t_end_exp = 0.0;

}

bool Futrobot::start_management()
{
#ifndef _SO_SIMULADO_
  // start_transmission();
#endif
  if(pthread_create(&thr_ger, NULL, management2, (void*)this)) return true;
  return false;
}

bool Futrobot::finish_management()
{
  pthread_join(thr_ger,NULL);

  return(false);
}

//Laco principal do programa
void Futrobot::management()
{
  initialize();// inicializado do export
  double myt_start, myt_end_cap, myt_end_acq, myt_end_loc, myt_end_str, myt_end_obs, myt_end_con, myt_end_tra, myt_end_exp;
  cout<<"management!"<<endl;
  if(gameState() == FINISH_STATE) cout <<"gameState() == FINISH_STATE"<<endl;
  //  cont_teste = 0;
  while (gameState() != FINISH_STATE) {
    // Espera por uma nova imagem
    if (gameState() != FINISH_STATE && acquisitionWait()) { //VERIFICAR USO DO AND OU OR
      finish();
      cerr << "Erro na espera por nova imagem!\n";
    }
    //  cout << "esperou imagem\n";
    myt_start = relogio();
    dt_amostr = myt_start - t_start;
    //teste para evitar divisoes por zero.
    if(dt_amostr == 0.0) dt_amostr = FRAME_RATE_INICIAL;
    // Lẽ a nova imagem
    if (gameState() != FINISH_STATE && acquisitionCapture()) {
      finish();
      cerr << "Erro na leitura da nova imagem!\n";
    }
    id_pos++;
    //    cout << "Pegou imagem\n";
    myt_end_cap = relogio();
    // Fornece a pose dos robos e a posicao da
    // bola em coordenadas de mundo.

    if (gameState() != FINISH_STATE && acquisition()) {
      finish();
      cerr << "Erro no processamento da imagem!\n";
    }
    //cout << "Processou imagem\n";
    //cout << pos.ball.x() << endl << pos.ball.y() << endl << endl;
    myt_end_acq = relogio();
    //Realiza a correcao e filtragem da pose dos robos e da bola.
    if (gameState() != FINISH_STATE && localization()) {
      finish();
      cerr << "Erro na localizacao dos robos!\n";
    }
    myt_end_loc = relogio();
    // Calcula as posicoes finais dos robos.
    if (gameState() != FINISH_STATE && strategy()) {
      finish();
      cerr << "Erro na estrategia do jogo!\n";
    }
    myt_end_str = relogio();
    // Altera as posicoes finais desejadas em funcao de possiveis
    // obstaculos no caminho
    if (gameState() != FINISH_STATE && obstacles()) {
      finish();
      cerr << "Erro no desvio de obstaculos!\n";
    }
    myt_end_obs = relogio();
    // Calcula o novo controle (ou seja, as novas tensoes dos motores)
    if (gameState() != FINISH_STATE && control()) {
      finish();
      cerr << "Erro no controle dos robos!\n";
    }
    myt_end_con = relogio();
    // Informa as tensoes para os robos.
    if (gameState() != FINISH_STATE && transmission()) {
      finish();
      cerr << "Erro na transmissao dos dados!\n";
    }
    myt_end_tra = relogio();
     //Modulo de exportacao dos dados
    if (gameState() != FINISH_STATE && exxport(ImBruta.getRawData())) {
       finish();
       cerr << "Erro na exportacao dos dados!\n";
     }

    myt_end_exp = relogio();

    t_start = myt_start;
    t_end_cap = myt_end_cap;
    t_end_acq = myt_end_acq;
    t_end_loc = myt_end_loc;
    t_end_str = myt_end_str;
    t_end_obs = myt_end_obs;
    t_end_con = myt_end_con;
    t_end_tra = myt_end_tra;
    t_end_exp = myt_end_exp;

    //    cont_teste++;
    //    cout << cont_teste << endl;
  }
  // para os robos em caso de fim de jogo
  for( int i=0; i<3; i++ ) {
    pwm.me[i].right = 0.0;
    pwm.me[i].left  = 0.0;
  }
  transmission();
}

static double format(double x)
{
    if (x==POSITION_UNDEFINED || fabs(x)>9.999) return 9.999;
    else return x;
}

/*
void Futrobot::print_state()
{
  char buffer[6];
  //move(0,0);
  //clrtobot();

  mvaddstr( 0,0,"+--------+--------------------------+--------------------------+--------+");
  mvaddstr( 1,0,"| EQUIPE |           MEUS           |        ADVERSARIOS       |  BOLA  |");
  mvaddstr( 2,0,"| ROBO   | 0 CIAN | 1 ROSA | 2 VERD | 0 CIAN | 1 ROSA | 2 VERD |        |");
  mvaddstr( 3,0,"+--------+--------------------------+--------------------------+--------+");
  mvaddstr( 4,0,"| Pos x  | ------ | ------ | ------ | ------ | ------ | ------ | ------ |");
  mvaddstr( 5,0,"| Pos y  | ------ | ------ | ------ | ------ | ------ | ------ | ------ |");
  mvaddstr( 6,0,"| Pos th | ------ | ------ | ------ | ------ | ------ | ------ | ------ |");
  mvaddstr( 7,0,"+--------+--------------------------+--------------------------+--------+");
  mvaddstr( 8,0,"| Ref x  | ------ | ------ | ------ | ------ | ------ | ------ | ------ |");
  mvaddstr( 9,0,"| Ref y  | ------ | ------ | ------ | ------ | ------ | ------ | ------ |");
  mvaddstr(10,0,"| Ref th | ------ | ------ | ------ | ------ | ------ | ------ | ------ |");
  mvaddstr(11,0,"+--------+--------------------------+--------------------------+--------+");
  mvaddstr(12,0,"| PWM di | ------ | ------ | ------ | ------ | ------ | ------ | ------ |");
  mvaddstr(13,0,"| PWM es | ------ | ------ | ------ | ------ | ------ | ------ | ------ |");
  mvaddstr(14,0,"+--------+--------------------------+--------------------------+--------+");
  mvaddstr(15,2,  "R - Refresh");
  mvaddstr(16,2,  "T - Finish");
  mvaddstr(17,2,  "--> ");

  for (int j=0; j<3; j++) {
    sprintf(buffer,"%+6.3f",format(pos.me[j].x()));
    mvaddstr(4,11+9*j,buffer);
    sprintf(buffer,"%+6.3f",format(pos.op[j].x()));
    mvaddstr(4,38+9*j,buffer);
    sprintf(buffer,"%+6.3f",format(pos.me[j].y()));
    mvaddstr(5,11+9*j,buffer);
    sprintf(buffer,"%+6.3f",format(pos.op[j].y()));
    mvaddstr(5,38+9*j,buffer);
    sprintf(buffer,"%+6.3f",format(pos.me[j].theta()));
    mvaddstr(6,11+9*j,buffer);
    sprintf(buffer,"%+6.3f",format(pos.op[j].theta()));
    mvaddstr(6,38+9*j,buffer);
    sprintf(buffer,"%+6.3f",format(ref.me[j].x()));
    mvaddstr(8,11+9*j,buffer);
    sprintf(buffer,"%+6.3f",format(ref.me[j].y()));
    mvaddstr(9,11+9*j,buffer);
    sprintf(buffer,"%+6.3f",format(ref.me[j].theta()));
    mvaddstr(10,11+9*j,buffer);
    sprintf(buffer,"%+6.3f",format(pwm.me[j].right));
    mvaddstr(12,11+9*j,buffer);
    sprintf(buffer,"%+6.3f",format(pwm.me[j].left));
    mvaddstr(13,11+9*j,buffer);
  }
  sprintf(buffer,"%+6.3f",format(pos.ball.x()));
  mvaddstr(4,65,buffer);
  sprintf(buffer,"%+6.3f",format(pos.ball.y()));
  mvaddstr(5,65,buffer);

  mvaddstr(19,0,"\nOpções disponíveis:\n");
  mvaddstr(20,0,"P - Para im(P)rimir posições atuais\n");
  mvaddstr(21,0,"T - Para (T)erminar a partida\n");

  refresh();

}
*/

void Futrobot::print_state() const
{
  printf("+--------+--------------------------+--------------------------+--------+\n");
  printf("| EQUIPE |           MEUS           |        ADVERSARIOS       |  BOLA  |\n");
  printf("| ROBO   | 0 CIAN | 1 ROSA | 2 VERD | 0 CIAN | 1 ROSA | 2 VERD |        |\n");
  printf("+--------+--------------------------+--------------------------+--------+\n");
  printf("| Pos x  | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f |\n",
	 format(pos.me[0].x()),format(pos.me[1].x()),format(pos.me[2].x()),
	 format(pos.op[0].x()),format(pos.op[1].x()),format(pos.op[2].x()),
	 format(pos.ball.x()));
  printf("| Pos y  | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f |\n",
	 format(pos.me[0].y()),format(pos.me[1].y()),format(pos.me[2].y()),
	 format(pos.op[0].y()),format(pos.op[1].y()),format(pos.op[2].y()),
	 format(pos.ball.y()));
  printf("| Pos th | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f | ------ |\n",
	 format(pos.me[0].theta()),format(pos.me[1].theta()),format(pos.me[2].theta()),
	 format(pos.op[0].theta()),format(pos.op[1].theta()),format(pos.op[2].theta()));
  printf("+--------+--------------------------+--------------------------+--------+\n");
  printf("| Ref x  | %+6.3f | %+6.3f | %+6.3f | ------ | ------ | ------ | ------ |\n",
	 format(ref.me[0].x()),format(ref.me[1].x()),format(ref.me[2].x()));
  printf("| Ref y  | %+6.3f | %+6.3f | %+6.3f | ------ | ------ | ------ | ------ |\n",
	 format(ref.me[0].y()),format(ref.me[1].y()),format(ref.me[2].y()));
  printf("| Ref th | %+6.3f | %+6.3f | %+6.3f | ------ | ------ | ------ | ------ |\n",
	 format(ref.me[0].theta()),format(ref.me[1].theta()),format(ref.me[2].theta()));
  printf("+--------+--------------------------+--------------------------+--------+\n");
  printf("| PWM di | %+6.3f | %+6.3f | %+6.3f | ------ | ------ | ------ | ------ |\n",
	 format(pwm.me[0].right),format(pwm.me[1].right),format(pwm.me[2].right));
  printf("| PWM es | %+6.3f | %+6.3f | %+6.3f | ------ | ------ | ------ | ------ |\n",
	 format(pwm.me[0].left),format(pwm.me[1].left),format(pwm.me[2].left));
  printf("+--------+--------------------------+--------------------------+--------+\n");
  printf("| Tempo de Amostragem:  %f    | Captura:     %f       |\n",
	 dt_amostr,
	 t_end_cap - t_start);
  printf("| Aquisicao:  %f              | Obstaculos:  %f       |\n",
	 t_end_acq - t_end_cap,
	 t_end_obs - t_end_str);
  printf("| Localizacao: %f             | Controle:    %f       |\n",
	 t_end_loc - t_end_acq,
	 t_end_con - t_end_obs);
  printf("| Estrategia:  %f             | Transmissao: %f       |\n",
	 t_end_str - t_end_loc,
	 t_end_tra - t_end_con);
  printf("| Exportacao:  %f             |\n",
	 t_end_exp - t_end_tra);
  printf("+--------+--------------------------+--------------------------+--------+\n");


  cout << "Opções disponíveis:" << endl;
  cout << "P - Para im(P)rimir posições atuais" << endl;
  cout << "T - Para (T)erminar a partida" << endl;
  cout << "S - Para (S)alvar a configuracao atual" << endl;
  cout << "M - Para (M)enu de acoes." << endl;
  cout <<   "--> " << endl;

}


// bool Futrobot::exportdat() {
//   PACKAGEDAT pack;
//   pack.refer = ref;
//   pack.pwm = pwm;
//   pack.posicao = pos;
//   pack.goleiro = goleiro;
//   pack.atacante = atacante;
//   pack.zagueiro = zagueiro;
//   pack.pap = expap();
//   //INICIO REGIAO CRITICA
//   pac_comp = pack;
//   img_comp = ImBruta;
//   //FINAL REGIAO CRITICA
//   return false;
// }
