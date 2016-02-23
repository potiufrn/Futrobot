#include <stdlib.h>
#include <time.h>
#include "matrix.h"

using namespace std;

int main(void)
{
  unsigned i,j;
  cout << "Numero de linhas:\n";
  cin >> i;
  cout << "Numero de colunas:\n";
  cin >> j;

  fmatrix a(i,j);
  fvector b(i);

  for (i=0; i<a.numlines(); i++) {
    for (j=0; j<a.numcolumns(); j++) {
      cout << "A[" << i << "][" << j << "] =\n";
      cin >> a[i][j];
    }
    cout << "b[" << i << "] = \n";
    cin >> b[i];
  }

  cout << "Solução = " << b%a << endl;

  return(0);
}
