#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QWidget>
#include <QProgressBar>
#include "futdatatable.h"
//#include "../futrobot.h"
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "../comunicacao.h"
#include "animacao.h"

//tipos necessarios para operacao com semaforo 
union semnum {
  int val;
  struct semid_ds * buf;
  unsigned short * array;
};

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QLineEdit;
class QRadioButton;
class QGroupBox;
class QLCDNumber;
class QPixmap;
class QCheckBox;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QRadioButton;
class QButtonGroup;
class QTimer;
class QStatusBar;
class QProgressBar;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT



public slots:
    void setPlay();
    void setPause();
    //void changeSide();
    void changeAdv();
    void setPenalty();
    void setFreekick();
    void setGoalkick();
    void setFreeball();
    void setInicialPosition();
    void cleanFutRobot();
    void setUpdate();


private:
    FutDataTable *datatable;

    int datatable_timerID;

    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *penaltyButton;
    QPushButton *freekickButton;
    QPushButton *goalkickButton;
    QPushButton *freeballButton;
    QPushButton *inicialPositionButton;
    //QPushButton *finishButton;


    QGroupBox * advGroup;
    QRadioButton * vantagem;
    QRadioButton * desvantagem;
    QButtonGroup * grpadv;

    QGroupBox * fundoGroup;
    QButtonGroup * grpsd;
    QCheckBox * imagem;

    QGroupBox * itemGroup;
    QButtonGroup * grpitem;
    QCheckBox * linhasCheck;
    QCheckBox * meusCheck;
    QCheckBox * advsCheck;
    QCheckBox * rotuCheck;
    QCheckBox * refsCheck;
    QCheckBox * futureBallCheck;

    QStatusBar * sb;
    //QProgressBar * pb;

    QTimer * tempor;

    Animacao * frame;

    //MEU TIME
    int my_team;//1 -> AMARELO  | 2 -> AZUL
    //Vantagem
    bool vant;

    //MEMBROS PARA MEMORIA E SEMAFORO
    int semid;
    sembuf semdo;
    semnum semdat;
        //Sharedmem sme;
    //void * ptmem;
    int smeid;
    //bool readonly;
    //bool ready;

    PACKAGEDAT * pt;


public:
    MainWindow(int te, QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
