#include <iostream>
#include <list>
#include <cmath>

#include "camera.h"


using namespace std;

typedef list<ImagemRGB>::iterator iter;

int main()
{
  // Camera cap(CAM_MIXREAL);
  // cap.capturando = true;
  //
  // for(unsigned i = 0; i < 50; i++)
  // {
  //   cap.waitforimage();
  //   cap.captureimage();
  // }
  //cap.ImBruta.save("Teste_Buffer.ppm");
   /*
  Camera cap(CAM_MIXREAL);
  unsigned NumAmostras = 100;
  list<ImagemRGB> amostras;

  float temp;


  ImagemRGB Media(0,0);
  ImagemRGB Desvio(0,0);

  cap.capturando = true;

  cap.waitforimage();
  cap.captureimage();
  amostras.push_back(cap.ImBruta);
  Media = cap.ImBruta;

  //Calcula a Imagem Media
  // e preenche uma lista de amostras
  for(unsigned i = 1; i < NumAmostras; i++)
  {
    cap.waitforimage();
    cap.captureimage();
    amostras.push_back(cap.ImBruta);

    // Media = (Media + NovaCaptura)/2
    for(unsigned c = 0; c < cap.ImBruta.ncol(); c ++)for(unsigned l = 0; l < cap.ImBruta.nlin(); l ++)
    {
      temp = Media[l][c].r;
      temp += cap.ImBruta[l][c].r;
      temp = temp/2.0;
      Media[l][c].r = temp;

      temp = Media[l][c].g;
      temp += cap.ImBruta[l][c].g;
      temp = temp/2.0;
      Media[l][c].g = temp;

      temp = Media[l][c].b;
      temp += cap.ImBruta[l][c].b;
      temp = temp/2.0;
      Media[l][c].b = temp;
    }
  }


  Media.save("Campo_Medio.ppm");
  amostras.back().save("primeira_Amostra.ppm");
  */
  /*
  cout << "Matriz de diferenca"<<endl;
  for(unsigned c = 0; c < 10; c ++)for(unsigned l = 0; l < 10; l ++)
  {
    cout << Media[l][c].r - cap.ImBruta[l][c].r <<" ";
    cout << Media[l][c].g - cap.ImBruta[l][c].g << " ";
    cout << Media[l][c].b - cap.ImBruta[l][c].b << " ";
  }
  */

  /*
  ImagemRGB sum(cap.ImBruta.ncol(),cap.ImBruta.nlin());

  cout << "Tamanho de ImBruta " << cap.ImBruta.nlin() << "  " << cap.ImBruta.ncol()<<endl;
  cout << "Tamanho de Sum " << sum.nlin() << "  " << sum.ncol()<<endl;

  // Desvio = sum (xi - x')²/N;
  //sum(xi - x)²
  for(iter i = amostras.begin(); i != amostras.end(); i++)
  {

    //sum = sum + (i - Media)²
    for(unsigned c = 0; c < cap.ImBruta.ncol(); c ++)for(unsigned l = 0; l < cap.ImBruta.nlin(); l ++)
    {

      temp = (double)(*i)[l][c].r - (float)Media[l][c].r;
      temp = temp*temp;
      sum[l][c].r = temp + sum[l][c].r;

      temp = (double)(*i)[l][c].g - (float)Media[l][c].g;
      temp = temp*temp;
      sum[l][c].g = temp + sum[l][c].g;

      temp = (double)(*i)[l][c].b - (float)Media[l][c].b;
      temp = temp*temp;
      sum[l][c].b = temp + sum[l][c].b;

    }
  }

  //sum = sqr(sum/N)
  for(unsigned c = 0; c < cap.ImBruta.ncol(); c ++)for(unsigned l = 0; l < cap.ImBruta.nlin(); l ++)
  {
    temp = sum[l][c].r;
    temp = sqrt(temp/NumAmostras);
    sum[l][c].r = temp;

    temp = sum[l][c].g;
    temp = sqrt(temp/NumAmostras);
    sum[l][c].g = temp;

    temp = sum[l][c].b;
    temp = sqrt(temp/NumAmostras);
    sum[l][c].b = temp;
  }

  Desvio = sum;

  Desvio.save("DesvioPadrao.ppm");


  cout << "Desvio Padrao"<<endl;
  for(unsigned c = 0; c < 10; c ++)for(unsigned l = 0; l < 10; l ++)
  {

    cout << (int)Desvio[l][c].r << (int)Desvio[l][c].g << (int)Desvio[l][c].b << " ";
    if(c%10 == 0) cout << endl;
  }
  */



  return 0;
}
