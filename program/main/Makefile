#BASEDIR = /futrobot
BASEDIR = ../..

CC = g++
CXX = g++
CPPFLAGS = -Wall -std=c++11	-O2 -I ${BASEDIR}/include
CFLAGS =
LDFLAGS = -O2 -L ${BASEDIR}/lib	`pkg-config --cflags protobuf`

#Use essa opcao para compilar o sistema real e o simulado
LDLIBS = -lcamera -limagem -lotim \
		 -lmatrix -lssocket -lpthread\
		 -lbluetoothAction -lbluetooth	\
		 -lsystem_tools	-lprotopacket	\
		 -lprotobuf	`pkg-config --libs protobuf`

#Use essa opcao para compilar somente o sistema simulado
# LDLIBS = -limagem -lmatrix -lpthread -lssocket	\
# 		 -lprotopacket -lprotobuf	`pkg-config --libs protobuf`

FILES = futdata.o acquisition.o localization.o strategy.o obstacles.o	\
control.o transmission.o export.o futrobot.o main.o

all: main

main: ${FILES} ../system.o ../dados_calibracao.o

main.o: main.cpp

acquisition.o: acquisition.h acquisition.cpp

localization.o: localization.h localization.cpp

strategy.o: strategy.h strategy.cpp

obstacles.o: obstacles.h obstacles.cpp

control.o: control.h control.cpp

transmission.o: transmission.h transmission.cpp

futdata.o: futdata.h futdata.cpp

futrobot.o: futrobot.h futrobot.cpp

export.o:	export.h	export.cpp

##### SPECIAL TARGETS ######

.PHONY: clean realclean

clean:
	-rm -f *.o *~

realclean: clean
	-rm -f main
