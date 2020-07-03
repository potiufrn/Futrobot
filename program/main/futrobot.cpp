#include <iostream>
#include <fstream>
#include "futrobot.h"
#include <sys/time.h>

#include <system_tools.h>

using namespace std;

inline void* management2( void *x )
{
  ((Futrobot*)x)->management();
  cout << "Saiu do management2." << endl;
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

  while (gameState() != FINISH_STATE) {
    // Espera por uma nova imagem

    if (gameState() != FINISH_STATE && acquisitionWait()) { //VERIFICAR USO DO AND OU OR
      finish();
      cerr << "Erro na espera por nova imagem!\n";
    }
    myt_start = relogio();
    dt_amostr = myt_start - t_start;
    //teste para evitar divisoes por zero.
    if(dt_amostr == 0.0) dt_amostr = 1.0/FPS;
    // Lẽ a nova imagem

    // printf("\n **** acquisitionCapture... **** \n");
    if (gameState() != FINISH_STATE && acquisitionCapture()){
      finish();
      cerr << "Erro na leitura da nova imagem!\n";
    }

    id_pos++;
    myt_end_cap = relogio();
    // Fornece a pose dos robos e a posicao da
    // bola em coordenadas de mundo.
    // printf("\n **** acquisition... **** \n");
    if (gameState() != FINISH_STATE && acquisition()){
      finish();
      cerr << "Erro no processamento da imagem!\n";
    }
    
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
    if (gameState() != FINISH_STATE && control()){
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

    // Modulo de exportacao dos dados
    if (gameState() != FINISH_STATE && exxport() ) {
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
  }
  // para os robos em caso de fim de jogo
  for( int i=0; i<3; i++ ){
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