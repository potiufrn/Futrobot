BASEDIR = ../..
INSTALLDIR = ${BASEDIR}

CC = g++
CXX = g++
CPPFLAGS = -Wall -O2 
CFLAGS =
LDFLAGS =	`pkg-config --cflags protobuf`
LDLIBS = -O2 -lprotobuf	-lssocket `pkg-config --libs protobuf`

OBJS = command.pb.o common.pb.o	packet.pb.o	replacement.pb.o

all:	libprotopacket.a

libprotopacket.a:	${OBJS}
	ar r $@ $?

command.pb.o:	command.pb.h command.pb.cc

common.pb.o:	common.pb.h	common.pb.cc

packet.pb.o:	packet.pb.h	packet.pb.cc

replacement.pb.o:	replacement.pb.h	replacement.pb.cc

install:	libprotopacket.a
	cp libprotopacket.a	${BASEDIR}/lib
	cp *.h ${BASEDIR}/include

realclean:	clean

clean:
	rm ${OBJS}
	rm libprotopacket.a