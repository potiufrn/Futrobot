BASEDIR	=	../../..
CC	=	g++
CXX	=	g++
CPPFLAGS	=	-Wall -std=c++11	-I	${BASEDIR}/include
LDFLAGS	=	-L	${BASEDIR}/lib
LDLIBS = -fopenmp -lbluetooth

PROGRAMAS	=	test	teste_SimplePlus#teste_captura	#teste_v4l2 teste_retas	teste_camera

all:	${PROGRAMAS}

${PROGRAMAS}:	../bluetoothAction.cpp ../bluetoothAction.h	

clean:
	rm	-f	${PROGRAMAS}
