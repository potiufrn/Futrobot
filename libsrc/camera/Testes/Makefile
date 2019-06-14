BASEDIR	=	../../..
CC	=	g++
CXX	=	g++
CPPFLAGS	=	-Wall -I	${BASEDIR}/include
LDFLAGS	=	-L	${BASEDIR}/lib
LDLIBS = -limagem	-lsystem_tools

PROGRAMAS	=	teste_camera	#teste_captura	#teste_v4l2 teste_retas	teste_camera

all:	${PROGRAMAS}

${PROGRAMAS}:	../camera.cpp ../camera.h

clean:
	rm	-f	${PROGRAMAS}	*.txt	*.ppm	*.IB
