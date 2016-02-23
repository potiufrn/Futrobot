#BASEDIR = /futrobot
BASEDIR = ../..

CC = g++
CXX = g++
CPPFLAGS = -Wall -O2 -I ${BASEDIR}/include
CFLAGS =
LDFLAGS = -O2 -L ${BASEDIR}/lib
LDLIBS =

teste: libmatrix.a teste.o
teste2: libmatrix.a teste2.o

libmatrix.a: mymatrix.o vector.o cvector.o matrix.o cmatrix.o
	ar r $@ $?

vector.o: vector.cpp tvector.cpp matrix.h mymatrix.h

cvector.o: cvector.cpp tvector.cpp matrix.h mymatrix.h

matrix.o: matrix.cpp tmatrix.cpp matrix.h mymatrix.h

cmatrix.o: cmatrix.cpp tmatrix.cpp matrix.h mymatrix.h

mymatrix.o: mymatrix.cpp matrix.h mymatrix.h

teste.o: teste.cpp matrix.h

teste2.o: teste2.cpp matrix.h

##### SPECIAL TARGETS ######

.PHONY: clean realclean install

clean:
	-rm -f *.o *~

realclean: clean
	-rm -f teste libmatrix.a

install: libmatrix.a
	-cp -f matrix.h ${BASEDIR}/include
	-cp -f libmatrix.a ${BASEDIR}/lib
