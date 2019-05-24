BASEDIR	=	../
CC	=	g++
CXX	=	g++
CPPFLAGS	=	-Wall -I	${BASEDIR}/include -I	${BASEDIR}/program/main
LDFLAGS	=	-L	${BASEDIR}/lib
LDLIBS = -limagem	-lcamera	-lmatrix	-lotim	-lssocket

PROGRAMAS	= acquisition_data teste_CampoVazio

all:	${PROGRAMAS}

${PROGRAMAS}:	../program/system.o	../program/main/acquisition.o	../program/dados_calibracao.o	../program/main/futdata.o

clean:
	rm	-f	${PROGRAMAS}	*.txt	*.ppm
