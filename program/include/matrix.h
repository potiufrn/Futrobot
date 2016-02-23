#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <iostream>
#include <complex>

// Os tipos de erro

typedef enum
{
  MATRIX_OK=0,
  MATRIX_MEMORY_ALLOCATION=-1,
  MATRIX_BAD_PARAMETER=-2,
  MATRIX_WRONG_DIMENSION=-3,
  MATRIX_OUT_OF_RANGE=-4,
  MATRIX_NO_CONVERGENCE=-5,
  MATRIX_DIVISION_ZERO=-10,
  MATRIX_NOT_SQUARE=-11,
  MATRIX_SINGULARITY=-12,
  MATRIX_OTHER=-9999
} MATRIX_STATUS;

// Variáveis globais

extern void (*matrix_error)(MATRIX_STATUS err, const char *msg);

/*#############################################################
##  As declarações prévias dos tipos de dados                ##
#############################################################*/

typedef std::complex<double> cdouble;

// Vetores

// Vetor real de dimensões alteráveis após a criação
class vvector;
// Vetor real de dimensões não alteráveis após a criação
class fvector;
// Vetor complexo de dimensões alteráveis após a criação
class vcvector;
// Vetor complexo de dimensões não alteráveis após a criação
class fcvector;

// Matrizes

// Matriz real de dimensões alteráveis após a criação
class vmatrix;
// Matriz real de dimensões não alteráveis após a criação
class fmatrix;
// Matriz complexa de dimensões alteráveis após a criação
class vcmatrix;
// Matriz complexa de dimensões não alteráveis após a criação
class fcmatrix;

/*#############################################################
##  VETORES E MATRIZES REAIS                                 ##
#############################################################*/

// Vetores de dimensão alterável

class vvector
{
private:
  static unsigned bufN[];
  static double *bufx[];
  static unsigned buffirst,bufnum;
  unsigned N;
  double *x;
  bool temp;
  MATRIX_STATUS create(unsigned);
  void copy(const vvector &);
  void move(vvector &);
  void destruct(void);
  inline void maketemp(void) {temp = true;}
public:
  explicit vvector(unsigned num=0);
  vvector(vvector &);
  vvector(const vvector &);
  ~vvector();

  const vvector &operator=(vvector &);
  const vvector &operator=(const vvector &);
  double& operator[](unsigned);
  double operator[](unsigned) const;
  inline const vvector &operator+(void) const {return *this;}
  const vvector operator-(void) const;
  const vvector operator+(const vvector &) const;
  const vvector operator-(const vvector &) const;
  const vvector operator*(double) const;
  const vvector operator/(double) const;
  const vvector operator+=(const vvector &);
  const vvector operator-=(const vvector &);
  const vvector operator*=(double);
  const vvector operator/=(double);
  double operator*(const vvector &) const;
  const vvector operator*(const vmatrix &) const;
  // O operador vetor/matriz equivale a inv(matriz)*vetor
  // Vai ser definido posteriormente com o "solve" das matrizes
  inline const vvector operator/(const vmatrix &b) const;
  // O operador vetor%matriz equivale a pseudoinv(matriz)*vetor
  // Vai ser definido posteriormente com o "bestsolve" das matrizes
  inline const vvector operator%(const vmatrix &b) const;

  inline unsigned order() const {return N;}
  const vvector extract(unsigned ini, unsigned fim) const;
  //inline const vvector extract(unsigned fim) const {return extract(0,fim);}
  const vvector append(const vvector &) const;
  void permutewith(vvector&);
  void permutewith(fvector&);
  double max() const;
  double min() const;
  double absmax() const;
  double absmin() const;
  double sum() const;
  inline double average() const {
    return sum()/order();}
  double sum2() const;
  inline double euclid() const {return sqrt(sum2());}

  friend const vvector operator*(double, const vvector &);
  friend std::ostream& operator<<(std::ostream& os, const vvector &);
  friend const vvector nullvector(unsigned);

  friend class vcvector;
  friend class vmatrix;
  friend class vcmatrix;
};

const vvector operator*(double, const vvector &);
std::ostream& operator<<(std::ostream& os, const vvector &);
const vvector nullvector(unsigned);

// Vetores de dimensão não alterável

class fvector : public vvector
{
public:
  explicit fvector(unsigned num=0): vvector(num){};
  fvector(vvector &b): vvector(b){};
  fvector(const vvector &b): vvector(b){};
  //Alguns compiladores reclamam caso não existam as linhas a seguir:
  fvector(fvector &b): vvector(b){};
  fvector(const fvector &b): vvector(b){};

  const fvector &operator=(vvector &);
  const fvector &operator=(const vvector &);
  void permutewith(vvector&);
};

// Matrizes de dimensão alterável

class vmatrix
{
private:
  static unsigned bufL[];
  static fvector *bufx[];
  static unsigned buffirst,bufnum;
  unsigned L;
  fvector *x;
  bool temp;
  MATRIX_STATUS create(unsigned, unsigned);
  void copy(const vmatrix &);
  void move(vmatrix &);
  void destruct(void);
  inline void maketemp(void) {temp = true;}
  MATRIX_STATUS LUinternal(vmatrix &LU, vvector &indx,
			   double &d) const;
  void singularvalues(vmatrix &U, vvector &W, vmatrix &V) const;
public:
  explicit vmatrix(unsigned dim=0);
  explicit vmatrix(unsigned, unsigned);
  vmatrix(vmatrix &);
  vmatrix(const vmatrix &);
  explicit vmatrix(const vvector &);
  ~vmatrix();
  
  const vmatrix &operator=(vmatrix &);
  const vmatrix &operator=(const vmatrix &);
  fvector& operator[](unsigned);
  const fvector operator[](unsigned) const;
  inline const vmatrix &operator+(void) const {return *this;}
  const vmatrix operator-(void) const;
  const vmatrix operator+(const vmatrix &) const;
  const vmatrix operator-(const vmatrix &) const;
  const vmatrix operator*(double) const;
  const vmatrix operator/(double) const;
  const vmatrix operator+=(const vmatrix &);
  const vmatrix operator-=(const vmatrix &);
  const vmatrix operator*=(double);
  const vmatrix operator/=(double);
  const vmatrix operator*(const vmatrix &) const;
  // O operador matriz1/matriz2 equivale a inv(matriz2)*matriz1.
  inline const vmatrix operator/(const vmatrix &b) const {
    return b.solve(*this);}
  // O operador matriz1%matriz2 equivale a pseudoinv(matriz2)*matriz1.
  inline const vmatrix operator%(const vmatrix &b) const {
    return b.bestsolve(*this);}
  const vvector operator*(const vvector &) const;
  const vmatrix operator!(void) const;	// Transposta
  const vmatrix operator~(void) const;	// Inversa

  inline unsigned numlines(void) const {return L;}
  inline unsigned numcolumns(void) const {return (x ? x[0].order() : 0);}
  const vmatrix extract(unsigned lini, unsigned cini,
			unsigned lfim, unsigned cfim) const;
  //inline const vmatrix extract(unsigned lfim, unsigned cfim) const {
  //  return extract(0,0,lfim,cfim);}
  const vmatrix appendright(const vmatrix &) const;
  const vmatrix appenddown(const vmatrix &) const;
  void permutewith(vmatrix&);
  void permutewith(fmatrix&);
  void permuterows(unsigned, unsigned);
  void permutecolumns(unsigned, unsigned);
  double max() const;
  double min() const;
  double absmax() const;
  double absmin() const;
  double sum() const;
  inline double average() const {
    return sum()/(numlines()*numcolumns());}
  double sum2() const;
  inline double euclid() const {return sqrt(sum2());}
  const vmatrix ident(unsigned, unsigned) const;
  inline const vmatrix ident() const {return ident(numlines(),numcolumns());}
  double determ() const;
  // O método a.solve(b) equivale a inv(a)*b
  // Baseia-se na decomposição LU
  const vvector solve(const vvector &b) const;
  const vmatrix solve(const vmatrix &b) const;
  // O método a.bestsolve(b) equivale a pseudoinv(b)*a
  // Baseia-se na decomposição em valores singulares
  const vvector bestsolve(const vvector &b) const;
  const vmatrix bestsolve(const vmatrix &b) const;
  // Decomposições
  void LU(vmatrix &L, vmatrix &U) const;
  void QR(vmatrix &Q, vmatrix &R) const;
  void LQ(vmatrix &L, vmatrix &Q) const;
  void SVD(vmatrix &U, vvector &W, vmatrix &V) const;
  // Autovalores
  const vcvector eigenvalues() const;

  friend const vvector vvector::operator*(const vmatrix &) const;
  friend const vmatrix operator*(double, const vmatrix &);
  friend std::ostream& operator<<(std::ostream& os, const vmatrix &);
  friend const vmatrix nullmatrix(unsigned,unsigned);
  friend const vmatrix identity(unsigned);

  friend class vvector;
  friend class vcvector;
  friend class vcmatrix;
};

const vmatrix operator*(double, const vmatrix &);
std::ostream& operator<<(std::ostream& os, const vmatrix &);
const vmatrix nullmatrix(unsigned,unsigned);
const vmatrix identity(unsigned);

// Matrizes de dimensão não alterável

class fmatrix : public vmatrix
{
public:
  explicit fmatrix(unsigned dim=0): vmatrix(dim){};
  explicit fmatrix(unsigned lin, unsigned col): vmatrix(lin,col){};
  fmatrix(vmatrix& b): vmatrix(b){};
  fmatrix(vmatrix const &b): vmatrix(b){};
  // Alguns compiladores reclamam caso não existam as linhas a seguir
  fmatrix(fmatrix& b): vmatrix(b){};
  fmatrix(fmatrix const &b): vmatrix(b){};

  const fmatrix &operator=(vmatrix &);
  const fmatrix &operator=(const vmatrix &);
  void permutewith(vmatrix&);
};

/*#############################################################
##  VETORES E MATRIZES COMPLEXOS                             ##
#############################################################*/

// Vetores de dimensão alterável

class vcvector
{
private:
  static unsigned bufN[];
  static cdouble *bufx[];
  static unsigned buffirst,bufnum;
  unsigned N;
  cdouble *x;
  bool temp;
  MATRIX_STATUS create(unsigned);
  void copy(const vcvector &);
  void move(vcvector &);
  void destruct(void);
  inline void maketemp(void) {temp = true;}
public:
  explicit vcvector(unsigned num=0);
  vcvector(vcvector &);
  vcvector(const vcvector &);
  vcvector(const vvector &);
  ~vcvector();

  const vcvector &operator=(vcvector &);
  const vcvector &operator=(const vcvector &);
  cdouble& operator[](unsigned);
  cdouble operator[](unsigned) const;
  inline const vcvector &operator+(void) const {return *this;}
  const vcvector operator-(void) const;
  const vcvector operator+(const vcvector &) const;
  const vcvector operator-(const vcvector &) const;
  const vcvector operator*(cdouble) const;
  const vcvector operator/(cdouble) const;
  const vcvector operator+=(const vcvector &);
  const vcvector operator-=(const vcvector &);
  const vcvector operator*=(cdouble);
  const vcvector operator/=(cdouble);
  cdouble operator*(const vcvector &) const;
  const vcvector operator*(const vcmatrix &) const;
  // O operador vetor/matriz equivale a inv(matriz)*vetor.
  // Vai ser definido posteriormente com o "solve" das matrizes
  inline const vcvector operator/(const vcmatrix &b) const;

  inline unsigned order(void) const {return N;}
  const vcvector extract(unsigned ini, unsigned fim) const;
  //inline const vcvector extract(unsigned fim) const {return extract(0,fim);}
  const vcvector append(const vcvector &) const;
  void permutewith(vcvector&);
  void permutewith(fcvector&);
  cdouble max() const;
  cdouble min() const;
  double absmax() const;
  double absmin() const;
  cdouble sum() const;
  inline cdouble average() const {
    return sum()/(double)order();}
  double sum2() const;
  inline double euclid() const {return sqrt(sum2());}
  const vvector real() const;
  const vvector imag() const;

  friend const vcvector operator*(cdouble, const vcvector &);
  friend std::ostream& operator<<(std::ostream& os, const vcvector &);
  friend const vcvector nullcvector(unsigned);

  friend class vvector;
  friend class vmatrix;
  friend class vcmatrix;
};

const vcvector operator*(cdouble, const vcvector &);
std::ostream& operator<<(std::ostream& os, const vcvector &);
const vcvector nullcvector(unsigned);

// Vetores de dimensão não alterável

class fcvector : public vcvector
{
public:
  explicit fcvector(unsigned num=0): vcvector(num){};
  fcvector(vcvector &b): vcvector(b){};
  fcvector(const vcvector &b): vcvector(b){};
  //Alguns compiladores reclamam caso não existam as linhas a seguir
  fcvector(fcvector &b): vcvector(b){};
  fcvector(const fcvector &b): vcvector(b){};

  const fcvector &operator=(vcvector &);
  const fcvector &operator=(const vcvector &);
  void permutewith(vcvector&);
};

// Matrizes de dimensão alterável

class vcmatrix
{
private:
  static unsigned bufL[];
  static fcvector *bufx[];
  static unsigned buffirst,bufnum;
  unsigned L;
  fcvector *x;
  bool temp;
  MATRIX_STATUS create(unsigned, unsigned);
  void copy(const vcmatrix &);
  void move(vcmatrix &);
  void destruct(void);
  inline void maketemp(void) {temp = true;}
  MATRIX_STATUS LUinternal(vcmatrix &LU, vvector &indx,
			   double &d) const;
  void singularvalues(vcmatrix &U, vvector &W, vcmatrix &V) const;
public:
  explicit vcmatrix(unsigned dim=0);
  explicit vcmatrix(unsigned, unsigned);
  vcmatrix(vcmatrix &);
  vcmatrix(const vcmatrix &);
  vcmatrix(const vmatrix &);
  vcmatrix(const vcvector &);
  ~vcmatrix();
  
  const vcmatrix &operator=(vcmatrix &);
  const vcmatrix &operator=(const vcmatrix &);
  fcvector& operator[](unsigned);
  const fcvector operator[](unsigned) const;
  inline const vcmatrix &operator+(void) const {return *this;}
  const vcmatrix operator-(void) const;
  const vcmatrix operator+(const vcmatrix &) const;
  const vcmatrix operator-(const vcmatrix &) const;
  const vcmatrix operator*(cdouble) const;
  const vcmatrix operator/(cdouble) const;
  const vcmatrix operator+=(const vcmatrix &);
  const vcmatrix operator-=(const vcmatrix &);
  const vcmatrix operator*=(cdouble);
  const vcmatrix operator/=(cdouble);
  const vcmatrix operator*(const vcmatrix &) const;
  // O operador matriz1/matriz2 equivale a inv(matriz2)*matriz1.
  inline const vcmatrix operator/(const vcmatrix &b) const {
    return b.solve(*this);}
  const vcvector operator*(const vcvector &) const;
  const vcmatrix operator!(void) const;	// Transposta
  const vcmatrix operator~(void) const;	// Inversa

  inline unsigned numlines(void) const {return L;}
  inline unsigned numcolumns(void) const {return (x ? x[0].order() : 0);}
  const vcmatrix extract(unsigned lini, unsigned cini,
			 unsigned lfim, unsigned cfim) const;
  //inline const vcmatrix extract(unsigned lfim, unsigned cfim) const {
  //  return extract(0,0,lfim,cfim);}
  const vcmatrix appendright(const vcmatrix &) const;
  const vcmatrix appenddown(const vcmatrix &) const;
  void permutewith(vcmatrix&);
  void permutewith(fcmatrix&);
  void permuterows(unsigned, unsigned);
  void permutecolumns(unsigned, unsigned);
  cdouble max() const;
  cdouble min() const;
  double absmax() const;
  double absmin() const;
  cdouble sum() const;
  inline cdouble average() const {
    return sum()/(double)(numlines()*numcolumns());}
  double sum2() const;
  inline double euclid() const {return sqrt(sum2());}
  const vmatrix real() const;
  const vmatrix imag() const;
  const vcmatrix ident(unsigned, unsigned) const;
  inline const vcmatrix ident() const {return ident(numlines(),numcolumns());}
  cdouble determ() const;
  // O método a.solve(b) equivale a inv(b)*a
  // Baseia-se na decomposição LU
  const vcvector solve(const vcvector &b) const;
  const vcmatrix solve(const vcmatrix &b) const;
  // O método a.bestsolve(b) equivale a pseudoinv(b)*a
  // Baseia-se na decomposição em valores singulares
  const vcvector bestsolve(const vcvector &b) const;
  const vcmatrix bestsolve(const vcmatrix &b) const;
  // Decomposições
  void LU(vcmatrix &L, vcmatrix &U) const;
  void QR(vcmatrix &Q, vcmatrix &R) const;
  void LQ(vcmatrix &L, vcmatrix &Q) const;
  void SVD(vcmatrix &U, vvector &W, vcmatrix &V) const;
  // Autovalores
  const vcvector eigenvalues() const;

  friend const vcvector vcvector::operator*(const vcmatrix &) const;
  friend const vcmatrix operator*(cdouble, const vcmatrix &);
  friend std::ostream& operator<<(std::ostream& os, const vcmatrix &);
  friend const vcmatrix nullcmatrix(unsigned,unsigned);
  friend const vcmatrix cidentity(unsigned dim);

  friend class vvector;
  friend class vcvector;
  friend class vmatrix;
};

// Matrizes de dimensão não alterável

class fcmatrix : public vcmatrix
{
public:
  explicit fcmatrix(unsigned dim=0): vcmatrix(dim){};
  explicit fcmatrix(unsigned lin, unsigned col): vcmatrix(lin,col){};
  fcmatrix(vcmatrix& b): vcmatrix(b){};
  fcmatrix(vcmatrix const &b): vcmatrix(b){};
  // Alguns compiladores reclamam caso não existam as linhas a seguir
  fcmatrix(fcmatrix& b): vcmatrix(b){};
  fcmatrix(fcmatrix const &b): vcmatrix(b){};

  const fcmatrix &operator=(vcmatrix &);
  const fcmatrix &operator=(const vcmatrix &);
  void permutewith(vcmatrix&);
};

// FUNÇÕES INLINE
// Não podem ser declaradas nas classes vetor porque as
// classes matriz ainda não haviam sido declaradas

inline const vvector vvector::operator/(const vmatrix &b) const
{return b.solve(*this);}

inline const vcvector vcvector::operator/(const vcmatrix &b) const
{return b.solve(*this);}

inline const vvector vvector::operator%(const vmatrix &b) const
{return b.bestsolve(*this);}

/*#############################################################
##  REDECLARAÇÃO DE ALGUNS MÉTODOS COMO FUNÇÕES              ##
#############################################################*/

inline double sum2(const vvector &b) {return b.sum2();}
inline double euclid(const vvector &b) {return b.euclid();}
inline unsigned order(const vvector &b) {return b.order();}

inline double sum2(const vmatrix &b) {return b.sum2();}
inline double euclid(const vmatrix &b) {return b.euclid();}
inline double determ(const vmatrix &b) {return b.determ();}
inline unsigned numlines(const vmatrix &b) {return b.numlines();}
inline unsigned numcolumns(const vmatrix &b) {return b.numcolumns();}

inline const vvector real(const vcvector &b) {return b.real();}
inline const vvector imag(const vcvector &b) {return b.imag();}
inline double sum2(const vcvector &b) {return b.sum2();}
inline double euclid(const vcvector &b) {return b.euclid();}
inline unsigned order(const vcvector &b) {return b.order();}

inline const vmatrix real(const vcmatrix &b) {return b.real();}
inline const vmatrix imag(const vcmatrix &b) {return b.imag();}
inline double sum2(const vcmatrix &b) {return b.sum2();}
inline double euclid(const vcmatrix &b) {return b.euclid();}
inline cdouble determ(const vcmatrix &b) {return b.determ();}
inline unsigned numlines(const vcmatrix &b) {return b.numlines();}
inline unsigned numcolumns(const vcmatrix &b) {return b.numcolumns();}

#endif
