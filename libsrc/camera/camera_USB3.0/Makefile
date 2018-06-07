BASEDIR = ../../..

CC = g++
CXX = g++
CPPFLAGS = -Wall -O2 -I ${BASEDIR}/include
CFLAGS =
LDFLAGS = -O2 -L ${BASEDIR}/lib
LDLIBS = -limagem	-lotim

PROGRAMAS	=	teste_camera	teste_captura	teste_v4l2	teste_CampoVazio	#teste_retas

all:	${PROGRAMAS}	libcamera.a

${PROGRAMAS}:	libcamera.a	../../../program/system.o

libcamera.a: camera.o
	ar r $@ $?

camera.o: camera.h camera.cpp

##### SPECIAL TARGETS ######

.PHONY: clean realclean install

clean:
	-rm -f *.o *~ *.bak	*.ppm

realclean: clean
	-rm -f libcamera.a

install: libcamera.a
	-cp -f camera.h ${BASEDIR}/include
	-cp -f libcamera.a ${BASEDIR}/lib
