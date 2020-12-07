#BASEDIR = /futrobot
BASEDIR = ../..
INSTALLDIR = ${BASEDIR}

CC = g++
CXX = g++
CPPFLAGS = -std=c++11 -Wall -O2 
CFLAGS =
LDFLAGS =
LDLIBS = -O2 -lpthread

all: cliente_tcp servidor_tcp cliente_udp servidor_udp

cliente_tcp: libssocket.a cliente_tcp.o

servidor_tcp:  libssocket.a servidor_tcp.o

cliente_udp: libssocket.a cliente_udp.o

servidor_udp:  libssocket.a servidor_udp.o

libssocket.a: ssocket.o
	ar r $@ $?

ssocket.o cliente_tcp.o servidor_tcp.o cliente_udp.o servidor_udp.o: ssocket.h

##### SPECIAL TARGETS ######

.PHONY: clean realclean

clean:
	-rm -f *.o *~

realclean: clean
	-rm -f libssocket.a cliente_tcp servidor_tcp cliente_udp servidor_udp

install: libssocket.a
	-cp -f ssocket.h ${INSTALLDIR}/include
	-cp -f libssocket.a ${INSTALLDIR}/lib
