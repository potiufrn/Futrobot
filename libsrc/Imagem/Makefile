#BASEDIR = /futrobot
BASEDIR = ../..

CC	=	g++
CXX = g++
CPPFLAGS = -Wall -O2 -I ${BASEDIR}/include
CFLAGS =
LDFLAGS = -O2 -L ${BASEDIR}/lib
LDLIBS = -lotim -lmatrix


all: libimagem.a


libimagem.a: imagem.o distradial.o homografia.o
	ar r $@ $?

imagem.o: imagem.h imagem.cpp

distradial.o: imagem.h distradial.cpp

homografia.o: imagem.h homografia.cpp

##### SPECIAL TARGETS ######

.PHONY: clean realclean install

clean:
	-rm -f *.o *~ *.bak *.fig Campo_*.ppm IMG_*.ppm

realclean: clean
	-rm -f ${PROGRAMAS} libimagem.a

install: libimagem.a
	-cp -f imagem.h ${BASEDIR}/include
	-cp -f libimagem.a ${BASEDIR}/lib
