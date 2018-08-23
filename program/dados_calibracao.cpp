#include "dados_calibracao.h"
#include "functions.h"

using namespace std;

PARAMETROS_CALIBRACAO::PARAMETROS_CALIBRACAO() :
  nPontosNotaveis(0),
  nCores(0),
  pontosImagem(NULL),
  pontosReais(NULL),
  limiarPInf(0),
  limiarPSup(100),
  limHPG(NULL),
  campoVazio(),
  desvioPadrao(NULL),
  const_Field(0),
  const_Object(0)
{

}

PARAMETROS_CALIBRACAO::~PARAMETROS_CALIBRACAO(){
  delete[] pontosImagem;
  delete[] pontosReais;
  delete[] limHPG;
  if(desvioPadrao != NULL)delete[] desvioPadrao;
}

bool PARAMETROS_CALIBRACAO::isField(unsigned i, unsigned j, uint8_t byte){
  #define IN_RANGE(inf, sup, value) ((value) <= (sup) && (value) >= (inf))?true:false

  uint8_t infLimit = campoVazio.getByte(i,j) - const_Field*desvioPadrao[POS(i,j)];
  uint8_t supLimit = campoVazio.getByte(i,j) + const_Field*desvioPadrao[POS(i,j)];

  return IN_RANGE(infLimit, supLimit, byte);
}
bool PARAMETROS_CALIBRACAO::isObject(unsigned i, unsigned j, uint8_t byte){
  #define OUT_RANGE(inf, sup, value) ((value) > (sup) || (value) < (inf))?true:false

  uint8_t infLimit = campoVazio.getByte(i,j) - const_Object*desvioPadrao[POS(i,j)];
  uint8_t supLimit = campoVazio.getByte(i,j) + const_Object*desvioPadrao[POS(i,j)];

  return OUT_RANGE(infLimit, supLimit, byte);
}

int PARAMETROS_CALIBRACAO::isDiff(unsigned i, unsigned j, uint8_t byte){
  //eh campo
  if(isField(i,j,byte))
    return 0;
  //eh objeto
  if(isObject(i,j,byte))
    return 1;
  //incerteza
  return -1;
}

bool PARAMETROS_CALIBRACAO::read(const char* arquivo)
{
  unsigned nPontosNotaveis_aux;
  unsigned nCores_aux;
  Coord2 *pontos_aux(NULL);
  Coord2 *pontosReais_aux(NULL);
  int limiarPInf_aux;
  int limiarPSup_aux;
  limitesHPG *limHPG_aux(NULL);

  bool OK = true;
  FILE *arq = fopen(arquivo,"r");
  if(arq == NULL){
    return true;
  }

  if(fscanf(arq,"Numero de Pontos: %d\n",&nPontosNotaveis_aux) != 1 ) {
    OK = false;
  }
  else {
    pontos_aux = new Coord2[nPontosNotaveis_aux];
    pontosReais_aux = new Coord2[nPontosNotaveis_aux];

    if(fscanf(arq,"Pontos Notaveis (MundoX, MundoY, ImagemX, ImagemY):\n") != 0 ) {
      OK = false;
    }
    for(unsigned i = 0; i < nPontosNotaveis_aux; i++){
      if(OK && fscanf(arq,"%lf %lf %lf %lf\n",
		      &pontosReais_aux[i].X,
		      &pontosReais_aux[i].Y,
		      &pontos_aux[i].X,
		      &pontos_aux[i].Y) != 4){ OK = false; }
      else{
	pontosReais_aux[i].X = pontosReais_aux[i].x()/FATOR_CONVERSAO;
	pontosReais_aux[i].Y = pontosReais_aux[i].y()/FATOR_CONVERSAO;
      }
    }


    if(OK && fscanf(arq,"Limites de P Inferior e Superior:\n") != 0 ){ OK = false; }
    if(OK && fscanf(arq,"%d %d\n",&limiarPInf_aux,&limiarPSup_aux) != 2 ){ OK = false; }
    if(OK && fscanf(arq,"Numero de Cores: %d\n",&nCores_aux) != 1 ) {
      OK = false;
    }
    if(OK){
      limHPG_aux = new limitesHPG[nCores_aux];
      if(OK && fscanf(arq,"Limites HPG das Cores\n") != 0){ OK = false; }
      for(unsigned i = 0; i < nCores_aux; i++){
	if(OK && fscanf(arq,"%d %d %d %d %d %d\n",
			&(limHPG_aux[i].H.min),
			&(limHPG_aux[i].H.max),
			&(limHPG_aux[i].P.min),
			&(limHPG_aux[i].P.max),
			&(limHPG_aux[i].G.min),
			&(limHPG_aux[i].G.max)) != 6) { OK = false; }
      }
    }
  }
  fclose(arq);

  if(!OK) {
    if (pontos_aux != NULL) delete[] pontos_aux;
    if (pontosReais_aux != NULL) delete[] pontosReais_aux;
    if (limHPG_aux != NULL) delete[] limHPG_aux;
    return true;
  }

  nPontosNotaveis = nPontosNotaveis_aux;
  nCores = nCores_aux;
  /*
    if(pontosImagem != NULL)
    delete pontosImagem;
    if(pontosReais != NULL)
    delete pontosReais;
  */
  pontosImagem = new Coord2[nPontosNotaveis];
  pontosReais =  new Coord2[nPontosNotaveis];
  for(unsigned i = 0; i < nPontosNotaveis_aux; i++){
    pontosImagem[i] = pontos_aux[i];
    pontosReais[i] = pontosReais_aux[i];
  }
  limiarPInf = limiarPInf_aux;
  limiarPSup = limiarPSup_aux;
  /*
    if(limHPG != NULL)
    delete limHPG;
  */
  limHPG = new limitesHPG[nCores];
  for(unsigned i = 0; i < nCores_aux; i++){
    limHPG[i] = limHPG_aux[i];
  }
  if (pontos_aux != NULL) delete[] pontos_aux;
  if (pontosReais_aux != NULL) delete[] pontosReais_aux;
  if (limHPG_aux != NULL) delete[] limHPG_aux;




  return false;
}
bool PARAMETROS_CALIBRACAO::write(const char* arquivo) const{

  FILE *arq=fopen(arquivo,"w");
  if(arq == NULL) return true;

  fprintf(arq,"Numero de Pontos: %d\n",nPontosNotaveis);
  fprintf(arq,"Pontos Notaveis (MundoX, MundoY, ImagemX, ImagemY):\n");
  for(unsigned i = 0; i < nPontosNotaveis; i++){
    fprintf(arq,"%d %d %d %d\n",
	    (int)(pontosReais[i].x()*FATOR_CONVERSAO),
	    (int)(pontosReais[i].y()*FATOR_CONVERSAO),
	    (int)pontosImagem[i].u(),
	    (int)pontosImagem[i].v());
  }
  fprintf(arq,"Limites de P Inferior e Superior:\n");
  fprintf(arq,"%d %d\n",limiarPInf,limiarPSup);
  fprintf(arq,"Numero de Cores: %d\n",nCores);
  fprintf(arq,"Limites HPG das Cores\n");
  for(unsigned i = 0; i < nCores; i++){
    fprintf(arq,"%d %d %d %d %d %d\n",
	    limHPG[i].H.min,
	    limHPG[i].H.max,
	    limHPG[i].P.min,
	    limHPG[i].P.max,
	    limHPG[i].G.min,
	    limHPG[i].G.max);
  }
  fclose(arq);
  return false;
}
bool PARAMETROS_CALIBRACAO::read(std::istream &I){
  unsigned nPontosNotaveis_aux;
  unsigned nCores_aux;
  Coord2 *pontos_aux(NULL);
  Coord2 *pontosReais_aux(NULL);
  int limiarPInf_aux;
  int limiarPSup_aux;
  limitesHPG *limHPG_aux(NULL);

  std::string str;
  bool OK = true;

  getline(I,str,':');
  if(str != "Numero de Pontos")return true;
  I >> nPontosNotaveis_aux;

  pontos_aux = new Coord2[nPontosNotaveis_aux];
  pontosReais_aux = new Coord2[nPontosNotaveis_aux];

  getline(I,str,':');
  if(str != "\nPontos Notaveis (MundoX, MundoY, ImagemX, ImagemY)")OK = false;

  for(unsigned i = 0; i < nPontosNotaveis_aux; i++){
    I >> pontosReais_aux[i].X;
    I >> pontosReais_aux[i].Y;
    I >> pontos_aux[i].X;
    I >> pontos_aux[i].Y;

    pontosReais_aux[i].X = pontosReais_aux[i].x()/FATOR_CONVERSAO;
  	pontosReais_aux[i].Y = pontosReais_aux[i].y()/FATOR_CONVERSAO;
  }

  getline(I,str,':');
  if(OK && str != "\nLimites de P Inferior e Superior")OK = false;
  I >> limiarPInf_aux >> limiarPSup_aux;

  getline(I,str,':');
  if(OK && str != "\nNumero de Cores")OK = false;
  I >> nCores_aux;

  if(OK){
    limHPG_aux = new limitesHPG[nCores_aux];
    I.ignore(1,'\n');
    getline(I,str,'\n');
    if(OK && str != "Limites HPG das Cores")OK = false;
    for(unsigned i = 0; i < nCores_aux && OK; i++){
      I >> limHPG_aux[i].H.min >> limHPG_aux[i].H.max;
      I >> limHPG_aux[i].P.min >> limHPG_aux[i].P.max;
      I >> limHPG_aux[i].G.min >> limHPG_aux[i].G.max;
    }
  }

  if(!OK) {
    if (pontos_aux != NULL) delete[] pontos_aux;
    if (pontosReais_aux != NULL) delete[] pontosReais_aux;
    if (limHPG_aux != NULL) delete[] limHPG_aux;
    return true;
  }

  //leitura dos dados do campo Vazio
  I.ignore(1,'\n');
  getline(I,str,'\n');
  if(str != "Constantes Objeto e Campo")return true;
  I >> const_Object >> const_Field;

  if(campoVazio.read("../.campoVazio") == false)return true;//falha na leitura da imagem do campo vazio
    //leitura da matriz de desvio padrao
  ifstream desvio("../.desvioPadrao");
  unsigned length;

  if(desvio.is_open() == false)return true;
  getline(desvio,str,':');
  desvio >> length;
  if(desvioPadrao != NULL)delete[] desvioPadrao;
  desvioPadrao = new uint8_t[length];

  I.ignore(1,'\n');

  char tmp;
  for(unsigned i = 0; i < length; i ++){
    desvio.get(tmp);
    desvioPadrao[i] = tmp;
  }
  if(str != "Length")return true;

  desvio.close();
  //fim da leitura dos dados do campo vazio
  nPontosNotaveis = nPontosNotaveis_aux;
  nCores = nCores_aux;

  pontosImagem = new Coord2[nPontosNotaveis];
  pontosReais =  new Coord2[nPontosNotaveis];
  for(unsigned i = 0; i < nPontosNotaveis_aux; i++){
    pontosImagem[i] = pontos_aux[i];
    pontosReais[i] = pontosReais_aux[i];
  }
  limiarPInf = limiarPInf_aux;
  limiarPSup = limiarPSup_aux;

  limHPG = new limitesHPG[nCores];
  for(unsigned i = 0; i < nCores_aux; i++){
    limHPG[i] = limHPG_aux[i];
  }
  if (pontos_aux != NULL) delete[] pontos_aux;
  if (pontosReais_aux != NULL) delete[] pontosReais_aux;
  if (limHPG_aux != NULL) delete[] limHPG_aux;



  return false;
}
std::ostream &PARAMETROS_CALIBRACAO::write(std::ostream &O)const{

  O << "Numero de Pontos: " << nPontosNotaveis << '\n';
  O << "Pontos Notaveis (MundoX, MundoY, ImagemX, ImagemY):\n";
  for(unsigned i = 0; i < nPontosNotaveis; i++){
    O << (int)(pontosReais[i].x()*FATOR_CONVERSAO) << ' '
      << (int)(pontosReais[i].y()*FATOR_CONVERSAO) << ' '
      << (int)pontosImagem[i].u()  << ' '
      << (int)pontosImagem[i].v() << '\n';
  }

  O << "Limites de P Inferior e Superior:\n";
  O << limiarPInf << ' ' << limiarPSup << '\n';
  O << "Numero de Cores: "<< nCores << '\n';
  O << "Limites HPG das Cores\n";

  for(unsigned i = 0; i < nCores; i++){
    O << limHPG[i].H.min << ' '
      << limHPG[i].H.max << ' '
      << limHPG[i].P.min << ' '
      << limHPG[i].P.max << ' '
      << limHPG[i].G.min << ' '
      << limHPG[i].G.max << '\n';
  }
  // dados referentes ao campo Vazio.
  campoVazio.write("../.campoVazio");
  ofstream desvio("../.desvioPadrao");
  unsigned length = campoVazio.getLength();
  desvio << "Length: "<<length<<'\n';
  for(unsigned i = 0; i < campoVazio.getLength(); i++)
    desvio << desvioPadrao[i];
  desvio.close();

  O << "Constantes Objeto e Campo"<< '\n';
  O << const_Object <<'\t'<< const_Field;

  return O;
}
//retorna a cor da qual o pixel do parametro percente. Usa somente os
//limiares dos componentes HPG para definir a cor do pixel.
int PARAMETROS_CALIBRACAO::getHardColor(
  const float H,
  const float P,
  const float G) const
{
  bool H_OK;
  // float H,P,G;
  // p.getHPG(H,P,G);

  int iH = (int)round((H/M_PI)*180.0),
    iP = (int)round(P*100.0),
    iG = (int)round(G*100.0);

  for( unsigned k = 0; k < nCores; k++){
    H_OK = false;
    if( limHPG[k].H.min <= limHPG[k].H.max ){
      if(iH >= limHPG[k].H.min && iH <= limHPG[k].H.max){
	       H_OK = true;
      }
    }else{
      if(iH >= limHPG[k].H.min || iH <= limHPG[k].H.max){
	       H_OK = true;
      }
    }

    if(H_OK &&
       iP >= limHPG[k].P.min && iP <= limHPG[k].P.max &&
       iG >= limHPG[k].G.min && iG <= limHPG[k].G.max){
      return k;
    }
  }
  return -1;

}
//metodo para pegar o rotulo baseado nas componentes do pixel. Usa o
//calculo da distancia caso o pixel nao se encontre dentro do limiar
//das componentes.
int PARAMETROS_CALIBRACAO::getSoftColor(
  const float H,
  const float P,
  const float G) const
{

  int min_ID = getHardColor(H,P,G);
  if(min_ID != -1)
    return min_ID;

  bool H_OK;
  // float H,P,G;
  // p.getHPG(H,P,G);
  float distH, fHmin, fHmax, fHmean, fHstd;
  float meanP, stdP, distP;
  float meanG, stdG, distG;

  float dist;
  float min_dist = 400.0; // min_dist inicial > sqrt(360*360 + 100*100 + 100*100);

  int iH = (int)round((H/M_PI)*180.0),
    iP=(int)round(P*100.0),
    iG=(int)round(G*100.0);

  for( unsigned k = 0; k < nCores; k++){
    H_OK = false;
    //testa se o H esta dentro dos limiares
    if( limHPG[k].H.min <= limHPG[k].H.max ){
      if(iH >= limHPG[k].H.min && iH <= limHPG[k].H.max){
      	H_OK = true;
      	distH = 0;
      }
    }else{
      if(iH >= limHPG[k].H.min || iH <= limHPG[k].H.max){
      	H_OK = true;
      	distH = 0;
      }
    }
    //se o H estiver fora dos limiares, ja calcula a distH
    if(!H_OK){
      fHmin = (((float)limHPG[k].H.min)/180.0)*M_PI;
      fHmax = (((float)limHPG[k].H.max)/180.0)*M_PI;

      fHmean = (ang_equiv(fHmax + fHmin))/2;
      fHstd = ang_equiv(fHmax - fHmean);
      distH = min(ang_equiv(H-fHmin),
		  ang_equiv(H-fHmax))/fHstd;
    }

    if(iP >= limHPG[k].P.min && iP <= limHPG[k].P.max){
      distP = 0;
    }else{
      if(limHPG[k].P.min < limHPG[k].P.max){
      	meanP = (limHPG[k].P.min + limHPG[k].P.max)/2;
      	stdP = limHPG[k].P.max - meanP;
    	   if(iP > limHPG[k].P.max){
    	      distP = (iP - limHPG[k].P.max)/stdP;
    	   }else if(iP < limHPG[k].P.min){
    	      distP = (limHPG[k].P.min - iP)/stdP;
    	   }
      }else{
      	distP = 100;
      }
    }

    if(iG >= limHPG[k].G.min && iG <= limHPG[k].G.max){
      distG = 0;
    }else{
      if(limHPG[k].G.min < limHPG[k].G.max){
      	meanG = (limHPG[k].G.min + limHPG[k].G.max)/2;
      	stdG = limHPG[k].G.max - meanG;
      	if(iG > limHPG[k].G.max){
      	  distG = (iG - limHPG[k].G.max)/stdG;
      	}else if(iG < limHPG[k].G.min){
      	  distG = (limHPG[k].G.min - iG)/stdG;
      	}
      }else{
      	distG = 100;
      }
    }
    //calcula a distancia geral
    dist = sqrt(distH*distH + distP*distP + distG*distG);

    //verifica se a distancia está abaixo do threshold de 1 desvio
    //padrao e se a distancia para a cor é menor do que a menor
    //distancia encontrada
    if(dist <= 1.0 && dist < min_dist){
      min_dist = dist;
      min_ID = k;
    }
  }
  //retorna a cor de menor distancia. Retorna -1 (cor indefinida)
  //quando nao encontrou nenhuma cor próxima o suficiente.
  return min_ID;
}
