#include "startwindow.h"
#include <QGridLayout>

StartWindow::StartWindow(int * newteam,/* int * newside,*/ QWidget * parent):
  QWidget(parent),
  myteam(newteam)
 // myside(newside)
  {
    *myteam= 0;

    team = new QGroupBox(tr ("Choose your team color:"));
    ylw = new QRadioButton(tr ("Yellow"));
    ble = new QRadioButton(tr ("Blue"));
    grpteam = new QButtonGroup ();
    grpteam->addButton(ylw, 1);
    grpteam->addButton(ble, 2);

    ok = new QPushButton(tr ("OK"));
    ok->setEnabled(false);
    close = new QPushButton(tr("Exit"));

        QVBoxLayout * radioteamlay = new QVBoxLayout;
        //radioteamlay->addWidget(team);
        radioteamlay->addWidget(ylw);
        radioteamlay->addWidget(ble);
        team->setLayout(radioteamlay);

      QHBoxLayout * radioLayout = new QHBoxLayout;
      radioLayout->addWidget(team);

      QHBoxLayout * buttonLayout = new QHBoxLayout;
      buttonLayout->addWidget(ok);
      buttonLayout->addWidget(close);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addLayout(radioLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setWindowTitle(tr("FutRobot - POTI/UFRN"));

    //Connections
    connect(ylw, SIGNAL(toggled(bool)),this, SLOT(optionsselected()));
    connect(ble, SIGNAL(toggled(bool)),this, SLOT(optionsselected()));
    connect(ok, SIGNAL (clicked()), this, SLOT(optionschoosen()));
    connect(ok, SIGNAL (clicked()), this, SLOT(close()));
    connect(close, SIGNAL (clicked()), this, SLOT(close()));

}

void StartWindow::optionsselected(){
    if(grpteam->checkedId() > 0 /*&& grpside->checkedId() > 0*/)
        ok->setEnabled(true);
    else
        ok->setEnabled(false);
}

void StartWindow::optionschoosen(){
    //grpteam->checkedId() retorna 1 para Amarelo
    //                             2 para Azul
     *myteam = grpteam->checkedId();
     //grpside->checkedId() retorna 1 para Esquerda
     //                             2 para Direita
     //*myside = grpside->checkedId();
}

StartWindow::~StartWindow(){
}
