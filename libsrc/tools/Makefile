BASEDIR	=	../..

CC	=	g++
CXX	=	g++
CPPFLAGS = -Wall -std=c++11	-I ${BASEDIR}/include
CFLAGS =
LDFLAGS = -L ${BASEDIR}/lib
LDLIBS =	-lpthread

GENERATED	=	teste_clock

all:	${GENERATED}

${GENERATED}:	libsystem_tools.a

libsystem_tools.a:	system_tools.o
	ar r $@ $?

system_tools.o:	system_tools.cpp	system_tools.h

install:	libsystem_tools.a
	cp -f libsystem_tools.a ${BASEDIR}/lib
	cp -f system_tools.h ${BASEDIR}/include

clean:
	rm -f ${GENERATED}	*.o
