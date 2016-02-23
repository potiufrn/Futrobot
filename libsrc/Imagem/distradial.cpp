#include <iostream>
#include <stdlib.h>
#include <otim.h>

#include "imagem.h"

using namespace std;

//
// Funções auxiliares
//

inline static double pow2(double x) {return x*x;}
inline static double max(double x, double y, double z) {
  return max(max(x,y),z);
}
inline static double min(double x, double y, double z) {
  return min(min(x,y),z);
}

// Resolve a equação de 3 grau dada por x*(1+A*x)^2 = L
static double solve3(double L, double A)
{
  if (A==0.0) return L;
  // A equação a resolver é (A^2)x3 + (2A)x2 + x - L = 0
  // Coeficientes da equação de 3o grau z3 + a2*z2 + a1*z + a0 = 0
  double a2(2.0/A), a1(1.0/(A*A)), a0(-L/(A*A));
  // Fazendo w=z+a2/3,
  // obtém-se a equação w3 + 3*b*w + 2*c = 0
  double b(a1/3.0-(a2*a2)/9.0), c(a0/2.0-(a1*a2)/6.0+(a2*a2*a2)/27.0);
  // Calculo da raiz real
  double delta=b*b*b+c*c;
  double w;
  if (delta<0.0) {
    cerr << "Equacao de terceiro grau com tres raizes reais\n";
    w = -2.0*sqrt(-b)*cos(acos(c/sqrt(-b*b*b))/3.0);
  }
  else {
    w = -(cbrt(c+sqrt(delta))+cbrt(c-sqrt(delta)));
  }
  return(w-a2/3.0);
}
// Resolve a equação de 2 grau dada por x*(1+A*x) = L
static double solve2(double L, double A)
{
  if (A==0.0) return L;
  // A equação a resolver é (A)x2 + (1)x - L = 0
  double delta=4*A*L+1.0;
  if (delta<0.0) {
    cerr << "Equacao de segundo grau com raizes complexas\n";
    return NAN;
  }
  return (sqrt(delta)-1.0)/(2*A);
}

//
// A classe Coord2
//

// Corrige a distorção radial de um ponto, dados o centro e o
// coeficiente de distorção radial
const Coord2 pontoCorrigido(const Coord2 &pd, bool dist2,
			    const Coord2 &C, double alpha)
{
  double Ld = (dist2 ? sum2(pd-C) : euclid(pd-C));
  Coord2 pc = pd + alpha*Ld*(pd-C);
  return pc;
}

// Calcula as coordenadas distorcidas de um ponto correto, dados o
// centro e o coeficiente de distorção radial
const Coord2 pontoDistorcido(const Coord2 &pc, bool dist2,
			     const Coord2 &C, double alpha)
{
  double Lc = (dist2 ? sum2(pc-C) : euclid(pc-C));
  double Ld = (dist2 ? solve3(Lc,alpha) : solve2(Lc,alpha));
  Coord2 pd = (1.0/(1.0+alpha*Ld))*(pc+alpha*Ld*C);
  return pd;
}

//
// A classe TCoord2
//

// Calcula posicao corrigida de 3 pontos dados o centro e o
// coeficiente de distorção radial
inline const TCoord2 pontosCorrigidos(const TCoord2 &C3, bool dist2,
				      const Coord2 &C, double alpha)
{
  return TCoord2(pontoCorrigido(C3.p0, dist2, C,alpha),
		 pontoCorrigido(C3.p1, dist2, C,alpha),
		 pontoCorrigido(C3.p2, dist2, C,alpha));
}

// Retorna o desalinhamento de um conjunto de três pontos, que é a
// distância (>=0) do ponto intermediário (p1) até a reta formada
// pelos pontos extremos (p0 e p2). Se os pontos forem colineares,
// retorna 0.
double desalinhamento(const TCoord2 C) {
  // Reta que passa pelos pontos extremos p0 e p2:
  // x(t2) = x0 + (x2-x0)*t2
  // y(t2) = y0 + (y2-y0)*t2
  // Reta que passa por p1 perpendicular à outra reta:
  // x(t1) = x1 + (y2-y0)*t1
  // y(t1) = y1 - (x2-x0)*t1
  double dx20 = C.p2.x()-C.p0.x();
  double dy20 = C.p2.y()-C.p0.y();
  double dx10 = C.p1.x()-C.p0.x();
  double dy10 = C.p1.y()-C.p0.y();
  // Cálculo do ponto de intercessão das duas retas
  double t1enc = (dx20*dy10-dx10*dy20)/(pow2(dx20)+pow2(dy20));
  Coord2 penc(C.p1.x()+dy20*t1enc, C.p1.y()-dx20*t1enc);
  // Cálculo da distância de p1 até o ponto intermediário
  return (hypot(C.p1.y()-penc.y(),C.p1.x()-penc.x()));
}

class myGenetico: public genetico {
private:
  const std::deque<TCoord2> *Rg;
  bool Dist2g;
public:
  inline myGenetico(const std::deque<TCoord2> *R, bool Dist2)
    :genetico(3,1000) {Rg=R; Dist2g=Dist2;}
  double funcao(const fvector &param) const;
};

// Retorna a média da medida de desalinhamento de todos as conjuntos
// de pontos para dados parâmetros
double myGenetico::funcao(const fvector &param) const
{
  // x[0] = U
  // x[1] = V
  // x[2] = alpha
  double soma=0.0;
  for (unsigned i=0; i<(*Rg).size(); i++) {
    soma += desalinhamento(pontosCorrigidos((*Rg)[i], Dist2g,
					    Coord2(param[0],param[1]),
					    param[2]));
  }
  return(soma/(*Rg).size());
}

//
// A classe DistRadial
//

// Construtor
DistRadial::DistRadial(unsigned dimU, unsigned dimV, bool dist2):
  Dim(dimU,dimV),C(dimU/2.0,dimV/2.0),alpha(0.0),
  Tadj(0.0,0.0),Kadj(1.0),Dist2(dist2) {

  if (dimU==0 || dimV==0) {
    cerr << "As dimensoes da imagem nao podem ser nulas\n";
    exit(1);
  }
}

// Distância mínima entre pontos para cálculo da distorção radial
#define DIST_MIN 5.0

// Retorna true em caso de erro na inclusão (pontos invalidos)
bool DistRadial::incluirPontos(const Coord2 &P0, const Coord2 &P1,
			       const Coord2 &P2)
{
  // Testa se os pontos estão dentro da imagem
  if ( P0.u()<0 || P0.u()>Dim.u() || P0.v()<0 || P0.v()>Dim.v() ||
       P1.u()<0 || P1.u()>Dim.u() || P1.v()<0 || P1.v()>Dim.v() ||
       P2.u()<0 || P2.u()>Dim.u() || P2.v()<0 || P2.v()>Dim.v() ) {
    cerr << "Coord2enadas do ponto invalidas (fora da imagem)\n";
    return true;
  }
  // Testa se os pontos estão suficientemente afastados
  double l20=hypot(P2.y()-P0.y(),P2.x()-P0.x());
  double l21=hypot(P2.y()-P1.y(),P2.x()-P1.x());
  double l10=hypot(P1.y()-P0.y(),P1.x()-P0.x());
  if (l20<DIST_MIN || l21<DIST_MIN || l10<DIST_MIN) {
    cerr << "(" << P0.x() << "," << P0.y() << ")"
	 << "(" << P1.x() << "," << P1.y() << ")"
	 << "(" << P2.x() << "," << P2.y() << ")"
	 << " Pontos muito proximos\n";
    return true;
  }
  TCoord2 C(P0,P1,P2);
  // Ordena de tal forma que os pontos 0 e 2 sejam os mais distantes
  if (l20<l21 || l20<l10) {
    if (l21>l10) {
      // Pontos mais distantes: 2 e 1
      // Trocar ponto 1 com ponto 0
      C.p1 = C.p0;
      C.p0 = P1;
    }
    else {
      // Pontos mais distantes: 1 e 0
      // Trocar ponto 1 com ponto 2
      C.p1 = C.p2;
      C.p2 = P1;
    }
  }
  R.push_back(C);
  return false;
}

// Calcula por otimização o centro e o coeficiente de distorção radial
// de forma a minimizar o erro
bool DistRadial::calibrar()
{
  if (R.size() < 3) {
    cerr << "O algoritmo precisa de 3 conjuntos de pontos ou mais.\n";
    return true;
  }

  fvector param(3);
  myGenetico otimizador(&R,Dist2);

  // Critérios de parada
  otimizador.fixa_criterios_parada(1000,1E-6);
  // Parâmetro 0 - Coord2enada U do centro
  otimizador.fixa_momentos(0, Dim.u()/2.0, Dim.u()/4.0);
  // Parâmetro 1 - Coord2enada V do centro
  otimizador.fixa_momentos(1, Dim.v()/2.0, Dim.v()/4.0);
  // Parâmetro 2 - alpha
  if(Dist2) {
    otimizador.fixa_momentos(2, 4.0E-7, 4.0E-7);
  }
  else {
    otimizador.fixa_momentos(2, 2.0E-4, 2.0E-4);
  }
  // Incluir um ponto que corresponde a não ter distorção radial
  param[0]=Dim.u()/2.0;
  param[1]=Dim.v()/2.0;
  param[2]=0.0;
  otimizador.fixa_ponto_inicial(0,param);

  if (otimizador.calcula_minimo(param)<0) {
    return true;
  }
  C.X = param[0];
  C.Y = param[1];
  alpha = param[2];

  // Depois de calibrada, a classe limpa os pontos
  limparPontos();

  // Tudo OK
  return false;
}

// Faz mudança de escala na imagem
bool DistRadial::ajustar(bool pordentro)
{
  Tadj.X = 0.0;
  Tadj.Y = 0.0;
  Kadj = 1.0;
  Coord2 Pnn=corrigir(Coord2(0,0));
  Coord2 Pnx=corrigir(Coord2(0,Dim.v()));
  Coord2 Pxn=corrigir(Coord2(Dim.u(),0));
  Coord2 Pxx=corrigir(Coord2(Dim.u(),Dim.v()));
  Coord2 Pdn=corrigir(Coord2(C.u(),0));
  Coord2 Pdx=corrigir(Coord2(C.u(),Dim.v()));
  Coord2 Pnd=corrigir(Coord2(0,C.v()));
  Coord2 Pxd=corrigir(Coord2(Dim.u(),C.v()));
  if (pordentro) {
    Tadj.X = max(Pnn.u(), Pnx.u(), Pnd.u());
    Tadj.Y = max(Pnn.v(), Pxn.v(), Pdn.v());
    Coord2 P2(min(Pxn.u(), Pxx.u(), Pxd.u()),
	     min(Pnx.v(), Pxx.v(), Pdx.v()));
    Kadj = max(Dim.u()/(P2.u()-Tadj.u()) , Dim.v()/(P2.v()-Tadj.v()));
  }
  else {
    Tadj.X = min(Pnn.u(), Pnx.u(), Pnd.u());
    Tadj.Y = min(Pnn.v(), Pxn.v(), Pdn.v());
    Coord2 P2(max(Pxn.u(), Pxx.u(), Pxd.u()),
	      max(Pnx.v(), Pxx.v(), Pdx.v()));
    Kadj = min(Dim.u()/(P2.u()-Tadj.u()) , Dim.v()/(P2.v()-Tadj.v()));
  }
  return false;
}

// Depois da classe calibrada, este é o método que deve ser
// utilizado para corrigir a posição dos pontos
const Coord2 DistRadial::corrigir(const Coord2 &pd) const
{
  Coord2 pc = pontoCorrigido(pd,Dist2,C,alpha);
  Coord2 pa = Kadj*(pc-Tadj);
  return pa;
}

// Calcular as coordenadas distorcidas de um ponto correto. Deve ser
// usado depois de calibrar; se chamar antes, retorna o próprio ponto.
const Coord2 DistRadial::distorcer(const Coord2 &pa) const
{
  Coord2 pc = ((1.0/Kadj)*pa)+Tadj;
  Coord2 pd = pontoDistorcido(pc,Dist2,C,alpha);
  return pd;
}
