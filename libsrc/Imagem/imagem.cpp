#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "imagem.h"

using namespace std;

// Converte um byte de 0 a 255 para um float na faixa 0.0 a 1.0
inline float byte2float(uint8_t b){
  return float(b)/255.0;
}
// Converte um float na faixa 0.0 a 1.0 para um byte de 0 a 255
inline uint8_t float2byte(float x){
  return((uint8_t)roundf(255.0*x));
}
// Converte um byte de MIN a MAX para um float na faixa 0.0 a 1.0
inline float byte2float(uint8_t b, uint8_t MAX, uint8_t MIN){
  return float(b-MIN)/float(MAX-MIN);
}
// Converte um float na faixa 0.0 a 1.0 para um byte de MIN a MAX
inline uint8_t float2byte(float x, uint8_t MAX, uint8_t MIN){
  return MIN+uint8_t(roundf(float(MAX-MIN)*x));
}

// Testa se um número está fora da faixa 0.0 a 1.0
inline bool fora_da_faixa_0a1(float X) {
  return (X<0.0 || X>1.0);
}

// Limita um número à faixa 0.0 a 1.0
inline float limita_faixa(float X) {
  return ( (X>=0.0 && X<=1.0) ? X : (X<0.0 ? 0.0 : 1.0) );
}

/* ==============================================================
   TIPOS BÁSICOS
   ============================================================== */

//
// PxRGB
//

uint8_t PxRGB::minPb = 0;
uint8_t PxRGB::maxPb = 255;

inline void swap(uint8_t &x, uint8_t &y){uint8_t p(x); x=y; y=p;}

/*
RGB=0, GRB=1, GBR=2, BGR=-3, BRG=-2, RBG=-1
*/

// Calcula a crominância H (de -PI a +PI)
float getH(const PxRGB &P, uint8_t &bmax, uint8_t &bmin)
{
  static short int f;
  static uint8_t bmed;
  static float C;

  // Iniciar supondo RGB
  bmax=P.r;
  bmin=P.b;
  if (P.r==P.g && P.g==P.b) {
    return 0.0;
  }
  f=0; //RGB
  bmed=P.g;
  if (bmax<bmed) {
    swap(bmax,bmed);
    f = 1; //GRB
  }
  // Agora, pode estar RGB=0 ou GRB=1
  if (bmed<bmin) {
    swap(bmed,bmin);
    f = (f==0 ? -1 : 2); // RGB ? RBG : GBR
  }
  // Agora, pode estar RGB=0, GRB=1, GBR=2 ou RBG=-1
  if (bmax<bmed) {
    swap(bmax,bmed);
    f = (f==-1 ? -2 : -3); // RBG ? BRG : BGR
  }
  // Agora está correto: qualquer das 6 faixas
  C = float(bmed-bmin)/float(bmax-bmin);
  return M_PI*(f+((f==0 || f==2 || f==-2) ? C : 1.0-C))/3.0;
}

// Gera um pixel a partir de um valor de crominancia e dos valores da
// maior e da menor componentes RGB
const PxRGB H2PxRGB(float H, uint8_t bmax, uint8_t bmin)
{
  static short int f;
  static float C;
  static uint8_t bmed;

  H *= 3.0/M_PI;
  //  f = (short int)truncf(H);
  f = (short int)floorf(H);
  C = (f==0||f==2||f==-2 ? H-f : 1.0-(H-f));
  bmed = float2byte(C, bmax, bmin);
  switch(f) {
  default:
  case 0: return PxRGB(bmax,bmed,bmin);
  case 1: return PxRGB(bmed,bmax,bmin);
  case 2: return PxRGB(bmin,bmax,bmed);
  case -3: return PxRGB(bmin,bmed,bmax);
  case -2: return PxRGB(bmed,bmin,bmax);
  case -1: return PxRGB(bmax,bmin,bmed);
  }
}

// Definicao de PxRGB a partir de um PxYUV
PxRGB::PxRGB(const PxYUV &P)
{
  float Y=byte2float(P.y,235,16);
  float U=byte2float(P.u,240,16)-0.5;
  float V=byte2float(P.v,240,16)-0.5;
  r = float2byte(limita_faixa(Y + 1.402*V));
  g = float2byte(limita_faixa(Y - 0.344136*U - 0.714136*V));
  b = float2byte(limita_faixa(Y + 1.772*U));
}

const PxRGB PxRGB::operator=(const PxYUV &P)
{
  PxRGB prov(P);
  r = prov.r;
  g = prov.g;
  b = prov.b;
  return prov;
}

// Definicao de PxRGB a partir de floats [0-1] em outro sistema de cor
void PxRGB::setHSL(float H, float S, float L)
{
  #ifdef _IMAGEM_WITH_CHECK_ERROS_
  if (fora_da_faixa_0a1(S) ||
      fora_da_faixa_0a1(L)) {
    cerr << "componente fora da faixa\n";
    r=g=b=0;
    return;
  }
  #endif
  uint8_t bmax,bmin;
  if (L<=0.5) {
    bmax = float2byte(L*(1.0+S));
    bmin = float2byte(L*(1.0-S));
  }
  else {
    bmax = float2byte(L*(1.0-S)+S);
    bmin = float2byte(L*(1.0+S)-S);
  }
  *this = H2PxRGB(H,bmax,bmin);
}

// Leitura de PxRGB atraves de floats [0-1] em outro sistema de cor
void PxRGB::getHSL(float &H, float &S, float &L) const
{
  static uint8_t bmax,bmin,Pb;

  H = getH(*this,bmax,bmin);
  Pb = bmax-bmin;
  if (Pb <= minPb) {
    L = (float(bmax)+float(bmin))/510.0;  // 510=2*255
    S = 0.0;
  }
  else if (Pb >= maxPb) {
    L = 0.5;
    S = 1.0;
    return;
  }
  else {
    L = (float(bmax)+float(bmin))/510.0;  // 510=2*255
    if (L <= 0.5) {
      S = Pb/(510.0*L);            // 510=2*255
    }
    else {
      S = Pb/(510.0*(1.0-L));      // 510=2*255
    }
  }
  S = limita_faixa(S);
  L = limita_faixa(L);
}

// Definicao de PxRGB a partir de floats [0-1] em outro sistema de cor
void PxRGB::setHSV(float H, float S, float V)
{
  #ifdef _IMAGEM_WITH_CHECK_ERROS_
  if (fora_da_faixa_0a1(S) ||
      fora_da_faixa_0a1(V)) {
    cerr << "componente fora da faixa\n";
    r=g=b=0;
    return;
  }
  #endif
  *this = H2PxRGB(H,float2byte(V),float2byte(V*(1.0-S)));
}

// Leitura de PxRGB atraves de floats [0-1] em outro sistema de cor
void PxRGB::getHSV(float &H, float &S, float &V) const
{
  static uint8_t bmax,bmin,Pb;

  H = getH(*this,bmax,bmin);
  Pb = bmax-bmin;
  if (Pb <= minPb) {
    V = bmax/255.0;
    S = 0.0;
  }
  else if (Pb >= maxPb) {
    V = 1.0;
    S = 1.0;
    return;
  }
  else {
    V = bmax/255.0;
    S = 1.0-float(bmin)/float(bmax);
  }
  V = limita_faixa(V);
  S = limita_faixa(S);
}

// Definicao de PxRGB a partir de floats [0-1] em outro sistema de cor
void PxRGB::setHPG(float H, float P, float G)
{
  #ifdef _IMAGEM_WITH_CHECK_ERROS_
  if (fora_da_faixa_0a1(P) ||
      fora_da_faixa_0a1(G)) {
    cerr << "componente fora da faixa\n";
    r=g=b=0;
    return;
  }
  #endif
  *this = H2PxRGB(H, float2byte(P+(1.0-P)*G), float2byte((1.0-P)*G));
}

// Leitura de PxRGB atraves de floats [0-1] em outro sistema de cor
void PxRGB::getHPG(float &H, float &P, float &G) const
{
  static uint8_t bmax,bmin,Pb;

  H = getH(*this,bmax,bmin);
  Pb = bmax-bmin;
  if (Pb >= maxPb) {
    //G = NAN;
    G = 0.0;
    P = 1.0;
  }
  else {
    G = limita_faixa(float(bmin)/float(255-Pb));
    if (Pb <= minPb) {
      //H = NAN;  // Jah eh feito no getH
      P = 0.0;
    }
    else {
      P = limita_faixa(Pb/255.0);
    }
  }
}

// Valor mínimo de P que define os pixels considerados acromáticos
void setMinP(float P)
{
  #ifdef _IMAGEM_WITH_CHECK_ERROS_
  if (fora_da_faixa_0a1(P)) {
    cerr << "componente fora da faixa\n";
    return;
  }
  #endif
  uint8_t Pb(float2byte(P));
  if (Pb >= PxRGB::maxPb) {
    cerr << "minP maior que maxP\n";
    return;
  }
  PxRGB::minPb = Pb;
}

// Valor máximo de P que define os pixels considerados cores puras
void setMaxP(float P)
{
  #ifdef _IMAGEM_WITH_CHECK_ERROS_
  if (fora_da_faixa_0a1(P)) {
    cerr << "componente fora da faixa\n";
    return;
  }
  #endif
  uint8_t Pb(float2byte(P));
  if (Pb <= PxRGB::minPb) {
    cerr << "maxP menor que minP\n";
    return;
  }
  PxRGB::maxPb = Pb;
}

// Altera o valor do pixel se estiver fora dos limites minP, maxP
void PxRGB::limitP(void)
{
  static uint8_t bmax,bmed,bmin,Pb;

  bmax=r; bmed=g; bmin=b;
  if (bmax<bmed) swap(bmax,bmed);
  if (bmed<bmin) swap(bmed,bmin);
  if (bmax<bmed) swap(bmax,bmed);
  Pb = bmax-bmin;
  if (Pb<minPb) {
    r = g = b = float2byte(float(bmin)/float(255-Pb));
  }
  if (Pb>maxPb) {
    r = (r==bmax ? 255 : (r==bmin ? 0 : float2byte(float(r-bmin)/Pb)));
    g = (g==bmax ? 255 : (g==bmin ? 0 : float2byte(float(g-bmin)/Pb)));
    b = (b==bmax ? 255 : (b==bmin ? 0 : float2byte(float(b-bmin)/Pb)));
  }
}

ostream& operator<<(ostream& os, const PxRGB &P)
{
  os << (unsigned short)P.r << "," << (unsigned short)P.g << ","
     << (unsigned short)P.b;
  return os;
}

//
// PxYUV
//

// Definicao de PxYUV a partir de um PxRGB
PxYUV::PxYUV(const PxRGB &P)
{
  float Y = limita_faixa((+0.299*P.r  
			   +0.587*P.g  
			   +0.114*P.b)/255.0);
  // Na realidade, eh U+0.5
  float U = limita_faixa(0.5+(-0.168736*P.r
			       -0.331264*P.g
			       +0.5*P.b)/255.0);
  // Na realidade, eh V+0.5
  float V = limita_faixa(0.5+(+0.5*P.r
			       -0.418688*P.g
			       -0.081312*P.b)/255.0);
  y = float2byte(Y,235,16);
  u = float2byte(U,240,16);
  v = float2byte(V,240,16);
}

const PxYUV PxYUV::operator=(const PxRGB &P)
{
  PxYUV prov(P);
  y = prov.y;
  u = prov.u;
  v = prov.v;
  return prov;
}

// Definicao de PxYUV a partir de floats Y,U,V
// Y em [0-1]; U,V em [-0.5,0.5]
void PxYUV::setYUV(float Y, float U, float V)
{
  #ifdef _IMAGEM_WITH_CHECK_ERROS_
  if (fora_da_faixa_0a1(Y) ||
      fora_da_faixa_0a1(U+0.5) ||
      fora_da_faixa_0a1(V+0.5) ||
      fora_da_faixa_0a1(Y+1.402*V) ||    // R
      fora_da_faixa_0a1(Y+1.772*U)) {    // B
    cerr << "componente fora da faixa\n";
    y=u=v=128;
    return;
  }
  #endif
  y=float2byte(Y,235,16); // y de 16 a 235, inclusive
  u=float2byte(U+0.5,240,16); // u de 16 a 240, inclusive
  v=float2byte(V+0.5,240,16); // v de 16 a 240, inclusive
}

// Leitura de PxYUV atraves de floats Y [0 a 1] e U,V [-0.5 a 0.5]
void PxYUV::getYUV(float &Y, float &U, float &V) const
{
  Y = limita_faixa(byte2float(y,235,16));
  U = limita_faixa(byte2float(u,240,16))-0.5;
  V = limita_faixa(byte2float(v,240,16))-0.5;
}

ostream& operator<<(ostream& os, const PxYUV &P)
{
  os << (unsigned short)P.y << "," << (unsigned short)P.u << ","
     << (unsigned short)P.v;
  return os;
}

/* ==============================================================
   IMAGENS
   ============================================================== */

#ifdef _IMAGEM_WITH_CHECK_ERROS_
const PxRGB LinhaImagemRGB::operator[](unsigned col) const
{
  if (col>=N) {
    cerr << "Indice invalido da coluna da imagem\n";
    exit(1);
  }
  return(lin[col]);
}

PxRGB &LinhaImagemRGB::operator[](unsigned col)
{
  if (col>=N) {
    cerr << "Indice invalido da coluna da imagem\n";
    exit(1);
  }
  return(lin[col]);
}
#endif

#define TAMANHO_CABECALHO 17

void ImagemRGB::geraCabecalho()
{
  if (sprintf((char *)ptPNM,"P6\n%4u %4u\n255\n",
	      Ncol,Nlin) != TAMANHO_CABECALHO) {
    cerr << "TAMANHO_CABECALHO incorreto\n";
    cerr << "O programa precisa ser refeito!\n";
    exit(1);
  }
}

bool ImagemRGB::create()
{
  ptPNM = malloc(sizeof(PxRGB)*(Ncol*Nlin)+TAMANHO_CABECALHO);
  if (ptPNM==NULL) {
    cerr << "Erro na alocacao de memoria!\n";
    Ncol=Nlin=0;
    ptPNM = img = NULL;
    return false;
  }
  img = (PxRGB*)((uint8_t*)ptPNM+TAMANHO_CABECALHO);
  geraCabecalho();
  return true;
}

bool ImagemRGB::destruct()
{
  free(ptPNM);
  Ncol = Nlin = 0;
  ptPNM = img = NULL;
  return true;
}

bool ImagemRGB::copy(const ImagemRGB &I)
{
  memcpy(img,I.img,sizeof(PxRGB)*(Ncol*Nlin));
  return true;
}

bool ImagemRGB::move(ImagemRGB &I)
{
  Ncol = I.Ncol;
  Nlin = I.Nlin;
  ptPNM = I.ptPNM;
  img = I.img;
  I.Ncol = I.Nlin = 0;
  I.ptPNM = I.img = NULL;
  return true;
}

ImagemRGB::ImagemRGB(unsigned Larg, unsigned Alt) :
  Ncol(Larg),Nlin(Alt)
{
  if (Ncol==0 || Nlin==0) {
    Ncol=Nlin=0;
  }
  if (Ncol>0) {
    create();
  }
  else {
    ptPNM=img=NULL;
  }
}

ImagemRGB::ImagemRGB(const ImagemRGB &I) :
  Ncol(I.Ncol),Nlin(I.Nlin)
{
  if (create()) {
    copy(I);
  }
}

ImagemRGB::ImagemRGB(const char *arq)
{
  Ncol=Nlin=0;
  ptPNM = img = NULL;
  FILE *f = fopen(arq, "r");
  if (f==NULL) {
    cerr << "Impossivel abrir arquivo para ler imagem\n";
    return;
  }

  // Leitura das caracteristicas da imagem
  unsigned tipo;
  if (fscanf(f,"P%u", &tipo) != 1) {
    cerr << "Erro na leitura do cabecalho do arquivo\n";
    return;
  }
  if (tipo<=0 || tipo>6) {
    cerr << "Tipo de imagem nao reconhecido\n";
    return;
  }
  fscanf(f," #%*[^\n]"); // Elimina eventuais linhas de comentário
  fscanf(f," #%*[^\n]"); // Elimina eventuais linhas de comentário
  fscanf(f," #%*[^\n]"); // Elimina eventuais linhas de comentário
  unsigned nl,nc;
  if (fscanf(f,"%u %u", &nc,&nl) != 2) {
    cerr << "Erro na leitura das dimensoes da imagem\n";
    return;
  }
  if (nc==0 || nl==0) {
    cerr << "Arquivo contem imagem de dimensao nula\n";
    return;
  }
  fscanf(f," #%*[^\n]"); // Elimina eventuais linhas de comentário
  fscanf(f," #%*[^\n]"); // Elimina eventuais linhas de comentário
  fscanf(f," #%*[^\n]"); // Elimina eventuais linhas de comentário
  unsigned valmax;
  if (tipo==1 || tipo==4) {
    valmax=0;
  }
  else {
    if (fscanf(f,"%u", &valmax) != 1) {
      cerr << "Erro na leitura do valor maximo do pixel\n";
      return;
    }
    if (valmax<=0 || valmax>=65536) {
      cerr << "Valor maximo do pixel invalido: " << valmax << endl;
      return;
    }
  }
      
  // Alocacao da imagem
  Ncol = nc;
  Nlin = nl;
  if (!create()) {
    return;
  }

  // Leitura dos pixels
  uint16_t ur,ug,ub;
  uint8_t br,bg,bb;

  fscanf(f," "); // Elimina os espacos em branco iniciais
  for (unsigned v=0; v<Nlin; v++) {
    for (unsigned u=0; u<Ncol; u++) {
      switch (tipo) {
      case 1:
	// Imagem preta e branca, em ASCII
	if (fscanf(f,"%hu",&ub)!=1) {
	  cerr << "Erro na leitura do pixel ("
	       << v << "," << u << ")\n";
	  return;
	}
	bb = (ub ? 255 : 0);
	img[u+Ncol*v]=PxRGB(bb,bb,bb);
	break;
      case 2:
	// Imagem cinza, em ASCII
	if (fscanf(f,"%hu",&ug)!=1) {
	  cerr << "Erro na leitura do pixel ("
	       << v << "," << u << ")\n";
	  return;
	}
	if (ug>valmax) {
	  cerr << "Valor do pixel (" << v << "," << u 
	       << ") acima do maximo " << valmax << ": ("
	       << ug << ")\n";
	  return;
	}
	bg = float2byte(float(ug)/valmax);
	img[u+Ncol*v]=PxRGB(bg,bg,bg);
	break;
      case 3:
	// Imagem colorida, em ASCII
	if (fscanf(f,"%hu %hu %hu",&ur,&ug,&ub)!=3) {
	  cerr << "Erro na leitura do pixel ("
	       << v << "," << u << ")\n";
	  return;
	}
	if (ur>valmax || ug>valmax || ub>valmax) {
	  cerr << "Valor do pixel (" << v << "," << u 
	       << ") acima do maximo " << valmax << ": ("
	       << ur << "," << ug << "," << ub << ")\n";
	  return;
	}
	br = float2byte(float(ur)/valmax);
	bg = float2byte(float(ug)/valmax);
	bb = float2byte(float(ub)/valmax);
	img[u+Ncol*v]=PxRGB(br,bg,bb);
	break;
      case 4:
	// Imagem preta e branca, em bits
	if (fscanf(f,"%c",&bg)!=1) {
	  cerr << "Erro na leitura do pixel ("
	       << v << "," << u << ")\n";
	  return;
	}
	for (unsigned i=0; i<8 && u<Ncol; i++) {
	  bb = (bg & 0x80);
	  img[u+Ncol*v]=PxRGB(bb,bb,bb);
	  bg <<= 1;
	  u++;
	}
	u--;
	break;
      case 5:
	// Imagem cinza, em uint8_ts
	if (valmax <= 255) {
	  if (fscanf(f,"%c",&bg) != 1) {
	    cerr << "Erro na leitura do pixel ("
		 << v << "," << u << ")\n";
	    return;
	  }
	  bg = float2byte(float(bg)/valmax);
	  img[u+Ncol*v]=PxRGB(bg,bg,bg);
	}
	else {
	  if (fscanf(f,"%2c",(uint8_t*)(&ug)) != 1) {
	    cerr << "Erro na leitura do pixel ("
		 << v << "," << u << ")\n";
	    return;
	  }
	  bg = float2byte(float(ug)/valmax);
	  img[u+Ncol*v]=PxRGB(bg,bg,bg);
	}
	break;
      case 6:
	// Imagem colorida, em uint8_ts
	if (valmax <= 255) {
	  if (fscanf(f,"%c%c%c",&br,&bg,&bb)!=3) {
	    cerr << "Erro na leitura do pixel ("
		 << v << "," << u << ")\n";
	    return;
	  }
	  br = float2byte(float(br)/valmax);
	  bg = float2byte(float(bg)/valmax);
	  bb = float2byte(float(bb)/valmax);
	  img[u+Ncol*v]=PxRGB(br,bg,bb);
	}
	else {
	  if (fscanf(f,"%2c%2c%2c",
		     (uint8_t*)(&ur),(uint8_t*)(&ug),(uint8_t*)(&ub)) != 3) {
	    cerr << "Erro na leitura do pixel ("
		 << v << "," << u << ")\n";
	    return;
	  }
	  br = float2byte(float(ur)/valmax);
	  bg = float2byte(float(ug)/valmax);
	  bb = float2byte(float(ub)/valmax);
	  img[u+Ncol*v]=PxRGB(br,bg,bb);
	}
	break;
      }
    }
  }
  fclose(f);
}

ImagemRGB::~ImagemRGB()
{
  destruct();
}

bool ImagemRGB::resize(unsigned Larg, unsigned Alt, bool keepData)
{
  if (Larg==0 || Alt==0) {
    cerr << "Dimensao nula para imagem\n";
    return false;
  }
  if (Larg!=Ncol || Alt!=Nlin) {
    ImagemRGB prov(Larg,Alt);
    if (prov.ptPNM==NULL) {
      return false;
    }
    if (keepData) {
      for (unsigned i=0; i<min(Nlin,Alt); i++) {
	memcpy(prov.img+Larg*i,img+Ncol*i,sizeof(PxRGB)*min(Ncol,Larg));
      }
    }
    destruct(); 
    move(prov);
  }
  return true;
}

bool ImagemRGB::load(const char *arq)
{
  ImagemRGB prov(arq);
  if (prov.ptPNM==NULL) {
    return false;
  }
  destruct(); 
  move(prov);
  return true;
}

void ImagemRGB::operator=(const ImagemRGB &I)
{
  if (Ncol!=I.Ncol || Nlin!=I.Nlin) {
    destruct();
    Ncol=I.Ncol;
    Nlin=I.Nlin;
    if (!create()) {
      return;
    }
  }
  copy(I);
}

#ifdef _IMAGEM_WITH_CHECK_ERROS_
const LinhaImagemRGB ImagemRGB::operator[](unsigned lin) const
{
  if (lin>=Nlin) {
    cerr << "Indice invalido da linha da imagem\n";
    exit(1);
  }
  return(LinhaImagemRGB(Ncol,img+Ncol*lin));
}

LinhaImagemRGB ImagemRGB::operator[](unsigned lin)
{
  if (lin>=Nlin) {
    cerr << "Indice invalido da linha da imagem\n";
    exit(1);
  }
  return(LinhaImagemRGB(Ncol,img+Ncol*lin));
}
#endif

void ImagemRGB::save(const char *arq, bool ascii) const
{
  FILE *f = fopen(arq, "w");
  if (f==NULL) {
    cerr << "Impossivel abrir arquivo para salvar imagem\n";
    return;
  }

  if (!ascii) {
    // Imagem colorida, em uint8_ts
    unsigned char c;
    for (unsigned i=0; i<sizeof(PxRGB)*(Ncol*Nlin)+TAMANHO_CABECALHO; i++) {
      c = *(((char*)ptPNM)+i);
      if (fprintf(f,"%c",c) < 0) {
	cerr << "Erro na escrita do uint8_t ("
	     << i << ")\n";
	return;
      }
    }
  }
  else {
    // Imagem colorida, em ASCII
    if (fprintf(f,"P3\n") < 0) {
      cerr << "Erro na escrita do cabecalho do arquivo\n";
      return;
    }
    if (fprintf(f,"%u %u\n", Ncol,Nlin) < 0) {
      cerr << "Erro na escrita das dimensoes da imagem\n";
      return;
    }
    // valmax
    if (fprintf(f,"255\n") < 0) {
      cerr << "Erro na escrita do valor maximo do pixel\n";
      return;
    }
      
    // Escrita dos pixels
    int nby,nbylinha=0;
    for (unsigned v=0; v<Nlin; v++) {
      for (unsigned u=0; u<Ncol; u++) {
	if (nbylinha > 60) {
	  // O numero maximo de caracteres por linha eh 70
	  // Cada pixel ocupa 10 caracteres
	  fprintf(f,"\n");
	  nbylinha = 0;
	}
	nby = fprintf(f,"%3u %3u %3u ", (unsigned)img[u+Ncol*v].r,
		      (unsigned)img[u+Ncol*v].g,(unsigned)img[u+Ncol*v].b);
	if (nby>0) {
	  nbylinha += nby;
	}
	else {
	  cerr << "Erro na escrita do pixel ("
	       << v << "," << u << ")\n";
	  return;
	}
      }
    }
  }
  fclose(f);
  return;
}

// Métodos de acesso aos dados de baixo nível
const void *ImagemRGB::getPNMData()
{
  return ptPNM;
}

const PxRGB *ImagemRGB::getRawData()
{
  return img;
}

size_t ImagemRGB::getPNMSize()
{
  return sizeof(PxRGB)*(Ncol*Nlin)+TAMANHO_CABECALHO;
}

size_t ImagemRGB::getRawSize()
{
  return sizeof(PxRGB)*(Ncol*Nlin);
}
