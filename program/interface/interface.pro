# -------------------------------------------------
# Project created by QtCreator 2010-08-10T09:28:53
# -------------------------------------------------
QT += opengl
TARGET = interface
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \ # field.cpp \
    startwindow.cpp \
    futdatatable.cpp \
    animacao.cpp
HEADERS += mainwindow.h \ # field.h \
    startwindow.h \
    futdatatable.h \
    animacao.h

 OBJECTS += ../dados_calibracao.o
# ../localization.o \
# ../futdata.o \
# ../obstacles.o \
# ../control.o \
# ../transmission.o \
# ../../system.o \
# ../strategy.o \
# ../export.o
INCLUDEPATH += ../../include
LIBS += -L../../lib \
    -limagem \
    -lssocket \
    -lmatrix \
    -lpthread \
    -lotim \
