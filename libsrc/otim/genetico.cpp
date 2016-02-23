#include <stdlib.h>
#include <math.h>

#include "otim.h"
#include "myotim.h"

/* #############################################################
   ##  O método dos algoritmos genéticos
   ############################################################# */

double genetico::combina_valores(unsigned i, unsigned numsobr,
				 unsigned pai, unsigned mae)
{
  double dist=(J[mae]-J[pai])/(J[numsobr-1]-J[0]);
  double percpai,pesopai = drand48()*(1+dist);
  double percmae,pesomae = drand48()*(1-dist);
  if (pesopai!=0.0 || pesomae!=0.0)
    {
      percpai = pesopai/(pesopai+pesomae);
      percmae = pesomae/(pesopai+pesomae);
    }
  else
    {
      percpai = 0.5;
      percmae = 0.5;
    }
  return (percpai*w[pai][i] + percmae*w[mae][i]);
}

genetico::genetico(unsigned ordem, unsigned numpt)
  :otim(ordem,numpt)
{
  tmut=OTIM_MUTACAO;
  tsobrev=OTIM_SOBREVIVENCIA;
}

genetico::genetico(const genetico &b)
  :otim(b)
{
  tmut=b.tmut;
  tsobrev=b.tsobrev;
}

genetico::genetico(const poliedro &b, unsigned numpt)
  :otim(b,numpt)
{
  tmut=OTIM_MUTACAO;
  tsobrev=OTIM_SOBREVIVENCIA;
}

OTIM_STATUS genetico::fixa_constantes(double taxamut, double taxasobrev)
{
  if (taxamut<0.0 || taxamut>1.0 || taxasobrev<=0.0 || taxasobrev>=1.0)
    {
      return(otim_error_int(OTIM_PARAMETRO_INVALIDO));
    }
  tmut=taxamut;
  tsobrev=taxasobrev;
  return(OTIM_OK);
}

long genetico::calc_minimo()
{
  const unsigned numsobr=(unsigned)round(tsobrev*numpt());
  unsigned pai,mae,k,i;
  long niter=0;
  
  // Cálculo iterativo do mínimo
  while (J[numsobr-1]-J[0]>tolerancia() &&
	 niter<nummaxiter())
    {
      // Gera os filhos em substituição aos piores indivíduos
      // Só os "numsobr" melhores elementos podem ser pai ou mae
      for (k=numsobr; k<numpt(); k++)
	{
	  pai = (unsigned)floor(drand48()*numsobr);
	  mae = (unsigned)floor(drand48()*numsobr);
	  if (drand48()<tmut)  // Prefere Mutação
	    for (i=0; i<ordem(); i++)
	      {
		w[k][i] = ( drand48()>tmut ?
			    gera_valor(i) :                       // Mutação
			    combina_valores(i,numsobr,pai,mae) ); // Cruzamento
	      }
	  else  // Prefere Cruzamento
	    for (i=0; i<ordem(); i++)
	      {
		w[k][i] = ( drand48()<tmut ?
			    gera_valor(i) :                       // Mutação
			    combina_valores(i,numsobr,pai,mae) ); // Cruzamento
	      }
	  J[k] = funcao(w[k]);
	}
      
      verifica_violacao_limites();
      ordena_pontos();
      if (print() && niter%numiterprint() == 0)
	{
	  std::cout << "-------------------------------------------------\n";
	  std::cout << "Geração " << niter << '\n';
	  std::cout << "Melhor sobrevivente: " << w[0]
		    << " - Custo: " << J[0] << '\n';
	  std::cout << "Pior sobrevivente: " << w[numsobr-1]
		    << " - Custo: " << J[numsobr-1] << '\n';
	  std::cout << "-------------------------------------------------\n";
	}
      niter++;
    }
  
  // Testa se o método falhou
  if (J[numsobr-1]-J[0] > tolerancia())
    {
      return(OTIM_NAO_CONVERGIU);
    }
  return(niter);
}
