BASEDIR = /futrobot

CC = g++
CXX = g++
CPPFLAGS = -Wall -O2 -I ${BASEDIR}/include #-DTRANSMISSOR_VELHO
CFLAGS =
LDFLAGS = -O2 -L ${BASEDIR}/lib

#Use essa opcao para compilar o sistema real e o simulado
LDLIBS = -lcamera -ldc1394 -lraw1394 -limagem -lotim -lmatrix -lpthread -lssocket -lftdi -lusbRobot

#Use essa opcao para compilar somente o sistema simulado
#LDLIBS = -lmatrix -lpthread -lssocket

FILES = futdata.o acquisition.o localization.o strategy.o obstacles.o	\
control.o futrobot.o main.o export.o futjoy.o
TRANS = transmission.o
OLDTRANS = oldtransmission.o

#Use essa opcao para compilar o sistema real e o simulado
#Se for soh simulado, comente as proximas tres linhas
all: main
#	./virasuroot
#	touch all

oldtrans: oldmain
#	./virasuroot
#	touch all

main: ${FILES} ${TRANS} ../system.o ../dados_calibracao.o

oldmain: ${FILES} ${OLDTRANS} ../system.o ../dados_calibracao.o
	$(CC) $(LDFLAGS) $(CPPFLAGS) $^ -o main $(LDLIBS)
#	./virasuroot
#	touch oldmain

main.o: main.cpp

acquisition.o: acquisition.h acquisition.cpp

localization.o: localization.h localization.cpp

strategy.o: strategy.h strategy.cpp

obstacles.o: obstacles.h obstacles.cpp

control.o: control.h control.cpp

transmission.o: transmission.h transmission.cpp

oldtransmission.o: transmission.h transmission.cpp
	$(CC) $(CPPFLAGS) -c -o $@ transmission.cpp -DTRANSMISSOR_VELHO

futdata.o: futdata.h futdata.cpp

futrobot.o: futrobot.h futrobot.cpp

futjoy.o: futjoy.h futjoy.cpp

##### SPECIAL TARGETS ######

.PHONY: clean realclean 

clean:
	-rm -f *.o *~ 

realclean: clean
	-rm -f main


