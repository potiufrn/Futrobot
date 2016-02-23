#ifndef _OTIM_H_
#define _OTIM_H_

#include <matrix.h>

// Os tipos de erro

typedef enum
{
  OTIM_OK=0,
  OTIM_ALOCACAO_MEMORIA=-1,
  OTIM_PARAMETRO_INVALIDO=-2,
  OTIM_DIMENSAO_INVALIDA=-3,
  OTIM_INDICE_INVALIDO=-4,
  OTIM_NAO_CONVERGIU=-5
} OTIM_STATUS;

// Os limites de um parâmetro

typedef struct
{
  bool gauss;
  union
  {
    double med;
    double max;
  };
  union
  {
    double desv;
    double min;
  };
} OTIM_LIMIT;

// Variáveis globais

extern void (*otim_error)(OTIM_STATUS err, const char *msg);

// Valores por default das constantes para todos os otimizadores

#define OTIM_MEDIA 0.0
#define OTIM_DESVIO 1.0
#define OTIM_ITERACOES 1000
#define OTIM_TOLERANCIA 0.001

/* #############################################################
   ##  A classe virtual                                       ##
   ############################################################# */

class otim
{
 private:
  // Dados
  OTIM_LIMIT *lim;
  unsigned ord;
  unsigned npt;
  long nmaxiter;
  long niterprint;
  double tol;
  // Funções
  OTIM_STATUS create(unsigned ordem, unsigned numpt);
  void gera_valores_iniciais(unsigned i);
 protected:
  vvector *w;
  double *J;
  // Funções
  virtual double funcao(const fvector &x) const = 0;
  virtual long calc_minimo() = 0;
  inline OTIM_LIMIT limite(unsigned i) const {return lim[i];}
  inline bool print() const {return niterprint>0;}
  inline long numiterprint() const {return niterprint;}
  double gera_valor(unsigned i) const;
  void verifica_violacao_limites(void);
  void ordena_pontos(void);
  otim(unsigned ordem, unsigned numpt);
  otim(const otim &b);
  otim(const otim &b, unsigned numpt);
 public:
  virtual ~otim();
  inline unsigned ordem() const {return ord;}
  inline unsigned numpt() const {return npt;}
  inline long nummaxiter() const {return nmaxiter;}
  inline double tolerancia() const {return tol;}
  void gera_pontos_iniciais(void);
  OTIM_STATUS fixa_criterios_parada(long nummaxiter=OTIM_ITERACOES,
				    double tolerancia=OTIM_TOLERANCIA);
  OTIM_STATUS fixa_ponto_inicial(unsigned k, const fvector &x);
  OTIM_STATUS fixa_momentos(unsigned i, double media, double desvio);
  OTIM_STATUS fixa_limites(unsigned i, double max, double min);
  OTIM_STATUS imprime(bool impr, long niter=1);
  long calcula_minimo(fvector &min);
};

class poliedro;
class genetico;

/* #############################################################
   ##  O método dos poliedros flexíveis
   ############################################################# */

#define OTIM_REFLEXAO 1.0    //OK: usualmente 1
#define OTIM_EXPANSAO 1.5    //OK: >1
#define OTIM_REDUCAO 0.75    //OK: <1
#define OTIM_CONTRACAO 0.75  //OK: <1

class poliedro: public otim
{
 private:
  double crefl, cexp, cred, ccontr;
  long calc_minimo();
 public:
  poliedro(unsigned ordem);
  poliedro(const poliedro &b);
  poliedro(const genetico &b);
  OTIM_STATUS fixa_constantes(double creflexao=OTIM_REFLEXAO,
			      double cexpansao=OTIM_EXPANSAO,
			      double creducao=OTIM_REDUCAO,
			      double ccontracao=OTIM_CONTRACAO);
};

/* #############################################################
   ##  O método dos algoritmos genéticos
   ############################################################# */

#define OTIM_POPULACAO 100
#define OTIM_MUTACAO 0.1
#define OTIM_SOBREVIVENCIA 0.5

class genetico: public otim
{
 private:
  double tmut,tsobrev;
  double combina_valores(unsigned i, unsigned numsobr,
			 unsigned pai, unsigned mae);
  long calc_minimo();
 public:
  genetico(unsigned ordem, unsigned numpt=OTIM_POPULACAO);
  genetico(const genetico &b);
  genetico(const poliedro &b, unsigned numpt=OTIM_POPULACAO);
  OTIM_STATUS fixa_constantes(double taxamut=OTIM_MUTACAO,
			      double taxasobrev=OTIM_SOBREVIVENCIA);
};

#endif
