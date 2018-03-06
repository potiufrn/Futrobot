#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "campo.h"
#include "../system.h"
#include "../functions.h"
#include "../parameters.h"
#include "modelo.h"

using namespace std;

#define INFINITO 1E+10

// Descrição do campo
static Campo campo;
 
// Funcões locais
static void calc_vertices(const EST_ROBO&, VERTICES&);
static bool testa_bola_paredes(const EST_BOLA&, CORRECAO&);
static bool testa_bola_robo(const EST_BOLA&, const VERTICES&, CORRECAO&);
static bool testa_robo_paredes(const VERTICES&, CORRECAO&);
static bool testa_robo_robo(const VERTICES&, const VERTICES&, CORRECAO&);
static bool testa_esfera_solido(const Coord2 &p, double r,
				const Coord2 &p0, const Coord2 &p1, double l,
				CORRECAO &correcao);
inline static bool testa_ponto_solido(const Coord2 &p, const Coord2 &p0,
				      const Coord2 &p1, double l,
				      CORRECAO &correcao)
{
  return testa_esfera_solido(p,0.0,p0,p1,l,correcao);
}

static void corrige_bola(EST_BOLA&, const CORRECAO&);
static void corrige_robo(EST_ROBO&, const CORRECAO&);
//static void colisao_bola_paredes(EST_BOLA &bola,
//				const CORRECAO &correcao);
//static void colisao_bola_robo(EST_BOLA &bola, EST_ROBO &robo,
//			      const CORRECAO &correcao);
//static void colisao_robo_paredes(EST_ROBO &robo,
//				const CORRECAO &correcao);
//static void colisao_robo_robo(EST_ROBO &robo1, EST_ROBO &robo2,
//			      const CORRECAO &correcao);

using std::cout;
using std::endl;

//FUNÇÕES PÚBLICAS
Modelo::Modelo()
{
  VLinMax=VEL_MOTOR*RAIO_RODA;
  VAngMax=VLinMax/(ROBOT_EDGE/2.0);
  MassaRobo=MASSA_ROBO;
  MomentoRobo=MassaRobo*pow2(AFAST_MASSA*(ROBOT_EDGE/2.0));
  TLin=CONST_TEMPO_LIN;
  TAng=pow2(AFAST_MASSA)*TLin;
  MassaBola=MASSA_BOLA;
  TBola=CONST_TEMPO_BOLA;

  posicao_inicial();
}

void Modelo::set_tempo(double tempo)
{
  tempo_simul=tempo; // altera tempo atual do objeto
}

void Modelo::posicao_inicial()
{
  bola.x() = 0.0;
  bola.y() = 0.0;
  bola.theta() = 0.0;
  bola.v = 0.0;

  azul[0].x() = -0.6; 
  azul[0].y() = 0.0;
  azul[0].theta() = 0.0;
  azul[0].v = 0.0;
  azul[0].w = 0.0;

  azul[1].x() = -0.3;
  azul[1].y() = 0.3;
  azul[1].theta() = 0.0;
  azul[1].v = 0.0;
  azul[1].w = 0.0;

  azul[2].x() = -0.3;
  azul[2].y() = -0.3;
  azul[2].theta() = 0.0;
  azul[2].v = 0.0;
  azul[2].w = 0.0;

  amrl[0].x() = 0.6;
  amrl[0].y() = 0.0;
  amrl[0].theta() = M_PI;
  amrl[0].v = 0.0;
  amrl[0].w = 0.0;

  amrl[1].x() = 0.3;
  amrl[1].y() = 0.3;
  amrl[1].theta() = M_PI;
  amrl[1].v = 0.0;
  amrl[1].w = 0.0;

  amrl[2].x() = 0.3;
  amrl[2].y() = -0.3;
  amrl[2].theta() = M_PI;
  amrl[2].v = 0.0;
  amrl[2].w = 0.0;

  tempo_simul = relogio();

  for(int i=0;i<3;i++){
    azul[i].alphae = 0.0;
    azul[i].alphad = 0.0;
    amrl[i].alphae = 0.0;
    amrl[i].alphad = 0.0;
  }
}

void Modelo::simular(double delta_t)
{
  double alphal,alphaw;
  double dv,dw,dx,dy,dtheta;
  int i,j;

  double dtempo = 0;

  while (dtempo < delta_t)
  {

    dtempo += PASSO_INTEGRACAO;
    for (i=0;i<3;i++)
    {
      //Azuis
      alphal = (azul[i].alphad+azul[i].alphae)/2.0;
      alphaw = (azul[i].alphad-azul[i].alphae)/2.0;
      dv = -TLin*azul[i].v +VLinMax*TLin*alphal;
      dw = -TAng*azul[i].w +VAngMax*TAng*alphaw;
      dx = azul[i].v*cos(azul[i].theta());
      dy = azul[i].v*sin(azul[i].theta());
      dtheta = azul[i].w;
      azul[i].v += dv*PASSO_INTEGRACAO;
      azul[i].w += dw*PASSO_INTEGRACAO;
      azul[i].x() += dx*PASSO_INTEGRACAO;
      azul[i].y() += dy*PASSO_INTEGRACAO;
      azul[i].theta() += dtheta*PASSO_INTEGRACAO;
      //Amarelos
      alphal = (amrl[i].alphad+amrl[i].alphae)/2.0;
      alphaw = (amrl[i].alphad-amrl[i].alphae)/2.0;
      dv = -TLin*amrl[i].v+VLinMax*TLin*alphal;
      dw = -TAng*amrl[i].w+VAngMax*TAng*alphaw;
      dx = amrl[i].v*cos(amrl[i].theta());
      dy = amrl[i].v*sin(amrl[i].theta());
      dtheta = amrl[i].w;
      amrl[i].v += dv*PASSO_INTEGRACAO;
      amrl[i].w += dw*PASSO_INTEGRACAO;
      amrl[i].x() += dx*PASSO_INTEGRACAO;
      amrl[i].y() += dy*PASSO_INTEGRACAO;
      amrl[i].theta() += dtheta*PASSO_INTEGRACAO;
    }
    //Bola
    dv = -TBola*bola.v;
    dx = bola.v*cos(bola.theta());
    dy = bola.v*sin(bola.theta());
    bola.v += dv*PASSO_INTEGRACAO;
    bola.x() += dx*PASSO_INTEGRACAO;
    bola.y() += dy*PASSO_INTEGRACAO;
    
    // TESTA COLISÕES
    
    VERTICES v_azul[3],v_amrl[3];
    CORRECAO correcao;

    //Calcula vertices
    for(i=0;i<3;i++)
    {
      calc_vertices(azul[i], v_azul[i]);
      calc_vertices(amrl[i], v_amrl[i]); 
    }
    for(i=0;i<3;i++)
    {
      // Testa colisoes dos robôs com as paredes
      if(testa_robo_paredes(v_azul[i],correcao)) {
	corrige_robo(azul[i], correcao);
	colisao_robo_paredes(BLUE_TEAM, i, correcao);
      }
      if(testa_robo_paredes(v_amrl[i],correcao)) {
	corrige_robo(amrl[i], correcao);
	colisao_robo_paredes(YELLOW_TEAM, i, correcao);
      }
      
      // Testa colisões da bola com os robôs
      if(testa_bola_robo(bola,v_azul[i],correcao)) {
	corrige_bola(bola,correcao);
	colisao_bola_robo(BLUE_TEAM, i, correcao);
      }
      if(testa_bola_robo(bola,v_amrl[i],correcao)) {
	corrige_bola(bola,correcao);
	colisao_bola_robo(YELLOW_TEAM, i, correcao);
      }
    }
  
    // Testa colisões da bola com as paredes
    if (testa_bola_paredes(bola,correcao)) {
      corrige_bola(bola,correcao);
      colisao_bola_paredes(correcao);
    }
    
    // Testa colisões entre robôs
    for(i=0;i<3;i++)
      for(j=0;j<3;j++)
      {
	if(i != j)
	{
	  if (testa_robo_robo(v_azul[i], v_azul[j], correcao)) 
	  {
	    corrige_robo(azul[i], correcao);
	    colisao_robo_robo(BLUE_TEAM, i, BLUE_TEAM, j, correcao);
	  }
	  if (testa_robo_robo(v_amrl[i], v_amrl[j], correcao)) 
	  {
	    corrige_robo(amrl[i], correcao);
	    colisao_robo_robo(YELLOW_TEAM, i, YELLOW_TEAM, j, correcao);
	  }
	}
	if (testa_robo_robo(v_azul[i], v_amrl[j], correcao)) 
	{
	  corrige_robo(azul[i], correcao);
	  colisao_robo_robo(BLUE_TEAM, i, YELLOW_TEAM, j, correcao);
	}
	if (testa_robo_robo(v_amrl[i], v_azul[j], correcao)) 
	{
	  corrige_robo(amrl[i], correcao);
	  colisao_robo_robo(YELLOW_TEAM, i, BLUE_TEAM, j, correcao);
	}
      }
  }

  tempo_simul += delta_t;
}

void Modelo::avancar_sem_simular(double delta_t)
{
  tempo_simul += delta_t;
}

void Modelo::atualizar_controle(TEAM time, int id, MOTOR m, double e)
{
  if (e>1.0) e=1.0;
  if (e<-1.0) e=-1.0;
  if( time == BLUE_TEAM ) {
    if (m == MOTOR_ESQUERDO) {
      azul[id].alphae = e;
    }
    else {
      azul[id].alphad = e;
    }
  }
  else {
    if (m == MOTOR_ESQUERDO) {
      amrl[id].alphae = e;
    }
    else {
      amrl[id].alphad = e;
    }
  }
}

void Modelo::le_posicao(POSICAO &pos)
{
  for(int i=0;i<3;i++)
  {
    pos.azul[i].x() = azul[i].x();
    pos.azul[i].y() = azul[i].y();
    pos.azul[i].theta() = azul[i].theta();
 
    pos.amrl[i].x() = amrl[i].x();
    pos.amrl[i].y() = amrl[i].y();
    pos.amrl[i].theta() = amrl[i].theta();

    pos.bola.x() = bola.x();
    pos.bola.y() = bola.y();
  }
}

void Modelo::set_posicao(const POSICAO &pos, double tb, double vb)
{
  for(int i=0;i<3;i++)
  {
    azul[i].x() = pos.azul[i].x();
    azul[i].y() = pos.azul[i].y();
    azul[i].theta() = pos.azul[i].theta();
    azul[i].v = 0.0;
    azul[i].w = 0.0;
 
    amrl[i].x() = pos.amrl[i].x();
    amrl[i].y() = pos.amrl[i].y();
    amrl[i].theta() = pos.amrl[i].theta();
    amrl[i].v = 0.0;
    amrl[i].w = 0.0;

    azul[i].alphae = 0.0;
    azul[i].alphad = 0.0;
    amrl[i].alphae = 0.0;
    amrl[i].alphad = 0.0;

    bola.x() = pos.bola.x();
    bola.y() = pos.bola.y();
    bola.theta() = tb;
    bola.v = vb;
  }
}

void Modelo::set_posicao_robo(TEAM time, int robo,
			      double x, double y, double theta)
{
  if (time == BLUE_TEAM) {
    azul[robo].x() = x;
    azul[robo].y() = y;
    azul[robo].theta() = theta;
  }
  else {
    amrl[robo].x() = x;
    amrl[robo].y() = y;
    amrl[robo].theta() = theta;
  }
}

void Modelo::set_posicao_bola(double x,double y)
{
  bola.x() = x;
  bola.y() = y;
}

// FUNÇÕES LOCAIS

// Se o robô está com orientação theta=0.0, então o vértice 0 é o
// superior direito, o vértice 1 é o inferior direito, o vértice 2 é o
// inferior esquerdo e o vértice 3 é o superior esquerdo. Esta
// numeração permanece mesmo que o robô gire. Por exemplo, a frente do
// robô será sempre o lado entre os vértices 0 e 1.

static void calc_vertices(const EST_ROBO &robo, VERTICES &v)
{
  v.centro.x() = robo.x();
  v.centro.y() = robo.y();
  for(int i=0;i<=3;i++)
  {
    v.vert[i].x() = robo.x() + ROBOT_RADIUS*cos(robo.theta()+(M_PI/4.0 - i*M_PI/2.0));
    v.vert[i].y() = robo.y() + ROBOT_RADIUS*sin(robo.theta()+(M_PI/4.0 - i*M_PI/2.0));
  }
}

// Testes de colisão entre objetos

static bool testa_bola_paredes(const EST_BOLA &bola,
			      CORRECAO &correcao)
{
  // OK
  if (fabs(bola.x()) < FIELD_WIDTH/2.0-max(CORNER_DIMENSION,BALL_RADIUS) &&
      fabs(bola.y()) < FIELD_HEIGHT/2.0-max(CORNER_DIMENSION,BALL_RADIUS) ) {
    return(false);
  }
  for (int j=0; j<NUM_ARESTAS; j++) {
    if (testa_esfera_solido(Coord2(bola.x(),bola.y()),BALL_RADIUS,campo.vertice(j),
			    campo.vertice(j+1),0.0,correcao)) {
      return(true);
    }
  }
  return(false);
}

static bool testa_bola_robo(const EST_BOLA &bola, const VERTICES &v,
			    CORRECAO &correcao)
{
  // OK
  if (hypot(bola.x()-v.centro.x(),bola.y()-v.centro.y()) > BALL_RADIUS+ROBOT_RADIUS) {
    return(false);
  }
  if (testa_esfera_solido(Coord2(bola.x(),bola.y()),BALL_RADIUS,
			  v.vert[2], v.vert[3], ROBOT_EDGE, correcao)) {
    return(true);
  }
  return(false);
}

// Testa se algum vértice do robô está dentro das paredes ou se
// alguma quina do campo está dentro do robô
static bool testa_robo_paredes(const VERTICES &v, CORRECAO &correcao)
{
  // OK
  if (fabs(v.centro.x()) < FIELD_WIDTH/2.0-max(CORNER_DIMENSION,ROBOT_RADIUS) &&
      fabs(v.centro.y()) < FIELD_HEIGHT/2.0-max(CORNER_DIMENSION,ROBOT_RADIUS))  {
    return(false);
  }
  for (int i=0; i<4; i++) {
    for (int j=0; j<NUM_ARESTAS; j++) {
      if (testa_ponto_solido(v.vert[i],campo.vertice(j),campo.vertice(j+1),
			     0.0,correcao)) {
	return(true);
      }
    }
  }
  // As quatro quinas que existem no campo (arestas 3, 6, 11 e 14)
  if ( testa_ponto_solido(campo.vertice(3),v.vert[2],v.vert[3],ROBOT_EDGE,
			  correcao) ||
       testa_ponto_solido(campo.vertice(6),v.vert[2],v.vert[3],ROBOT_EDGE,
			  correcao) ||
       testa_ponto_solido(campo.vertice(11),v.vert[2],v.vert[3],ROBOT_EDGE,
			  correcao) ||
       testa_ponto_solido(campo.vertice(14),v.vert[2],v.vert[3],ROBOT_EDGE,
			  correcao) ) {
    // A correção calculada seria para afastar a parede.
    // Precisa ser invertida para afastar o robô
    correcao.plano -= M_PI;
    correcao.erro.x() *= -1.0;
    correcao.erro.y() *= -1.0;
    return(true);
  }
  return(false);
}

// Testa se algum vértice do robô 1 está dentro do robô 2. A função
// não faz o teste simétrico (2 dentro do 1): você deve chamá-la
// novamente, invertendo os robôs.
static bool testa_robo_robo(const VERTICES &v1, const VERTICES &v2,
			    CORRECAO& correcao)
{
  // OK
  if (hypot(v1.centro.x()-v2.centro.x(),
	    v1.centro.y()-v2.centro.y()) > 2*ROBOT_RADIUS) {
    return(false);
  }
  for (int i=0; i<4; i++) {
    if (testa_ponto_solido(v1.vert[i],v2.vert[2],v2.vert[3],ROBOT_EDGE,
			   correcao)) {
      return(true);
    }
  }
  return(false);
}

// Testa se uma esfera de centro "p" e raio "r" está em colisão com um
// sólido. O raio "r" pode ser 0.0, caso em que se testa se um ponto
// está em colisão com um sólido. O sistema de coordenadas P de
// descrição do sólido tem a origem no ponto p0. O eixo y do sistema P
// vai do ponto p0 (que tem coordenadas x = y = 0 no sistema P) ao
// ponto p1 (que tem coordenadas x = 0 e y positiva no sistema P). O
// sólido tem largura "l" na direção x do sistema P. Se l = 0,
// entende-se que o sólido tem largura infinita.
//
// A variável "correcao" retorna (a) as coordenadas do ponto em que
// ocorreu a colisão; (b) quanto tem que ser adicionado à posição do
// ponto "p" para que o ponto saia do sólido; e (c) o ângulo da face
// onde houve a colisão (todos expressos no sistema de coordenadas
// global, embora inicialmente sejam calculados no sistema P para só
// no final serem convertidos para o sistema global)

static bool testa_esfera_solido(const Coord2 &p, double r,
				const Coord2 &p0, const Coord2 &p1, double l,
				CORRECAO &correcao)
{
  // "d" é a altura do sólido na direção y do sistema P
  double deltax = p1.x()-p0.x(), deltay = p1.y()-p0.y();
  double d = hypot(deltax,deltay);
  // xl e yl são as coordenadas do centro da esfera descrito no sistema
  // de coordenadas P
  double xl = (deltay*(p.x()-p0.x()) - deltax*(p.y()-p0.y()))/d;
  double yl = (deltax*(p.x()-p0.x()) + deltay*(p.y()-p0.y()))/d;

  bool houve_colisao = false;
  // colx, colx são as coordenadas do ponto onde houve a colisão
  // errx, erry é o vetor de correção a ser adicionado a p
  // plan é o ângulo Da face de colisão
  double colx,coly,errx,erry,plan;

  if (l <= 0.0) l = INFINITO;
  r = fabs(r);
  // Testa se a esfera está batendo em um dos lados
  if ( houve_colisao = ( ( xl>=-r && xl<=(l+r) &&
			   yl>=0.0 && yl<=d ) || 
			 ( xl>=0.0 && xl<=l && 
			   yl>=-r && yl<=(d+r) ) ) ) {
    if (min(xl,l-xl) < min(yl,d-yl)) {
      // Correção em x
      if (xl < l-xl) {
	// Bateu do lado esquerdo
	colx = 0.0;
	coly = yl;
	errx = -(r+xl);
	erry = 0.0;
	plan = M_PI_2;
      }
      else {
	// Bateu do lado direito
	colx = l;
	coly = yl;
	errx = r+(l-xl);
	erry = 0.0;
	plan = -M_PI_2;
      }
    }
    else {
      // Correção em y
      if (yl < d-yl) {
	// Bateu em baixo
	colx = xl;
	coly = 0.0;
	errx = 0.0;
	erry = -(r+yl);
	plan = M_PI;
      }
      else {
	// Bateu em cima
	colx = xl;
	coly = d;
	errx = 0.0;
	erry = r+(d-yl);
	plan = 0.0;
      }
    }
  }

  // Testa se a esfera está batendo em uma das quinas
  if (!houve_colisao && r>0.0) {
    double xq,yq,dist;
    int ix,iy;
    for (ix=0; !houve_colisao && ix<=(l<INFINITO ? 1 : 0); ix++) {
      xq = ix*l;
      for (iy=0; !houve_colisao && iy<=1; iy++) {
	yq = iy*d;
	dist = hypot(xq-xl,yq-yl);
	if ( houve_colisao = ( dist < r ) ) {
	  colx = xq;
	  coly = yq;
	  errx = (r-dist)*(xl-xq)/dist;
	  erry = (r-dist)*(yl-yq)/dist;
	  plan = arc_tang(xq-xl,yl-yq);
	}
      }
    }
  }

  if (houve_colisao) {
    // Transforma para o sistema de coordenadas global
    correcao.plano = ang_equiv(plan+arc_tang(-deltax,deltay));
    correcao.colisao.x() = p0.x()+(deltay*colx + deltax*coly)/d;
    correcao.colisao.y() = p0.y()+(-deltax*colx + deltay*coly)/d;
    correcao.erro.x() = (deltay*errx + deltax*erry)/d;
    correcao.erro.y() = (-deltax*errx + deltay*erry)/d;
    return (true);
  }
  return(false);
}

// Correção da interpenetração entre objetos em colisão

// Fator de correção para evitar interpenetração
#define EPSILON 0.01

static void corrige_bola(EST_BOLA &bola, const CORRECAO &correcao)
{
  bola.x() += (1+EPSILON)*correcao.erro.x();
  bola.y() += (1+EPSILON)*correcao.erro.y();
}

static void corrige_robo(EST_ROBO &robo, const CORRECAO &correcao)
{
  robo.x() += (1+EPSILON)*correcao.erro.x();
  robo.y() += (1+EPSILON)*correcao.erro.y();
}

// Modelagem dinâmica dos efeitos da colisão

// Durante a colisão, um corpo sofre o efeito de uma força de grande
// intensidade F (F tendendo para infinito) e fase beta, durante um
// pequeno intervalo de tempo deltat (deltat tendendo para zero). F e
// deltat não podem ser determinados, mas pode ser calculado o momento
// P da força, dado por P=F*deltat. A fase beta pode ser determinada a
// partir do plano da colisão, pois a força é sempre perpendicular a
// este plano.
//
// Uma bola de massa m_b tem velocidade vi_b fase thetai_b antes da
// colisão. Após a colisão, terá velocidade vf_b fase thetaf_b:
//
// vf_b = sqrt( vi_b^2 + (P/m_b)^2 + 2*(P/m_b)*vi_b*cos(beta_b-thetai_b) )
// thetaf_b = arc_tang( vi_b*sin(thetai_b) + (P/m_b)*sin(beta_b),
//                      vi_b*cos(thetai_b) + (P/m_b)*cos(beta_b) )
//
// Um robô de massa m_r e momento de inércia J_r tem velocidades linear
// vi_r e angular wi_r antes da colisão. A colisão ocorre em um ponto a
// uma distância d_c e angulo theta_c do centro do robô. Após a colisão,
// terá velocidades linear vf_r e angular wf_r:
//
// vf_r = vi_r + (P/m_r)*cos(beta_r-theta_r)
// wf_r = wi_r + (P*d_c/J_r)*sin(beta_r-theta_c)
//
// O momento P pode ser calculado a partir do fato que a energia
// cinética total antes e depois da colisão se mantém constante
// (supondo colisão perfeitamente elástica)

void Modelo::colisao_bola_paredes(const CORRECAO &correcao)
{
  const double theta_b=bola.theta(), v_b=bola.v;
  const double beta_b = correcao.plano + M_PI_2;

  double P = -2*( v_b*cos(beta_b-theta_b) );
  P /= ( 1/MassaBola );
  
  // O módulo da velocidade permanece inalterado
  //bola.v = sqrt( pow2(v_b) + pow2(P/MassaBola) +
  //		   2*(P/MassaBola)*v_b*cos(beta_b-theta_b));
  bola.theta() = arc_tang( v_b*sin(theta_b) + (P/MassaBola)*sin(beta_b),
			 v_b*cos(theta_b) + (P/MassaBola)*cos(beta_b) );
}

void Modelo::colisao_bola_robo(TEAM time, int id, const CORRECAO &correcao)
{
  EST_ROBO *robo;
  if (time == BLUE_TEAM) {
    robo = &(azul[id]);
  }
  else {
    robo = &(amrl[id]);
  }
  const double theta_b=bola.theta(), v_b=bola.v;
  const double x_r=robo->x(), y_r=robo->y(), theta_r=robo->theta(),
    w_r=robo->w, v_r=robo->v;
  const double beta_b = correcao.plano + M_PI_2;
  const double beta_r = correcao.plano - M_PI_2;
  const double theta_c = arc_tang(correcao.colisao.y()-y_r,
				  correcao.colisao.x()-x_r);
  const double d_c = hypot(correcao.colisao.y()-y_r,correcao.colisao.x()-x_r);
  
  double P = -2*( v_r*cos(beta_r-theta_r) +
		  d_c*w_r*sin(beta_r-theta_c) +
		  v_b*cos(beta_b-theta_b) );
  P /= ( pow2(cos(beta_r-theta_r))/MassaRobo +
	 pow2(d_c*sin(beta_r-theta_c))/MomentoRobo +
	 1/MassaBola );

  bola.v = sqrt( pow2(v_b) + pow2(P/MassaBola) +
		 2*(P/MassaBola)*v_b*cos(beta_b-theta_b));
  bola.theta() = arc_tang( v_b*sin(theta_b) + (P/MassaBola)*sin(beta_b),
			 v_b*cos(theta_b) + (P/MassaBola)*cos(beta_b) );
  robo->w += (P*d_c/MomentoRobo)*sin(beta_r-theta_c);
  robo->v += (P/MassaRobo)*cos(beta_r-theta_r);
}

void Modelo::colisao_robo_paredes(TEAM time, int id, const CORRECAO &correcao)
{
  EST_ROBO *robo;
  if (time == BLUE_TEAM) {
    robo = &(azul[id]);
  }
  else {
    robo = &(amrl[id]);
  }
  const double x_r=robo->x(), y_r=robo->y(), theta_r=robo->theta(),
    w_r=robo->w, v_r=robo->v;
  const double beta_r = correcao.plano + M_PI_2;
  const double theta_c = arc_tang(correcao.colisao.y()-y_r,
				  correcao.colisao.x()-x_r);
  const double d_c = hypot(correcao.colisao.y()-y_r,correcao.colisao.x()-x_r);
  
  double P = -2*( v_r*cos(beta_r-theta_r) +
		  d_c*w_r*sin(beta_r-theta_c) );
  P /= ( pow2(cos(beta_r-theta_r))/MassaRobo +
	 pow2(d_c*sin(beta_r-theta_c))/MomentoRobo );

  robo->w += (P*d_c/MomentoRobo)*sin(beta_r-theta_c);
  robo->v += (P/MassaRobo)*cos(beta_r-theta_r);
}

// O robô 1 bateu de quina em uma face do robô 2
void Modelo::colisao_robo_robo(TEAM time1, int id1, TEAM time2, int id2,
			      const CORRECAO &correcao)
{
  EST_ROBO *robo1,*robo2;
  if (time1 == BLUE_TEAM) {
    robo1 = &(azul[id1]);
  }
  else {
    robo1 = &(amrl[id1]);
  }
  if (time2 == BLUE_TEAM) {
    robo2 = &(azul[id2]);
  }
  else {
    robo2 = &(amrl[id2]);
  }
  const double x_r1=robo1->x(), y_r1=robo1->y(), theta_r1=robo1->theta(),
    w_r1=robo1->w, v_r1=robo1->v;
  const double x_r2=robo2->x(), y_r2=robo2->y(), theta_r2=robo2->theta(),
    w_r2=robo2->w, v_r2=robo2->v;
  const double beta_r1 = correcao.plano + M_PI_2;
  const double beta_r2 = correcao.plano - M_PI_2;
  const double theta_c1 = arc_tang(correcao.colisao.y()-y_r1,
				  correcao.colisao.x()-x_r1);
  const double d_c1 = hypot(correcao.colisao.y()-y_r1,correcao.colisao.x()-x_r1);
  const double theta_c2 = arc_tang(correcao.colisao.y()-y_r2,
				  correcao.colisao.x()-x_r2);
  const double d_c2 = hypot(correcao.colisao.y()-y_r2,correcao.colisao.x()-x_r2);

  double P = -2*( v_r1*cos(beta_r1-theta_r1) +
		  d_c1*w_r1*sin(beta_r1-theta_c1) +
		  v_r2*cos(beta_r2-theta_r2) +
		  d_c2*w_r2*sin(beta_r2-theta_c2) );
  P /= ( pow2(cos(beta_r1-theta_r1))/MassaRobo +
	 pow2(d_c1*sin(beta_r1-theta_c1))/MomentoRobo +
	 pow2(cos(beta_r2-theta_r2))/MassaRobo +
	 pow2(d_c2*sin(beta_r2-theta_c2))/MomentoRobo );

  robo1->w += (P*d_c1/MomentoRobo)*sin(beta_r1-theta_c1);
  robo1->v += (P/MassaRobo)*cos(beta_r1-theta_r1);
  robo2->w += (P*d_c2/MomentoRobo)*sin(beta_r2-theta_c2);
  robo2->v += (P/MassaRobo)*cos(beta_r2-theta_r2);
}
