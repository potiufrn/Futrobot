#include "calibratorprocessor.h"
#include "variaveisglobais.h"
#include <iostream>
#include <cmath>
#include <cstring>

#define RAIO_SELECIONADO 5.0
// #define NUM_BUFFERS 4
// //#define NUM_BUFFERS 10
// #define ISO_SPEED DC1394_ISO_SPEED_400
// #define FRAME_RATE DC1394_FRAMERATE_30
// //#define DEBAYER_METHOD DC1394_BAYER_METHOD_NEAREST
// #define DEBAYER_METHOD DC1394_BAYER_METHOD_BILINEAR
// #define VIDEO_MODE DC1394_VIDEO_MODE_640x480_MONO8


CalibratorProcessor::CalibratorProcessor() :
  Camera(1),
  ImProcessada(),
  ImBruta(),
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
  true_color(false)
{
  //nada
}

CalibratorProcessor::CalibratorProcessor(const char* arquivo) :
  Camera(),
  ImProcessada(),
  ImBruta(),
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
  true_color(false)
{
  if(readFile(arquivo)){
    exit(1);
  }

}

void CalibratorProcessor::loadImage(){
  uint8_t * data =  (uint8_t*)Camera::getDataImage();
  unsigned length = Camera::getDataSize();
  uint8_t pxFormat = Camera::getPxFormat();
  unsigned largura = Camera::getWidth();
  unsigned altura = Camera::getHeight();

  ImBruta.loadFromData(data, length,  pxFormat, largura, altura);
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
    cores[i].b= (uint8_t)B;
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
  return calibracaoParam.read(text);
}


bool CalibratorProcessor::fileSave(const char* arquivo)
{
  return calibracaoParam.write(arquivo);
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
  return ImBruta.load(arq);
}

void CalibratorProcessor::saveImage(const char* arq){
  ImProcessada.save(arq);
}


bool CalibratorProcessor::processImage(){
  unsigned int i,j;
  int count=0, cor_pixel=0;
  float H, P, G;

  //Cores padroes a serem usadas no processamento
  static PxRGB PxPreto(0,0,0);
  static PxRGB PxVermelho(255,0,0);
  static PxRGB PxCinza(127,127,127);


  // Camera::toRGB(ImBruta);


  switch(modo){
  case CALIBRATOR_IMAGEM_REAL:
    // for(i = 0; i < ImProcessada.getHeight(); i++){
    //   for(j = 0; j < ImProcessada.getWidth(); j++){
    //     // ImProcessada[i][j] = ImBruta[offset_v + i][offset_u + j];
    //     ImProcessada.atRGB(i,j) = ImBruta.atRGB(offset_v + i, offset_u + j);
    //   }
    // }
    ImProcessada = ImBruta;
    break;
  case CALIBRATOR_LIMITES_P_E_PONTOS:
  case CALIBRATOR_LIMITES_P:
    /*
    //gera a imagem processada com os limites min e max de P
    ptBruta = (PxRGB*)ImBruta.getRawData();
    ptProcessada = (PxRGB*)ImProcessada.getRawData();
    //	unsigned i, j2, base1, base2;
    for(i=0;i<ImBruta.getHeight()*ImBruta.getWidth();i++){
    ptBruta->getHPG(H,P,G);
    ptProcessada->setHPG(H,P,G);
    ptBruta++;
    ptProcessada++;
    }
    */
    // for(i = 0; i < ImProcessada.getHeight(); i++){
    //   for(j = 0; j < ImProcessada.getWidth(); j++){
    //   	ImBruta.getHPG(i,j,H,P,G);
      	// ImProcessada.setHPG(i,j,H,P,G);
    //   }
    // }
    ImProcessada = ImBruta;
    if(modo == CALIBRATOR_LIMITES_P){
      break;
    }
  case CALIBRATOR_PONTOS:
    //desenha os pontos
    if(modo == CALIBRATOR_PONTOS){
      // for(i = 0; i < ImProcessada.getHeight(); i++){
      // 	for(j = 0; j < ImProcessada.getWidth(); j++){
      // 	  ImProcessada.atRGB(i,j) = ImBruta.atRGB(i+offset_v,j+ offset_u);
      // 	 }
      // }
      ImProcessada = ImBruta;
    }
    int jj,kk;
    for(i = 0; i < calibracaoParam.nPontosNotaveis; i++){
      for(jj = -2; jj <=2; jj++){
      	for(kk = -2; kk <=2; kk++){
      	  if((calibracaoParam.pontosImagem[i].u() + jj - offset_u) >= 0 &&
      	     (calibracaoParam.pontosImagem[i].u() + jj - offset_u) < ImProcessada.getWidth() &&
      	     (calibracaoParam.pontosImagem[i].v() + kk - offset_v) >= 0 &&
      	     (calibracaoParam.pontosImagem[i].v() + kk - offset_v) < ImProcessada.getHeight()){
      	    // ImProcessada[(int)round(calibracaoParam.pontosImagem[i].v() + kk-offset_v)][(int)round(calibracaoParam.pontosImagem[i].u() + jj - offset_u)] = PxVermelho;
            ImProcessada.atRGB((int)round(calibracaoParam.pontosImagem[i].v() + kk-offset_v), (int)round(calibracaoParam.pontosImagem[i].u() + jj - offset_u)) = PxVermelho;
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
      	  // ImProcessada[v][u] = PxVermelho;
          ImProcessada.atRGB(v,u) = PxVermelho;
      	}
      }
    }
    break;
  case CALIBRATOR_COR_ETIQUETADA:
  case CALIBRATOR_COR_ETIQUETADA_SOFT:
  case CALIBRATOR_IMAGEM_ERROS:
    for(i = 0; i < ImProcessada.getHeight(); i++){
      for(j = 0; j < ImProcessada.getWidth(); j++){
      	count = 0;
        ImBruta.getHPG(i + offset_v, j + offset_u, H, P, G);
      	if(modo ==   CALIBRATOR_COR_ETIQUETADA_SOFT)
      	    cor_pixel = calibracaoParam.getSoftColor(H,P,G);
      	else
      	    cor_pixel = calibracaoParam.getHardColor(H,P,G);

      	if(cor_pixel >= 0) count++;

      	if(count == 0){
      	  ImProcessada.atRGB(i,j) = PxCinza;
      	}else if(count == 1){
      	  if(modo == CALIBRATOR_IMAGEM_ERROS){
      	    ImProcessada.atRGB(i,j) = PxPreto;
      	  }else{
      	    ImProcessada.atRGB(i,j) = cores[cor_pixel];
      	  }
      	}else{
      	  ImProcessada.atRGB(i,j) = PxVermelho;
      	}
      }
    }
    break;
  case CALIBRATOR_COR_ATUAL:
    for(i = 0; i < ImProcessada.getHeight(); i++){
      for(j = 0; j < ImProcessada.getWidth(); j++){
        ImBruta.getHPG(i,j,H,P,G);
      	cor_pixel = calibracaoParam.getHardColor(H,P,G);
      	if(cor_pixel == corAtual){
      	  ImProcessada.atRGB(i,j) = ImBruta.atRGB(i+offset_v, j+offset_u);
      	}else{
      	  ImProcessada.atRGB(i,j) = coresInversas[corAtual];
      	}
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
  float myH,myP,myG;
  R = (int)round((ImProcessada.atRGB(y,x).r/255.0)*100.0);
  G1 = (int)round((ImProcessada.atRGB(y,x).g/255.0)*100.0);
  B = (int)round((ImProcessada.atRGB(y,x).b/255.0)*100.0);
  // R = (int)round((ImProcessada[y][x].r/255.0)*100.0);
  // G1 = (int)round((ImProcessada[y][x].g/255.0)*100.0);
  // B = (int)round((ImProcessada[y][x].b/255.0)*100.0);
  // ImProcessada[y][x].getHPG(myH,myP,myG);
  ImProcessada.getHPG(y, x, myH, myP, myG);
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

/*
  void calibrador::desenharCampoPequeno()
  {
  int i, j, k, count;
  QRgb ponto;
  int cor_pixel = 0;
  bool H_OK = false;
  switch(comboExibicao1->currentItem() ){
  case 0: //Real Image
  imagemGrande_processada = imagem.copy();
  pixmapGrande2.convertFromImage(imagemGrande_processada);
  imagemGrande2Suja = false;
  break;
  case 1: //Current Color Image
  for( i = 0; i < imagem.width(); i++){
  for( j = 0; j < imagem.height(); j++){
  ponto = imagem.pixel(i,j);
  pixel.r = qRed(ponto);
  pixel.g = qGreen(ponto);
  pixel.b = qBlue(ponto);
  pixel.getHPG(H, P_, G_);
  H_OK = false;
  if( limitesHPG[comboCores->currentItem()][0] <=
  limitesHPG[comboCores->currentItem()][1] ){
  if((int)round(H*100.0) >= limitesHPG[comboCores->currentItem()][0] &&
  (int)round(H*100.0) <= limitesHPG[comboCores->currentItem()][1]){
  H_OK = true;
  }
  }else{
  if((int)round(H*100.0) >= limitesHPG[comboCores->currentItem()][0] ||
  (int)round(H*100.0) <= limitesHPG[comboCores->currentItem()][1]){
  H_OK = true;
  }
  }

  if(H_OK &&
  (int)round(P_*100.0) >= limitesHPG[comboCores->currentItem()][2] &&
  (int)round(P_*100.0) <= limitesHPG[comboCores->currentItem()][3] &&
  (int)round(G_*100.0) >= limitesHPG[comboCores->currentItem()][4] &&
  (int)round(G_*100.0) <= limitesHPG[comboCores->currentItem()][5]){
  imagemGrande_processada.setPixel(i,j,ponto);
  }else{
  if( comboCores->currentItem() != 0 ){
  imagemGrande_processada.setPixel(i,j,qRgb(0,0,0));
  }else{
  imagemGrande_processada.setPixel(i,j,qRgb(255,255,255));
  }
  }
  }
  }
  pixmapGrande2.convertFromImage(imagemGrande_processada);
  break;
  case 2: //Tagged Image
  case 3: //Error Image
  for( i = 0; i < imagem.width(); i++){
  for( j = 0; j < imagem.height(); j++){
  ponto = imagem.pixel(i,j);
  pixel.r = qRed(ponto);
  pixel.g = qGreen(ponto);
  pixel.b = qBlue(ponto);
  pixel.getHPG(H, P_, G_);
  count = 0;

  for( k = 0; k < NUM_CORES-NUM_CORES_ADV; k++){
  H_OK = false;
  if( limitesHPG[k][0] <=
  limitesHPG[k][1] ){
  if((int)round(H*100.0) >= limitesHPG[k][0] &&
  (int)round(H*100.0) <= limitesHPG[k][1]){
  H_OK = true;
  }
  }else{
  if((int)round(H*100.0) >= limitesHPG[k][0] ||
  (int)round(H*100.0) <= limitesHPG[k][1]){
  H_OK = true;
  }
  }

  if(H_OK &&
  (int)round(P_*100.0) >= limitesHPG[k][2] &&
  (int)round(P_*100.0) <= limitesHPG[k][3] &&
  (int)round(G_*100.0) >= limitesHPG[k][4] &&
  (int)round(G_*100.0) <= limitesHPG[k][5]){
  count++;
  cor_pixel = k;
  }
  }
  if(count == 0){
  imagemGrande_processada.setPixel(i,j,qRgb(127,127,127)); //cinza
  }else if(count == 1){
  if(comboExibicao1->currentItem() == 2){
  switch(cor_pixel){
  case 0:
  imagemGrande_processada.setPixel(i,j,qRgb(0,0,0));
  break;
  case 1:
  imagemGrande_processada.setPixel(i,j,qRgb(255,255,255));
  break;
  case 2:
  imagemGrande_processada.setPixel(i,j,qRgb(0,0,255));
  break;
  case 3:
  imagemGrande_processada.setPixel(i,j,qRgb(255,255,0));
  break;
  case 4:
  imagemGrande_processada.setPixel(i,j,qRgb(255,128,0));
  break;
  case 5:
  imagemGrande_processada.setPixel(i,j,qRgb(0,255,255));
  break;
  case 6:
  imagemGrande_processada.setPixel(i,j,qRgb(255,0,255));
  break;
  case 7:
  imagemGrande_processada.setPixel(i,j,qRgb(0,255,0));
  break;
  case 8:
  case 9:
  case 10:
  imagemGrande_processada.setPixel(i,j,qRgb(128,64,0));
  break;
  }
  }else{
  imagemGrande_processada.setPixel(i,j,qRgb(0,0,0));
  }
  }else{
  imagemGrande_processada.setPixel(i,j,qRgb(255,0,0));
  }
  }
  }
  pixmapGrande2.convertFromImage(imagemGrande_processada);
  break;
  }

  //pixmapPequeno1.convertFromImage(imagemPequena);
  pixmapLabelGrande2->setPixmap(pixmapGrande2);

  }
*/
