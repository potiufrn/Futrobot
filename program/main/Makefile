#BASEDIR = /futrobot
BASEDIR = ../..

CC = g++
CXX = g++
CPPFLAGS = -Wall -O2 -I ${BASEDIR}/include 
CFLAGS =
LDFLAGS = -O2 -L ${BASEDIR}/lib

#Use essa opcao para compilar o sistema real e o simulado
LDLIBS = -lcamera -limagem -lotim -lmatrix -lpthread -lssocket -lbluetoothAction -lbluetooth

#Use essa opcao para compilar somente o sistema simulado
#LDLIBS = -lmatrix -lpthread -lssocket

FILES = futdata.o acquisition.o localization.o strategy.o obstacles.o	\
control.o transmission.o futrobot.o main.o export.o

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

##### SPECIAL TARGETS ######

.PHONY: clean realclean 

clean:
	-rm -f *.o *~ 

realclean: clean
	-rm -f main


