TEMPLATE = app
TARGET = calibrador

CONFIG	+= qt warn_on release

LIBS	+= -g ../dados_calibracao.o -L../../lib -lcamera -lmatrix -limagem

INCLUDEPATH	+= ../../include

QT += widgets

HEADERS	+= pixmap_label.h \
	variaveisglobais.h \
	calibratorprocessor.h

SOURCES	+= main.cpp \
	pixmap_label.cpp \
	calibratorprocessor.cpp

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS	+= calibrador.ui

DEFINES += QT_DEPRECATED_WARNINGS

#IMAGES	= images/filenew \
#	images/fileopen \
#	images/filesave \
#	images/print \
#	images/undo \
#	images/redo \
#	images/editcut \
#	images/editcopy \
#	images/editpaste \
#	images/searchfind \
#	fotos/foto01.bmp

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



#The following line was inserted by qt3to4
#QT +=  qt3support
#The following line was inserted by qt3to4
#CONFIG += uic c++11

