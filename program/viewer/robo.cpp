#include <GL/glut.h>
#include <math.h>

#include "robo.h"
#include "../parameters.h"

void
desenhaRobo ( const GLfloat * color, const GLfloat * color_ind )
{
  const GLfloat dirColor[ ] = { 0.2, 0.2, 0.2, 1.0 };
  //const GLfloat azul[ ] = { 0.0, 0.0, 0.8, 1.0 };
  GLfloat p = (100*ROBOT_EDGE/2);

  /* corpo do robo */
  glColor4fv ( dirColor );
  glBegin ( GL_QUADS );
  glVertex3f ( -p, p, 0.0 );
  glVertex3f ( -p, -p, 0.0 );
  glVertex3f ( p, -p, 0.0 );
  glVertex3f ( p, p, 0.0 );
  glEnd ( );


  /* etiquetas do robo */
  glColor4fv ( color );
  /* triangulo superior */
  glBegin(GL_TRIANGLES);
  glVertex2f(0.0, -p);
  glVertex2f(0.0, +p);
  glVertex2f(+p, 0.0);
  glEnd();

  /* retangulo inferior */
  glColor4fv ( color_ind );
  glBegin ( GL_POLYGON );
  glVertex2f(0.0, -p);
  glVertex2f(-p, -p);
  glVertex2f(-p, p);
  glVertex2f(0.0, +p);
  glEnd ( );
}

void
desenhaRobobola ( const GLfloat * color, const GLfloat * color_ind )
{
  const GLfloat dirColor[ ] = { 0.2, 0.2, 0.2, 1.0 };
  const GLfloat azul[ ] = { 0.0, 0.0, 0.8, 1.0 };
  GLfloat p = (100*ROBOT_EDGE)/2.0;
  GLUquadricObj * q = gluNewQuadric ();
  GLUquadricObj * q2 = gluNewQuadric ();


  /* corpo do robo */
  glColor4fv ( dirColor );
  glBegin ( GL_QUADS );
  glVertex3f ( -p, p, 0.0 );
  glVertex3f ( -p, -p, 0.0 );
  glVertex3f ( p, -p, 0.0 );
  glVertex3f ( p, p, 0.0 );
  glEnd ( );


  p /= 2.0;
  glColor4fv ( color_ind );
  if( color[ 0 ] == azul[ 0 ] )
    {
      glColor4fv ( color_ind );
      glTranslatef (p,p,0.0);
      gluDisk ( q, 0.0, (100*ROBOT_MARK_RAY), 8, 1);

      glColor4fv ( color );
      glTranslatef (-2*p,-2*p,0.0);
      gluDisk ( q2, 0.0, (100*ROBOT_MARK_RAY), 8, 1);
    }
  else
    {
      glColor4fv ( color_ind );
      glTranslatef (p,p,0.0);
      gluDisk ( q, 0.0, (100*ROBOT_MARK_RAY), 8, 1);

      glColor4fv ( color );
      glTranslatef (-2*p,-2*p,0.0);
      gluDisk ( q2, 0.0, (100*ROBOT_MARK_RAY), 8, 1);
    }
}

void desenhaCampo ( void )
{
  GLdouble larg = (100*FIELD_HEIGHT)/2.0;
  GLdouble comp = (100*FIELD_WIDTH)/2.0;
  double ang, x, y;


  /* campo */
  glColor4f ( 0.0, 0.0, 0.0, 1.0 );
  glBegin ( GL_POLYGON );

  glVertex3f ( -comp, larg -(100*CORNER_DIMENSION), 0.0 );
  glVertex3f ( -comp, -larg+(100*CORNER_DIMENSION), 0.0 );
  glVertex3f ( -comp+(100*CORNER_DIMENSION), -larg, 0.0 );
  glVertex3f ( comp-(100*CORNER_DIMENSION), -larg, 0.0 );
  glVertex3f ( comp, -larg+(100*CORNER_DIMENSION), 0.0 );
  glVertex3f ( comp, larg-(100*CORNER_DIMENSION), 0.0 );
  glVertex3f ( comp-(100*CORNER_DIMENSION), larg, 0.0 );
  glVertex3f ( -comp+(100*CORNER_DIMENSION), larg, 0.0 );

  glEnd ( );

  /* gols */
  glBegin ( GL_QUADS );

  /* esquerda */
  glVertex3f ( -comp-(100*GOAL_WIDTH), (100*GOAL_HEIGHT)/2.0, 0.0 );
  glVertex3f ( -comp-(100*GOAL_WIDTH), -(100*GOAL_HEIGHT)/2.0, 0.0 );
  glVertex3f ( -comp-LINHA_LARGURA, -(100*GOAL_HEIGHT)/2.0, 0.0 );
  glVertex3f ( -comp-LINHA_LARGURA, (100*GOAL_HEIGHT)/2.0, 0.0 );

  /* direita */
  glVertex3f ( comp+LINHA_LARGURA, (100*GOAL_HEIGHT)/2.0, 0.0 );
  glVertex3f ( comp+LINHA_LARGURA, -(100*GOAL_HEIGHT)/2.0, 0.0 );
  glVertex3f ( comp+(100*GOAL_WIDTH), -(100*GOAL_HEIGHT)/2.0, 0.0 );
  glVertex3f ( comp+(100*GOAL_WIDTH), (100*GOAL_HEIGHT)/2.0, 0.0 );
 
  glEnd ( );


  /* cor e largura das linhas de marcacao */
  glColor4f ( 1.0, 1.0, 1.0, 1.0 );
  glPointSize ( LINHA_LARGURA );

  glBegin ( GL_LINES );

  /* linha central */
  glVertex3f ( 0.0, -larg, 0.0 ); 
  glVertex3f ( 0.0, larg, 0.0 );

  /* area esquerda */
  glVertex3f ( -comp+(100*GOAL_FIELD_WIDTH), (100*GOAL_FIELD_HEIGHT)/2.0, 0.0 );
  glVertex3f ( -comp+(100*GOAL_FIELD_WIDTH), -(100*GOAL_FIELD_HEIGHT)/2.0, 0.0 );

  glVertex3f ( -comp+(100*GOAL_FIELD_WIDTH), (100*GOAL_FIELD_HEIGHT)/2.0, 0.0 );
  glVertex3f ( -comp, (100*GOAL_FIELD_HEIGHT)/2.0, 0.0 );

  glVertex3f ( -comp+(100*GOAL_FIELD_WIDTH), -(100*GOAL_FIELD_HEIGHT)/2.0, 0.0 );
  glVertex3f ( -comp, -(100*GOAL_FIELD_HEIGHT)/2.0, 0.0 );

  /* area direita */
  glVertex3f ( comp-(100*GOAL_FIELD_WIDTH), (100*GOAL_FIELD_HEIGHT)/2.0, 0.0 );
  glVertex3f ( comp-(100*GOAL_FIELD_WIDTH), -(100*GOAL_FIELD_HEIGHT)/2.0, 0.0 );


  glVertex3f ( comp-(100*GOAL_FIELD_WIDTH), (100*GOAL_FIELD_HEIGHT)/2.0, 0.0 );
  glVertex3f ( comp, (100*GOAL_FIELD_HEIGHT)/2.0, 0.0 );

  glVertex3f ( comp-(100*GOAL_FIELD_WIDTH), -(100*GOAL_FIELD_HEIGHT)/2.0, 0.0 );
  glVertex3f ( comp, -(100*GOAL_FIELD_HEIGHT)/2.0, 0.0 );

  glEnd ( );


  /* circulo central */
  glBegin ( GL_POINTS );

  for ( ang = 0.0; ang < 360.0; ang += 1.0 ){
    x = (100*CIRCLE_RAY) * cos ( M_PI * ang/180.0 );
    y = (100*CIRCLE_RAY) * sin ( M_PI * ang/180.0 );
    glVertex3f ( x, y, 0.0 );
  }

  glEnd ( );
}


void
desenhaBola ( const GLfloat * color )
{
  GLUquadricObj * q = gluNewQuadric ();

  glColor4fv ( color );
  gluDisk ( q, 0.0, 100*BALL_RAY, 8, 1);
}
