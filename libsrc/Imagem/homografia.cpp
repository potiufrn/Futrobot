#include <iostream>
#include <stdlib.h>
#include <otim.h>

#include "imagem.h"

using namespace std;

Homografia::Homografia()
  :Hom(identity(3)),HomInv(identity(3)),Erro(0.0),ErroMax(0.0)
{
}

Homografia::Homografia(const Homografia &P)
  :Hom(P.Hom),HomInv(P.HomInv),Erro(P.Erro),ErroMax(P.ErroMax)
{
}

Homografia::Homografia(const vmatrix &M)
{
  if (M.numlines()!=3 || M.numcolumns()!=3) {
    std::cerr << "Homografia a partir de matriz com tamanho != [3x3]\n";
    exit(1);
  }
  Hom = M;
  HomInv = ~Hom;
  calculaErros();
}

bool Homografia::incluirPonto(const Coord2 &pm, const Coord2 &pi)
{
  P.push_back(DCoord2(pm,pi));
  Erro = ErroMax = NAN;
  return false;
}

bool Homografia::calibrar()
{
  if (P.size() < 4) {
    cerr << "O calculo da homografia precisa de 4 pontos ou mais.\n";
    cerr << "Encerrando o programa...\n";
    exit(1);
  }
  fmatrix A(2*P.size(),8);
  fvector b(2*P.size());
  for (unsigned i=0; i<P.size(); i++) {
    A[2*i][0] = P[i].m.x();
    A[2*i][1] = P[i].m.y();
    A[2*i][2] = 1.0;
    A[2*i][3] = 0.0;
    A[2*i][4] = 0.0;
    A[2*i][5] = 0.0;
    A[2*i][6] = -P[i].i.u()*P[i].m.x();
    A[2*i][7] = -P[i].i.u()*P[i].m.y();

    b[2*i] = P[i].i.u();

    A[2*i+1][0] = 0.0;
    A[2*i+1][1] = 0.0;
    A[2*i+1][2] = 0.0;
    A[2*i+1][3] = P[i].m.x();
    A[2*i+1][4] = P[i].m.y();
    A[2*i+1][5] = 1.0;
    A[2*i+1][6] = -P[i].i.v()*P[i].m.x();
    A[2*i+1][7] = -P[i].i.v()*P[i].m.y();

    b[2*i+1] = P[i].i.v();
  }
  fvector x=A.bestsolve(b);
  Hom[0][0] = x[0];
  Hom[0][1] = x[1];
  Hom[0][2] = x[2];
  Hom[1][0] = x[3];
  Hom[1][1] = x[4];
  Hom[1][2] = x[5];
  Hom[2][0] = x[6];
  Hom[2][1] = x[7];
  Hom[2][2] = 1.0;
  HomInv = ~Hom;

  calculaErros();
  limparPontos();

  return false;
}

void Homografia::calculaErros() {
  Erro = ErroMax = 0.0;
  if (P.size() == 0) {
    return;
  }
  Coord2 Pm;
  double erro;
  for (unsigned i=0; i<P.size(); i++) {
    Pm = paraMundo(P[i].i);
    erro = euclid(P[i].m - Pm);
    Erro += erro;
    if (erro>ErroMax) ErroMax=erro;
  }
  Erro /= P.size();
}

const Coord2 Homografia::paraImagem(const Coord2 &pm) const
{
  static fvector vm(3);
  vm[0] = pm.x();
  vm[1] = pm.y();
  vm[2] = 1.0;
  fvector vi=Hom*vm;
  if (vi[2]==0.0) {
    cerr << "Impossivel normalizar o ponto em coordenadas homogeneas\n";
    exit(1);
  }
  vi /= vi[2];
  return(Coord2(vi[0],vi[1]));
}


const Coord2 Homografia::paraMundo(const Coord2 &pi) const
{
  static fvector vi(3);
  vi[0] = pi.u();
  vi[1] = pi.v();
  vi[2] = 1.0;
  fvector vm=HomInv*vi;
  if (vm[2]==0.0) {
    cerr << "Impossivel normalizar o ponto em coordenadas homogeneas\n";
    exit(1);
  }
  vm /= vm[2];
  return(Coord2(vm[0],vm[1]));
}
