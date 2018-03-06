#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>

#include <GL/glut.h>

#include <fcntl.h>
#include <fstream>
#include <unistd.h>
#include <math.h>

#include <stdlib.h>

#include <matrix.h>
#include "robo.h"
#include "../data.h"
#include "../comunicacao.h"
#include "../system.h"

#include "../parameters.h"

/* semaforo e memoria compartilhada */
int sem_id;
int shm_id;

int x_janela = 400;
int y_janela = 300;

static SITUACAO *psit = NULL;

POSICAO robos;
Coord2 bola;
Coord3 Times[ 2 ][ 3 ];

bool salvar = 0;
int frame = 0;
char nome_arq[] = {'0','0','.','p','p','m','\0'};

void itoa2 ( char s[ ], int n )
{
  s[ 0 ] = ( char )( ( n - n % 10 ) / 10 ) % 10 + '0';
  s[ 1 ] = ( char )( n % 10 + '0');
  s[ 2 ] = '\0';
}

void getPixelValue(int x,int y)
{
  unsigned char buf[3*x*y];

  /* le os pixels do frame buffer */
  glReadPixels( 0, 0, x-1, y-1, GL_RGB, GL_UNSIGNED_BYTE, &buf);

  /* salva tela em arquivo ppm */
  itoa2(nome_arq,frame++);
  nome_arq[2]='.';

  std::ofstream fout;
  fout.open(nome_arq);
  fout<<"P6\n "<<x<<" "<<y<<" 255\n";
  fout.close();

  int fd;
  fd = open(nome_arq,O_WRONLY|O_APPEND);
  write(fd, buf, 3*x*y);
  close(fd);
  // system("convert 00.ppm 00.jpg");

}

void idle (void) {
  /* atualizacao do posicionamento e orientacao dos
     robos */
  static SITUACAO sitLocal;
  int i;

  if (entrar_regiao_critica (sem_id)) {
    perror ("Erro de acesso.\n");
    _exit (1);
  }

  memcpy(&sitLocal,psit,sizeof(SITUACAO));

  if (sair_regiao_critica (sem_id)) {
    perror ("saida da regia critica.\n");
    _exit (1);
  }

  for (i = 0; i < 3; i++) {
    Times[ 0 ][ i ].x() = sitLocal.pos.azul[ i ].x() * 100;
    Times[ 0 ][ i ].y() = - sitLocal.pos.azul[ i ].y() * 100;
    Times[ 0 ][ i ].theta() = sitLocal.pos.azul[ i ].theta();

    Times[ 1 ][ i ].x() = sitLocal.pos.amrl[ i ].x() * 100;
    Times[ 1 ][ i ].y() = - sitLocal.pos.amrl[ i ].y() * 100;
    Times[ 1 ][ i ].theta() = -sitLocal.pos.amrl[ i ].theta();
  }

  bola.x() = sitLocal.pos.bola.x() * 100;
  bola.y() = -sitLocal.pos.bola.y()* 100;
  // Falta ler sitLocal.estado
  
  if(salvar && frame<1000)
    getPixelValue( x_janela, y_janela);
  //	 system("convert nome_arq.ppm nome_arq.jpg");

  glutPostRedisplay ();
  usleep(99999);
}


void renderScene ( void )
{
  int i, j;

  const GLfloat l[ ] = { 0.8, 0.4, 0.0, 1.0 };

  const GLfloat azul[ ] = { 0.0, 0.0, 0.8, 1.0 };
  const GLfloat amarelo[ ] = { 0.96, 1.0, 0.0, 1.0 };
  const GLfloat ciano[ ] = { 0.3, 0.8, 1.0, 1.0 };
  const GLfloat rosa[ ] = { 0.97, 0.3, 1.0, 1.0 };
  const GLfloat verde[ ] = { 0.3, 0.5, 0.3, 1.0 };



  glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glPushMatrix ( );
  desenhaCampo ();
  glPopMatrix ();


  for (i = 0; i < 2; ++ i) {
    for (j = 0; j < 3; ++ j) {
      glPushMatrix ();
      glTranslatef ( Times[ i ][ j ].x(),
		     - Times[ i ][ j ].y(), 0.0 );
      //			glRotatef ( -Times[ i ][ j ].theta()*180.0/M_PI, 0.0, 0.0, 1.0 );
      if( i == 0 )
	{
	  glRotatef ( Times[ i ][ j ].theta()*180.0/M_PI, 0.0, 0.0, 1.0 );
	  if(j==0)
	    desenhaRobo ( azul , ciano );
	  else
	    {
	      if(j==1)
		desenhaRobo ( azul , rosa );
	      else
		desenhaRobo ( azul, verde );
	    }
	}
      else
	{
	  glRotatef ( -Times[ i ][ j ].theta()*180.0/M_PI, 0.0, 0.0, 1.0 );
	  if(j==0)
	    desenhaRobo ( amarelo , ciano );
	  else
	    {
	      if(j==1)
		desenhaRobo ( amarelo , rosa );
	      else
		desenhaRobo ( amarelo, verde );
	    }
	}
      glPopMatrix ();
    }
  }

  glPushMatrix ();
  glTranslatef ( bola.x(), -bola.y(), 0.0 );
  desenhaBola ( l );
  // exibePlacar ???
  glPopMatrix ();

  glutSwapBuffers ( );
  glutIdleFunc (idle);
}



void reshape ( GLsizei w, GLsizei h )
{
  x_janela = w;
  y_janela = h;

  GLfloat ratio;


  if ( h == 0 ) h = 1;
  ratio = ( GLfloat ) w / ( GLfloat ) h;


  glViewport ( 0, 0, w, h );

  glMatrixMode ( GL_PROJECTION );
  glLoadIdentity ( );

  gluPerspective ( 30.0, ratio, 0.1, 300.0 );

  glMatrixMode ( GL_MODELVIEW );
  glLoadIdentity ( );
  glTranslatef ( 0.0, 0.0, -290.0 );
}


void keyboard ( unsigned char key, int x, int y )
{
  switch ( key ) {
  case 27:
    _exit ( 0 );
    break;
  case 's':
  case 'S':
    salvar = !salvar;
    break;
  default:
    break;

  }
}


void init ( void )
{
  glClearColor ( 1.0, 1.0, 1.0, 1.0 );


  sem_id = semget (KEY_SEM_COMPART, 1, 0666);
  if (sem_id == -1) {
    perror ("simulador1");
    _exit (1);
  }
  
  shm_id = shmget (KEY_MEM_COMPART, sizeof (SITUACAO), 0);
  if (shm_id == -1) {
    perror ("simulador2");
    _exit (1);
  }

  psit = (SITUACAO *) shmat (shm_id, NULL, 0);
  if (psit == NULL) {
    perror ("simulador3");
    _exit (1);
  }
  
}


int main ( int argc, char ** argv )
{
  glutInit ( & argc, argv );
  glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB );

  glutInitWindowPosition ( 0, 0 );
  //	 glutInitWindowSize ( 675, 555 );
  glutInitWindowSize ( x_janela, y_janela );
  glutCreateWindow ( "Simulação para Futebol de mini-Robôs DCA/UFRN" );

  init ( );

  glutReshapeFunc ( reshape );
  glutDisplayFunc ( renderScene );
  glutKeyboardFunc ( keyboard );

  glutMainLoop ( );

  return 0;
}
