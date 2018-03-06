BASEDIR = ..

CC = g++ 
CXX = g++ 
CPPFLAGS = -Wall -O2 -I ${BASEDIR}/include
CFLAGS =
LDFLAGS = -L ${BASEDIR}/lib
LDLIBS = 

PROGRAMS = system.o dados_calibracao.o functions.o

all: makeprogram makecalibrador makeinterface makesimple makemain

old: makeprogram makecalibrador makeinterface makesimpleold makemainold


makeprogram: ${PROGRAMS}

makemain: makeprogram
	cd main && ${MAKE}

makemainold: makeprogram
	cd main && ${MAKE} old

makecalibrador: makeprogram
	cd calibrador && ${MAKE}

makeinterface: makeprogram makemain
	cd interface && ${MAKE}

makesimple: 
	cd simple && ${MAKE}

makesimpleold: 
	cd simple && ${MAKE} old

system.o: system.h system.cpp

functions.o: functions.h functions.cpp

dados_calibracao.o: dados_calibracao.h dados_calibracao.cpp

##### SPECIAL TARGETS ######

.PHONY: clean cleanall 

clean:
	-rm -f *.o *~ 
	cd main && ${MAKE} clean
	cd calibrador && ${MAKE} clean
	cd interface && ${MAKE} clean
	cd simple && ${MAKE} clean

cleanall: clean
	cd main && ${MAKE} cleanall
	cd calibrador && ${MAKE} clean
	cd interface && ${MAKE} clean
	cd simple && ${MAKE} cleanall



