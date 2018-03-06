#include "matrix.h"
#include "mymatrix.h"
#include <cstring>

/* ***************************************************************** */
/*                       VETORES VARIÁVEIS                           */
/* ***************************************************************** */

//Variáveis globais

unsigned TIPOVETOR::bufN[MATRIX_BUFFER_SIZE] =
  {0,0,0,0,0,0,0,0,0,0};
TIPODADO* TIPOVETOR::bufx[MATRIX_BUFFER_SIZE] =
  {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
unsigned TIPOVETOR::buffirst=0;
unsigned TIPOVETOR::bufnum=0;

//Métodos privados

MATRIX_STATUS TIPOVETOR::create(unsigned num)
{
  x = NULL;
  if (num!=0) {
    unsigned i,k1,k2;
    // Procura uma área do mesmo tamanho no buffer
    for (i=bufnum; x==NULL && i>0; i--) {
      k1 = (buffirst+i-1)%MATRIX_BUFFER_SIZE;
      if (bufN[k1]==num) {
	x = bufx[k1];
      }
    }
    if (!x) {
      // Se não encontrou no buffer, aloca uma nova área
      x = new TIPODADO[num];
    }
    else {
      // Se encontrou, retira a área em questão do buffer
      bufnum--;
      for (; i<bufnum; i++) {
	k1 = (buffirst+i)%MATRIX_BUFFER_SIZE;
	k2 = (buffirst+i+1)%MATRIX_BUFFER_SIZE;
	bufN[k1] = bufN[k2];
	bufx[k1] = bufx[k2];
      }
    }
    if (!x) {
      N = 0;
      return(matrix_error_int(MATRIX_MEMORY_ALLOCATION));
    }
  }
  N = num;
  temp = false;
  return(MATRIX_OK);
}

void TIPOVETOR::copy(const TIPOVETOR &b)
{
  if (order() > 0) {
    memcpy(x, b.x, order()*sizeof(TIPODADO));
  }
}

void TIPOVETOR::move(TIPOVETOR &b)
{
  N = b.order();
  x = b.x;
  temp = false;
  b.N = 0;
  b.x = NULL;
}

void TIPOVETOR::destruct(void)
{
  unsigned k1;

  k1 = (buffirst+bufnum)%MATRIX_BUFFER_SIZE;
  if (bufnum==MATRIX_BUFFER_SIZE) {
    delete [] bufx[k1];
  }
  bufx[k1] = x;
  bufN[k1] = N;
  if (bufnum==MATRIX_BUFFER_SIZE) {
    buffirst += 1;
    buffirst %= MATRIX_BUFFER_SIZE;
  }
  else {
    bufnum++;
  }
}

//Construtores

TIPOVETOR::TIPOVETOR(unsigned num)
{
  create(num);
}

TIPOVETOR::TIPOVETOR(TIPOVETOR &b)
{
  if (b.temp) {
    move(b);
  }
  else {
    if (!create(b.order())) {
      copy(b);
    }
  }
}

TIPOVETOR::TIPOVETOR(const TIPOVETOR &b)
{
  if (!create(b.order())) {
    copy(b);
  }
}

//Destrutores

TIPOVETOR::~TIPOVETOR()
{
  destruct();
}

//Operadores

const TIPOVETOR &TIPOVETOR::operator=(TIPOVETOR& b)
{
  if (&b != this) {
    if (b.temp) {
      destruct();
      move(b);
    }
    else {
      if (order() != b.order()) {
	destruct();
	if (create(b.order())) {
	  return(*this);
	}
      }
      copy(b);
    }
  }
  return(*this);
}

const TIPOVETOR &TIPOVETOR::operator=(const TIPOVETOR &b)
{
  if (&b != this) {
    if (order() != b.order()) {
      destruct();
      if (create(b.order())) {
	return(*this);
      }
    }
    copy(b);
  }
  return(*this);
}

TIPODADO& TIPOVETOR::operator[](unsigned i)
{
  static TIPODADO dt_return_erro=(TIPODADO)0.0;
  if (i >= order()) {
    matrix_error_int(MATRIX_OUT_OF_RANGE);
    return(dt_return_erro);
  }
  return (x[i]);
}

TIPODADO TIPOVETOR::operator[](unsigned i) const
{
  if (i >= order()) {
    matrix_error_int(MATRIX_OUT_OF_RANGE);
    return((TIPODADO)0.0);
  }
  return (x[i]);
}

const TIPOVETOR TIPOVETOR::operator-(void) const
{
  TIPOVETOR prov(order());
  
  for (unsigned i=0; i<prov.order(); i++) {
    prov.x[i] = -x[i];
  }
  prov.maketemp();
  return(prov);
}

const TIPOVETOR TIPOVETOR::operator+(const TIPOVETOR & b) const
{
  if (order()!=b.order()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
    return(TIPOVETOR(0));
  }
  TIPOVETOR prov(order());
  for (unsigned i=0; i<prov.order(); i++) {
    prov.x[i] = x[i]+b.x[i];
  }
  prov.maketemp();
  return(prov);
}

const TIPOVETOR TIPOVETOR::operator-(const TIPOVETOR & b) const
{
  if (order()!=b.order()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
    return(TIPOVETOR(0));
  }
  TIPOVETOR prov(order());
  for (unsigned i=0; i<prov.order(); i++) {
    prov.x[i] = x[i]-b.x[i];
  }
  prov.maketemp();
  return(prov);
}

const TIPOVETOR TIPOVETOR::operator*(TIPODADO b) const
{
  TIPOVETOR prov(order());
  for (unsigned i=0; i<prov.order(); i++) {
    prov.x[i] = x[i]*b;
  }
  prov.maketemp();
  return(prov);
}

const TIPOVETOR TIPOVETOR::operator/(TIPODADO b) const
{
  if (b==(TIPODADO)0.0) {
    matrix_error_int(MATRIX_DIVISION_ZERO);
    return(*this);
  }
  TIPOVETOR prov(order());
  for (unsigned i=0; i<prov.order(); i++) {
    prov.x[i] = x[i]/b;
  }
  prov.maketemp();
  return(prov);
}

const TIPOVETOR TIPOVETOR::operator+=(const TIPOVETOR & b)
{
  if (order() != b.order()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
  }
  else for (unsigned i=0; i<order(); i++) {
    x[i] += b.x[i];
  }
  return(*this);
}

const TIPOVETOR TIPOVETOR::operator-=(const TIPOVETOR & b)
{
  if (order() != b.order()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
  }
  else for (unsigned i=0; i<order(); i++) {
    x[i] -= b.x[i];
  }
  return(*this);
}

const TIPOVETOR TIPOVETOR::operator*=(TIPODADO b)
{
  for (unsigned i=0; i<order(); i++) {
    x[i] *= b;
  }
  return(*this);
}

const TIPOVETOR TIPOVETOR::operator/=(TIPODADO b)
{
  if (b==(TIPODADO)0.0) {
    matrix_error_int(MATRIX_DIVISION_ZERO);
  }
  else for (unsigned i=0; i<order(); i++) {
    x[i] /= b;
  }
  return(*this);
}

TIPODADO TIPOVETOR::operator*(const TIPOVETOR & b) const
{
  TIPODADO prov=(TIPODADO)0.0;
  if (order()!=b.order()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
  }
  else for (unsigned i=0; i<order(); i++) {
    prov += x[i]*b.x[i];
  }
  return(prov);
}

const TIPOVETOR TIPOVETOR::operator*(const TIPOMATRIZ & b) const
{
  if (order()!=b.numlines()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
    return(TIPOVETOR(0));
  }
  TIPOVETOR prov(b.numcolumns());
  for (unsigned j=0; j<prov.order(); j++) {
    prov.x[j] = (TIPODADO)0.0;
    for (unsigned k=0; k<order(); k++) {
      prov.x[j] += x[k]*b.x[k][j];
    }
  }
  prov.maketemp();
  return(prov);
}

// Funções amigas

const TIPOVETOR operator*(TIPODADO b, const TIPOVETOR & a)
{
  TIPOVETOR prov(a.order());
  for (unsigned i=0; i<prov.order(); i++) {
    prov.x[i] = a.x[i]*b;
  }
  prov.maketemp();
  return(prov);
}

std::ostream& operator<<(std::ostream& os, const TIPOVETOR & a)
{
  for (unsigned i=0; i<a.order(); i++) {
    if (i!=0) {
      os << ',';
    }
    os << a.x[i];
  }
  return(os);
}

// Extração

const TIPOVETOR TIPOVETOR::extract(unsigned ini, unsigned fim) const
{
  if (ini>=order() || fim>=order() || fim<ini) {
    matrix_error_int(MATRIX_OUT_OF_RANGE);
    return(*this);
  }
  TIPOVETOR prov(fim-ini+1);
  for (unsigned i=0; i<prov.order(); i++) {
    prov.x[i] = x[i+ini];
  }
  prov.maketemp();
  return(prov);
}

// Concatenação

const TIPOVETOR TIPOVETOR::append(const TIPOVETOR &b) const
{
  TIPOVETOR prov(order()+b.order());
  for (unsigned i=0; i<prov.order(); i++) {
    prov.x[i] = (i<order() ? x[i] : b.x[i-order()]);
  }
  prov.maketemp();
  return(prov);
}

TIPODADO TIPOVETOR::max() const
{
  if (order()==0) {
    return((TIPODADO)0.0);
  }
  TIPODADO max=x[0];
  for (unsigned i=1; i<order(); i++) {
    if (::real(x[i]) > ::real(max)) max = x[i];
  }
  return(max);
}

TIPODADO TIPOVETOR::min() const
{
  if (order()==0) {
    return((TIPODADO)0.0);
  }
  TIPODADO min=x[0];
  for (unsigned i=1; i<order(); i++) {
    if (::real(x[i]) < ::real(min)) min = x[i];
  }
  return(min);
}

double TIPOVETOR::absmax() const
{
  if (order()==0) {
    return(0.0);
  }
  double prov,max=modulo(x[0]);
  for (unsigned i=1; i<order(); i++) {
    prov = modulo(x[i]);
    if (prov > max) max = prov;
  }
  return(max);
}

double TIPOVETOR::absmin() const
{
  if (order()==0) {
    return(0.0);
  }
  double prov,min=modulo(x[0]);
  for (unsigned i=1; i<order(); i++) {
    prov = modulo(x[i]);
    if (prov < min) min = prov;
  }
  return(min);
}

TIPODADO TIPOVETOR::sum() const
{
  TIPODADO somat=(TIPODADO)0.0;
  for (unsigned i=0; i<order(); i++) {
    somat += x[i];
  }
  return(somat);
}

double TIPOVETOR::sum2() const
{
  double somat=0.0;
  for (unsigned i=0; i<order(); i++) {
    somat += modulo2(x[i]);
  }
  return(somat);
}

void TIPOVETOR::permutewith(TIPOVETOR& b)
{
  unsigned tempN;
  TIPODADO *tempx;

  tempN = order();
  tempx = x;
  N = b.order();
  x = b.x;
  b.N = tempN;
  b.x = tempx;
}

void TIPOVETOR::permutewith(TIPOFVETOR& b)
{
  ((TIPOFVETOR*)this)->permutewith(b);
}

/* ***************************************************************** */
/*                           VETORES FIXOS                           */
/* ***************************************************************** */

const TIPOFVETOR &TIPOFVETOR::operator=(TIPOVETOR& b)
{
  if (order() != b.order()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
  }
  else {
    TIPOVETOR::operator=(b);
  }
  return(*this);
}

const TIPOFVETOR &TIPOFVETOR::operator=(const TIPOVETOR &b)
{
  if (order() != b.order()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
  }
  else {
    TIPOVETOR::operator=(b);
  }
  return(*this);
}

void TIPOFVETOR::permutewith(TIPOVETOR& b)
{
  if (order() != b.order()) {
    matrix_error_int(MATRIX_WRONG_DIMENSION);
    return;
  }
  TIPOVETOR::permutewith(b);
}
