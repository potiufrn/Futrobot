#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "otim.h"
#include "myotim.h"

// Funções auxiliares

static double rnd(double med, double desv)
{
  double soma = 0.0;
  for (unsigned int i=0; i<12; i++)
    {
      soma += drand48();
    }
  soma -= 6.0;
  return(desv*soma+med);
}

static inline double entre(double max, double min)
{
  return(min+(max-min)*drand48());
}

/* #############################################################
   ##  A classe virtual
   ############################################################# */

OTIM_STATUS otim::create(unsigned ordem, unsigned numpt)
{
  if (ordem==0 || numpt==0)
    {
      ord=0;
      npt=0;
      return(otim_error_int(OTIM_PARAMETRO_INVALIDO));
    }
  ord = ordem;
  npt = numpt;
  lim = new OTIM_LIMIT[ord];
  //w = new vvector[npt](ord);
  w = new vvector[npt];
  for (unsigned i=0; i<npt; i++) {
    w[i] = vvector(ord);
  }
  J = new double[npt];
  if (w==NULL || lim==NULL || J==NULL)
    {
      ord=0;
      npt=0;
      return(otim_error_int(OTIM_ALOCACAO_MEMORIA));
    }
  return(OTIM_OK);
}

double otim::gera_valor(unsigned i) const
{
  return ( lim[i].gauss ?
	   rnd(lim[i].med,lim[i].desv) :
	   entre(lim[i].max,lim[i].min) );
}

void otim::gera_valores_iniciais(unsigned i)
{
  for (unsigned k=0; k<npt; k++)
    {
      w[k][i] = gera_valor(i);
    }
}

// Verifica se os pontos não estão violando os limites
void otim::verifica_violacao_limites(void)
{
  unsigned i,k;
  for (i=0; i<ord; i++) if (!limite(i).gauss)
    {
      for (k=0; k<npt; k++)
	{
	  if (w[k][i] < lim[i].min)
	    {
	      w[k][i] = lim[i].min;
	      J[k] = funcao(w[k]);
	    }
	  else if (w[k][i] > lim[i].max)
	    {
	      w[k][i] = lim[i].max;
	      J[k] = funcao(w[k]);
	    }
	}
    }
}
  
// Coloca os pontos em ordem crescente de custo J[]
// w[0][] = melhor ponto (menor J); w[npt-1][] = pior ponto (maior J)
// Método "shell" de ordenamento
void otim::ordena_pontos(void)
{
  unsigned i,j,inc=1;
  double ultJ;
  vvector ultw;

  while (inc < npt)
    {
      inc *= 3;
      inc++;
    }
  while (inc > 1)
    {
      inc /= 3;
      for (i=inc; i<npt; i++)
	{
	  ultJ = J[i];
	  ultw.permutewith(w[i]);
	  j = i;
	  while (j>=inc && J[j-inc]>ultJ)
	    {
	      J[j] = J[j-inc];
	      w[j].permutewith(w[j-inc]);
	      j -= inc;
	    }
	  J[j] = ultJ;
	  w[j].permutewith(ultw);
	}
    }
}

// CONSTRUTORES

otim::otim(unsigned ordem, unsigned numpt)
{
  srand48(time(NULL));
  if (!create(ordem,numpt))
    {
      for (unsigned i=0; i<ord; i++)
	{
	  lim[i].gauss = true;
	  lim[i].med = OTIM_MEDIA;
	  lim[i].desv = OTIM_DESVIO;
	}
      gera_pontos_iniciais();
      nmaxiter = OTIM_ITERACOES;
      niterprint = -1;
      tol = OTIM_TOLERANCIA;
    }
}

otim::otim(const otim &b)
{
  if (!create(b.ord,b.npt))
    {
      for (unsigned i=0; i<ord; i++)
	{
	  lim[i].gauss = b.lim[i].gauss;
	  if (lim[i].gauss)
	    {
	      lim[i].med = b.lim[i].med;
	      lim[i].desv = b.lim[i].desv;
	    }
	  else
	    {
	      lim[i].max = b.lim[i].max;
	      lim[i].min = b.lim[i].min;
	    }
	}
      for (unsigned k=0; k<npt; k++)
	{
	  w[k] = b.w[k];
	  J[k] = b.J[k];
	}
      nmaxiter = b.nmaxiter;
      niterprint = b.niterprint;
      tol = b.tol;
    }
}

otim::otim(const otim &b, unsigned numpt)
{
  if (!create(b.ord,numpt))
    {
      for (unsigned i=0; i<ord; i++)
	{
	  lim[i].gauss = b.lim[i].gauss;
	  if (lim[i].gauss)
	    {
	      lim[i].med = b.lim[i].med;
	      lim[i].desv = b.lim[i].desv;
	    }
	  else
	    {
	      lim[i].max = b.lim[i].max;
	      lim[i].min = b.lim[i].min;
	    }
	}
      if (npt > b.npt)
	{
	  // Tem que "inventar" novos pontos
	  gera_pontos_iniciais();
	  // Copia os pontos antigos sobre alguns dos novos
	  for (unsigned k=0; k<b.npt; k++)
	    {
	      w[k] = b.w[k];
	      J[k] = b.J[k];
	    }
	}
      else
	{
	  // Copia os primeiros pontos antigos para os novos
	  for (unsigned k=0; k<npt; k++)
	    {
	      w[k] = b.w[k];
	      J[k] = b.J[k];
	    }
	}
      nmaxiter = b.nmaxiter;
      niterprint = b.niterprint;
      tol = b.tol;
    }
}

otim::~otim()
{
  delete lim;
  delete J;
  delete [] w;
}

// Métodos

void otim::gera_pontos_iniciais(void)
{
  for (unsigned i=0; i<ord; i++)
    {
      gera_valores_iniciais(i);
    }
}

OTIM_STATUS otim::fixa_criterios_parada(long nummaxiter, double tolerancia)
{
  if (nummaxiter<=0 || tolerancia<=0.0)
    {
      return(otim_error_int(OTIM_PARAMETRO_INVALIDO));
    }
  nmaxiter=nummaxiter;
  tol=tolerancia;
  return(OTIM_OK);
}

OTIM_STATUS otim::fixa_ponto_inicial(unsigned k, const fvector &x)
{
  if (ord != x.order())
    {
      return(otim_error_int(OTIM_DIMENSAO_INVALIDA));
    }
  if (k>=npt)
    {
      return(otim_error_int(OTIM_INDICE_INVALIDO));
    }
  w[k] = x;
  return(OTIM_OK);
}

OTIM_STATUS otim::fixa_momentos(unsigned i, double media, double desvio)
{
  if (i>=ord)
    {
      return(otim_error_int(OTIM_INDICE_INVALIDO));
    }
  lim[i].gauss = true;
  lim[i].med = media;
  lim[i].desv = fabs(desvio);
  gera_valores_iniciais(i);
  return(OTIM_OK);
}

OTIM_STATUS otim::fixa_limites(unsigned i, double max, double min)
{
  if (i>=ord)
    {
      return(otim_error_int(OTIM_INDICE_INVALIDO));
    }
  if (max<min)
    {
      double prov=min;
      min=max;
      max=prov;
    }
  lim[i].gauss = false;
  lim[i].max = max;
  lim[i].min = min;
  gera_valores_iniciais(i);
  return(OTIM_OK);
}

OTIM_STATUS otim::imprime(bool impr, long niter)
{
  if (!impr)
    {
      niterprint = -1;
      return(OTIM_OK);
    }
  if (niter <= 0)
    {
      return(otim_error_int(OTIM_PARAMETRO_INVALIDO));
    }
  niterprint = niter;
  return(OTIM_OK);
}

long otim::calcula_minimo(fvector &min)
{
  if (ord != min.order())
    {
      return(otim_error_int(OTIM_DIMENSAO_INVALIDA));
    }
  for (unsigned k=0; k<numpt(); k++)
    {
      J[k] = funcao(w[k]);
    }
  verifica_violacao_limites();
  ordena_pontos();
  if (print())
    {
      std::cout << "MINIMIZAÇÃO INICIADA\n";
    }
  long numiter = calc_minimo();
  // Calcula o ponto de mínimo como sendo o melhor ponto
  if (print())
    {
      std::cout << "Mínimo: " << w[0]
		<< " Custo: " << J[0] << '\n';
    }
  min = w[0];
  return(numiter);
}
  
