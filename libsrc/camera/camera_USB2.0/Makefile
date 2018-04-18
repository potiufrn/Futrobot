BASEDIR = ../../..

CC = g++
CXX = g++
CPPFLAGS = -Wall -O2 -I ${BASEDIR}/include
CFLAGS =
LDFLAGS = -O2 -L ${BASEDIR}/lib
LDLIBS = -limagem -lxfig -lotim -lmatrix -lpthread

all: camera.o libcamera.a

libcamera.a: camera.o
	ar r $@ $?

camera.o: camera.h camera.cpp

##### SPECIAL TARGETS ######

.PHONY: clean realclean install

clean:
	-rm -f *.o *~ *.bak

realclean: clean
	-rm -f libcamera.a

install: libcamera.a
	-cp -f camera.h ${BASEDIR}/include
	-cp -f baseframebuffer.h ${BASEDIR}/include
	-cp -f libcamera.a ${BASEDIR}/lib
