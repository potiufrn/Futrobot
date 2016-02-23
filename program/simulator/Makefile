BASEDIR = /futrobot

CC = g++
CPPFLAGS = -Wall -I ${BASEDIR}/include
CFLAGS = 
LDFLAGS = -L ${BASEDIR}/lib
LDLIBS = -lmatrix -lpthread -lssocket

##### RULES #####

#simulador: sistema.o ssocket.o campo.o simulador.o modelo.o

simulador: ../system.o simulador.o modelo.o campo.o

#sistema.o: sistema.h

#ssocket.o: ssocket.h

campo.o: ../data.h ../parameters.h campo.h

modelo.o: ../data.h ../parameters.h ../system.h ../functions.h modelo.h

simulador.o: ../data.h ../parameters.h ../comunicacao.h ../system.h ../functions.h modelo.h

##### SPECIAL TARGETS ######

.PHONY: clean realclean

clean:
	-rm -f *.o *~

realclean: clean
	-rm -f simulador
