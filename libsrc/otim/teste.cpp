#include <iostream>
#include <sys/time.h>
#include <matrix.h>
#include "otim.h"

/*Este é um programa de teste que ilustra a utilização das classes de
  otimização (determinação de pontos de mínimo) de funções numéricas
  multivariáveis. Há uma classe para cada método de otimização, todas
  capazes de lidar com problemas de qualquer ordem. A ordem de um
  problema de otimização multivariável é o número de parâmetros da
  função a minimizar.*/

/*A classe "poliedro" implementa o método dos poliedros flexíveis. O
  método dos poliedros flexíveis cria um poliedro cujo número de
  vértices é igual à ordem do problema + 1 e calcula o valor da função
  para cada um dos vértices. A cada passo, o pior vértice é refletido
  em relação à face formada pelos outros vértices (processo de
  REFLEXÃO) e o valor da função é calculado para este novo ponto. Se o
  valor for muito bom (ou seja, menor do que todos os anteriores),
  anda-se mais um pouco nesta direção e o poliedro aumenta de tamanho
  (processo de EXPANSÃO). Se o ponto for ruim, o poliedro diminui de
  tamanho (processos de REDUÇÃO ou CONTRAÇÃO). Se o ponto for nem
  muito bom nem muito ruim, apenas a reflexão é feita.*/

/*A classe "genetico" implementa o método dos algoritmos genéticos.
  Neste método, é gerado inicialmente um certo número ("numpt") de
  conjuntos de valores para os parâmetros da função, com cada conjunto
  formando um indivíduo. Todos os indivíduos formam a população. Para
  cada indivíduo é calculado o valor da função a ser minimizada. A
  cada iteração (ou geração) um percentual ("taxasobrev") dos
  indivíduos é escolhido para permanecer (ou sobreviver) na população,
  sendo os demais substituídos por novos indivíduos gerados a partir
  de combinações de dois dos sobreviventes (conhecidos como pais). Os
  indivíduos eliminados a cada geração são os que têm o valor da
  função mais altos. Os parâmetros dos novos indivíduos normalmente
  são gerados a partir da combinação dos valores dos parâmetros
  correspondentes nos pais, ponderados probabilisticamente a partir do
  valor da função de cada um dos pais. Um percentual dos parâmetros
  ("taxamut") é gerada não por combinação, mas de maneira aleatória.*/

/*O primeiro passo para utilização das classes é se definir a função a
  ser minimizada. No exemplo, trata-se da função "g".*/
/*A função deve obrigatoriamente retornar um "double" e receber como
  parâmetro de entrada um ponteiro para "const double". No exemplo, a
  função tem dois parâmetros de entrada, que correspondem aos dois
  elementos do vetor para onde o ponteiro aponta. A função não deve
  alterar o valor destes parâmetros.*/
/*Deve ser criada uma classe derivada da classe genetico ou da classe
  poliedro que implementa a função a ser minimizada no metodo
  funcao(). No exemplo a seguir, para não ter que declarar a mesma
  função duas vezes dentro das classes myGenetico e myPoliedro, g()
  foi declarada como uma função C externa às classes e funcao() apenas
  chama-a.*/

inline double pow2(double x){return x*x;}

double g(const fvector &x)
{
  //Esta função possui vários mínimos locais e um mínimo global em
  //x[0]=1.254, x[1]=-1.746. Neste ponto, g()=-0.655
  return( pow2(x[0]-1) + pow2(x[1]+2) +
	  exp(-pow2(x[0]+x[1]))*cos(2*M_PI*(x[0]+x[1])) );
}

/*O programa dever criar uma variável de uma das classes myPoliedro
  e/ou myGenetico. O construtor para as classes base (poliedro ou
  genetico) exige ao menos um parâmetro: a ordem (número de parâmetros
  de entrada da função). É importante que a função não utilize nem
  mais nem menos parâmetros do que a ordem do problema. No exemplo, a
  ordem é de 2 e a função "g" utiliza dois parâmetros (x[0] e x[1]). O
  construtor para a classe "genetico" pode receber um terceiro
  parâmetro: o número de indivíduos na populacão.*/

class myPoliedro: public poliedro
{
private:
  inline double funcao(const fvector &x) const {return g(x);}
public:
  inline myPoliedro():poliedro(2) {;}
};

class myGenetico: public genetico
{
private:
  inline double funcao(const fvector &x) const {return g(x);}
public:
  inline myGenetico():genetico(2,100) {;}
};

int main(void)
{
  fvector x(2);

  myPoliedro p;
  myGenetico a;

  /*Cada classe tem constantes que governam o comportamento do método
    de otimização. A classe tem valores por default aceitáveis para
    estas constantes, que podem contudo ser alterados pelo método
    "fixa_constantes".*/
  /*Na "poliedro", o quanto o poliedro aumenta ou diminui em cada
    passo é fixado por quatro constantes: a de reflexão vale
    usualmente 1.0, a de expansão deve ser maior que 1.0 e as de
    redução e contração devem ser menores que 1.0.*/
  p.fixa_constantes(1,1.5,0.5,0.5);
  /*Na "genetico", as taxas de mutação e de sobrevivência devem estar
    compreendidas entre 0.0 e 1.0.*/
  a.fixa_constantes(0.15,0.5);

  /*A classe se encarrega de gerar os pontos iniciais de maneira
    aleatória, mas o usuário pode definir alguns ou todos destes
    pontos utilizando o método "fixa_ponto_inicial", que recebe como
    parâmetros o índice do ponto que se está fixando e um vetor de
    "ordem" elementos.*/
  /*Na "poliedro" o índice do ponto que se está fixando pode variar de
    0 a "ordem".*/
  x[0]=0; x[1]=1;
  p.fixa_ponto_inicial(0,x);
  x[0]=-1; x[1]=-1;
  p.fixa_ponto_inicial(2,x);
  /*Na "genetico" o índice pode variar de 0 a "numpt-1".*/
  x[0]=0; x[1]=1;
  a.fixa_ponto_inicial(0,x);
  x[0]=-1; x[1]=-1;
  a.fixa_ponto_inicial(99,x);

  /*Ao invés de fixar os pontos iniciais diretamente, o usuário pode
    controlar a maneira como eles são gerados aleatoriamente (pode
    também não fazer nenhuma das duas coisas). Este controle é feito
    parâmetro a parâmetro. Cada parâmetro pode ser gerado obedecendo a
    uma distribuição gaussiana ou uniforme. Para um parâmetro
    gaussiano, o usuário informa a média e o desvio padrão através do
    método "fixa_momentos", enquanto que para um parâmetro
    uniformemente distribuído o usuário informa o máximo e o mínimo
    através do método "fixa_limites".*/
  p.fixa_limites(1,-3,3);
  a.fixa_limites(1,-3,3);
  //p.fixa_momentos(1,0,3);
  //a.fixa_momentos(1,0,3);

  /*Geralmente não se faz o que está sendo feito neste exemplo, ao se
    utilizar tanto a fixação de pontos iniciais quanto o controle da
    geração aleatória em um mesmo problema. Isto porque, ao se chamar
    algum dos métodos "fixa_momentos" ou "fixa_limites", o parâmetro
    correspondente é gerado aleatoriamente para todos os pontos
    iniciais, sobrescrevendo eventuais valores que tenham sido
    informados com "fixa_ponto_inicial".*/
  /*No exemplo da classe "poliedro", onde foram fixados os pontos 0 e
    2 e controlada a forma de geração do parâmetro 1, nesta ordem, os
    pontos resultantes são os seguintes:*/
  /*                PONTO 0        PONTO 1        PONTO 2
    PARÂMETRO 0       0.0            ???           -1.0
    PARÂMETRO 1  Entre -3 e 3   Entre -3 e 3   Entre -3 e 3
  */

  /*O método numérico de minimização para quanto for atingido um
    número máximo de iterações permitido ou quando um certo valor de
    tolerância for alcançado. A classe adota valores por default para
    estes critérios de parada, que podem ser alterados pelo método
    "fixa_criterios_parada". Os parâmetros deste método são o número
    máximo de iterações (um inteiro maior que zero) e a tolerância (um
    número positivo pequeno).*/
  /*A classe "poliedro" para quando a maior distância entre o melhor
    vértice e os outros vértices for menor que o valor de tolerância
    (o que equivale a ter um poliedro muito pequeno).*/
  p.fixa_criterios_parada(1000,0.00001);
  /*A classe "genetico" para quando a diferença entre a função do pior
    indivíduo e do melhor indivíduo for menor que o valor de
    tolerância (o que equivale a ter uma população muito homogênea).*/
  a.fixa_criterios_parada(1000,0.00001);

  int nit;
  struct timeval antes,depois;
  double tempo;
  for (int i=0; i<50; i++)
    {
      std::cout << "===============================================\n";
      std::cout << "Passo " << i << ":\n";

      /*O método "calcula_minimo" é quem implementa efetivamente a
	otimização via poliedros flexíveis. O ponto de mínimo é
	retornado em um vetor que o método recebe como parâmetro. O
	valor de retorno da função é o número de iterações: caso não
	convirja, retorna um número negativo (o código do erro,
	definido no tipo OTIM_ERRO).*/
      gettimeofday(&antes,NULL);
      nit=p.calcula_minimo(x);
      gettimeofday(&depois,NULL);
      tempo = 1000.0*(depois.tv_sec-antes.tv_sec) +
	(depois.tv_usec-antes.tv_usec)/1000.0;
      std::cout << "\nTempo de cálculo: " << tempo << "ms.\n";
      if (nit<0)
	{
	  std::cout << "Poliedro flexível não convergiu!\n";
	}
      else
	{
	  std::cout << "Poliedro flexível convergiu em "
		    << nit << " iterações.\n";
	  std::cout << "Ponto de mínimo:\n";
	  for (int i=0; i<2; i++)
	    {
	      std::cout << x[i] << '\n';
	    }
	  std::cout << "Valor da funcao: " << g(x);
	}
      std::cout << '\n';

      /*O método "calcula_minimo" é quem implementa efetivamente a
	otimização via algoritmos genéticos. O ponto de mínimo é
	retornado em um vetor que o método recebe como parâmetro. O
	valor de retorno da função é o número de gerações: caso não
	convirja, retorna um número negativo (o código do erro,
	definido no tipo OTIM_ERRO).*/
      gettimeofday(&antes,NULL);
      nit=a.calcula_minimo(x);
      gettimeofday(&depois,NULL);
      tempo = 1000.0*(depois.tv_sec-antes.tv_sec) +
	(depois.tv_usec-antes.tv_usec)/1000.0;
      std::cout << "\nTempo de cálculo: " << tempo << "ms.\n";
      if (nit<0)
	{
	  std::cout << "Algoritmo genético não convergiu!\n";
	}
      else
	{
	  std::cout << "Algoritmo genético convergiu em "
		    << nit << " gerações.\n";
	  std::cout << "Ponto de mínimo:\n";
	  for (int i=0; i<2; i++)
	    {
	      std::cout << x[i] << '\n';
	    }
	  std::cout << "Valor da funcao: " << g(x);
	}
      std::cout << '\n';

      /* O método "gera_pontos_iniciais" gera novos pontos iniciais de
	 maneira aleatória (gaussiana ou uniformemente distribuída). É
	 útil para situações, como neste exemplo, onde se pretende
	 achar várias vezes o mínimo de uma mesma função, partindo de
	 condições iniciais diferentes.*/
      p.gera_pontos_iniciais();
      a.gera_pontos_iniciais();

      std::cout << "===============================================\n";
    }
  return 0;
}
