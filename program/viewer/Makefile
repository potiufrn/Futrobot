BASEDIR = /futrobot

CC=g++ 
CPPFLAGS = -g -Wall -I/usr/X11R6/include -I/usr/include -I${BASEDIR}/include
CFLAGS=
LDFLAGS = -L/usr/X11/lib -L/usr/X11R6/lib -L${BASEDIR}/lib
LDLIBS = -lmatrix -lX11 -lXext -lXmu -lXt -lXi -lSM -lICE -lglut -lGLU -lGL -lm

visualizador: visualizador.o robo.o ../system.o

visualizador.o: robo.h ../data.h ../comunicacao.h ../system.h

robo.o: robo.h ../parameters.h

clean:
	rm -f *~
	rm -f core
	rm -f *.o

realclean: clean
	rm -f visualizador
