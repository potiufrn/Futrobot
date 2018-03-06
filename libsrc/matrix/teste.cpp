#include <stdlib.h>
#include <time.h>
#include "matrix.h"

double rnd(void)
{
  double prov = 2.0*drand48()-1.0;
  return(prov/(1-prov*prov));
}

using std::cout;
using std::endl;

#define ORDEM 3

int main(void)
{
  fmatrix A(ORDEM,ORDEM+1);
  vmatrix U,V,W2;
  vvector W;

  srand48(time(NULL));
  for (unsigned i=0; i<A.numlines(); i++) {
    for (unsigned j=0; j<A.numcolumns(); j++)
      {
	A[i][j] = rnd();
	//A[i][j] = (i>j ? 0.0 : rnd());
	//A[i][j] = cdouble(rnd(),rnd());
	//A[i][j] = ORDEM*i+j+drand48()/10;
      }
  }
  cout << "A = " << A << endl;

  A.SVD(U,W,V);

  /*
  W2 = nullmatrix(W.order(),W.order());
  for (unsigned i=0; i<W.order(); i++) W2[i][i] = W[i];

  cout << "U = " << U << endl;
  cout << "W = " << W << endl;
  cout << "V = " << V << endl;
  cout << "U*W*Vt = " << U*W2*!V << endl;
  */
  return(0);
}
