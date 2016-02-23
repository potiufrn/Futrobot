#ifndef _IMAGEM_H_
#define _IMAGEM_H_

#include <stdint.h>
#include <deque>
#include <matrix.h>

// Comente a linha seguinte se quiser que a biblioteca seja compilada
// de forma a ter um funcionamento mais rápido porém sem checagem de
// erros (buffer overflow, etc.)
#define _IMAGEM_WITH_CHECK_ERROS_

/* ==============================================================
   TIPOS BÁSICOS
   ============================================================== */

class PxYUV;

// O tipo PxRGB armazena um pixel colorido em coordenadas RGB. Este eh
// o tipo basico para armazenamento de pixels
class PxRGB
{
 private:
  static uint8_t minPb,maxPb;
 public:
  uint8_t r,g,b;
  // Definicao e leitura de PxRGB a partir de bytes R,G,B
  inline PxRGB(uint8_t R=0, uint8_t G=0, uint8_t B=0):
    r(R), g(G), b(B) {}
  // Definicao de PxRGB a partir de outro PxRGB
  inline PxRGB(const PxRGB &P): r(P.r), g(P.g), b(P.b) {}
  inline const PxRGB operator=(const PxRGB &P) {
    r=P.r; g=P.g; b=P.b; return (*this);
  }
  // Definicao de PxRGB a partir de um PxYUV
  PxRGB(const PxYUV &P);
  const PxRGB operator=(const PxYUV &P);
  // Teste de igualdade
  inline bool operator==(const PxRGB &P) const {
    return (r==P.r && g==P.g && b==P.b);
  }
  inline bool operator!=(const PxRGB &P) const {
    return (r!=P.r || g!=P.g || b!=P.b);
  }
  // Definicao e leitura de PxRGB a partir de floats [0-1] em outros
  // sistemas de cores (HSL,HSV,YUV,etc)
  void setHSL(float H, float S, float L); // 0 <= H,S,L <= 1.0
  void getHSL(float &H, float &S, float &L) const; // 0 <= H,S,L <= 1.0
  void setHSV(float H, float S, float V); // 0 <= H,S,V <= 1.0
  void getHSV(float &H, float &S, float &V)const ; // 0 <= H,S,V <= 1.0
  void setHPG(float H, float P, float G); // 0 <= H,G,P <= 1.0
  void getHPG(float &H, float &P, float &G) const; // 0 <= H,G,P <= 1.0
  // Definicao de caracteristas da classe como um todo
  friend void setMinP(float P);
  friend void setMaxP(float P);
  // Altera o pixel se estiver fora dos limites de P (minP, maxP)
  void limitP(void);
  // Impressao
  friend std::ostream& operator<<(std::ostream& os, const PxRGB &);
};

// Gera um pixel a partir de um valor de crominancia e dos valores das
// componentes de cor máxima e mínima
const PxRGB H2PxRGB(float H, uint8_t bmax=255, uint8_t bmin=0);
// Fixa os valores maximo e minimo de pixels com cor pura e cinza puro
void setMinP(float P);
void setMaxP(float P);

// O tipo PxYUV armazena um pixel colorido em coordenadas YUV. A
// classe na realidade é baseada o padrao YCbCr; porém, incorrendo em
// um "erro" já consagrado, utilizaremos o termo YUV para nos
// referirmos ao padrão YCbCr
class PxYUV
{
 private:
 public:
  uint8_t y,u,v;
  // Definicao e leitura de PxYUV a partir de bytes Y,U,V
  // CUIDADO ao utilizar este construtor: lembre-se que no padrao
  // YUV os valores de y,u e v não podem assumir todos os valores de
  // 0 a 255! Sem esse cuidado, podem ser gerados pixels inválidos.
  // y de 16 a 235, inclusive
  // u de 16 a 240, inclusive
  // v de 16 a 240, inclusive
  // Mesmo que todos os componentes estejam dentro da faixa, o pixel
  // resultante pode nao ter equivalente RGB
  inline PxYUV(uint8_t Y=128, uint8_t U=128, uint8_t V=128):
    y(Y), u(U), v(V) {}
  // Definicao de PxYUV a partir de outro PxYUV
  inline PxYUV(const PxYUV &P): y(P.y), u(P.u), v(P.v) {}
  inline const PxYUV operator=(const PxYUV &P) {
    y=P.y; u=P.u; v=P.v; return (*this);
  }
  // Definicao de PxYUV a partir de um PxRGB
  PxYUV(const PxRGB &P);
  const PxYUV operator=(const PxRGB &P);
  // Teste de igualdade
  inline bool operator==(const PxYUV &P) const {
    return (y==P.y && u==P.u && v==P.v);
  }
  inline bool operator!=(const PxYUV &P) const {
    return (y!=P.y || u!=P.u || v!=P.v);
  }
  // Definicao e leitura de PxYUV a partir de floats Y,U,V
  // Y em [0 a 1]; U,V em [-0.5 a0.5]
  void setYUV(float Y, float U, float V);
  void getYUV(float &Y, float &U, float &V) const;
  inline void set(float Y, float U, float V) {setYUV(Y,U,V);}
  inline void get(float &Y, float &U, float &V) const {getYUV(Y,U,V);}
  // Impressao
  friend std::ostream& operator<<(std::ostream& os, const PxYUV &);
};

// As classes para armazenar coordenadas de pontos

// Ponto 2D

struct Coord2{
  double X,Y;
  inline Coord2(double pX=0.0, double pY=0.0):X(pX),Y(pY) {}
  inline Coord2 operator+(const Coord2 &C) const {
    return Coord2(X+C.X, Y+C.Y);
  }
  inline Coord2 operator-(const Coord2 &C) const {
    return Coord2(X-C.X, Y-C.Y);
  }
  inline double x() const {return X;}
  inline double y() const {return Y;}
  inline double u() const {return X;}
  inline double v() const {return Y;}
  inline double &x() {return X;}
  inline double &y() {return Y;}
  inline double &u() {return X;}
  inline double &v() {return Y;}
};
inline Coord2 operator*(double K, const Coord2 &C) {
  return Coord2(K*C.X, K*C.Y);
}
inline double sum2(const Coord2 &C) {
  return C.X*C.X+C.Y*C.Y;
}
inline double euclid(const Coord2 &C) {
  return sqrt(sum2(C));
}
inline std::ostream& operator<<(std::ostream& OS, const Coord2 &C) {
  return OS << '(' << C.X << ',' << C.Y << ')';
}

// Conjunto de dois pontos 2D

struct DCoord2
{
  Coord2 m,i;
  inline DCoord2():
    m(),i() {}
  inline DCoord2(const Coord2 &pm, const Coord2 &pi):
    m(pm),i(pi) {}
};

// Um conjunto de três pontos 2D

struct TCoord2
{
  Coord2 p0,p1,p2;
  inline TCoord2():
    p0(),p1(),p2() {}
  inline TCoord2(const Coord2 &P0, const Coord2 &P1, const Coord2 &P2):
    p0(P0),p1(P1),p2(P2) {}
  inline const Coord2 operator[](unsigned i) const {
    return (i==0 ? p0 : (i==1 ? p1 : p2) );
  }
  inline Coord2 &operator[](unsigned i) {
    return (i==0 ? p0 : (i==1 ? p1 : p2));
  }
};

// Ponto 3D

struct Coord3{
  double X,Y,Z;
  inline Coord3(double pX=0.0, double pY=0.0, double pZ=0.0):
    X(pX),Y(pY),Z(pZ) {}
  inline double x() const {return X;}
  inline double y() const {return Y;}
  inline double z() const {return Z;}
  inline double u() const {return X;}
  inline double v() const {return Y;}
  inline double theta() const { return Z; }
  inline double &x() {return X;}
  inline double &y() {return Y;}
  inline double &z() {return Z;}
  inline double &u() {return X;}
  inline double &v() {return Y;}
  inline double &theta() { return Z; }
};

// Um conjunto de três pontos 3D

struct TCoord3
{
  Coord3 p0,p1,p2;
  inline TCoord3():
    p0(),p1(),p2() {}
  inline TCoord3(const Coord3 &P0, const Coord3 &P1, const Coord3 &P2):
    p0(P0),p1(P1),p2(P2) {}
  inline const Coord3 operator[](unsigned i) const {
    return (i==0 ? p0 : (i==1 ? p1 : p2) );
  }
  inline Coord3 &operator[](unsigned i) {
    return (i==0 ? p0 : (i==1 ? p1 : p2));
  }
};

/* ==============================================================
   IMAGENS
   ============================================================== */

// A classe LinhaImagemRGB é definida apenas por razoes de
// implementacao. É o dado de retorno do operator[] da classe
// ImagemRGB: ele existe para que se possa implementar acesso a um
// pixel da imagem da forma IRGB[][]. Nao se consegue (nem se deve)
// criar nenhum objeto desta classe, pois o construtor é private

class LinhaImagemRGB
{
 private:
  unsigned N;
  PxRGB *lin;
  inline LinhaImagemRGB(unsigned Num, PxRGB *Lin) {N=Num; lin=Lin;}
  friend class ImagemRGB;
 public:
  #ifdef _IMAGEM_WITH_CHECK_ERROS_
  const PxRGB operator[](unsigned col) const;
  PxRGB &operator[](unsigned col);
  #else
  inline const PxRGB operator[](unsigned col) const {return(lin[col]);}
  inline PxRGB &operator[](unsigned col) {return(lin[col]);}
  #endif
};

// A classe ImagemRGB lê e salva imagens no formato PNM (PPM, PGM,
// PBM)

class ImagemRGB
{
 private:
  unsigned Ncol,Nlin;
  PxRGB *img;
  void *ptPNM;
  void geraCabecalho();
  bool create();
  bool destruct();
  bool copy(const ImagemRGB &I);
  bool move(ImagemRGB &I);
 public:
  ImagemRGB(unsigned Larg, unsigned Alt);
  ImagemRGB(const ImagemRGB &I);
  explicit ImagemRGB(const char *arq);
  ~ImagemRGB();
  bool resize(unsigned Larg, unsigned Alt, bool keepData=false);
  bool load(const char *arq);
  void operator=(const ImagemRGB &I);
  inline unsigned ncol() const {return Ncol;}
  inline unsigned nlin() const {return Nlin;}
  #ifdef _IMAGEM_WITH_CHECK_ERROS_
  const LinhaImagemRGB operator[](unsigned lin) const;
  LinhaImagemRGB operator[](unsigned lin);
  #else
  const LinhaImagemRGB operator[](unsigned lin) const {
    return(LinhaImagemRGB(Ncol,img+Ncol*lin)); }
  LinhaImagemRGB operator[](unsigned lin) {
    return(LinhaImagemRGB(Ncol,img+Ncol*lin)); }
  #endif
  void save(const char *arq, bool ascii=false) const;
  // Métodos de acesso aos dados de baixo nível
  // Cuidado ao usar! Nao altere os dados para onde os ponteiros apontam...
  const void *getPNMData();
  size_t getPNMSize();
  const PxRGB *getRawData();
  size_t getRawSize();
};

/* ==============================================================
   DISTORÇÃO RADIAL
   ============================================================== */

// Existem dois sistemas de coordenadas: um sistema distorcido,
// identificado pela letra "d", que é associado à imagem visível, e o
// sistema "correto", identificado pela letra "c". O que se deseja é,
// para um pixel que aparece na imagem nas coordenadas (ud,vd),
// calcular as coordenadas (uc,vc) onde este pixel deve estar na
// imagem "correta", eliminando o efeito da distorção radial.

// O modelo de distorção radial que estamos considerando é o seguinte:
//
// Para passar do distorcido para o corrigido:
// Ld=(ud-U)^2+(vd-V)^2 -> quadrado da distância até o centro de distorção
// [uc vc]^T = [ud vd]^T + alpha*Ld*[ud-U vd-V]^T
//
// Para passar do corrigido para o distorcido:
// Lc=(uc-U)^2+(vc-V)^2 -> quadrado da distância até o centro de distorção
// Ld=raiz de [ Ld*(1+alpha*Ld)^2 = Lc]
// [ud vd]^T = (1/(1+alpha*Ld))*([uc vc]^T + alpha*Ld*[U V]^T)

// Um outro modelo possível (usando distancia e nao o quadrado dela) é:
//
// Para passar do distorcido para o corrigido:
// Ld=sqrt((ud-U)^2+(vd-V)^2) -> distância até o centro de distorção
// [uc vc]^T = [ud vd]^T + alpha*Ld*[ud-U vd-V]^T
//
// Para passar do corrigido para o distorcido:
// Lc=sqrt((uc-U)^2+(vc-V)^2) -> distância até o centro de distorção
// Ld=raiz de [ Ld*(1+alpha*Ld) = Lc]
// [ud vd]^T = (1/(1+alpha*Ld))*([uc vc]^T + alpha*Ld*[U V]^T)

// Classe DistRadial

// Classe que corrige a distorção radial de uma lente

class DistRadial
{
 private:
  Coord2 Dim;
  Coord2 C;     // centro óptico da distorção radial
  double alpha; // coeficiente de distorção
  Coord2 Tadj;  // translação da imagem
  double Kadj;  // fator de ajuste
  bool Dist2;   // Usar o modelo baseado no quadrado da distância
  std::deque<TCoord2> R;
 public:
  // Os parâmetros do construtor são a dimensão das imagens tratadas
  DistRadial(unsigned dimU, unsigned dimV, bool dist2=false);
  // Retornar os parâmetros
  inline const Coord2 dim() const {return Dim;}
  inline const Coord2 centro() const {return C;}
  inline double coef() const {return alpha;}
  inline const Coord2 translacao() const {return Tadj;}
  inline double ganho() const {return Kadj;}
  // Fixar os parâmetros
  inline void setCentro(const Coord2 &Centro) {C=Centro;}
  inline void setCoef(double Coef) {alpha=Coef;}
  inline void setTranslacao(const Coord2 &Trans) {Tadj=Trans;}
  inline void setGanho(double Ganho) {Kadj=Ganho;}
  // Devem ser passadas as coordenadas (U,V) do CENTRO de três pixels
  // que na imagem aparecem em posições que se sabe corresponderem a
  // uma mesma reta no mundo real e que portanto deveriam ser
  // colineares na imagem. Para que o algoritmo possa determinar a
  // distorção radial a partir de pelo menos três destes conjuntos de
  // pontos, é preciso que os três pontos de cada conjunto não sejam
  // colineares e que a reta que os une não passe pelo centro de
  // distorção ótica. Retorna true em caso de erro na inclusão (pontos
  // colineares)
  bool incluirPontos(const Coord2 &P0, const Coord2 &P1,
		     const Coord2 &P2);
  // Excluir todas as curvas
  inline void limparPontos() {R.clear();}
  // Calcular os parâmetros de distorção radial. Só pode ser chamada
  // depois de chamar o método incluirPontos pelo menos três vezes
  bool calibrar();
  // Translada e inclui um fator de conversão na imagem corrigida de
  // modo a ajustá-la aos limites da imagem "real" (distorcida). Se
  // pordentro==true, faz o ajuste de forma a que todos os pontos da
  // imagem corrigida tenham correspondente na imagem real, correndo o
  // risco de desprezar regiões da imagem real. Caso contrário,
  // todos os pontos da imagem real serão mapeados na imagem
  // corrigida, correndo o risco de ter pontos "negros" (sem
  // correspondente) na imagem corrigida.
  bool ajustar(bool pordentro=false);
  // Calcular as coordenadas corretas de um ponto distorcido. Deve ser
  // chamado depois de calibrar; senão, retorna o próprio ponto.
  const Coord2 corrigir(const Coord2 &pd) const;
  //inline const Coord2 operator*(const Coord2 &pd) const {
  //  return corrigir(pd);
  //}
  // Calcular as coordenadas distorcidas de um ponto correto. Deve ser
  // chamado depois de calibrar; senão, retorna o próprio ponto.
  const Coord2 distorcer(const Coord2 &pc) const;
  //inline friend const Coord2 operator/(const Coord2 &pc,
  //					  const DistRadial &D) {
  //  return D.distorcer(pc);
  //}
};

/* ==============================================================
   HOMOGRAFIA
   ============================================================== */

// Classe Homografia

// Classe que mapeia pontos em um plano no mundo para o plano da imagem

class Homografia
{
 private:
  fmatrix Hom;
  fmatrix HomInv;
  double Erro,ErroMax;
  std::deque<DCoord2> P;
  void calculaErros();
 public:
  Homografia();
  Homografia(const Homografia &H);
  Homografia(const vmatrix &M);
  // Retornar os parâmetros
  inline const fmatrix H() const {return Hom;}
  inline const fmatrix HInv() const {return HomInv;}
  // Incluir um dos pontos a serem usados para cálculo da homografia
  bool incluirPonto(const Coord2 &pm, const Coord2 &pi);
  // Excluir todos os pontos
  inline void limparPontos() {P.clear();}
  // Calcular a homografia. Só pode ser chamada depois de chamar o
  // método incluirPontos pelo menos quatro vezes
  bool calibrar();
  // Retorna o erro medio e o erro maximo entre a posição calculada e
  // a desejada dos pontos no mundo
  inline double erro() const {return Erro;}
  inline double erroMax() const {return ErroMax;}
  // Calcular as coordenadas na imagem de um ponto no mundo. Deve ser
  // chamado depois de calibrar; senão, retorna o próprio ponto.
  const Coord2 paraImagem(const Coord2 &pm) const;
  inline const Coord2 operator*(const Coord2 &pm) const {
    return paraImagem(pm);
  }
  // Calcular as coordenadas no mundo de um ponto na imagem. Deve ser
  // chamado depois de calibrar; senão, retorna o próprio ponto.
  const Coord2 paraMundo(const Coord2 &pi) const;
  inline friend const Coord2 operator/(const Coord2 &pi,
				      const Homografia &H) {
    return H.paraMundo(pi);
  }
};

#endif
