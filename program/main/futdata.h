#ifndef _FUTDATA_H_
#define _FUTDATA_H_

#include "../data.h"
#include <ssocket.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <camera.h>

// Todos os dados do jogo
class FutData{
private:
  TEAM my_team;
  SIDE my_side;
  GAME_MODE game_mode;
  GAME_STATE game_state;
  bool advantage;
  int shmid;
  CONFIG * mem;
protected:
  std::string _multicast_address;
  std::string _server_address;
  unsigned int _vision_port;
  unsigned int _command_port;
  unsigned int _referee_port;
  unsigned int _replacer_port;
  // socket para enviar comandos ao servidor
  udpSocket sock_cmd;
  // socket para recepção de dados do servidor
  udpSocket sock_vision;
  udpSocket sock_referee;
  udpSocket sock_replacer;

  IDQUADRO id_pos;
  Coord2 pointsImg[NUM_POINTS];  //Limites do campo
  CONFIG ant; //Armazena a posicao calculada na iteracao anterior,
	      //necessaria para certos calculos do programa
  CONFIG pos; //Armazena a pose dos robos e a posicao da bola em
	      //coordenadas de mundo.
  ROLES papeis; //Armazena os papeis do robo.
  bool bypassControl[3];
  bool bloqueado[3];
  REFERENCES ref; //Contem a referencia futura dos meus robos.
  PWM_ROBOTS pwm;  //Contem o valor de PWM a ser transmitido para os robos.

  PARAM_CONTROLE logControle[3];  // armazena as variáveis internas do controlador

  double dt_amostr;
public:
  FutData(TEAM team, SIDE side, GAME_MODE mode);
  inline void setTeam(TEAM newTeam) {my_team = newTeam;}
  inline TEAM myTeam() const {return my_team;}
  inline void setSide(SIDE newSide) {my_side = newSide;}
  inline SIDE mySide() const {return my_side;}
  inline GAME_MODE gameMode() const {return game_mode;}
  inline GAME_STATE gameState() const {return game_state;}
  inline int goleiro() const { return (papeis.me[0].funcao==GOLEIRO ? 0 : (papeis.me[1].funcao==GOLEIRO?1:2) ); }
  inline int com_bola() const { return (papeis.me[0].funcao==COM_BOLA ? 0 : (papeis.me[1].funcao==COM_BOLA?1:2) ); }
  inline int sem_bola() const { return (papeis.me[0].funcao==SEM_BOLA ? 0 : (papeis.me[1].funcao==SEM_BOLA?1:2) ); }

  inline bool getAdvantage() const {return advantage;}
  inline void setGameState(GAME_STATE new_state){game_state = new_state;}
  inline void setAdvantage(bool new_advantage){advantage = new_advantage;}
  inline void finish() {game_state = FINISH_STATE;}
  void start_data(TEAM, SIDE, GAME_MODE);
};

#endif
