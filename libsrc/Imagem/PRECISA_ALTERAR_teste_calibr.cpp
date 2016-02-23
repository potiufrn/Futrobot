#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <xfig.h>

#include "imagem.h"

using namespace std;

#define NPONTOS 28

#define PONTOS_NOTAVEIS "dados/Pontos_Notaveis.txt"
#define IMAGEM "dados/Campo.ppm"

#define COMBINADO "Combinado.fig"
#define FIGURA "Figura.fig"

#define IMAGEM_CORRIGIDA "Campo_Corrigida.ppm"
#define COMBINADO_CORRIGIDA "Combinado_Corrigida.fig"
#define FIGURA_CORRIGIDA "Figura_Corrigida.fig"

// Salvar combinado
void salvaCombinado(const char *arqImg, const Coord2 *PImg, const char *arqComb)
{
  cout << "Salvando combinado na figura "
       << arqComb << endl;
  XFig Comb(XF_PORTRAIT, false);
  Comb.setUnit(0.03);  // Cada px = 0.03cm
  //id = Comb.addFigure(arqImg, 0,0, 640,480);
  int i,id = Comb.addFigure(arqImg, 0,480, 640,0);
  Comb[id].setDepth(60);
  for (i=0; i<NPONTOS; i++) {
    id = Comb.addCircleRad(PImg[i].u(),PImg[i].v(),5);
    Comb[id].setColor(XF_COLOR_RED);
    Comb[id].setFillColor(XF_COLOR_RED);
  }
  id = Comb.addPolygon(PImg[0].u(),PImg[0].v(),
		       PImg[1].u(),PImg[1].v(),
		       PImg[2].u(),PImg[2].v());
  Comb[id].setColor(XF_COLOR_RED);
  for (i=3; i<=21; i++) {
    Comb[id].addLinearPoint(PImg[i].u(),PImg[i].v());
  }
  Comb.save(arqComb);
}

// Salvar figura
void salvaFigura(const Coord2 *PNominal, const Coord2 *PCalc, const char *arqFig)
{
  cout << "Salvando figura no arquivo "
       << arqFig << endl;
  XFig Fig(XF_PORTRAIT, false);
  Fig.setUnit(10);  // Cada m = 10cm
  int i;
  // Pontos nominais
  int id = Fig.addPolygon(PNominal[0].u(),PNominal[0].v(),
			  PNominal[1].u(),PNominal[1].v(),
			  PNominal[2].u(),PNominal[2].v());
  for (i=3; i<=21; i++) {
    Fig[id].addLinearPoint(PNominal[i].u(),PNominal[i].v());
  }
  Fig[id].setColor(XF_COLOR_BLACK);
  // Pontos calculados
  id = Fig.addPolygon(PCalc[0].u(),PCalc[0].v(),
		      PCalc[1].u(),PCalc[1].v(),
		      PCalc[2].u(),PCalc[2].v());
  for (i=3; i<=21; i++) {
    Fig[id].addLinearPoint(PCalc[i].u(),PCalc[i].v());
  }
  Fig[id].setColor(XF_COLOR_RED);
  Fig[id].setDepth(45);

  Fig.save(arqFig);
}

int main()
{
  int i;

  //
  // SISTEMA ORIGINAL: SEM CORRECAO DE DISTORCAO RADIAL
  //

  cout << "---\n";

  // Ler imagem
  cout << "Lendo imagem original do arquivo " << IMAGEM << endl;
  ImagemRGB JOrig(IMAGEM);

  // Ler pontos notaveis
  cout << "Lendo pontos notaveis do arquivo " << PONTOS_NOTAVEIS << endl;
  Coord2 PImgOrig[NPONTOS];
  Coord2 PCampoNominal[NPONTOS];
  {
    FILE *f = fopen(PONTOS_NOTAVEIS,"r");
    double ux,vy;
    if (f==NULL) {
      cerr << "Nao consegui abrir arquivo de pontos\n";
      exit(1);
    }
    for (i=0; i<NPONTOS; i++) {
      if (fscanf(f,"%lf %lf",&ux,&vy) != 2) {
	cerr << "Erro na leitura do ponto numero " << i << " na imagem\n";
	exit(1);
      }
      PImgOrig[i].X = ux;
      PImgOrig[i].Y = vy;
      if (fscanf(f,"%lf %lf",&ux,&vy) != 2) {
	cerr << "Erro na leitura do ponto numero " << i << " no campo\n";
	exit(1);
      }
      PCampoNominal[i].X = ux;
      PCampoNominal[i].Y = vy;
    }
    fclose(f);
  }

  // Salvar combinado sem correcao radial
  salvaCombinado(IMAGEM, PImgOrig, COMBINADO);

  // Homografia sem correcao radial
  cout << "Calculo da homografia sem correcao radial\n";
  Homografia HOrig;
  for (i=0; i<NPONTOS; i++) {
    HOrig.incluirPonto(PCampoNominal[i],PImgOrig[i]);
  }
  HOrig.calibrar();
  cout << "  H=" << HOrig.H() << endl;
  cout << "  HInv=" << HOrig.HInv() << endl;
  cout << "  Erro medio dos pontos no mundo: " << HOrig.erro() << endl;
  cout << "  Erro maximo dos pontos no mundo: " << HOrig.erroMax() << endl;

  // Posicao calculada sem correcao radial dos pontos no mundo
  Coord2 PCampoOrig[NPONTOS];
  for (i=0; i<NPONTOS; i++) {
    PCampoOrig[i] = PImgOrig[i]/HOrig;
  }

  // Salvar figura sem correcao radial
  salvaFigura(PCampoNominal, PCampoOrig, FIGURA);

  //
  // CORRECAO DE DISTORCAO RADIAL
  //

  cout << "---\n";

  // Calculo da distorcao radial
  cout << "Calculo da distorcao radial\n";
  DistRadial drCorrig(JOrig.ncol(),JOrig.nlin(),true);

  // Retas horizontais
  if ( drCorrig.incluirPontos(PImgOrig[2],PImgOrig[1],PImgOrig[0]) ) {
    cerr << "Reta superior foi desprezada\n";
  } 
  if ( drCorrig.incluirPontos(PImgOrig[4],PImgOrig[22],PImgOrig[20]) ) {
    cerr << "Reta area-superior1 foi desprezada\n";
  }
  if ( drCorrig.incluirPontos(PImgOrig[4],PImgOrig[26],PImgOrig[20]) ) {
    cerr << "Reta area-superior2 foi desprezada\n";
  }
  if ( drCorrig.incluirPontos(PImgOrig[5],PImgOrig[24],PImgOrig[19]) ) {
    cerr << "Reta centro-superior foi desprezada\n";
  }
  if ( drCorrig.incluirPontos(PImgOrig[8],PImgOrig[25],PImgOrig[16]) ) {
    cerr << "Reta centro-inferior foi desprezada\n";
  }
  if ( drCorrig.incluirPontos(PImgOrig[9],PImgOrig[23],PImgOrig[15]) ) {
    cerr << "Reta area-inferior1 foi desprezada\n";
  }
  if ( drCorrig.incluirPontos(PImgOrig[9],PImgOrig[27],PImgOrig[15]) ) {
    cerr << "Reta area-inferior2 foi desprezada\n";
  }
  if ( drCorrig.incluirPontos(PImgOrig[11],PImgOrig[12],PImgOrig[13]) ) {
    cerr << "Reta inferior foi desprezada\n";
  }
  // Retas verticais
  if ( drCorrig.incluirPontos(PImgOrig[3],PImgOrig[4],PImgOrig[10]) ) {
    cerr << "Reta esquerda1 foi desprezada\n";
  }
  if ( drCorrig.incluirPontos(PImgOrig[3],PImgOrig[5],PImgOrig[10]) ) {
    cerr << "Reta esquerda2 foi desprezada\n";
  }
  if ( drCorrig.incluirPontos(PImgOrig[3],PImgOrig[8],PImgOrig[10]) ) {
    cerr << "Reta esquerda3 foi desprezada\n";
  }
  if ( drCorrig.incluirPontos(PImgOrig[3],PImgOrig[9],PImgOrig[10]) ) {
    cerr << "Reta esquerda4 foi desprezada\n";
  }
  if ( drCorrig.incluirPontos(PImgOrig[1],PImgOrig[24],PImgOrig[12]) ) {
    cerr << "Reta central1 foi desprezada\n";
  }
  if ( drCorrig.incluirPontos(PImgOrig[1],PImgOrig[25],PImgOrig[12]) ) {
    cerr << "Reta central2 foi desprezada\n";
  }
  if ( drCorrig.incluirPontos(PImgOrig[21],PImgOrig[20],PImgOrig[14]) ) {
    cerr << "Reta direita1 foi desprezada\n";
  }
  if ( drCorrig.incluirPontos(PImgOrig[21],PImgOrig[19],PImgOrig[14]) ) {
    cerr << "Reta direita2 foi desprezada\n";
  }
  if ( drCorrig.incluirPontos(PImgOrig[21],PImgOrig[16],PImgOrig[14]) ) {
    cerr << "Reta direita3 foi desprezada\n";
  }
  if ( drCorrig.incluirPontos(PImgOrig[21],PImgOrig[15],PImgOrig[14]) ) {
    cerr << "Reta direita4 foi desprezada\n";
  }

  drCorrig.calibrar();
  drCorrig.ajustar(false);

  cout << "  Parâmetros da distorcao radial:\n";
  cout << "    Centro=" << drCorrig.centro() << endl;
  cout << "    Coeficiente=" << drCorrig.coef() << endl;
  cout << "  Parâmetros de redimensionamento da imagem:\n";
  cout << "    Translacao=" << drCorrig.translacao() << endl;
  cout << "    Fator de escala=" << drCorrig.ganho() << endl;

  // Geracao da imagem corrigida
  cout << "Salvando imagem corrigida no arquivo "
       << IMAGEM_CORRIGIDA << endl;
  ImagemRGB JCorrig(JOrig.ncol(),JOrig.nlin()); 
  {
    Coord2 CPdist;
    PxRGB Pdist;
    int udist,vdist;
    for (unsigned v=0; v<JCorrig.nlin(); v++) {
      for (unsigned u=0; u<JCorrig.ncol(); u++) {
	CPdist = drCorrig.distorcer(Coord2(u,v));
	udist=(int)round(CPdist.u());
	vdist=(int)round(CPdist.v());
	if (udist<0 || udist>=(int)JOrig.ncol() ||
	    vdist<0 || vdist>=(int)JOrig.nlin()) {
	  Pdist = PxRGB(0,0,0);
	}
	else {
	  Pdist = JOrig[(unsigned)((int)JOrig.nlin()-1-vdist)][(unsigned)udist];
	}
	JCorrig[(unsigned)((int)JCorrig.nlin()-1-v)][u] = Pdist;
      }
    }
  }
  JCorrig.save(IMAGEM_CORRIGIDA,false);

  // Geracao dos pontos notaveis corrigidos
  Coord2 PImgCorrig[NPONTOS];
  for (i=0; i<NPONTOS; i++) {
    PImgCorrig[i] = drCorrig.corrigir(PImgOrig[i]);
  }

  // Salvar combinado com correcao radial
  salvaCombinado(IMAGEM_CORRIGIDA, PImgCorrig, COMBINADO_CORRIGIDA);

  // Homografia com correcao radial
  cout << "Calculo da homografia com correcao radial\n";
  Homografia HCorrig;
  for (i=0; i<NPONTOS; i++) {
    HCorrig.incluirPonto(PCampoNominal[i],PImgCorrig[i]);
  }
  HCorrig.calibrar();
  cout << "  H=" << HCorrig.H() << endl;
  cout << "  HInv=" << HCorrig.HInv() << endl;
  cout << "  Erro medio dos pontos no mundo: " << HCorrig.erro() << endl;
  cout << "  Erro maximo dos pontos no mundo: " << HCorrig.erroMax() << endl;

  // Posicao calculada com correcao radial dos pontos no mundo
  Coord2 PCampoCorrig[NPONTOS];
  for (i=0; i<NPONTOS; i++) {
    PCampoCorrig[i] = PImgCorrig[i]/HCorrig;
  }

  // Salvar figura com correcao radial
  salvaFigura(PCampoNominal, PCampoCorrig, FIGURA_CORRIGIDA);

  cout << "---\n";

}
