#include <cstdlib>
#include <iostream>

#include "futrobot.h"
#include "../system.h"

using namespace::std;

void myErrorSocket(SOCKET_STATUS err, const char *msg){
  //cerr << "Erro libsocket " << err << ": " << msg << endl;
}

void print_menu_botoes(){
  cout << "Opcoes: "<<endl;
  cout << "1 - PAUSE"<<endl;
  cout << "2 - PENALTY"<<endl;
  cout << "3 - FREEKICK"<<endl;
  cout << "4 - GOALKICK"<<endl;
  cout << "5 - FREEBALL"<<endl;
  cout << "6 - INICIAL POSITION"<<endl;
  cout << "7 - CELEBRATE"<<endl;
  cout << "8 - PLAY"<<endl;
  cout << ">> ";

}

int main( int argc, char **argv )
{

  TEAM myTeam;
  SIDE mySide;
  GAME_MODE mode;

  char tecla, tecla1;

  socket_error = myErrorSocket;

  cout << "**************************************\n";
  cout << "*           FUTEBOL DE ROBOS         *\n";
  cout << "**************************************\n";


#ifndef _SO_SIMULADO_
  cout << "\nOpções disponíveis:\n";
  cout << "1 - Jogo real!\n";
  cout << "2 - Jogo simulado!\n";
  
  do {
    cout << "SUA OPÇÃO: ";
    cin >> tecla;
  }while(tecla!='1' && tecla!='2');
  cout << endl;

  if (tecla=='1') {
    mode=REAL_MODE;
  }
  else {
    mode=SIMULATED_MODE;
  }
#else
  mode=SIMULATED_MODE;
#endif

  cout << "\nOpções disponíveis:\n";
  cout << "1 - Uniforme azul!\n";
  cout << "2 - Uniforme amarelo!\n";
  cout << "SUA OPÇÃO: ";
  
  do {
    cin >> tecla;
  }while(tecla != '1' && tecla!='2');
  cout << endl;

  if (tecla=='1') {
    myTeam=BLUE_TEAM;
  }
  else {
    myTeam=YELLOW_TEAM;
  }

  cout << "\nOpções disponíveis:\n";
  cout << "1 - Lado esquerdo!\n";
  cout << "2 - Lado direito!\n";
  cout << "SUA OPÇÃO: ";
  
  do {
    cin >> tecla;
  }while(tecla!='1' && tecla!='2');
  cout << endl;

  if (tecla=='1') {
    mySide=LEFT_SIDE;
  }
  else {
    mySide=RIGHT_SIDE;
  }
  
  // Na criação desta variável, o construtor lança a thread que faz o
  // controle do sistema propriamente dito
  Futrobot fut(myTeam,mySide,mode);
  
  //le arquivo de calibracao
  if(mode == SIMULATED_MODE){
    char ip[80];
    cout << "Informe a maquina do simulador (ip ou nome): ";
    cin >> ip;
    if(fut.configAcquisition(ip)){
      cout << "Erro ao conectar com o simulador\n";
      exit(1);
    }
  }
  else{
    if(fut.configAcquisition("../../etc/calibracao.cal")){
      cout << "Erro ao ler os arquivos de calibracao\n";
      exit(1);
    }
  }
 
  fut.setGameState(PAUSE_STATE);
  if(fut.start_management()){
    cout<< "Problema ao iniciar a thread de gerenciamento.\n";
    exit(1);
  }

  //  fut.print_state();
  while (fut.gameState() != FINISH_STATE) {
    //imprime a tabela e as opcoes
    do {
      fut.print_state();
      do {
        cin >> tecla;
      }
      while (tecla==EOF && fut.gameState()!=FINISH_STATE);
      
      if (fut.gameState()==FINISH_STATE) {
        tecla = 'T';
        cout << tecla;
      }
      else {
        tecla = toupper(tecla);
      }
      cout << '\n';
    }
    while (tecla != 'T' && tecla != 'P' && tecla != 'S' && tecla != 'M');
    
    switch (tecla) {
    case 'T':
      fut.finish();
      break;
    case 'P':
      fut.print_state();
      break;
    case 'S':
      fut.save_image();
      break;
    case 'M':
      print_menu_botoes();
      do {
        cin >> tecla1;
      }
      while (tecla1==EOF && fut.gameState()!=FINISH_STATE);
      tecla1 = toupper(tecla1);
      switch(tecla1){
      case '1':
	fut.setGameState(PAUSE_STATE);
	break;
      case '2':
	fut.setGameState(PENALTY_STATE);
	break;
      case '3':
	fut.setGameState(FREEKICK_STATE);
	break;
      case '4':
	fut.setGameState(GOALKICK_STATE);
	break;
      case '5':
	fut.setGameState(FREEBALL_STATE);
	break;
      case '6':
	fut.setGameState(INICIALPOSITION_STATE);
	break;
      case '7':
	fut.setGameState(CELEBRATION_STATE);
	break;
      case '8':
	fut.setGameState(PLAY_STATE);
	break;
      default:
	cout <<"Funcao Invalida." << endl;
	break;
      }
      break;
    }
  }
  fut.finish_management();
  return(0);
}

