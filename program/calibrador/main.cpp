#include <QApplication>
#include "calibrador.h"
#include "calibratorprocessor.h"
#include "variaveisglobais.h"
#include <iostream>
#include <cstdlib>

// using namespace std;

//bool terminar = false;
CalibratorProcessor X;

void *thread_camera(void *arg)
{
  ((CalibratorProcessor*)arg)->run();
  pthread_exit((void *)NULL);
};

int main( int argc, char ** argv )
{

    if(argc >2){
    	std::cerr<<"Parametros invalidos!"<<'\n';
    	std::cerr<<"Use ./calibrador ou ./calibrador <arquivo_de_configuracao>"<<'\n';
    	exit(1);
    }

    //CalibratorProcessor CP(a.argc() == 2? a.argv()[1] : NULL);
    if(X.readFile(argc == 2? argv[1] : "")){
    	std::cerr<<"Erro abrindo arquivo de definicao do calibrador!"<<'\n';
    	exit(1);
    }

    QApplication a( argc, argv );


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


    calibrador w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    a.exec();


    pthread_join(a_thread,NULL);

    return 0;

}
