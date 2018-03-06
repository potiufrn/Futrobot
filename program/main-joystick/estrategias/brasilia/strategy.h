#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include "futdata.h"

// Possíveis papeis dos jogadores
enum PAPEL_JOGADOR {
  ESTACIONAR,
  CONTORNAR, ALINHAR, GIRAR, CHUTAR_GOL,
  ALINHAR_PAREDE, DESCOLAR_PAREDE,      // Atacante
  DESCOLAR_QUINA,
  ESPERA_UP, ESPERA_DOWN, ZAGUEIRO,    // Defensor
  GOLEIRO, TIRAR_DO_GOL                // Goleiro
};

struct PAPEIS {
  PAPEL_JOGADOR me[3];
};

struct RPAPEL {
  bool estacionar, contornar, alinhar, girar, chutar_gol, alinhar_parede, 
    descolar_parede, descolar_quina, espera_up, espera_down, zagueiro, goleiro, tirar_do_gol;
};


/* class Rpapel  { */
/*  private: */
/*   bool estacionar, contornar, alinhar, girar;  */
/*   bool chutar_gol, alinhar_parede, descolar_parede, descolar_quina; */
/*   bool  espera_up, espera_down, zagueiro, goleiro, tirar_do_gol; */
/* } */

/* class Planejador { */

/* } */


class Strategy : public virtual FutData
{
private:
  double t;

  //  bool bloqueado[3];
  int contador_parado[3];
  int atacante;
  int zagueiro;
  int goleiro;
  PAPEIS papeis;
  int sinal;
   
  PAPEL_JOGADOR resp_atacante[256];
  void preenchimento();
 private: 
  RPAPEL plano_atacante (bool bola_perigosa, bool atras_bola, bool pode_ali, bool pode_chutar, bool fora_r_permit, bool precisa_descolar, bool precisa_desc_quina);

public:
  Strategy(TEAM time, SIDE lado, GAME_MODE gameMode);
  ~Strategy();
  bool strategy();

  void goleiro_floripa(int id);
  void goleiro_brasilia(int id);
  void atacante_floripa(int id);
  void defensor_floripa(int id, int atacante, int goleiro);
};

#endif
