BASEDIR = ../..

CC = g++
CXX = g++
CPPFLAGS = -Wall -O2 -I ${BASEDIR}/include	-fopenmp
CFLAGS =
LDFLAGS = -O2 -L ${BASEDIR}/lib
LDLIBS = -lbluetoothAction -lbluetooth

PROGRAMS	= 

all: simple

simple: simple.o

##### SPECIAL TARGETS ######

.PHONY: clean realclean

clean:
	-rm -f *.o *~	${PROGRAMS}


realclean: clean
	-rm -f  *.a simple
