#include <iostream>
#include "imagem.h"

using namespace std;

int main()
{
  PxRGB p1,p2;
  PxYUV q1;

  unsigned N=0,T=0;
  for (unsigned r=0; r<=255; r+=5)
    for (unsigned g=0; g<=255; g+=5)
      for (unsigned b=0; b<=255; b+=5) {
	T++;
	p2 = q1 = p1 = PxRGB((uint8_t)r,(uint8_t)g,(uint8_t)b);
	if (p1 != p2) {
	  cout << "P1=" << p1
	       << "\tQ1=" << q1
	       << "\tP2=" << p2 << endl;
	  N++;
	}
      }
  cout << "Total=" << T << "\tN=" << N << endl;
}
