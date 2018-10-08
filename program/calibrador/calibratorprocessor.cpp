#include "calibratorprocessor.h"
#include "variaveisglobais.h"
#include <iostream>
#include <cmath>
#include <cstring>

// Apenas para teste
#include <fstream>

#define RAIO_SELECIONADO 5.0
// #define NUM_BUFFERS 4
// //#define NUM_BUFFERS 10
// #define ISO_SPEED DC1394_ISO_SPEED_400
// #define FRAME_RATE DC1394_FRAMERATE_30
// //#define DEBAYER_METHOD DC1394_BAYER_METHOD_NEAREST
// #define DEBAYER_METHOD DC1394_BAYER_METHOD_BILINEAR
// #define VIDEO_MODE DC1394_VIDEO_MODE_640x480_MONO8

CalibratorProcessor::CalibratorProcessor() :
  Camera(),
  ImProcessada(0,0),
  modo(CALIBRATOR_IMAGEM_REAL),

  nRetas(0),
  LarguraCaptura(0),
  AlturaCaptura(0),

  pontosImagemIniciais(NULL),

  retas(NULL),

  nomeCor(NULL),
  cores(NULL),
  coresInversas(NULL),

  corAtual(0),
  offset_u(0),
  offset_v(0),
  true_color(false),
  campoVazio_capturado(false),
  calculating(false)
{
  //nada
}

CalibratorProcessor::CalibratorProcessor(const char* arquivo) :
  Camera(),
  ImProcessada(0,0),
  modo(CALIBRATOR_IMAGEM_REAL),

  nRetas(0),
  LarguraCaptura(0),
  AlturaCaptura(0),

  pontosImagemIniciais(NULL),

  retas(NULL),

  nomeCor(NULL),
  cores(NULL),
  coresInversas(NULL),

  corAtual(0),
  offset_u(0),
  offset_v(0),
  true_color(false),
  campoVazio_capturado(false),
  calculating(false)
{
  calibracaoParam.limiarPSup = 100;
  calibracaoParam.limiarPInf = 0;
  if(readFile(arquivo)){
    exit(1);
  }
}

CalibratorProcessor::~CalibratorProcessor(){
  Camera::terminar(); // e' da Camera
  delete[] cores;
  delete[] coresInversas;
  delete[] retas;
  delete[] pontosImagemIniciais;
}

bool CalibratorProcessor::readFile(const char* arquivo)
{
  FILE *f;
  if(strlen(arquivo) == 0){
    f = fopen("../../etc/calibradorConfig.conf","r");
    //f = fopen("calibradorConfig.conf","r");
  }else{
    f = fopen(arquivo,"r");
  }
  if(f == NULL) return true;
  int true_color_local;
  if(fscanf(f,"TAMANHO DA IMAGEM:\n(Width Height TrueColor?)\n%d %d %d\n\n",
	    &LarguraCaptura,&AlturaCaptura,&true_color_local) != 3){
    fclose(f);
    return true;
  }
  true_color = (bool)true_color_local;
  // ImBruta = Imagem(LarguraCaptura,AlturaCaptura);
  // ImProcessada = Imagem(LARGURA_EXIBICAO,ALTURA_EXIBICAO);

  if(fscanf(f,"NUMERO DE PONTOS: %d\n(Xw Yw Xi Yi)\n",&calibracaoParam.nPontosNotaveis) != 1){
    fclose(f);
    return true;
  }
  calibracaoParam.pontosImagem = new Coord2[calibracaoParam.nPontosNotaveis];
  pontosImagemIniciais  = new Coord2[calibracaoParam.nPontosNotaveis];
  calibracaoParam.pontosReais = new Coord2[calibracaoParam.nPontosNotaveis];

  for(unsigned i=0;i<calibracaoParam.nPontosNotaveis;i++){
    if(fscanf(f,"%lf %lf %lf %lf\n",
	      &calibracaoParam.pontosReais[i].x(),
	      &calibracaoParam.pontosReais[i].y(),
	      &calibracaoParam.pontosImagem[i].u(),
	      &calibracaoParam.pontosImagem[i].v()) != 4){
      fclose(f);
      return true;
    }
    pontosImagemIniciais[i] = calibracaoParam.pontosImagem[i];
  }

  if(fscanf(f,"\nNUMERO DE RETAS: %d\n(p1 p2)\n",&nRetas) != 1){
    fclose(f);
    return true;
  }

  retas = new RETA[nRetas];

  for(unsigned i=0;i<nRetas;i++){
    if(fscanf(f,"%d %d\n",&retas[i].p1,&retas[i].p2) != 2){
      fclose(f);
      return true;
    }
  }

  if(fscanf(f,"\nNUMERO DE CORES: %d\n(strLen Nome R G B)\n",&calibracaoParam.nCores) != 1){
    fclose(f);
    return true;
  }

  calibracaoParam.limHPG = new limitesHPG[calibracaoParam.nCores];
  nomeCor = new char*[calibracaoParam.nCores];
  cores = new PxRGB[calibracaoParam.nCores];
  coresInversas = new PxRGB[calibracaoParam.nCores];

  int str_len;
  unsigned R,G,B;
  for(unsigned i=0; i < calibracaoParam.nCores; i++){
    if(fscanf(f,"%d ",&str_len) != 1){
      fclose(f);
      return true;
    }
    nomeCor[i] = new char[str_len+1];
    if(fscanf(f,"%s %u %u %u\n",
	      nomeCor[i],
	      &R, &G, &B) != 4){
      fclose(f);
      return true;
    }
    cores[i].r = (uint8_t)R;
    cores[i].g = (uint8_t)G;
    cores[i].b = (uint8_t)B;
    coresInversas[i].r = 255 - cores[i].r;
    coresInversas[i].g = 255 - cores[i].g;
    coresInversas[i].b = 255 - cores[i].b;

  }

  fclose(f);
  resetHPG();

  return false;
}

bool CalibratorProcessor::saveCameraParam(const char* arquivo){
  return Camera::write(arquivo);
}

bool CalibratorProcessor::loadCameraParam(const char* arquivo){
  return Camera::read(arquivo);
}

bool CalibratorProcessor::fileOpen(const char* text)
{
  std::ifstream I(text);
  std::string str;

  if(!I.is_open())return true;

  getline(I,str,'\n');
  if(str != "Parametros da Camera"){
    I.close();
    return true;
  }
  Camera::read(I);
  I.ignore(1,'\n');

  getline(I,str,'\n');
  if(str != "Parametros de Calibracao"){
    I.close();
    return true;
  }
  if (calibracaoParam.read(I))return true;
  I.close();

  campoVazio_capturado = true;

  return false;
}


bool CalibratorProcessor::fileSave(const char* arquivo)
{
  if(!campoVazio_capturado)return true;
  std::ofstream O(arquivo);
  if(!O.is_open())return true;
  O << "Parametros da Camera\n";
  Camera::write(O);
  O << "\nParametros de Calibracao\n";
  calibracaoParam.write(O);
  O.close();
  return false;
}

void CalibratorProcessor::resetPixelsNotaveis(){
  for(unsigned i = 0; i < calibracaoParam.nPontosNotaveis; i++){
    calibracaoParam.pontosImagem[i] = pontosImagemIniciais[i];
  }
}

void CalibratorProcessor::resetHPG(){
  calibracaoParam.limiarPInf = PG_MIN_VALUE;
  calibracaoParam.limiarPSup = PG_MAX_VALUE;
  for(unsigned i = 0; i < calibracaoParam.nCores; i++){
    calibracaoParam.limHPG[i].H.min = H_MIN_VALUE;
    calibracaoParam.limHPG[i].H.max = H_MAX_VALUE;
    calibracaoParam.limHPG[i].P.min = PG_MIN_VALUE;
    calibracaoParam.limHPG[i].P.max =  PG_MAX_VALUE;
    calibracaoParam.limHPG[i].G.min = PG_MIN_VALUE;
    calibracaoParam.limHPG[i].G.max =  PG_MAX_VALUE;
  }
}

bool CalibratorProcessor::loadImage(const char* arq){
  return ImProcessada.load(arq);
}

void CalibratorProcessor::saveImage(const char* arq){
  ImProcessada.save(arq);
}


bool CalibratorProcessor::processImage(){
  #define IS_VALID(i,j) (( (i)<ImBruta.getHeight() && (j)<ImBruta.getWidth())?true:false)
  unsigned int i,j;
  int count=0, cor_pixel=0;
  float H, P, G;
  int r;
  unsigned qtdDiff;
  ImagemBruta tmp(calibracaoParam.campoVazio.getWidth(),calibracaoParam.campoVazio.getHeight());
  calibracaoParam.campoVazio.copyTo(tmp);
  //Cores padroes a serem usadas no processamento
  static PxRGB PxPreto(0,0,0);
  static PxRGB PxVermelho(255,0,0);
  static PxRGB PxCinza(127,127,127);
  static PxRGB PxBranco(255,255,255);

  switch(modo){
  case CALIBRATOR_IMAGEM_REAL:
    //TODO a ideia deste modo eh utilizar os sliders da GUI para selecionar a
    // regiao da imagem que sera processada, ou seja, um corte da imagem via software
    // for(i = 0; i < ImProcessada.getHeight(); i++){
    //   for(j = 0; j < ImProcessada.getWidth(); j++){
    //     // ImProcessada[i][j] = ImBruta[offset_v + i][offset_u + j];
    //     ImProcessada.atRGB(i,j) = ImBruta.atRGB(offset_v + i, offset_u + j);
    //   }
    // }
    //WARNING esse modo esta incompleto, pois nao esta habilitado a
    //funcao de corte da imagem
    ImProcessada = ImBruta;
    break;
  case CALIBRATOR_LIMITES_P_E_PONTOS:
  case CALIBRATOR_LIMITES_P:
    //TODO gera a imagem processada com os limites min e max de P
    //WARNING este modo nao esta completo, por isso apenas esta apenas
    //processando a imagem Real
    if(campoVazio_capturado)
      ImProcessada = calibracaoParam.campoVazio;
    else
      ImProcessada = ImBruta;
    break;
  case CALIBRATOR_PONTOS:
    //desenha os pontos
    // ImProcessada = ImBruta;
    if(campoVazio_capturado)
      ImProcessada = calibracaoParam.campoVazio;
    else
      ImProcessada = ImBruta;
    int jj,kk;
    for(i = 0; i < calibracaoParam.nPontosNotaveis; i++){
      for(jj = -2; jj <=2; jj++){
      	for(kk = -2; kk <=2; kk++){
      	  if((calibracaoParam.pontosImagem[i].u() + jj - offset_u) >= 0 &&
      	     (calibracaoParam.pontosImagem[i].u() + jj - offset_u) < ImProcessada.getWidth() &&
      	     (calibracaoParam.pontosImagem[i].v() + kk - offset_v) >= 0 &&
      	     (calibracaoParam.pontosImagem[i].v() + kk - offset_v) < ImProcessada.getHeight()){
      	    ImProcessada[(int)round(calibracaoParam.pontosImagem[i].v() + kk-offset_v)][(int)round(calibracaoParam.pontosImagem[i].u() + jj - offset_u)] = PxVermelho;
      	  }
      	}
      }
    }

    //desenha as retas
    double dU,dV,dM;
    unsigned u,v,j;
    for(i=0;i<nRetas;i++){
      dU = calibracaoParam.pontosImagem[retas[i].p2].u() -
	    calibracaoParam.pontosImagem[retas[i].p1].u();
      dV = calibracaoParam.pontosImagem[retas[i].p2].v() -
	    calibracaoParam.pontosImagem[retas[i].p1].v();
      dM = round(std::max(fabs(dU),fabs(dV)));
      for(j=0;j<(unsigned)dM;j++){
      	u = (unsigned)round(calibracaoParam.pontosImagem[retas[i].p1].u() + (dU/dM)*j) -offset_u;
      	v = (unsigned)round(calibracaoParam.pontosImagem[retas[i].p1].v() + (dV/dM)*j) -offset_v;
      	if(u < ImProcessada.getWidth()  &&
      	   v < ImProcessada.getHeight()) {
      	  ImProcessada[v][u] = PxVermelho;
      	}
      }
    }
    break;
  //sem campo vazio
  case CALIBRATOR_COR_ETIQUETADA:
    for(i = 0; i < ImProcessada.getHeight(); i++){
      for(j = 0; j < ImProcessada.getWidth(); j++){
        ImBruta.atHPG(i,j,H,P,G);
        if(modo ==   CALIBRATOR_COR_ETIQUETADA_SOFT)
            cor_pixel = calibracaoParam.getSoftColor(H,P,G);
        else
            cor_pixel = calibracaoParam.getHardColor(H,P,G);

        ImProcessada[i][j] = cores[cor_pixel];
      }
    }
  break;
  //com campo vazio
  case CALIBRATOR_COR_ETIQUETADA_SOFT:

    for(i = 0; i < ImProcessada.getHeight(); i++){
      for(j = 0; j < ImProcessada.getWidth(); j++){
        ImBruta.atHPG(i,j,H,P,G);
        r = calibracaoParam.isDiff(i,j, ImBruta.atByte(i,j));
        qtdDiff = 0;

      	if(modo ==   CALIBRATOR_COR_ETIQUETADA_SOFT)
      	    cor_pixel = calibracaoParam.getSoftColor(H,P,G);
      	else
      	    cor_pixel = calibracaoParam.getHardColor(H,P,G);
        if(r == 1){//eh objeto
          ImProcessada[i][j] = cores[cor_pixel];
        }else if(r == 0)//eh campo
          ImProcessada[i][j] = PxPreto;
        else{//incerteza
          if(IS_VALID(i-1,j-1))
            qtdDiff += (calibracaoParam.isDiff(i-1,j-1,ImBruta.atByte(i-1,j-1)) == 1)?1:0;
          if(IS_VALID(i-1,j+1))
            qtdDiff += (calibracaoParam.isDiff(i-1,j+1,ImBruta.atByte(i-1,j+1)) == 1)?1:0;
          if(IS_VALID(i+1,j+1))
            qtdDiff += (calibracaoParam.isDiff(i+1,j+1,ImBruta.atByte(i+1,j+1)) == 1)?1:0;
          if(IS_VALID(i+1,j-1))
            qtdDiff += (calibracaoParam.isDiff(i+1,j-1,ImBruta.atByte(i+1,j-1)) == 1)?1:0;

          if(qtdDiff > 2)
            ImProcessada[i][j] = cores[cor_pixel];
          else
            ImProcessada[i][j] = PxPreto;
        }

      }
    }
    break;
  case CALIBRATOR_IMAGEM_ERROS:
    ImProcessada = ImBruta;

    for(i = 0; i < ImProcessada.getHeight(); i++)
      for(j = 0; j < ImProcessada.getWidth(); j++){
        r = calibracaoParam.isDiff(i,j,ImBruta.atByte(i,j));
        qtdDiff = 0;
        if(r == 1)//nao eh campo
          ImProcessada[i][j] = PxBranco;
        else if(r == 0 )//eh campo
          ImProcessada[i][j] = PxPreto;
        else{//
          if(IS_VALID(i-1,j-1))
            qtdDiff += (calibracaoParam.isDiff(i-1,j-1,ImBruta.atByte(i-1,j-1)) == 1)?1:0;
          if(IS_VALID(i-1,j+1))
            qtdDiff += (calibracaoParam.isDiff(i-1,j+1,ImBruta.atByte(i-1,j+1)) == 1)?1:0;
          if(IS_VALID(i+1,j+1))
            qtdDiff += (calibracaoParam.isDiff(i+1,j+1,ImBruta.atByte(i+1,j+1)) == 1)?1:0;
          if(IS_VALID(i+1,j-1))
            qtdDiff += (calibracaoParam.isDiff(i+1,j-1,ImBruta.atByte(i+1,j-1)) == 1)?1:0;

          if(qtdDiff > 2)
            ImProcessada[i][j] = PxBranco;
          else
            ImProcessada[i][j] = PxPreto;

            // Area de teste
            // ImProcessada[240-60-1][320-80-1] = PxRGB(255,0,0);
            // ImProcessada[240-60][320-80] = PxRGB(255,0,0);
            // ImProcessada[240-60+1][320-80+1] = PxRGB(255,0,0);
            //
            // ImProcessada[240+50-1][320+50-1] = PxRGB(255,0,0);
            // ImProcessada[240+50][320+50] = PxRGB(255,0,0);
            // ImProcessada[240+50+1][320+50+1] = PxRGB(255,0,0);
            //
            // std::ofstream desvios_arq("desvios.teste");
            // std::ofstream medias_arq("medias.teste");
            // std::ofstream resultados_arq("resultados.teste");
            // std::ofstream imagem_y_arq("imagem_GBRG.teste");
            // std::cout << "Constantes" << '\n';
            // std::cout << "Const object:\t"<<calibracaoParam.const_Object << '\n';
            // std::cout << "Const Field:\t"<<calibracaoParam.const_Field << '\n';
            //
            // for(unsigned i = 240-50; i < 240+50; i++){
            //   for(unsigned j = 320-50; j < 320+50; j++){
            //     desvios_arq << (int)calibracaoParam.desvioPadrao[2*(i*ImBruta.getWidth() + j)] << ' ';
            //     medias_arq << (int)calibracaoParam.campoVazio.atByte(i,j).b1 << ' ';
            //     resultados_arq << calibracaoParam.isDiff(i,j, ImBruta.atByte(i,j)) << ' ';
            //     imagem_y_arq << (int)ImBruta.atByte(i,j).b1 << ' ';
            //   }
            //   desvios_arq << '\n';
            //   medias_arq << '\n';
            //   resultados_arq << '\n';
            //   imagem_y_arq << '\n';
            // }
            // desvios_arq.close();
            // medias_arq.close();
            // resultados_arq.close();
            // imagem_y_arq.close();
            //
            // ImProcessada[240-50][320-50] = PxRGB(255,0,0);
            // ImProcessada[240+50][320+50] = PxRGB(255,0,0);
            // ImProcessada.save("imagem_teste.ppm");
            //
            // exit(1);
            // FIM DA AREA DE TESTE
        }

      }
    break;
  case CALIBRATOR_COR_ATUAL:

    // for(unsigned i = 0; i < calibracaoParam.campoVazio.getLength(); i++)
    //   tmp.atByte(i) = calibracaoParam.desvioPadrao[i];
    // ImProcessada = tmp;
    for(i = 0; i < ImProcessada.getHeight(); i++){
      for(j = 0; j < ImProcessada.getWidth(); j++){
          ImBruta.atHPG(i,j,H,P,G);
          cor_pixel = calibracaoParam.getHardColor(H,P,G);
          if(cor_pixel == corAtual){
            ImProcessada[i][j] = ImBruta.atRGB(i + offset_v, j + offset_u);
          }else{
            // ImProcessada[i][j] = coresInversas[corAtual];
            ImProcessada[i][j] = PxPreto;
          }

          // ImBruta.atHPG(i,j,H,P,G);
          // if(  calibracaoParam.isColor(H,P,G,corAtual) ){
          //   ImProcessada[i][j] = ImBruta.atRGB(i, j);
          // }else{
          //   ImProcessada[i][j] = PxPreto;
          // }


      }
    }
    break;
  default:
    std::cerr<<"Qual modo vc espera que eu processe?\n";
    exit(1);
    break;
  }
  return false;
}

void CalibratorProcessor::getPxValor(int x, int y,
				     int &R, int &G1, int &B,
				     int &H, int &P, int &G2){
  /*O PROBLEMA ESTA AQUI!

    Por algum motivo ao acessar a ImProcessada para ler os valores e
    setar na interface, é gerado um segmentation fault. Provavelmente
    devido ao acesso a mesma variavel por threads diferentes.
   */
  // R = (int)round((ImProcessada.atRGB(y,x).r/255.0)*100.0);
  // G1 = (int)round((ImProcessada.atRGB(y,x).g/255.0)*100.0);
  // B = (int)round((ImProcessada.atRGB(y,x).b/255.0)*100.0);
  float myH,myP,myG;
  R = (int)round((ImProcessada[y][x].r/255.0)*100.0);
  G1 = (int)round((ImProcessada[y][x].g/255.0)*100.0);
  B = (int)round((ImProcessada[y][x].b/255.0)*100.0);
  ImProcessada[y][x].getHPG(myH,myP,myG);
  H = (int)round((myH/M_PI)*180);
  P = (int)round(myP*100);
  G2 = (int)round(myG*100);
}

/*
  Fincao para descobrir se um ponto foi selecionado.
  Retorna o indice do ponto selecionado caso exista, e -1 caso nao exista.
*/
int CalibratorProcessor::pontoSelecionado(int u,int v){
  if( !posicaoValida((unsigned int) u, (unsigned int)v) )
    return -1;

  int selec = -1;
  unsigned i;
  double dist, menor_dist = hypot((double)LarguraCaptura,(double)AlturaCaptura);

  for(i=0;i<calibracaoParam.nPontosNotaveis;i++){
    dist = hypot(calibracaoParam.pontosImagem[i].u() - (double)(u+offset_u),
		 calibracaoParam.pontosImagem[i].v() - (double)(v+offset_v));
    if(dist <= RAIO_SELECIONADO &&
       dist < menor_dist){
      selec = i;
      menor_dist = dist;
    }
  }
  return selec;
}

/*
  Move o ponto indicado para a posicao (x,y)
*/
void CalibratorProcessor::moverPonto(int ponto,int u,int v){
  if( !posicaoValida((unsigned int) u, (unsigned int)v) )
    return;
  calibracaoParam.pontosImagem[ponto].u() = (double)(u+offset_u);
  calibracaoParam.pontosImagem[ponto].v() = (double)(v+offset_v);
}

void CalibratorProcessor::calImgMedia(unsigned numAmostras){
  //essas duas linhas param a captura em paralelo
  // this->calculating = true;
  // Camera::encerrar = true;

  unsigned length = 0;
  uint64_t *sum2;
  uint64_t *sum1;

  unsigned width = ImBruta.getWidth();
  unsigned height= ImBruta.getHeight();
  length= ImBruta.getLength();

  sum1  = new uint64_t[length];
  sum2  = new uint64_t[length];

  waitforimage();

  if(calibracaoParam.desvioPadrao != NULL)
    delete[] calibracaoParam.desvioPadrao;
  calibracaoParam.desvioPadrao = new uint8_t[length];

  ImBruta.copyTo(calibracaoParam.campoVazio);

  for(unsigned pos = 0; pos < length; pos++){
    sum1[pos] = 0;
    sum2[pos] = 0;
    calibracaoParam.desvioPadrao[pos] = 0;
  }

  uint8_t byte;
  // captureimage();
  for(unsigned i = 0; i < numAmostras; i++){
    waitforimage();
    // captureimage();
    for(unsigned pos = 0; pos < length; pos++){
      byte = ImBruta.atByte(pos);
      sum1[pos] += byte;
      sum2[pos] += ceil(byte*byte);
    }
  }

  double var;

  for(unsigned pos = 0; pos < length; pos++){
    calibracaoParam.campoVazio.atByte(pos) = ceil(sum1[pos]/numAmostras);
    var = (sum2[pos] - (1.0/numAmostras)*(double)sum1[pos]*sum1[pos])/(numAmostras-1.0);
    calibracaoParam.desvioPadrao[pos] = ceil(sqrt(var));
  }
  campoVazio_capturado = true;
  calibracaoParam.const_Field = 1.0;
  calibracaoParam.const_Object= 3.0;

  delete[] sum1;
  delete[] sum2;

  // captureimage();
  //retomada da captura em paralelo
  // Camera::encerrar = false;
  // this->calculating = false;
}
