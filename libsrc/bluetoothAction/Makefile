BASEDIR = ../..
INSTALLDIR = ${BASEDIR}

CC = g++
CXX = g++
CPPFLAGS = -std=c++11 -Wall -O2 -I ${BASEDIR}/include
CFLAGS =
LDFLAGS = -O2 -L ${BASEDIR}/lib
LDLIBS = -lbluetooth	-fopenmp

all: libbluetoothAction.a

libbluetoothAction.a: bluetoothAction.o
	ar r $@ $?

bluetoothAction.o: bluetoothAction.cpp bluetoothAction.h

##### SPECIAL TARGETS ######

.PHONY: clean realclean

clean:
	-rm -f *.o *~

realclean: clean
	-rm -f libbluetoothAction.a

install: libbluetoothAction.a
	-cp -f bluetoothAction.h ${INSTALLDIR}/include
	-cp -f libbluetoothAction.a ${INSTALLDIR}/lib
