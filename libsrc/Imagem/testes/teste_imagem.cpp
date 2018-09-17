#include <iostream>
#include "imagem.h"

int main(){
  Imagem I;
  Imagem I2;
  float H = 0,P = 0,G = 0;

  I.resize(100,100);

  for(unsigned lin = 0; lin < 100; lin ++)
  for(unsigned col = 0; col < 100; col++)
    I.atRGB(lin,col) = PxRGB(250,0,0);

  I.getHPG(0,0,H,P,G);

  for(unsigned lin = 0; lin < 100; lin ++)
  for(unsigned col = 0; col < 100; col++)
    I.setHPG(lin,col,H,P,G);

  I.save("teste_imagem.ppm");
  I2 = I;
  I2.save("imagem_2_teste.ppm");

  std::cout << "H P G "<< H <<"  "<< P <<"  "<< G << '\n';
  return 0;
}
