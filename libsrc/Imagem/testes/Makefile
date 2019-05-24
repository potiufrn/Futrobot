#BASEDIR = /futrobot
BASEDIR = ../../..

CC = g++
CXX = g++
CPPFLAGS = -Wall -O2 -I ${BASEDIR}/include
CFLAGS =
LDFLAGS = -O2 -L ${BASEDIR}/lib
LDLIBS = -lotim -lmatrix

PROGRAMAS = teste_imagem	#teste_HPG teste_YUV teste_tempo

all: ${PROGRAMAS}

${PROGRAMAS}:	../libimagem.a
##### SPECIAL TARGETS ######

clean:
	-rm -f *.o *~ *.bak *.fig Campo_*.ppm IMG_*.ppm

realclean: clean
	-rm -f ${PROGRAMAS}
