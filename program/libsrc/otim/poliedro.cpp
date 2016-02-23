#include <math.h>

#include "otim.h"
#include "myotim.h"

static inline double pow2(double x){return x*x;}

/* #############################################################
   ##  O método dos poliedros flexíveis
   ############################################################# */

poliedro::poliedro(unsigned ordem)
  :otim(ordem,ordem+1)
{
  crefl = OTIM_REFLEXAO;
  cexp = OTIM_EXPANSAO;
  cred = OTIM_REDUCAO;
  ccontr = OTIM_CONTRACAO;
}

poliedro::poliedro(const poliedro &b)
  :otim(b)
{
  crefl = b.crefl;
  cexp = b.cexp;
  cred = b.cred;
  ccontr = b.ccontr;
}

poliedro::poliedro(const genetico &b)
  :otim(b,b.ordem()+1)
{
  crefl = OTIM_REFLEXAO;
  cexp = OTIM_EXPANSAO;
  cred = OTIM_REDUCAO;
  ccontr = OTIM_CONTRACAO;
}

OTIM_STATUS poliedro::fixa_constantes(double creflexao, double cexpansao,
				    double creducao, double ccontracao)
{
  if (creflexao<=0.0 || cexpansao<1.0 || creducao>=1.0 || ccontracao>=1.0)
    {
      return(otim_error_int(OTIM_PARAMETRO_INVALIDO));
    }
  crefl = creflexao;
  cexp = cexpansao;
  cred = creducao;
  ccontr = ccontracao;
  return(OTIM_OK);
}

long poliedro::calc_minimo()
{
  double P,JwR;
  fvector c(ordem()),wR(ordem());
  unsigned k;
  long niter=0;

  // Cálculo do "maior lado" do poliedro (critério de parada)
  P = 0.0;
  for (k=1; k<numpt(); k++)
    {
      double prov = euclid(w[k]-w[0]);
      if (prov > P) P=prov;
    }

  // Cálculo iterativo do mínimo
  while (P>tolerancia() &&
	 niter<nummaxiter())
    {

      // Cálculo do centróide da face oposta ao pior ponto
      c = nullvector(ordem());
      for (k=0; k<numpt()-1; k++)
	{
	  c += w[k];
	}
      c /= numpt()-1;

      // Cálculo do ponto de reflexão
      wR  = c + crefl*(c-w[k]);
      JwR = funcao(wR);

      if (JwR < J[0])
	{
	  // Expansão (o ponto de reflexão é melhor que o melhor atual)
	  // Anda ainda mais na direção da reflexão e substitui o
	  // pior ponto por este novo ponto
	  w[numpt()-1] = c + cexp*(wR-c);
	  J[numpt()-1] = funcao(w[numpt()-1]);
	}
      else if (JwR >= J[numpt()-1])
	{
	  // Redução (o ponto de reflexão é pior que o pior atual)
	  // Move todos os pontos (exceto o melhor) para mais próximo
	  // do melhor ponto
	  for (k=1; k<numpt(); k++)
	    {
	      w[k] = w[0] + cred*(w[k]-w[0]);
	      J[k] = funcao(w[k]);
	    }
	}
      else if (JwR > J[numpt()-2])
	{
	  // Contração (o ponto de reflexão é pior do que todos,
	  // com exceção do pior atual)
	  // Move o pior ponto para mais próximo do centróide
	  w[numpt()-1] = c + ccontr*(w[numpt()-1]-c);
	  J[numpt()-1] = funcao(w[numpt()-1]);
	}
      else
	{
	  // Reflexão
	  // Substitui o pior ponto pelo ponto de reflexão
	  w[numpt()-1] = wR;
	  J[numpt()-1] = JwR;
	}
      
      verifica_violacao_limites();
      ordena_pontos();
      // Cálculo do "maior lado" do poliedro (critério de parada)
      P = 0.0;
      for (k=1; k<numpt(); k++)
	{
	  double prov = euclid(w[k]-w[0]);
	  if (prov > P) P=prov;
	}
      if (print() && niter%numiterprint() == 0)
	{
	  std::cout << "-------------------------------------------------\n";
	  std::cout << "Iteração " << niter << '\n';
	  std::cout << "Melhor ponto: " << w[0]
		    << " - Custo: " << J[0] << '\n';
	  std::cout << "Pior ponto: " << w[numpt()-1]
		    << " - Custo: " << J[numpt()-1] << '\n';
	  std::cout << "-------------------------------------------------\n";
	}
      niter++;
    }

  // Testa se o método falhou
  if (P > tolerancia())
    {
      return(OTIM_NAO_CONVERGIU);
    }
  return(niter);
}
