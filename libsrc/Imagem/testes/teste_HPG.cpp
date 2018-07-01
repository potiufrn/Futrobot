#include <iostream>
#include <stdio.h>
#include <sys/time.h>

#include "imagem.h"

using namespace std;

#define NIVEL_COR 0.25
#define NIVEL_PB 0.35

int main()
{
  // Ler imagem original
  ImagemRGB I("dados/Campo.ppm"); 

  ImagemRGB Jcor(I.ncol(),I.nlin()); 
  ImagemRGB Jcinza(I.ncol(),I.nlin()); 
  ImagemRGB JH(I.ncol(),I.nlin()); 
  ImagemRGB Jpb(I.ncol(),I.nlin()); 
  {
    PxRGB Porig,Pzero(0,0,0);
    float H,G,P;

    for (unsigned v=0; v<I.nlin(); v++) {
      for (unsigned u=0; u<I.ncol(); u++) {
	Porig = I[v][u];
	Porig.getHPG(H,P,G);
	if (P<NIVEL_COR) {
	  // Cálculo da imagem cinza
	  Jcinza[v][u] = Porig;
	  Jpb[v][u].r = Jpb[v][u].g = Jpb[v][u].b = (G<NIVEL_PB ? 0 : 255);
	  // Imagem colorida sem nada
	  Jcor[v][u] = Pzero;
	  JH[v][u] = Pzero;
	}
	else {
	  // Cálculo do pixel da imagem colorida
	  Jcor[v][u] = Porig;
	  JH[v][u] = H2PxRGB(H);
	  // Imagem cinza sem nada
	  Jcinza[v][u] = Pzero;
	  Jpb[v][u] = Pzero;
	}
      }
    }
  }
  Jcor.save("IMG_Cor.ppm",false);
  JH.save("IMG_H.ppm",false);
  Jcinza.save("IMG_Cinza.ppm",false);
  Jpb.save("IMG_PB.ppm",false);
  // Teste da mudanca de tamanho
  I.resize(700,500,true);
  I.save("Campo_maior.ppm");
  I.resize(400,300,true);
  I.save("Campo_menor.ppm");
}
