#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <iostream>
#include <sys/time.h>
#include <string.h>

#include "acquisition.h"
#include "../comunicacao.h"
#include "../functions.h"

using namespace::std;

#ifndef _SO_SIMULADO_
//PARAMETROS DE CONFIGURACAO
//*******************************************************
#define MAX_STACK_SIZE 1000
#define MAX_REGIONS 30
//#define MIN_PIXELS 60 //CHECAR
#define MIN_PIXELS 20 //CHECAR
#define LINE_THRESHOLD 45


#define ROBOT_EDGE_PIXELS 35 //CHECAR

//Keep this value up to date with the number
//of colors in REG_COLOR
#define NUM_COLORS 11

#define RADIUS 12 //CHECAR
#define MIN_RADIUS 12 //CHECAR
#define MAX_RADIUS 25 //CHECAR
#define RADIUS_STEP 1 //CHECAR

//DEFINICOES DE CLASSES
//*******************************************************
//struct HPG_LIMITS{
//  double hMin, hMax, gMin, gMax, pMin, pMax;
//};

class STACK
{
private:
  int X[MAX_STACK_SIZE];
  int Y[MAX_STACK_SIZE];
  int size;
public:
  inline STACK() {empty();}
  inline void empty() {size=0;}
  bool push(unsigned int x, unsigned int y);
  bool pop(unsigned int &x, unsigned int &y);
};

// Returns false if error
bool STACK::push(unsigned int x, unsigned int y)
{
  if (size>=MAX_STACK_SIZE) return false;
  X[size]=x;
  Y[size++]=y;
  return true;
}

// return false if error
bool STACK::pop(unsigned int &x, unsigned int &y)
{
  if (size==0) return false;
  x = X[--size];
  y = Y[size];
  return true;
}

// Uma classe para manter uma informação binária sobre cada pixel. A
// classe é construída usando field bits para tornar mais rápida a
// inicialização com um mesmo valor

struct BinaryBools {
  bool b15:1,b14:1,b13:1,b12:1,b11:1,b10:1,b9:1,b8:1,
    b7:1,b6:1,b5:1,b4:1,b3:1,b2:1,b1:1,b0:1;
};

union BinaryInt {
  uint16_t i;
  BinaryBools b;
};

class BinaryMap {
private:
  unsigned Ncol,Nlin,N;
  BinaryInt *m;
public:
  BinaryMap(unsigned width, unsigned height);
  ~BinaryMap();
  bool operator()(unsigned col, unsigned lin);
  void setValue(unsigned col, unsigned lin, bool value);
  void setAllValues(bool value);
};

BinaryMap::BinaryMap(unsigned width, unsigned height):
  Ncol(width),Nlin(height)
{
  if (Ncol*Nlin == 0) {
    cerr << "Mapa binario de dimensao nula\n";
    exit(1);
  }
  N = (Ncol*Nlin)/16;           // Divisao inteira: despreza o resto
  while (16*N < Ncol*Nlin) N++; // So deve fazer no maximo uma vez
  m = new BinaryInt[N];
  if (m == NULL) {
    cerr << "Problema na alocacao do mapa binario\n";
    exit(1);
  }
}

BinaryMap::~BinaryMap()
{
  delete[] m;
}

bool BinaryMap::operator()(unsigned col, unsigned lin)
{
  unsigned i = Ncol*lin + col;
  unsigned n = i/16;
  i -= 16*n;
  switch(i) {
  case 0: return m[n].b.b0;
  case 1: return m[n].b.b1;
  case 2: return m[n].b.b2;
  case 3: return m[n].b.b3;
  case 4: return m[n].b.b4;
  case 5: return m[n].b.b5;
  case 6: return m[n].b.b6;
  case 7: return m[n].b.b7;
  case 8: return m[n].b.b8;
  case 9: return m[n].b.b9;
  case 10: return m[n].b.b10;
  case 11: return m[n].b.b11;
  case 12: return m[n].b.b12;
  case 13: return m[n].b.b13;
  case 14: return m[n].b.b14;
  case 15: return m[n].b.b15;
  }
  cerr << "Erro misterioso 1!\n";
  return false;
}

void BinaryMap::setValue(unsigned col, unsigned lin, bool value)
{
  unsigned i = Ncol*lin + col;
  unsigned n = i/16;
  i -= 16*n;
  switch(i) {
  case 0: m[n].b.b0 = value; return;
  case 1: m[n].b.b1 = value; return;
  case 2: m[n].b.b2 = value; return;
  case 3: m[n].b.b3 = value; return;
  case 4: m[n].b.b4 = value; return;
  case 5: m[n].b.b5 = value; return;
  case 6: m[n].b.b6 = value; return;
  case 7: m[n].b.b7 = value; return;
  case 8: m[n].b.b8 = value; return;
  case 9: m[n].b.b9 = value; return;
  case 10: m[n].b.b10 = value; return;
  case 11: m[n].b.b11 = value; return;
  case 12: m[n].b.b12 = value; return;
  case 13: m[n].b.b13 = value; return;
  case 14: m[n].b.b14 = value; return;
  case 15: m[n].b.b15 = value; return;
  }
  cerr << "Erro misterioso 2!\n";
}

void BinaryMap::setAllValues(bool value)
{
  uint16_t Value = (value ? 0xFFFF : 0x0000);
  for (unsigned i=0; i<N; i++) {
    m[i].i = Value;
  }
}

//***********************************************************
//static bool analisedPixel[IMAGE_WIDTH][IMAGE_HEIGHT];
static BinaryMap analisedPixel(IMAGE_WIDTH,IMAGE_HEIGHT);
//static HPG_LIMITS colorLimit[NUM_COLORS];


static std::ostream& operator<<(std::ostream& os, REGION &r){
  os << r.center << " C-> " << r.colorID << " N-> " << r.nPixel;
  return os;
}

//Retorna a media entre 2 angulos
static double media_ang(double t1, double t2){
  if(fabs(t1 - t2) > M_PI){
    if(t1 < 0){
      t1 += 2*M_PI;
    }else{
      t2 += 2*M_PI;
    }
  }

  return ang_equiv((t1 + t2)/2.0);
}

#endif

Acquisition::Acquisition(TEAM team, SIDE side, GAME_MODE mode) :
  FutData(team,side,mode)
#ifndef _SO_SIMULADO_
  ,Camera(),
  RDistortion(getWidth(),getHeight())
#endif
{
  id_pos = id_ant = 0;

#ifndef _SO_SIMULADO_
  saveNextImage = false;
  MinU = IMAGE_WIDTH;
  MaxU = 0;
  MinV = IMAGE_HEIGHT;
  MaxV = 0;
  capturando = true;

#endif
}

Acquisition::~Acquisition(){
  if(gameMode() == SIMULATED_MODE){
    sock.close();
  }
}

//TODO: Casar essa leitura com a nova escrita dos parametros do calibrador
bool Acquisition::configAcquisition(const char *str)
{
  cout << "Configurando aquisicao..." << endl;
  if(gameMode() == SIMULATED_MODE){
    if(sock.connect(str, myTeam() == YELLOW_TEAM? PORTA_AMRL :
		    PORTA_AZUL) != SOCKET_OK){
      cerr << "Erro ao criar o socket\n";
      return true;
    }
    return false;
  }
#ifndef _SO_SIMULADO_
  unsigned i;
  string key;
  unsigned numDevices = 0;
  unsigned index = 0;
  std::ifstream I(str);

  if(!I.is_open())return true;

  cout << "\tConfigurando Camera..."<<endl;

  do{
    numDevices = Camera::listDevices(true);
    if(numDevices == 0){
      std::cerr << "\nWARNING: Nenhum Dispositivo de video conectado" << '\n';
      exit(1);
    }
    std::cout << "\nInforme o index da Camera : " << '\n';
    std::cin >> index;
    std::cin.ignore(1,'\n');
  }while(index >= numDevices);
  if(Camera::Open(index) == false){
    std::cerr << "Config. Acquisition ERRO: Falha ao abrir dispositivo" << '\n';
    I.close();
    exit(1);
  }

  //seta os parametros da camera
  getline(I,key,'\n');
  if(key != "Parametros da Camera"){
    I.close();
    return true;
  }

  if(Camera::read(I)){
    printf("Error Loading Camera Parameters!\n");
    I.close();
    return true;
  };

  I.ignore(1,'\n');
  getline(I,key,'\n');
  if(key != "Parametros de Calibracao")return true;
  //SetParameters(cameraParam);
  cout << "\tConfigurando calibracao..."<<endl;

  if(calibracaoParam.read(I)){
    printf("Error Loading Calibration Parameters!\n");
    return true;
  }
  cout << "\tLeu Parametros"<<endl;

  //testa se o arquivo lido tem as dimensoes corretas de pontos
  if(calibracaoParam.nPontosNotaveis != NUM_POINTS ||
     calibracaoParam.nCores != NUM_COLORS){
    printf("nPontosNotaveis: %d -- NUM_POINTS: %d\n",
	   calibracaoParam.nPontosNotaveis, NUM_POINTS);
    printf("nCores: %d -- NUM_COLORS: %d\n",
	   calibracaoParam.nCores, NUM_COLORS);
    return true;
  }

  printf("Parameters Loaded from File!\n");
  for(i = 0; i < calibracaoParam.nPontosNotaveis; i++) {
    if(calibracaoParam.pontosImagem[i].u() < MinU)
      MinU = (unsigned int)floor(calibracaoParam.pontosImagem[i].u());
    if(calibracaoParam.pontosImagem[i].u() > MaxU)
      MaxU = (unsigned int)ceil(calibracaoParam.pontosImagem[i].u());
    if(calibracaoParam.pontosImagem[i].v() < MinV)
      MinV = (unsigned int)floor(calibracaoParam.pontosImagem[i].v());
    if(calibracaoParam.pontosImagem[i].v() > MaxV)
      MaxV = (unsigned int)ceil(calibracaoParam.pontosImagem[i].v());
  }

  //CALCULO DE DISTORCAO RADIAL E HOMOGRAFIA
  cout << "\tConfigurando Distorcao Radial..."<<endl;

  // Retas horizontais
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[2],
				 calibracaoParam.pontosImagem[1],
				 calibracaoParam.pontosImagem[0]) ) {
    cerr << "Reta superior foi desprezada\n";
  }
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[4],
				 calibracaoParam.pontosImagem[22],
				 calibracaoParam.pontosImagem[20]) ) {
    cerr << "Reta area-superior1 foi desprezada\n";
  }
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[4],
				 calibracaoParam.pontosImagem[26],
				 calibracaoParam.pontosImagem[20]) ) {
    cerr << "Reta area-superior2 foi desprezada\n";
  }
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[5],
				 calibracaoParam.pontosImagem[24],
				 calibracaoParam.pontosImagem[19]) ) {
    cerr << "Reta centro-superior foi desprezada\n";
  }
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[8],
				 calibracaoParam.pontosImagem[25],
				 calibracaoParam.pontosImagem[16]) ) {
    cerr << "Reta centro-inferior foi desprezada\n";
  }
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[9],
				 calibracaoParam.pontosImagem[23],
				 calibracaoParam.pontosImagem[15]) ) {
    cerr << "Reta area-inferior1 foi desprezada\n";
  }
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[9],
				 calibracaoParam.pontosImagem[27],
				 calibracaoParam.pontosImagem[15]) ) {
    cerr << "Reta area-inferior2 foi desprezada\n";
  }
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[11],
				 calibracaoParam.pontosImagem[12],
				 calibracaoParam.pontosImagem[13]) ) {
    cerr << "Reta inferior foi desprezada\n";
  }
  // Retas verticais
  //ATENCAO: ESTA RETA ESTAVA COMENTADA
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[3],
 				 calibracaoParam.pontosImagem[4],
 				 calibracaoParam.pontosImagem[10]) ) {
    cerr << "Reta esquerda1 foi desprezada\n";
  }
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[3],
				 calibracaoParam.pontosImagem[5],
				 calibracaoParam.pontosImagem[10]) ) {
    cerr << "Reta esquerda2 foi desprezada\n";
  }
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[3],
				 calibracaoParam.pontosImagem[8],
				 calibracaoParam.pontosImagem[10]) ) {
    cerr << "Reta esquerda3 foi desprezada\n";
  }
  //ATENCAO: ESTA RETA ESTAVA COMENTADA
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[3],
 				 calibracaoParam.pontosImagem[9],
 				 calibracaoParam.pontosImagem[10]) ) {
    cerr << "Reta esquerda4 foi desprezada\n";
  }
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[1],
				 calibracaoParam.pontosImagem[24],
				 calibracaoParam.pontosImagem[12]) ) {
    cerr << "Reta central1 foi desprezada\n";
  }
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[1],
				 calibracaoParam.pontosImagem[25],
				 calibracaoParam.pontosImagem[12]) ) {
    cerr << "Reta central2 foi desprezada\n";
  }
  //ATENCAO: ESTA RETA ESTAVA COMENTADA
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[21],
 				 calibracaoParam.pontosImagem[20],
 				 calibracaoParam.pontosImagem[14]) ) {
    cerr << "Reta direita1 foi desprezada\n";
  }
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[21],
				 calibracaoParam.pontosImagem[19],
				 calibracaoParam.pontosImagem[14]) ) {
    cerr << "Reta direita2 foi desprezada\n";
  }
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[21],
				 calibracaoParam.pontosImagem[16],
				 calibracaoParam.pontosImagem[14]) ) {
    cerr << "Reta direita3 foi desprezada\n";
  }
  //ATENCAO: ESTA RETA ESTAVA COMENTADA
  if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[21],
 				 calibracaoParam.pontosImagem[15],
 				 calibracaoParam.pontosImagem[14]) ) {
    cerr << "Reta direita4 foi desprezada\n";
  }

  cout << "\tCalibrando distorcao..."<<endl;

  RDistortion.calibrar();
  RDistortion.ajustar(false);

  cout << "\tCorrigindo distorcao..."<<endl;

  Coord2 PImgCorrig[calibracaoParam.nPontosNotaveis];
  for (i=0; i<calibracaoParam.nPontosNotaveis; i++) {
    PImgCorrig[i] = RDistortion.corrigir(calibracaoParam.pontosImagem[i]);
    if(isnan(PImgCorrig[i].x()) ||
       isnan(PImgCorrig[i].y()) ){
      cerr<<"CORRECAO DE DISTORCAO RADIAL GERANDO NANs!!! "
	  <<__FILE__<<" "<<__LINE__ <<endl;
    }
  }
  cout << "\tConfigurando Homografia..."<<endl;

  for (i=0; i<calibracaoParam.nPontosNotaveis; i++) {
    Homography.incluirPonto(calibracaoParam.pontosReais[i],PImgCorrig[i]);
  }
  cout << "\tCalibrando Homografia..."<<endl;

  Homography.calibrar();

#endif

  cout << "Aquisicao Calibrada!" << endl;

  return false;
}

#ifndef _SO_SIMULADO_

bool Acquisition::canBePainted(REG_COLOR colorID, unsigned u, unsigned v){
  float H,P,G;
  ImBruta.atHPG(v,u, H,P,G);
  return (!analisedPixel(u,v) && calibracaoParam.getHardColor(H,P,G) == colorID);
}

REGION Acquisition::seedFill( REG_COLOR colorID, unsigned int u, unsigned int v){
  static STACK s;
  REGION region;

  double su=0, sv=0, suu=0, svv=0, suv=0;
  int nPixel=0;
  unsigned int v1;
  unsigned int vMax=0,vMin=ImBruta.getHeight();
  unsigned int uMax=0,uMin=ImBruta.getWidth();
  bool expanLeft, expanRight;

  region.nPixel=0;
  region.colorID = colorID;

  if ( !canBePainted(colorID,u,v) )return region;
  s.empty();

  if (!s.push(u,v)) {
    cerr << "Buffer estourou 1!\n";
    return region;
  }

  while(s.pop(u,v)) {
    v1 = v;

    while(v1>0 && canBePainted(colorID,u,v1-1)) v1--;

    expanLeft = expanRight = false;

    while(v1<ImBruta.getHeight() && canBePainted(colorID,u,v1) ) {
      analisedPixel.setValue(u,v1,true);
      if(v1 < vMin) vMin = v1;
      if(v1 > vMax) vMax = v1;
      if(u < uMin) uMin = u;
      if(u > uMax) uMax = u;


      su += u; sv += v1; suu += u*u; svv += v1*v1; suv += u*v1;
      nPixel++;

      if(!expanLeft && u>0 && canBePainted(colorID,u-1,v1)) {
        if(!s.push(u-1,v1)) {
          cerr << "Buffer estourou 2!\n";
          return region;
        }
        expanLeft = true;
      }else if(expanLeft && u>0 && !canBePainted(colorID,u-1,v1)) {
        expanLeft = false;
      }

      if(!expanRight && u<(ImBruta.getWidth()-1) && canBePainted(colorID,u+1,v1)){
        if(!s.push(u+1,v1)) {
          cerr << "Buffer estourou 3!\n";
          return region;
        }
        expanRight = true;
      }
    else if(expanRight && u<(ImBruta.getWidth()-1) &&
	      !canBePainted(colorID,u+1,v1)) {
        expanRight = false;
      }
      v1++;
    }

  }
  //testa se a regiao é uma linha vertical ou horizontal
  if((vMax-vMin) > LINE_THRESHOLD ||
     (uMax-uMin) > LINE_THRESHOLD)
    return region;

  region.center.u() = su/nPixel;
  region.center.v() = sv/nPixel;
  region.nPixel = nPixel;

  double varu,  varv,  varuv;

  varu  = suu/nPixel - (su/nPixel)*(su/nPixel); //a
  varv  = svv/nPixel - (sv/nPixel)*(sv/nPixel); //c
  varuv = suv/nPixel - (su/nPixel)*(sv/nPixel); //b

  //testa se a regiao eh simetrica, ou seja, nao tem como calcular o angulo do segundo momento.
  double lim_zero = 0.001;
  if(fabs(varuv) < lim_zero && fabs(varu-varv) < lim_zero){
    //a figura é simetrica
    region.symetric = true;
    region.orientation = 0.0;
  }else{
    region.symetric = false;
    region.orientation = -atan2(varuv,varu-varv)/2.0;
  }
  return region;
}

bool Acquisition::calculaMinhaPoseAproximada(REGION regTeam, double angCorrecao,
					     int &index, POS_ROBO &teamPose){
  //Não achou marca auxiliar. Utiliza informação da posicao anterior
  //para definir qual o ID do robo e o angulo de correcão.
  regTeam.center = (RDistortion.corrigir(regTeam.center))/Homography;
  teamPose.x() = regTeam.center.x();
  teamPose.y() = regTeam.center.y();

  double dist, dist_min, dist2_min;
  int id_min = 0;
  dist_min = hypot(teamPose.y() - ant.me[0].y(),
		   teamPose.x() - ant.me[0].x());
  dist2_min = 1000*dist_min;
  for(int i = 1; i < 3; i++){
    dist = hypot(teamPose.y() - ant.me[i].y(),
		 teamPose.x() - ant.me[i].x());
    if(dist < dist_min){
      dist2_min = dist_min;
      dist_min = dist;
      id_min = i;
    }
    else if(dist < dist2_min){
      dist2_min = dist;
    }
  }
  if(dist_min < dist2_min/2.0){
    index = id_min;
    double orient1 = ang_equiv(regTeam.orientation + angCorrecao);
    double orient2 = ang_equiv(orient1 - M_PI);

    double diff1 = fabs(ang_equiv(orient1 - ant.me[index].theta()));
    double diff2 = fabs(ang_equiv(orient2 - ant.me[index].theta()));

    if(diff1 < diff2/2.0){
      teamPose.theta() = orient1;
    }
    else if(diff2 < diff1/2.0){
      teamPose.theta() = orient2;
    }
    else{
      teamPose.theta() = POSITION_UNDEFINED;
    }
    return false;
  }

  return true;
}

bool Acquisition::calculaMinhaPose(REGION regTeam, double angBusca,
				   double angCorrecao,int &index,
				   POS_ROBO &teamPose){
  //Macro para testar se uma coordenada, na imagem, eh valida.
  #define IS_VALID(i,j) ( (i)<ImBruta.getHeight() && (j)<ImBruta.getWidth())?true:false
  int u,v,ui,vi;
  REG_COLOR colorID;
  REGION region;
  float H,P,G;

  u = (int)round(regTeam.center.u()
		 + RADIUS*cos(regTeam.orientation+angBusca));
  v = (int)round(regTeam.center.v()
		 - RADIUS*sin(regTeam.orientation+angBusca));
  int r;
  unsigned qtdDiff;

  for(ui = u-2; ui <= u+2; ui++)
    for(vi = v-2; vi <= v+2; vi++){
      if(ui >= 0 && ui < (int)ImBruta.getWidth() && vi >= 0 && vi < (int)ImBruta.getHeight()){
        //WARNING usando informacoes do campo vazio
        r = calibracaoParam.isDiff(vi,ui,ImBruta.atByte(vi,ui));//compara o byte com o do campo vazio.
        qtdDiff = 0;
        //Trata caso em que nao foi possivel determina se o byte eh do campo ou nao
        //testa os pixels(byte na img bruta), conta quantos deles sao, com certeza, nao campo(objeto).
        if(r == IS_UNDEF){
          if(IS_VALID(vi-1,ui-1))
            qtdDiff += (calibracaoParam.isDiff(vi-1,ui-1,ImBruta.atByte(vi-1,ui-1)) == IS_OBJECT)?1:0;
          if(IS_VALID(vi-1,ui+1))
            qtdDiff += (calibracaoParam.isDiff(vi-1,ui+1,ImBruta.atByte(vi-1,ui+1)) == IS_OBJECT)?1:0;
          if(IS_VALID(vi+1,ui+1))
            qtdDiff += (calibracaoParam.isDiff(vi+1,ui+1,ImBruta.atByte(vi+1,ui+1)) == IS_OBJECT)?1:0;
          if(IS_VALID(vi+1,ui-1))
            qtdDiff += (calibracaoParam.isDiff(vi+1,ui-1,ImBruta.atByte(vi+1,ui-1)) == IS_OBJECT)?1:0;
        }
        //Caso em que o pixel nao eh considerado como do campo vazio, ou seja, deve pertence a algum
        //dos robos ou da bola.
        if(r == IS_OBJECT || qtdDiff > 2){
          //No trecho seguinte de codigo, eh feito a identificacao da cor,
          //e um seedFill para determinar a regiao a que este pixel pertence,
          //para entao calcular o centro deste objeto como sua orientacao.
          ImBruta.atHPG(vi, ui, H, P, G);
          colorID = (REG_COLOR)calibracaoParam.getHardColor(H, P, G);
          if( colorID != REG_COLOR_YELLOW &&
            colorID != REG_COLOR_BLUE &&
            colorID != REG_COLOR_ORANGE &&
            colorID != REG_COLOR_BLACK &&
            colorID != REG_COLOR_WHITE &&
            colorID != REG_COLOR_UNDEFINED){
              region = seedFill(colorID,ui,vi);
              if( region.nPixel >= MIN_PIXELS ) {
                switch(region.colorID){
                  case REG_COLOR_CIAN:
                  index = 0;
                  break;
                  case REG_COLOR_PINK:
                  index = 1;
                  break;
                  case REG_COLOR_GREEN:
                  index = 2;
                  break;
                  default:
                  index = -1;
                  break;
                }
                if(index != -1){
                  regTeam.center = (RDistortion.corrigir(regTeam.center))/Homography;
                  region.center = (RDistortion.corrigir(region.center))/Homography;


                  teamPose.x() = regTeam.center.x();
                  teamPose.y() = regTeam.center.y();
                  double theta1 = ang_equiv(regTeam.orientation + angCorrecao);
                  double theta2 = ang_equiv(arc_tang((region.center.y() -
                  regTeam.center.y()),
                  (region.center.x() -
                  regTeam.center.x()))
                  -M_PI_4);

                  teamPose.theta() = media_ang(theta1,theta2);
                  return false;
                }
              }
            }
        }
      }
    }

  return true;
}

bool Acquisition::calculaPoseAdv(REGION regTeam, int &index,POS_ROBO &teamPose,
				 double corrX, double corrY, double corrTheta){
  //REGION regAux;
  //   double min_dist,ang,dist;
  //   int u,v;
  //   REG_COLOR colorID;
  //REGION region;

  //   regAux.colorID = REG_COLOR_UNDEFINED;
  //   min_dist = MAX_RADIUS*10;
  //   for(ang = 0.0; ang < 2*M_PI; ang += M_PI/36){
  //     u = (int)round(regTeam.center.u() + RADIUS*cos(ang));
  //     v = (int)round(regTeam.center.v() + RADIUS*sin(ang));
  //     if(u >= 0 && u < (int)ImBruta.getWidth() &&
  //        v >= 0 && v < (int)ImBruta.getHeight()){

  //       colorID = (REG_COLOR)calibracaoParam.getHardColor(ImBruta[v][u]);
  //       //ImBruta[v][u].setRGB(1.0,1.0,1.0);

  //       if( colorID != REG_COLOR_YELLOW &&
  // 	  colorID != REG_COLOR_BLUE &&
  // 	  colorID != REG_COLOR_ORANGE &&
  // 	  colorID != REG_COLOR_BLACK &&
  // 	  colorID != REG_COLOR_WHITE &&
  // 	  colorID != REG_COLOR_UNDEFINED){
  // 	region = seedFill(colorID,u,v);
  // 	if ( region.nPixel >= MIN_PIXELS ){
  // 	  dist = euclid(regTeam.center - region.center);
  // 	  if(dist < min_dist){
  // 	    regAux = region;
  // 	    min_dist = dist;
  // 	  }
  // 	}
  //       }
  //     }
  //   }
  //   switch(regAux.colorID){
  //   case REG_COLOR_CIAN:
  //     index = 0;
  //     break;
  //   case REG_COLOR_PINK:
  //     index = 1;
  //     break;
  //   case REG_COLOR_GREEN:
  //     index = 2;
  //     break;
  //   default:
  //     //cerr<<"Nao encontrou cor auxiliar valida para um rotulo azul!\n";
  //     index = -1;
  //     break;
  //   }
  //   if(index != -1){
  regTeam.center = (RDistortion.corrigir(regTeam.center))/Homography;
  if(isnan(regTeam.center.x()) ||
     isnan(regTeam.center.y()) ){
    cerr<<"CORRECAO DE DISTORCAO RADIAL GERANDO NANs!!! "
	<<__FILE__<<" "<<__LINE__ <<endl;
  }

  //     regAux.center = (RDistortion.corrigir(regAux.center))/Homography;
  //     if(isnan(regAux.center.x()) ||
  //        isnan(regAux.center.y()) ){
  //       cerr<<"CORRECAO DE DISTORCAO RADIAL GERANDO NANs!!! "
  // 	  <<__FILE__<<" "<<__LINE__ <<endl;
  //     }

  teamPose.x() = regTeam.center.x();
  teamPose.y() = regTeam.center.y();
  teamPose.theta() = POSITION_UNDEFINED;
  //     teamPose.x() = (regTeam.center.x() +
  // 		    regAux.center.x())/2.0;
  //     teamPose.y() = (regTeam.center.y() +
  // 		    regAux.center.y())/2.0;
  //     teamPose.theta() = arc_tang( (regTeam.center.y() -
  // 				  regAux.center.y()),
  // 				 (regTeam.center.x() -
  // 				  regAux.center.x()) );
  //CORRIGIR A POSE CALCULADA EM FUNCAO DA MARCA DO ROBO ADVERSARIO
  //     teamPose.x() = teamPose.x() + corrX;
  //     teamPose.y() = teamPose.y() + corrY;
  //     teamPose.theta() = ang_equiv(teamPose.theta() + corrTheta);


  return false; //sem erro
  //   }
  //   return true; //com erro
}

bool Acquisition::processGameState(){

  //para cada cor está sendo criado um vetor de tamanho igual ao dobro
  //do numero de regioes esperadas daquela cor, pensando na
  //possibilidade de erros.
  static REGION regOrange;
  static REGION regBlue[3];
  static REGION regYellow[3];

  static REGION regAuxYellow[3], regAuxBlue[3];

  // static POS_ROBO blueTeam[3];
  // static POS_ROBO yellowTeam[3];

  int nRegionsFound(0);

  int nRegOrange(0), nRegBlue(0), nRegYellow(0);

  REGION region, region_aux;
  REG_COLOR colorID;
  int r;
  unsigned qtdDiff;
  int u,v;
  int i;
  float H,P,G;

  //inicio da procura dos robos

  analisedPixel.setAllValues(false);

  for( i = 0; i < 3; i++){
    pos.me[i].x() = POSITION_UNDEFINED;
    pos.me[i].y() = POSITION_UNDEFINED;
    pos.me[i].theta() = POSITION_UNDEFINED;

    pos.op[i].x() = POSITION_UNDEFINED;
    pos.op[i].y() = POSITION_UNDEFINED;
    pos.op[i].theta() = POSITION_UNDEFINED;

  }

  pos.ball.x() = POSITION_UNDEFINED;
  pos.ball.y() = POSITION_UNDEFINED;

  //PASSO 1: Busca por regiões amarelas e azuis
  for( v = MinV; v <= (int)MaxV && nRegionsFound < MAX_REGIONS; v+=6 ){
    for( u = MinU; u <= (int)MaxU && nRegionsFound < MAX_REGIONS; u+=6 ){
      //WARNING usando informacoes do campo vazio
      r = calibracaoParam.isDiff(v,u,ImBruta.atByte(v,u));//compara o byte com o do campo vazio.
      qtdDiff = 0;
      //Trata caso em que nao foi possivel determina se o byte eh do campo ou nao
      //testa os pixels(byte na img bruta), conta quantos deles sao, com certeza, nao campo(objeto).
      if(r == IS_UNDEF){
        if(IS_VALID(v-1,u-1))
          qtdDiff += (calibracaoParam.isDiff(v-1,u-1,ImBruta.atByte(v-1,u-1)) == IS_OBJECT)?1:0;
        if(IS_VALID(v-1,u+1))
          qtdDiff += (calibracaoParam.isDiff(v-1,u+1,ImBruta.atByte(v-1,u+1)) == IS_OBJECT)?1:0;
        if(IS_VALID(v+1,u+1))
          qtdDiff += (calibracaoParam.isDiff(v+1,u+1,ImBruta.atByte(v+1,u+1)) == IS_OBJECT)?1:0;
        if(IS_VALID(v+1,u-1))
          qtdDiff += (calibracaoParam.isDiff(v+1,u-1,ImBruta.atByte(v+1,u-1)) == IS_OBJECT)?1:0;
      }
      //Caso em que o pixel nao eh considerado como do campo vazio, ou seja, deve pertence a algum
      //dos robos ou da bola.
      if(r == IS_OBJECT || qtdDiff > 2){
        ImBruta.atHPG(v,u,H,P,G);
        colorID = (REG_COLOR)calibracaoParam.getHardColor(H,P,G);

        // eliminar os pixels sem cor
        if( colorID == REG_COLOR_YELLOW || colorID == REG_COLOR_BLUE || colorID == REG_COLOR_ORANGE ){

          region = seedFill(colorID,u,v);

          if ( region.nPixel >= MIN_PIXELS ) {
            switch(colorID){
              case REG_COLOR_BLUE:
              if(nRegBlue < 3){

                regBlue[nRegBlue] = region;
                nRegBlue++;
              }else{
                for(i = 0; i < 3; i++){
                  if(region.nPixel > regBlue[i].nPixel){
                    region_aux = regBlue[i];
                    regBlue[i] = region;
                    region = region_aux;
                  }
                }
              }
              break;
              case REG_COLOR_YELLOW:
              if(nRegYellow < 3){

                regYellow[nRegYellow] = region;
                nRegYellow++;
              }else{
                for(i = 0; i < 3; i++){
                  if(region.nPixel > regYellow[i].nPixel){
                    region_aux = regYellow[i];
                    regYellow[i] = region;
                    region = region_aux;
                  }
                }
              }
              break;
              case REG_COLOR_ORANGE:

              if(nRegOrange < 1){

                regOrange = region;
                nRegOrange++;
              }else{
                if(region.nPixel > regOrange.nPixel){
                  regOrange = region;
                }
              }
              break;
              default:
              cerr<<"Nao deveria chegar aqui 1!";
              return true;
              break;
            }
          }

      }
      }
    }
  }

  //PASSO 2: Calcula a pose dos meus robos
  REG_COLOR  mycolor = ((myTeam() == YELLOW_TEAM)? REG_COLOR_YELLOW : REG_COLOR_BLUE);
  POS_ROBO teamPose;
  int index = 0;

  switch(mycolor){
  case REG_COLOR_YELLOW:
    for(i = 0; i < nRegYellow; i++){
      if(!calculaMinhaPose(regYellow[i],M_PI_2,M_PI_4,index,teamPose)){
      	pos.me[index] = teamPose;
      }else if(!calculaMinhaPose(regYellow[i],-M_PI_2,-M_PI_2-M_PI_4,index,teamPose)){
      	pos.me[index] = teamPose;
      }else if(!calculaMinhaPoseAproximada(regYellow[i],M_PI_4,index,teamPose)){
        pos.me[index] = teamPose;
      }
    }
    break;
  case REG_COLOR_BLUE:
    for(i = 0; i < nRegBlue; i++){
      if(!calculaMinhaPose(regBlue[i],M_PI_2,M_PI_4,index,teamPose)){
	      pos.me[index] = teamPose;
      }else if(!calculaMinhaPose(regBlue[i],-M_PI_2,-M_PI_2-M_PI_4,index,teamPose)){
	      pos.me[index] = teamPose;
      }else if(!calculaMinhaPoseAproximada(regBlue[i],M_PI_4,index,teamPose)){
      	pos.me[index] = teamPose;
      }
    }
    break;
  default:
    //nuca deve chegar aqui...
    cerr <<"COR INVALIDA PARA ROTULO PRINCIPAL\n";
    return true;
    break;
  }
  //Aqui poderia usar informações do passo anterior ou casar uma marca
  //restante com o unico robô não identificado.

  //PASSO 3: Calcula a pose dos robôs adversários
  switch(mycolor){
  case REG_COLOR_YELLOW:
    //se sou o time amarelo, os meus adversarios são os azuis
    for(i = 0; i < nRegBlue; i++) {
      if(!calculaPoseAdv(regBlue[i],index,teamPose,0.0,0.0,0.0)){
	//	pos.op[index] = teamPose;
	pos.op[i] = teamPose;

      }
    }
    break;
  case REG_COLOR_BLUE:
    //se sou o time azul, os meus adversarios são os amarelos
    for(i = 0; i < nRegYellow; i++) {
      if(!calculaPoseAdv(regYellow[i],index,teamPose,0.0,0.0,0.0)){
	//pos.op[index] = teamPose;
	pos.op[i] = teamPose;
      }
    }
    break;
  default:
    //nuca deve chegar aqui...
    cerr <<"COR INVALIDA PARA ROTULO PRINCIPAL\n";
    return true;
    break;
  }


  //############################################################
  if(nRegOrange > 0) {
    regOrange.center = (RDistortion.corrigir(regOrange.center))/Homography;
    if(isnan(regOrange.center.x()) ||
       isnan(regOrange.center.y()) ) {
      cerr<<"CORRECAO DE DISTORCAO RADIAL GERANDO NANs!!! "
	  <<__FILE__<<" "<<__LINE__ <<endl;
    }

    pos.ball.x() = regOrange.center.x();
    pos.ball.y() = regOrange.center.y();

  }


  if(saveNextImage){
    cout << "Regioes encontradas:\n";
    cout << "Orange: "<< nRegOrange << endl;
    cout << "Azul: "<< nRegBlue << endl;
    cout << "Amarelo: "<< nRegYellow << endl;


    for(i = 0; i < nRegBlue; i++){
      cout << "Azul " << i << "\t" << regBlue[i];
      if(regAuxBlue[i].colorID != REG_COLOR_UNDEFINED){
	       cout << "\t" << regAuxBlue[i];
      }
      cout << endl;
    }

    for(i = 0; i < nRegYellow; i++) {
      cout << "Amarelo " << i << "\t" << regYellow[i];
      if(regAuxYellow[i].colorID != REG_COLOR_UNDEFINED){
	        cout << "\t" << regAuxYellow[i];
      }
      cout << endl;
    }

    cout << "Laranja " << regOrange << endl;
    ImagemRGB(ImBruta).save("img_salva.ppm");
    saveNextImage = false;
  }
  return false; //no errors
}
#endif

bool Acquisition::readGameState()
{
  static SITUACAO minhaSit;
  SOCKET_STATUS retorno;
  static fsocket fila;

  // Espera por uma imagem no socket; le todas as disponoveis e guarda a ultima
  if (!sock.connected()) {
    cerr << "Servidor nao conectado\n";
    return true;
  }
  do {
    retorno = sock.read((void *)&minhaSit, sizeof(SITUACAO));
    if( retorno == SOCKET_OK ) {
      fila.clean();
      fila.include(sock);
      retorno = fila.wait_read(0);
      if( retorno != SOCKET_TIMEOUT && retorno != SOCKET_OK ) {
	cerr << "Erro na fila interna de socket" << endl;
	return true;
      }
    }
    else {
      cerr << "Erro na leitura da camera do simulador" << endl;
      sock.close();
      return true;
    }
  } while( retorno == SOCKET_OK );
  id_pos = minhaSit.id;
  if (id_pos-id_ant!=1 && id_ant!=0) {
    cerr << "Quadros perdidos: anterior=" << id_ant
	 << " atual=" << id_pos << endl;
  }
  for (int i=0; i<3; i++) {
    if (myTeam() == BLUE_TEAM) {
      pos.me[i].x() = minhaSit.pos.azul[i].x();
      pos.me[i].y() = minhaSit.pos.azul[i].y();
      pos.me[i].theta() = minhaSit.pos.azul[i].theta();
      pos.op[i].x() = minhaSit.pos.amrl[i].x();
      pos.op[i].y() = minhaSit.pos.amrl[i].y();
      pos.op[i].theta() = minhaSit.pos.amrl[i].theta();
    }
    else {
      pos.op[i].x() = minhaSit.pos.azul[i].x();
      pos.op[i].y() = minhaSit.pos.azul[i].y();
      pos.op[i].theta() = minhaSit.pos.azul[i].theta();
      pos.me[i].x() = minhaSit.pos.amrl[i].x();
      pos.me[i].y() = minhaSit.pos.amrl[i].y();
      pos.me[i].theta() = minhaSit.pos.amrl[i].theta();
    }
  }
  pos.ball.x() = minhaSit.pos.bola.x();
  pos.ball.y() = minhaSit.pos.bola.y();

  if(saveNextImage){
    printf("Nao ha o que imprimir no modo simulado.\n");
    saveNextImage = false;
  }

  return false;
}

bool Acquisition::acquisitionWait(){
#ifndef _SO_SIMULADO_
  if(gameMode() == REAL_MODE){
    //    return !WaitImage();
    return waitforimage();
  }
#endif
  //fazer a funcao WaitSocket();
  return false;
}


bool Acquisition::acquisition(){
#ifndef _SO_SIMULADO_
  if(gameMode() == REAL_MODE){
    return processGameState();
  }
#endif
  return readGameState();
}

bool Acquisition::acquisitionCapture(){
#ifndef _SO_SIMULADO_
  // if(gameMode() == REAL_MODE){
  //   if((Width() == 640) && (Height() == 480)){
  //     return Camera::captureimage();
  //   }
  //   else{
  //     cerr<<"Aquisition ERRO: Dimensoes da resolucao de camera nao suportados!"<<endl;
  //     return true;
  //   }
  // }
  return Camera::captureimage();
#endif
  return false;
}

void Acquisition::save_image()
{
  saveNextImage = true;
}
