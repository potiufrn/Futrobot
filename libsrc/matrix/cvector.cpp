#define TIPODADO cdouble
#define TIPOVETOR vcvector
#define TIPOFVETOR fcvector
#define TIPOMATRIZ vcmatrix

#include "tvector.cpp"

vcvector::vcvector(const vvector &b)
{
  if (!create(b.order())) {
    for (unsigned i=0; i<order(); i++) {
      x[i] = cdouble(b.x[i]);
    }
  }
}

const vvector vcvector::real() const
{
  vvector prov(order());
  for (unsigned i=0; i<prov.order(); i++) {
    prov.x[i] = x[i].real();
  }
  prov.maketemp();
  return(prov);
}

const vvector vcvector::imag() const
{
  vvector prov(order());
  for (unsigned i=0; i<prov.order(); i++) {
    prov.x[i] = x[i].imag();
  }
  prov.maketemp();
  return(prov);
}

const vcvector nullcvector(unsigned n)
{
  vcvector prov(n);
  for (unsigned i=0; i<prov.order(); i++) {
    prov.x[i] = cdouble(0.0,0.0);
  }
  prov.maketemp();
  return(prov);
}
