#include "calibrador.h"
#include "calibratorprocessor.h"
#include "variaveisglobais.h"
#include <QApplication>
#include <QPalette>
#include <iostream>
#include <cstdlib>

using namespace std;

CalibratorProcessor X; // Gambiarra extrema! Retirar isso daqui!!

void *thread_camera(void *arg)
{
    ((CalibratorProcessor*)arg)->run();
    pthread_exit((void *)NULL);
};

int main(int argc, char *argv[])
{
    //---------------------------------------<
    if(argc > 2){
        std::cerr<<"Parametros invalidos!"<<'\n';
        std::cerr<<"Use ./calibrador ou ./calibrador <arquivo_de_configuracao>"<<'\n';
        exit(1);
    }

    //CalibratorProcessor CP(a.argc() == 2? a.argv()[1] : NULL);
    if(X.readFile(argc == 2? argv[1] : "")){
        std::cerr<<"Erro abrindo arquivo de definicao do calibrador!"<<'\n';
        exit(1);
    }

    int res;
    pthread_t a_thread;
    unsigned numDevices = 0;
    unsigned index = 0;
    do{
        numDevices = X.listDevices();
        if(numDevices == 0){
            std::cerr << "\nWARNING: Nenhum Dispositivo de video conectado" << '\n';
            return 1;
        }
        std::cout << "\nInforme um index valido da Camera : " << '\n';
        std::cin >> index;
    }while(index >= numDevices);
    std::cin.ignore(1,'\n');

    X.Open(index);

    res = pthread_create(&a_thread, NULL, thread_camera, (void *)&X);

    if (res != 0) {
        perror("A criacao da Thread falhou\n");
        exit(EXIT_FAILURE);
    }

    //--------------------------------------->


    QApplication a(argc, argv);
    calibrador w;
    w.show();

    //    Tema dark
    //    QPalette darkPalette;
    //    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    //    darkPalette.setColor(QPalette::WindowText, Qt::white);
    //    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    //    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    //    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    //    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    //    darkPalette.setColor(QPalette::Text, Qt::white);
    //    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    //    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    //    darkPalette.setColor(QPalette::BrightText, Qt::red);
    //    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    //    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    //    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    //    a.setPalette(darkPalette);

    QObject::connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(quitProgram()));

    //return a.exec();
    a.exec();

    pthread_join(a_thread,NULL);

    return 0;
}
