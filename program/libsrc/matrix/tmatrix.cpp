#include "matrix.h"
#include "mymatrix.h"

/* ***************************************************************** */
/*                       MATRIZES VARIÁVEIS                          */
/* ***************************************************************** */

//Variáveis globais

unsigned TIPOMATRIZ::bufL[MATRIX_BUFFER_SIZE] =
  {0,0,0,0,0,0,0,0,0,0};
TIPOFVETOR* TIPOMATRIZ::bufx[MATRIX_BUFFER_SIZE] =
  {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
unsigned TIPOMATRIZ::buffirst=0;
unsigned TIPOMATRIZ::bufnum=0;

//Métodos privados básicos (gestão da memória)

MATRIX_STATUS TIPOMATRIZ::create(unsigned lin, unsigned col)
{
  x = NULL;
  L = 0;
  if (lin*col != 0) {
    unsigned i,k1,k2;
    // Procura uma área do mesmo tamanho no buffer
    for (i=bufnum; x==NULL && i>0; i--) {
      k1 = (buffirst+i-1)%MATRIX_BUFFER_SIZE;
      if (bufL[k1]==lin && bufx[k1][0].order()==col) {
	x = bufx[k1];
      }
    }
    if (!x) {
      // x = new TIPOFVETOR[lin](col);
      TIPOVETOR *provx = new TIPOVETOR[lin];
      for (i=0; i<lin; i++)
	provx[i] = TIPOFVETOR(col);
      x = (TIPOFVETOR *)provx;
    }
    else {
      // Se encontrou, retira a área em questão do buffer
      bufnum--;
      for (; i<bufnum; i++) {
	k1 = (buffirst+i)%MATRIX_BUFFER_SIZE;
	k2 = (buffirst+i+1)%MATRIX_BUFFER_SIZE;
	bufL[k1] = bufL[k2];
	bufx[k1] = bufx[k2];
      }
    }
    if (x==NULL) {
      return(matrix_error_int(MATRIX_MEMORY_ALLOCATION));
    }
    L = lin;
  }
  temp = false;
  return(MATRIX_OK);
}

void TIPOMATRIZ::copy(const TIPOMATRIZ &b)
{
  for (unsigned i=0; i<L; i++)
    x[i] = b.x[i];
}

void TIPOMATRIZ::move(TIPOMATRIZ &b)
{
  L = b.numlines();
  x = b.x;
  temp = false;
  b.L = 0;
  b.x = NULL;
}

void TIPOMATRIZ::destruct(void)
{
  unsigned k1;

  k1 = (buffirst+bufnum)%MATRIX_BUFFER_SIZE;
  if (bufnum==MATRIX_BUFFER_SIZE) {
    delete [] bufx[k1];
  }
  bufx[k1] = x;
  bufL[k1] = L;
  if (bufnum==MATRIX_BUFFER_SIZE) {
    buffirst += 1;
    buffirst %= MATRIX_BUFFER_SIZE;
  }
  else {
    bufnum++;
  }
}

//Construtores e destrutores

TIPOMATRIZ::TIPOMATRIZ(unsigned dim)
{
  create(dim,dim);
}

TIPOMATRIZ::TIPOMATRIZ(unsigned lin, unsigned col)
{
  create(lin,col);
}

TIPOMATRIZ::TIPOMATRIZ(TIPOMATRIZ &b)
{
  if (b.temp) {
    move(b);
  }
  else {
    if (!create(b.numlines(),b.numcolumns())) {
      copy(b);
    }
  }
}

TIPOMATRIZ::TIPOMATRIZ(const TIPOMATRIZ &b)
{
  if (!create(b.numlines(),b.numcolumns())) {
    copy(b);
  }
}

TIPOMATRIZ::TIPOMATRIZ(const TIPOVETOR &b)
{
  if (!create(1,b.order())) {
    x[0] = b;
  }
}

TIPOMATRIZ::~TIPOMATRIZ()
{
  destruct();
}

// Operadores básicos

const TIPOMATRIZ &TIPOMATRIZ::operator=(TIPOMATRIZ& b)
{
  if (&b != this) {
    if (b.temp) {
      destruct();
      move(b);
    }
    else {
      if (numlines()!=b.numlines() || numcolumns()!=b.numcolumns()) {
	destruct();
	if (create(b.numlines(),b.numcolumns())) {
	  return(*this);
	}
      }
      copy(b);
    }
  }
  return(*this);
}

const TIPOMATRIZ &TIPOMATRIZ::operator=(const TIPOMATRIZ & b)
{
  if (&b != this) {
    if (numlines()!=b.numlines() || numcolumns()!=b.numcolumns()) {
      destruct();
      if (create(b.numlines(),b.numcolumns())) {
	return(*this);
      }
    }
    copy(b);
  }
  return(*this);
}

TIPOFVETOR& TIPOMATRIZ::operator[](unsigned i)
{
  static TIPOFVETOR TIPOFVETOR_return_erro(0);
  if (i>=numlines()) {
    matrix_error_int(MATRIX_OUT_OF_RANGE);
    return(TIPOFVETOR_return_erro);
  }
  return (x[i]);
}

const TIPOFVETOR TIPOMATRIZ::operator[](unsigned i) const
{
  if (i>=numlines()) {
    matrix_error_int(MATRIX_OUT_OF_RANGE);
    return(TIPOFVETOR(0));
  }
  return (x[i]);
}

const TIPOMATRIZ TIPOMATRIZ::operator-(void) const
{
  TIPOMATRIZ prov(numlines(),numcolumns());
  
  for (unsigned i=0; i<prov.numlines(); i++) {
    prov.x[i] = -x[i];
  }
  prov.maketemp();
  return(prov);
}

const TIPOMATRIZ TIPOMATRIZ::operator+(const TIPOMATRIZ & b) const
{
  if (numlines()!=b.numlines() || numcolumns()!=b.numcolumns()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
    return(TIPOMATRIZ(0));
  }
  TIPOMATRIZ prov(numlines(),numcolumns());
  for (unsigned i=0; i<prov.numlines(); i++) {
    prov.x[i] = x[i] + b.x[i];
  }
  prov.maketemp();
  return(prov);
}

const TIPOMATRIZ TIPOMATRIZ::operator-(const TIPOMATRIZ & b) const
{
  if (numlines()!=b.numlines() || numcolumns()!=b.numcolumns()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
    return(TIPOMATRIZ(0));
  }
  TIPOMATRIZ prov(numlines(),numcolumns());
  for (unsigned i=0; i<prov.numlines(); i++) {
    prov.x[i] = x[i]-b.x[i];
  }
  prov.maketemp();
  return(prov);
}

const TIPOMATRIZ TIPOMATRIZ::operator*(TIPODADO b) const
{
  TIPOMATRIZ prov(numlines(),numcolumns());
  for (unsigned i=0; i<prov.numlines(); i++) {
    prov.x[i] = x[i]*b;
  }
  prov.maketemp();
  return(prov);
}

const TIPOMATRIZ TIPOMATRIZ::operator/(TIPODADO b) const
{
  if (b==(TIPODADO)0.0) {
    matrix_error_int(MATRIX_DIVISION_ZERO);
    return(*this);
  }
  TIPOMATRIZ prov(numlines(),numcolumns());
  for (unsigned i=0; i<prov.numlines(); i++) {
    prov.x[i] = x[i]/b;
  }
  prov.maketemp();
  return(prov);
}

const TIPOMATRIZ TIPOMATRIZ::operator+=(const TIPOMATRIZ & b)
{
  if (numlines()!=b.numlines() || numcolumns()!=b.numcolumns()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
  }
  else for (unsigned i=0; i<numlines(); i++) {
    x[i] += b.x[i];
  }
  return(*this);
}

const TIPOMATRIZ TIPOMATRIZ::operator-=(const TIPOMATRIZ & b)
{
  if (numlines()!=b.numlines() || numcolumns()!=b.numcolumns()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
  }
  else for (unsigned i=0; i<numlines(); i++) {
    x[i] -= b.x[i];
  }
  return(*this);
}

const TIPOMATRIZ TIPOMATRIZ::operator*=(TIPODADO b)
{
  for (unsigned i=0; i<numlines(); i++) {
    x[i] *= b;
  }
  return(*this);
}

const TIPOMATRIZ TIPOMATRIZ::operator/=(TIPODADO b)
{
  if (b==(TIPODADO)0.0) {
    matrix_error_int(MATRIX_DIVISION_ZERO);
  }
  else for (unsigned i=0; i<numlines(); i++) {
    x[i] /= b;
  }
  return(*this);
}

const TIPOMATRIZ TIPOMATRIZ::operator*(const TIPOMATRIZ & b) const
{
  if (numcolumns()!=b.numlines()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
    return(TIPOMATRIZ(0));
  }
  TIPOMATRIZ prov(numlines(),b.numcolumns());
  for (unsigned i=0; i<prov.numlines(); i++)
    for (unsigned j=0; j<prov.numcolumns(); j++) {
      prov.x[i][j] = (TIPODADO)0.0;
      for (unsigned k=0; k<numcolumns(); k++) {
	prov.x[i][j] += x[i][k]*b.x[k][j];
      }
    }
  prov.maketemp();
  return(prov);
}

const TIPOVETOR TIPOMATRIZ::operator*(const TIPOVETOR &b) const
{
  if (numcolumns()!=b.order()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
    return(TIPOVETOR(0));
  }
  TIPOVETOR prov(numlines());
  for (unsigned i=0; i<prov.order(); i++) {
    prov.x[i] = (TIPODADO)0.0;
    for (unsigned k=0; k<numcolumns(); k++) {
      prov.x[i] += x[i][k]*b.x[k];
    }
  }
  prov.maketemp();
  return(prov);
}

const TIPOMATRIZ operator*(TIPODADO b, const TIPOMATRIZ & a)
{
  TIPOMATRIZ prov(a.numlines(),a.numcolumns());
  for (unsigned i=0; i<prov.numlines(); i++) {
    prov.x[i] = a.x[i]*b;
  }
  prov.maketemp();
  return(prov);
}

std::ostream& operator<<(std::ostream& os, const TIPOMATRIZ & a)
{
  for (unsigned i=0; i<a.numlines(); i++) {
    if (i!=0) {
      os << ';';
    }
    os << a.x[i];
  }
  return(os);
}

// Transposta

const TIPOMATRIZ TIPOMATRIZ::operator!(void) const
{
  TIPOMATRIZ prov(numcolumns(),numlines());
  
  for (unsigned i=0; i<prov.numlines(); i++)
    for (unsigned j=0; j<prov.numcolumns(); j++) {
      prov.x[i][j] = conjugado(x[j][i]);
    }
  prov.maketemp();
  return(prov);
}

// Extração

const TIPOMATRIZ TIPOMATRIZ::extract(unsigned lini, unsigned cini,
				     unsigned lfim, unsigned cfim) const
{
  if (lini>=numlines() || lfim>=numlines() || lfim<lini ||
      cini>=numcolumns() || cfim>=numcolumns() || cfim<cini) {
    matrix_error_int(MATRIX_OUT_OF_RANGE);
    return(*this);
  }
  TIPOMATRIZ prov(lfim-lini+1,cfim-cini+1);
  for (unsigned i=0; i<prov.numlines(); i++) {
    prov.x[i] = x[i+lini].extract(cini,cfim);
  }
  prov.maketemp();
  return(prov);
}

// Concatenação

const TIPOMATRIZ TIPOMATRIZ::appendright(const TIPOMATRIZ &b) const
{
  if (numlines() != b.numlines()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
    return(*this);
  }
  TIPOMATRIZ prov(numlines(),numcolumns()+b.numcolumns());
  for (unsigned i=0; i<prov.numlines(); i++) {
    prov.x[i] = x[i].append(b.x[i]);
  }
  prov.maketemp();
  return(prov);
}

const TIPOMATRIZ TIPOMATRIZ::appenddown(const TIPOMATRIZ &b) const
{
  if (numcolumns() != b.numcolumns()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
    return(*this);
  }
  TIPOMATRIZ prov(numlines()+b.numlines(),numcolumns());
  for (unsigned i=0; i<prov.numlines(); i++) {
    prov.x[i] = (i<numlines() ? x[i] : b.x[i-numlines()]);
  }
  prov.maketemp();
  return(prov);
}

// Permuta de matrizes

void TIPOMATRIZ::permutewith(TIPOMATRIZ& b)
{
  unsigned tempL;
  TIPOFVETOR *tempx;

  tempL = numlines();
  tempx = x;
  L = b.numlines();
  x = b.x;
  b.L = tempL;
  b.x = tempx;
}

void TIPOMATRIZ::permutewith(TIPOFMATRIZ& b)
{
  ((TIPOFMATRIZ*)this)->permutewith(b);
}

// Permuta de linhas/colunas

void TIPOMATRIZ::permuterows(unsigned l1, unsigned l2)
{
  if (l1>=numlines() || l2>=numlines()) {
    matrix_error_int(MATRIX_OUT_OF_RANGE);
    return;
  }
  x[l1].permutewith(x[l2]);
}

void TIPOMATRIZ::permutecolumns(unsigned c1, unsigned c2)
{
  if (c1>=numlines() || c2>=numlines()) {
    matrix_error_int(MATRIX_OUT_OF_RANGE);
    return;
  }
  for (unsigned i=0; i<numlines(); i++) {
    swap(x[i][c1],x[i][c2]);
  }
}

TIPODADO TIPOMATRIZ::max() const
{
  if (numlines()==0 || numcolumns()==0) {
    return((TIPODADO)0.0);
  }
  TIPODADO prov,max=x[0].max();
  for (unsigned i=1; i<numlines(); i++) {
    prov = x[i].max();
    if (::real(prov) > ::real(max)) max = prov;
  }
  return(max);
}

TIPODADO TIPOMATRIZ::min() const
{
  if (numlines()==0 || numcolumns()==0) {
    return((TIPODADO)0.0);
  }
  TIPODADO prov,min=x[0].min();
  for (unsigned i=1; i<numlines(); i++) {
    prov = x[i].min();
    if (::real(prov) < ::real(min)) min = prov;
  }
  return(min);
}

double TIPOMATRIZ::absmax() const
{
  if (numlines()==0 || numcolumns()==0) {
    return(0.0);
  }
  double prov,max=x[0].absmax();
  for (unsigned i=1; i<numlines(); i++) {
    prov = x[i].absmax();
    if (prov > max) max = prov;
  }
  return(max);
}

double TIPOMATRIZ::absmin() const
{
  if (numlines()==0 || numcolumns()==0) {
    return(0.0);
  }
  double prov,min=x[0].absmin();
  for (unsigned i=1; i<numlines(); i++) {
    prov = x[i].absmin();
    if (prov < min) min = prov;
  }
  return(min);
}

TIPODADO TIPOMATRIZ::sum() const
{
  TIPODADO somat=(TIPODADO)0.0;
  for (unsigned i=0; i<numlines(); i++) {
    somat += x[i].sum();
  }
  return(somat);
}

// Soma do quadrado do módulo dos elementos

double TIPOMATRIZ::sum2() const
{
  double somat=0.0;
  for (unsigned i=0; i<numlines(); i++) {
    somat += x[i].sum2();
  }
  return(somat);
}

// Identidade

const TIPOMATRIZ TIPOMATRIZ::ident(unsigned lin, unsigned col) const
{
  TIPOMATRIZ prov(lin,col);
  for (unsigned i=0; i<prov.numlines(); i++)
    for (unsigned j=0; j<prov.numcolumns(); j++)
      prov.x[i][j] = (i==j ? (TIPODADO)1.0 : (TIPODADO)0.0);
  prov.maketemp();
  return(prov);
}

// Este método (privado) faz a decomposição da matriz em suas componentes
// L e U, tais que L*U = *this.  A matriz "L" tem 1s na diagonal e 0s na
// metade superior, enquanto "U" tem 0s na metade inferior. O método retorna
// as duas componentes nas duas metades da mesma matriz "a". É efetuado
// também um pivotamento, de modo que o produto de L*U dá uma permutação
// das linhas da matriz original: o vetor indx retorna os índices das
// linhas que foram permutadas.

MATRIX_STATUS TIPOMATRIZ::LUinternal(TIPOMATRIZ &a, vvector &indx,
				     double &d) const
{
  if(numlines() != numcolumns()) {
    return(MATRIX_NOT_SQUARE);
  }

  vvector vv(numlines());
  unsigned i,imax,j,k;
  double dmax,d0;
  TIPODADO p0;

  a = *this;
  indx = vvector(a.numlines());
  d = 1.0;

  for (i=0; i<numlines(); i++) {
    dmax = a[i].absmax();
    if (dmax == 0.0) {
      return(MATRIX_SINGULARITY);
    }
    else {
      vv.x[i] = 1.0/dmax;
    }
  }

  for (j=0; j<numcolumns(); j++) {
    for (i=0; i<j; i++) {
      p0 = a.x[i][j];
      for (k=0; k<i; k++) p0 -= a.x[i][k]*a.x[k][j];
      a.x[i][j] = p0;
    }
    dmax = 0.0;
    for (i=j; i<a.numlines(); i++) {
      p0 = a.x[i][j];
      for (k=0; k<j; k++) {
	p0 -= a.x[i][k]*a.x[k][j];
      }
      a.x[i][j] = p0;
      if ( (d0=vv.x[i]*modulo(p0)) >= dmax) {
	dmax = d0;
	imax = i;
      }
    }
    if (j != imax) {
      a.x[j].permutewith(a.x[imax]);
      d = -d;
      vv.x[imax] = vv.x[j];
    }
    indx[j] = imax;
    if ( modulo(a.x[j][j]) == 0.0) {
      return(MATRIX_SINGULARITY);
    }
    if (j != a.numcolumns()-1) {
      p0 = 1.0/a.x[j][j];
      for (i=j+1; i<a.numlines(); i++) a.x[i][j] *= p0;
    }
  }
  return(MATRIX_OK);
}

// Resolve o sistema linear (*this).x = b, onde b é vetor
const TIPOVETOR TIPOMATRIZ::solve(const TIPOVETOR &b) const
{
  if(numlines() != numcolumns()) {
    matrix_error_int(MATRIX_NOT_SQUARE);
    return(TIPOVETOR(0));
  }
  if (numlines() != b.order()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
    return(TIPOVETOR(0));
  }

  TIPOMATRIZ a;
  vvector indx;
  double d;
  MATRIX_STATUS err=LUinternal(a,indx,d);
  if (err != MATRIX_OK) {
    matrix_error_int(err);
    return(TIPOVETOR(0));
  }

  TIPOVETOR x(b);
  unsigned i,k,ip;
  TIPODADO sum;

  for (i=0; i<a.numlines(); i++) {
    ip = (unsigned)indx[i];
    sum = x.x[ip];
    x.x[ip] = x.x[i];
    for (k=0; k<i; k++) sum -= a.x[i][k]*x.x[k];
    x.x[i] = sum;
  }
  for (i=a.numlines()-1; i<a.numlines(); i--) {
    sum = x.x[i];
    for (k=i+1; k<a.numcolumns(); k++) sum -= a.x[i][k]*x.x[k];
    x.x[i] = sum/a.x[i][i];
  }
  x.maketemp();
  return(x);
}

// Resolve o sistema linear (*this).x = b, onde b é matriz
const TIPOMATRIZ TIPOMATRIZ::solve(const TIPOMATRIZ &b) const
{
  if(numlines() != numcolumns()) {
    matrix_error_int(MATRIX_NOT_SQUARE);
    return(TIPOMATRIZ(0,0));
  }
  if (numlines() != b.numlines()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
    return(TIPOMATRIZ(0,0));
  }

  TIPOMATRIZ a;
  vvector indx;
  double d;
  MATRIX_STATUS err=LUinternal(a,indx,d);

  if (err != MATRIX_OK) {
    matrix_error_int(err);
    return(TIPOMATRIZ(0,0));
  }

  TIPOMATRIZ x(b);
  unsigned i,j,k,ip;
  TIPODADO sum;

  for (j=0; j<a.numcolumns(); j++) {
    for (i=0; i<a.numlines(); i++) {
      ip = (unsigned)indx[i];
      sum = x.x[ip][j];
      x.x[ip][j] = x.x[i][j];
      for (k=0; k<i; k++) sum -= a.x[i][k]*x.x[k][j];
      x.x[i][j] = sum;
    }
    for (i=a.numlines()-1; i<a.numlines(); i--) {
      sum = x.x[i][j];
      for (k=i+1; k<a.numcolumns(); k++) sum -= a.x[i][k]*x.x[k][j];
      x.x[i][j] = sum/a.x[i][i];
    }
  }
  x.maketemp();
  return(x);
}

// Inversa
const TIPOMATRIZ TIPOMATRIZ::operator~(void) const
{
  if(numlines() != numcolumns()) {
    matrix_error_int(MATRIX_NOT_SQUARE);
    return(*this);
  }

  TIPOMATRIZ prov = solve(ident());
  prov.maketemp();
  return(prov);
}

// Determinante
TIPODADO TIPOMATRIZ::determ() const
{
  if(numlines() != numcolumns()) {
    matrix_error_int(MATRIX_NOT_SQUARE);
    return((TIPODADO)0.0);
  }
  // Matriz 2x2
  if(numlines()==2) {
    return(x[0][0]*x[1][1]-x[0][1]*x[1][0]);
  }
  // Matriz 3x3
  if(numlines()==3) {
    return(+x[0][0]*x[1][1]*x[2][2]
	   +x[0][1]*x[1][2]*x[2][0]
	   +x[0][2]*x[1][0]*x[2][1]
	   -x[0][2]*x[1][1]*x[2][0]
	   -x[0][1]*x[1][0]*x[2][2]
	   -x[0][0]*x[1][2]*x[2][1]);
  }

  TIPOMATRIZ a;
  vvector indx;
  double d;
  MATRIX_STATUS err=LUinternal(a,indx,d);

  if (err == MATRIX_SINGULARITY) {
    return(0.0);
  }
  if (err != MATRIX_OK) {
    matrix_error_int(err);
    return(0.0);
  }
  TIPODADO det=d;
  for (unsigned i=0; i<a.numlines(); i++) det *= a.x[i][i];
  return(det);
}

// Decomposição LU: decompõe A nas matrizes L e U, tais que A = L*U. L
// é triangular inferior (os elementos acima da diagonal são nulos) e
// os elementos da diogonal principal são iguais a 1.0. R é triangular
// superior (os elementos abaixo da diagonal são nulos).

void TIPOMATRIZ::LU(TIPOMATRIZ &L, TIPOMATRIZ &U) const
{
  if(numlines() != numcolumns()) {
    L = U = TIPOMATRIZ(0);
    matrix_error_int(MATRIX_NOT_SQUARE);
    return;
  }

  L = U = nullmatrix(numlines(),numcolumns());

  TIPOMATRIZ a;
  vvector indx;
  double d;
  MATRIX_STATUS err=LUinternal(a,indx,d);

  if (err != MATRIX_OK) {
    matrix_error_int(err);
    return;
  }

  unsigned i,j;
  for (i=0; i<numlines(); i++) {
    L.x[i][i] = 1.0;
    U.x[i][i] = a.x[i][i];
    for (j=0; j<i; j++) {
      L.x[i][j] = a.x[i][j];
      U.x[j][i] = a.x[j][i];
    }
  }
  for (i=numlines()-2; /*i>=0*/i<numlines()-1; i--) {
    if ((unsigned)indx[i] != i) {
      L.x[i].permutewith(L.x[(unsigned)indx[i]]);
    }
  }
}

// Decomposição QR: decompõe A nas matrizes Q e R, tais que A = Q*R.
// Q é ortogonal (Q*Qt=I). R é triangular superior (os elementos abaixo
// da diagonal são nulos).

void TIPOMATRIZ::QR(TIPOMATRIZ &Q, TIPOMATRIZ &R) const
{
  unsigned i,j,k;
  TIPODADO seno,t1,t2;
  double coss,m1,m2,tau;

  Q = ident(numlines(),numlines());
  R = *this;
  for (j=0; j<numcolumns(); j++) {
    for (i=numlines()-1; i>j; i--) {
      if (R.x[i][j] != 0.0) {
	// Cálculo da matriz de Givens G=[c s; -s* c], Gt*G = I
	// de tal forma que G*[x y]T = [z 0]T
	m1 = modulo(R.x[i-1][j]);
	m2 = modulo(R.x[i][j]);
	tau = hypot(m1,m2);
	// modulo(coss) = m1/tau  fase(coss)=0
	// modulo(seno) = m2/tau  fase(seno)=(fase(R[i-1][j])-fase(R[i][j]))
	coss = m1/tau;
	seno = conjugado(R.x[i][j])/tau;
	if (R.x[i-1][j] != 0.0) {
	  seno *= R.x[i-1][j]/m1;
	}
	// Altera R = G*R
	for (k=j; k<numcolumns(); k++) {
	  t1 = R.x[i-1][k];
	  t2 = R.x[i][k];
	  R.x[i-1][k] = coss*t1 + seno*t2;
	  R.x[i][k] = -conjugado(seno)*t1 + coss*t2;
	}
	R.x[i][j] = 0.0; // Garantia contra erros numéricos
	// Altera Q = Q*Gt
	for (k=0; k<numlines(); k++) {
	  t1 = Q.x[k][i-1];
	  t2 = Q.x[k][i];
	  Q.x[k][i-1] = coss*t1 + conjugado(seno)*t2;
	  Q.x[k][i] = -seno*t1 + coss*t2;
	}
      }
    }
  }
}

// Decomposição LQ: decompõe A nas matrizes L e Q, tais que A = L*Q.
// Q é ortogonal (Q*Qt=I). L é triangular inferior (os elementos acima
// da diagonal são nulos).

void TIPOMATRIZ::LQ(TIPOMATRIZ &L, TIPOMATRIZ &Q) const
{
  unsigned i,j,k;
  TIPODADO seno,t1,t2;
  double coss,m1,m2,tau;

  L = *this;
  Q = ident(numcolumns(),numcolumns());
  for (i=0; i<numlines(); i++) {
    for (j=numcolumns()-1; j>i; j--) {
      if (L.x[i][j] != 0.0) {
	// Cálculo da matriz de Givens G=[c -s;s* c], Gt*G = I
	// de tal forma que [x y]*G = [0 z]
	m1 = modulo(L.x[i][j-1]);
	m2 = modulo(L.x[i][j]);
	tau = hypot(m1,m2);
	// modulo(coss) = m1/tau  fase(coss)=0
	// modulo(seno) = m2/tau  fase(seno)=(fase(L[i][j])-fase(L[i][j-1]))
	coss = m1/tau;
	seno = L.x[i][j]/tau;
	if (L.x[i][j-1] != 0.0) {
	  seno *= conjugado(L.x[i][j-1])/m1;
	}
	// Altera L = L*G
	for (k=0; k<numlines(); k++) {
	  t1 = L.x[k][j-1];
	  t2 = L.x[k][j];
	  L.x[k][j-1] = coss*t1 + conjugado(seno)*t2;
	  L.x[k][j] = -seno*t1 + coss*t2;
	}
	L.x[i][j] = 0.0; // Garantia contra erros numéricos
	// Altera Q = Gt*Q
	for (k=0; k<numcolumns(); k++) {
	  t1 = Q.x[j-1][k];
	  t2 = Q.x[j][k];
	  Q.x[j-1][k] = coss*t1 + seno*t2;
	  Q.x[j][k] = -conjugado(seno)*t1 + coss*t2;
	}
      }
    }
  }
}

// Decomposição em valores singulares. Dada uma matriz A =
// *this[1..m][1..n], esta rotina calcula sua decomposição em valores
// singulares, A = U*W*V^t . As matrizes U e V são ortogonais: U*Ut =
// V*Vt = I. A matriz U tem dimensões U[0..m-1][0..n-1]. A matriz
// diagonal de valores singulares W é gerada como um vetor
// W[0..n-1]. A matriz V (não a transposta) é gerada como
// V[0..n-1][0..n-1].

void TIPOMATRIZ::SVD(TIPOMATRIZ &U, vvector &W, TIPOMATRIZ &V) const
{
  singularvalues(U,W,V);
}

// Resolve (no sentido de mínimos quadrados) o sistema linear
// (*this) * x = b, onde b é vetor. Retorna x.

const TIPOVETOR TIPOMATRIZ::bestsolve(const TIPOVETOR &b) const
{
  if (numlines() != b.order()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
    return(TIPOVETOR(0));
  }

  TIPOMATRIZ U,V;
  vvector w;
  SVD(U,w,V);
  double maior = w.absmax();

  TIPOVETOR x = !U*b;
  for (unsigned i=0; i<x.order(); i++) {
    if ( (modulo(w[i])+maior) == maior ) x.x[i] = 0.0;
    else x.x[i] /= w[i];
  }
  x = V*x;
  x.maketemp();
  return(x);
}

// Resolve (no sentido de mínimos quadrados) o sistema linear
// (*this) * x = b, onde b é matriz. Retorna x.

const TIPOMATRIZ TIPOMATRIZ::bestsolve(const TIPOMATRIZ &b) const
{
  if (numlines() != b.numlines()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
    return(TIPOMATRIZ(0,0));
  }

  TIPOMATRIZ U,V;
  vvector w;
  SVD(U,w,V);
  double maior = w.absmax();

  TIPOMATRIZ x = !U*b;
  for (unsigned i=0; i<x.numlines(); i++) {
    if ( (modulo(w[i])+maior) == maior ) {
      for (unsigned j=0; j<x.numcolumns(); j++) x.x[i][j] = 0.0;
    }
    else x.x[i] /= w[i];
  }
  x = V*x;
  x.maketemp();
  return(x);
}

// Determinação de auto-valores

const vcvector TIPOMATRIZ::eigenvalues() const
{
  TIPOMATRIZ A(*this);
  unsigned n=A.numlines();
  if(n != A.numcolumns()) {
    matrix_error_int(MATRIX_NOT_SQUARE);
    return(vcvector(0));
  }

  TIPODADO p0,p1,p2;
  double d0,d1,d2,d3;
  unsigned i,j,k;
  bool fim;

  // Calcula uma matriz numericamente bem balanceada com os mesmos
  // autovalores da matriz original

  fim=false;
  while (!fim) {
    fim=true;
    for (i=0; i<n; i++) {
      d2=d1=0.0;
      for (j=0; j<n; j++) if (j != i) {
	d1 += modulo(A.x[j][i]);
	d2 += modulo(A.x[i][j]);
      }
      if (d1!=0.0 && d2!=0.0) {
	d0=1.0;
	d3=d1+d2;
	while (d1 < d2/2.0) {
	  d0 *= 2.0;
	  d1 *= 4.0;
	}
	while (d1 > 2.0*d2) {
	  d0 /= 2.0;
	  d1 /= 4.0;
	}
	if ((d1+d2)/d0 < 0.95*d3) {
	  fim=false;
	  for (j=0; j<n; j++) A.x[i][j] /= d0;
	  for (j=0; j<n; j++) A.x[j][i] *= d0;
	}
      }
    }
  }

  // Redução à forma de Hessenberg (todos os elementos abaixo da
  // sub-diagonal inferior tornam-se nulos, sem alterar os
  // autovalores)

  if (n > 2) {
    for (j=0; j<n-2; j++) {
      k = j+1;
      p1 = A.x[k][j];
      for (i=j+2; i<n; i++) {
	if (modulo(A.x[i][j]) > modulo(p1)) {
	  p1=A.x[i][j];
	  k=i;
	}
      }
      if (k != j+1) {
	A.permuterows(k,j+1);
	A.permutecolumns(k,j+1);
      }
      if (p1 != 0.0) {
	for (i=j+2; i<n; i++) {
	  if ((p2=A.x[i][j]) != 0.0) {
	    p2 /= p1;
	    A.x[i][j] = 0.0;
	    for (k=j+1; k<n; k++) A.x[i][k] -= p2*A.x[j+1][k];
	    for (k=0; k<n; k++) A.x[k][j+1] += p2*A.x[k][i];
	  }
	}
      }
    }
  }

  // Determinação dos auto-valores
  // Método QR com shifts

  TIPOMATRIZ R(n,n);
  vcvector eing(n);

  unsigned its;
  double normA=0.0;

  for (i=0;i<n;i++)
    for (j=(i>0 ? i-1 : 0);j<n;j++)
      normA += modulo(A.x[i][j]);

  while (n > 0) {
    its=0;
    fim=false;
    do {
      // Zera dois últimos elementos da subdiagonal se muito pequenos
      for (i=n-1; i>0 && i+2>=n; i--) {
	d3=modulo(A.x[i-1][i-1])+modulo(A.x[i][i]);
	if (d3 == 0.0) d3=normA;
	if ((modulo(A.x[i][i-1]) + d3) == d3) {
	  A.x[i][i-1]=0.0;
	}
      }
      // Se o último elemento da subdiagonal é nulo, um autovalor encontrado
      if ( (fim=(n==1 || A.x[n-1][n-2]==0.0)) ) {
	eing.x[n-1] = A.x[n-1][n-1];
	n--;
      }
      // Procura autovalores da sub-matriz 2x2 mais inferior
      if (!fim && n>=2) {
	p1 = -(A.x[n-2][n-2]+A.x[n-1][n-1]); // p1 = coef b = -soma raízes
	p2 = A.x[n-2][n-2]*A.x[n-1][n-1]
	  -A.x[n-2][n-1]*A.x[n-1][n-2];      // p2 = coef c = produto raízes
	// Testa a possibilidade de ter encontrado um par de autovalores
	if ( (fim=(n==2 || A.x[n-2][n-3]==0.0)) ) {
	  cdouble sdelta = sqrt(cdouble(p1*p1-4.0*p2));
	  eing.x[--n] = (-p1+sdelta)/2.0;
	  eing.x[--n] = (-p1-sdelta)/2.0;
	}
      }
      // Se não achou autovalor, faz mais uma iteração dupla
      if (!fim && n>=3) {
	if (its%10==0 && its>0) {
	  // Mudança de rumo caso não esteja convergindo
	  p1 = -1.5*(modulo(A.x[n-1][n-2])+modulo(A.x[n-2][n-3]));
	  p2 = pow2(modulo(A.x[n-1][n-2])+modulo(A.x[n-2][n-3]));
	}
	// Decomposição QR
	//(A*A + p1*A + p2*identity(n)).QR(Q,R);
	for (i=0; i<n; i++)
	  for (j=0; j<n; j++) {
	    R.x[i][j] = 0.0;
	    // R += A*A
	    for (k=(i<2 ? 0 : i-1); k<(j+2<n ? j+2 : n); k++) {
	      R.x[i][j] += A.x[i][k]*A.x[k][j];
	    }
	    // R += p1*A
	    if (j+1>=i) {
	      R.x[i][j] += p1*A.x[i][j];
	    }
	    // R += p2*I
	    if (j==i) {
	      R.x[i][j] += p2;
	    }
	  }
	for (j=0; j<n-1; j++) {
	  for (i=(j+2<n-1 ? j+2 : n-1); i>j; i--) {
	    if (R.x[i][j] != 0.0) {
	      // Cálculo da matriz de Givens G=[d0 p0; -p0* d0], Gt*G = I
	      // de tal forma que G*[x y]T = [z 0]T
	      d1 = modulo(R.x[i-1][j]);
	      d2 = modulo(R.x[i][j]);
	      d3 = hypot(d1,d2);
	      d0 = d1/d3;
	      p0 = conjugado(R.x[i][j])/d3;
	      if (d1 != 0.0) {
		p0 *= R.x[i-1][j]/d1;
	      }
	      // Altera R = G*R
	      for (k=j; k<n; k++) {
		p1 = R.x[i-1][k];
		p2 = R.x[i][k];
		R.x[i-1][k] = d0*p1 + p0*p2;
		R.x[i][k] = -conjugado(p0)*p1 + d0*p2;
	      }
	      R.x[i][j] = 0.0; // Garantia contra erros numéricos
	      // Altera A = G*A
	      for (k=0; k<n; k++) {
		p1 = A.x[i-1][k];
		p2 = A.x[i][k];
		A.x[i-1][k] = d0*p1 + p0*p2;
		A.x[i][k] = -conjugado(p0)*p1 + d0*p2;
	      }
	      // Altera A = A*Gt
	      for (k=0; k<n; k++) {
		p1 = A.x[k][i-1];
		p2 = A.x[k][i];
		A.x[k][i-1] = d0*p1 + conjugado(p0)*p2;
		A.x[k][i] = -p0*p1 + d0*p2;
	      }
	    }
	  }
	}
	// Garantia contra erros numéricos
	for (j=0; j<n-2; j++) {
	  for (i=j+2; i<n; i++) {
	    A.x[i][j] = 0.0;
	  }
	}
	its++;
      }
    } while (!fim && its<MATRIX_NUM_ITERACOES);
    if (!fim) {
      matrix_error_int(MATRIX_NO_CONVERGENCE);
      n = 0;
    }
  }
  eing.maketemp();
  return(eing);
}

/* ***************************************************************** */
/*                          MATRIZES FIXAS                           */
/* ***************************************************************** */

const TIPOFMATRIZ &TIPOFMATRIZ::operator=(TIPOMATRIZ& b)
{
  if (numlines() != b.numlines() || numcolumns() != b.numcolumns()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
  }
  else {
    TIPOMATRIZ::operator=(b);
  }
  return(*this);
}

const TIPOFMATRIZ& TIPOFMATRIZ::operator=(const TIPOMATRIZ & b)
{
  if (numlines() != numlines() || numcolumns() != b.numcolumns()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
  }
  else {
    TIPOMATRIZ::operator=(b);
  }
  return(*this);
}

void TIPOFMATRIZ::permutewith(TIPOMATRIZ& b)
{
  if (numlines() != b.numlines() || numcolumns() != b.numcolumns()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
    return;
  }
  TIPOMATRIZ::permutewith(b);
}
