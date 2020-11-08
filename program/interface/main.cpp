#include <QApplication>
#include "mainwindow.h"
#include "startwindow.h"
#include <stdio.h>
//#include "../futrobot.h"
#include <QLocale>
#include <iostream>

using namespace::std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int idTeam;// idSide;
    //idTeam e side receberam valores que indicam cor do time e lado do jogo
    //        idTeam         idSide
    //0 ->    nao escolheu   nao escolheu
    //1 ->    Amarelo        Esquerda
    //2 ->    Azul           Direita
    StartWindow sw(&idTeam/*, &idSide*/);
    sw.show();
    a.exec();

    //       valores validos
    if(idTeam == 1 || idTeam == 2){//  1 <= idTeam <= 2

        MainWindow w(idTeam);
        QObject::connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(cleanFutRobot()));
        w.show();
        return a.exec();
    }
    return 0;
}
