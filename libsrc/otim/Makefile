#BASEDIR = /futrobot
BASEDIR = ../..

CC = g++
CXX = g++
CPPFLAGS = -Wall -O2 -I ${BASEDIR}/include
CFLAGS =
LDFLAGS = -O2 -L ${BASEDIR}/lib
LDLIBS = -lmatrix

teste: libotim.a teste.o

libotim.a: otim.o myotim.o genetico.o poliedro.o
	ar r $@ $?

otim.o: otim.cpp otim.h

myotim.o: myotim.cpp otim.h

genetico.o: genetico.cpp otim.h

poliedro.o: poliedro.cpp otim.h

teste.o: teste.cpp otim.h

##### SPECIAL TARGETS ######

.PHONY: clean realclean install

clean:
	-rm -f *.o *~

realclean: clean
	-rm -f teste libotim.a

install: libotim.a
	-cp -f otim.h ${BASEDIR}/include
	-cp -f libotim.a ${BASEDIR}/lib
