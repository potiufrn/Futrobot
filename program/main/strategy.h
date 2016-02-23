#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include "futdata.h"

#define TSIZE_ZAG 1024
#define TSIZE_ATA 512
#define TSIZE_GOL 2

class Strategy : public virtual FutData
{
private:
  POS_ROBO ref_desc[3];

  double t;
  
  double yalin[3], ypos_fut[3], thetapos_fut[3], dang_desc[3], dlin_desc[3];

  //predicados para os adversarios
  bool adv_na_area[3], adv_mais_prox_bola[3];

  double dist_meu[3], dist_adv[3];

  //a principio os booleanos dos jogadores tem uso especifico, mas mesmo assim foram criados para os tres jogadores 

  //booleanos do predicados do goleiro
  bool meu_lado_area[3], bola_frente_x_do_meu[3];

  //booleanos para ambos 
  bool meu_na_area[3];

  //booleanos do predicado com bola
  bool meu_na_area_hist[3], meu_na_frente_bola[3], meu_na_frente_bola_hist[3], 
     meu_posicionado_descolar[3], meu_furou[3], meu_frente_area[3], 
     meu_alinhado_chutar[3], meu_bem_alinhado_chutar[3], bola_frente_y_do_meu[3],
     bola_frente_y_do_meu_hist[3], meu_posicionado_isolar[3], meu_posicionado_isolar_hist[3], 
     meu_alinhado_isolar[3], meu_bem_alinhado_isolar[3];

  // Analise da situacao da bola
  bool bola_parada, bola_no_ataque, bola_minha_area_lateral;

  // So um desses predicados pode ser true ao mesmo tempo
  bool bola_dentro_gol, bola_area_defesa, bola_frente_area, bola_lado_area,
    bola_parede_fundo, bola_quina_fundo, bola_fundo, bola_parede_lateral,
    bola_lateral, bola_quina_frente, bola_parede_frente, bola_frente,
    bola_regiao_central;

  // Para determinar jogadores bloqueados
  unsigned contador_parado[3];

  // Para determinar jogadores perdidos
  unsigned contador_perdido[3];
  bool perdido[3];

  // Lado do campo
  int sinal;

  //ACTION resp_com_bola[512];
  //ACTION resp_sem_bola[1024];

  //void preenchimento();

  int estado_penalty;
 private: 
  //bool setint (FUNCTION f, const char * binario, ACTION resp, bool silence);
  //bool full_table (FUNCTION f);
  void analisa_jogadores();
  void analisa_adversarios();
  void analisa_bola();
  PWM_WHEEL descolar_parede(int id);
  POS_ROBO posicao_para_descolar_bola();
  POS_ROBO calcula_origem_parede();
  void detecta_bloqueados();
  void escolhe_funcoes();
  void acao_goleiro(int id);
  void acao_goleiro_play(int id);
  void acao_com_bola(int id); 
  void acao_com_bola_play(int id);
  void acao_sem_bola(int id);
  void calcula_referencias(int id);
public:
  Strategy(TEAM time, SIDE lado, GAME_MODE gameMode);
  ~Strategy();
  bool strategy();

};

#endif
