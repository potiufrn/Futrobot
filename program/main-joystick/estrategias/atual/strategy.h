#ifndef _STRATEGY_H_
#define _STRATEGY_H_


#include "futdata.h"

#define TSIZE_ZAG 1024
#define TSIZE_ATA 512
#define TSIZE_GOL 2

/* // Possíveis papeis dos jogadores */
/* enum PAPEL_JOGADOR { */
/*   NAO_DEFINIDO, */
/*   IMPOSSIVEL, */
/*   ESTACIONAR, POSICIONAR, IR_BOLA, DESVIAR, */
/*   CONTORNAR, ALINHAR, GIRAR, CHUTAR_GOL, NOVO_ALIN, */
/*   ALINHAR_PAREDE, DESCOLAR_PAREDE, DESCOLAR_QUINA, // Atacante */
/*   POSICIONAR_ZAGA, DEFENDER, TOCAR_BOLA, */
/*   TIRAR_BOLA, IR_BOLA_ZAG, TIRAR_BOLA_MEIO, SAIR_DA_AREA, CONTORNAR_ZAG, POSICIONAR_RETIRADA,  */
/*   LEVAR_BOLA, POSICIONAR_CHUTE, RETIRAR_BOLA_PAREDE, // Defensor */
/*   GOLEIRO, TIRAR_DO_GOL,                // Goleiro */
/*   IR_CENTRO */
/* }; */

/* struct PAPEIS { */
/*   PAPEL_JOGADOR me[3]; */
/* }; */

struct RPAPEL {
  bool estacionar, contornar, alinhar, girar, chutar_gol, alinhar_parede, posicionar, ir_bola, desviar,
    descolar_parede, descolar_quina, posicionar_zaga, defender, tocar_bola, tirar_bola, ir_bola_zag, 
    contornar_zag, levar_bola, posicionar_chute, retirar_bola_parede, goleiro, tirar_do_gol;
};



class Strategy : public virtual FutData
{
private:
  double t;


  //  bool bloqueado[3];
  int contador_parado[3];

  //Foi passado para o futdata:
  //para o localization e o expor utilizarem
  //  int atacante;
  //  int zagueiro;
  //  int goleiro;
 

 PAPEIS papeis;
  int sinal;
   
  PAPEL_JOGADOR resp_atacante[512];
  PAPEL_JOGADOR resp_zagueiro[1024];

  void preenchimento();

  // POSICOES ANTIGAS (2 segundos atras) 
  // para caracterizar travamentos
  POS_ROBO pos_antiga_ata;
  POS_ROBO pos_antiga_zag;
  POS_ROBO ant_antiga_gol;
  double tata;
  double tzag;
  double tgol;

 private: 
  RPAPEL plano_atacante (bool bola_na_area_adv, bool bola_no_ataque, bool bola_na_parede, bool bola_comigo, 
			 bool bola_com_adv, bool atras_da_bola, bool trajeto_livre, bool pode_posicionar);

  /*RPAPEL plano_zagueiro (bool bola_na_area, bool bola_na_zaga, bool bola_na_parede, bool alinhado, 
			 bool bola_comigo,  bool bola_com_adv, bool caminho_livre, bool atras_da_bola, 
			 bool pode_ficar_atras, bool bola_vindo);
  */
  bool setint (int id, const char * binario, PAPEL_JOGADOR resp, bool silence);
  bool full_table (int id);

public:
  Strategy(TEAM time, SIDE lado, GAME_MODE gameMode);
  ~Strategy();
  bool strategy();

  void goleiro_floripa(int id);
  void goleiro_brasilia(int id);
  void atacante_floripa(int id);
  void atacante_tradi(int id); 
  void defensor_floripa(int id, int atacante, int goleiro);
  void novo_atacante(int id);
  PAPEIS * expap (){return &papeis;} 
};

#endif
