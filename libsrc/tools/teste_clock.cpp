#include <iostream>

#include "system_tools.h" //usa o que esta na pasta atual (para testar antes de instalar)
// #include <system_tools.h> //usa o que esta na pasta lib (para testar apos instalar)


int main(){
  std::cout << "Aguardando 5s e 30ms" << '\n';
  double start = tools::clock();
  while( (tools::clock() - start) < 5.03){}
  double end = tools::clock();

  std::cout << "Tempo gasto decorrido:\t" << end - start <<'\n';
  std::cout << "teste getData\t"<<tools::getData() << '\n';
  return 0;
}
