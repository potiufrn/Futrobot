#define TIPODADO cdouble
#define TIPOVETOR vcvector
#define TIPOFVETOR fcvector
#define TIPOMATRIZ vcmatrix
#define TIPOFMATRIZ fcmatrix

#include "tmatrix.cpp"

TIPOMATRIZ::TIPOMATRIZ(const vmatrix &b)
{
  if (!create(b.numlines(),b.numcolumns())) {
    for (unsigned i=0; i<numlines(); i++) {
      x[i] = fcvector(b.x[i]);
    }
  }
}

const vmatrix TIPOMATRIZ::real() const
{
  vmatrix prov(numlines(),numcolumns());
  for (unsigned i=0; i<prov.numlines(); i++) {
    prov.x[i] = x[i].real();
  }
  prov.maketemp();
  return(prov);
}

const vmatrix TIPOMATRIZ::imag() const
{
  vmatrix prov(numlines(),numcolumns());
  for (unsigned i=0; i<prov.numlines(); i++) {
    prov.x[i] = x[i].imag();
  }
  prov.maketemp();
  return(prov);
}

const TIPOMATRIZ nullcmatrix(unsigned lin, unsigned col)
{
  TIPOMATRIZ prov(lin,col);
  for (unsigned i=0; i<prov.numlines(); i++) {
    prov.x[i] = nullcvector(col);
  }
  prov.maketemp();
  return(prov);
}

const TIPOMATRIZ cidentity(unsigned dim)
{
  TIPOMATRIZ prov(dim,dim);
  for (unsigned i=0; i<prov.numlines(); i++)
    for (unsigned j=0; j<prov.numcolumns(); j++)
      prov.x[i][j] = (i==j ? (TIPODADO)1.0 : (TIPODADO)0.0);
  prov.maketemp();
  return(prov);
}

// Decomposição em valores singulares

void vcmatrix::singularvalues(vcmatrix &U, vvector &W, vcmatrix &V) const
{
  matrix_error_int(MATRIX_OTHER);
}
