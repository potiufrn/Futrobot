#define TIPODADO double
#define TIPOVETOR vvector
#define TIPOFVETOR fvector
#define TIPOMATRIZ vmatrix

#include "tvector.cpp"

const vvector nullvector(unsigned n)
{
  vvector prov(n);
  for (unsigned i=0; i<prov.order(); i++) {
    prov.x[i] = (double)0.0;
  }
  prov.maketemp();
  return(prov);
}
