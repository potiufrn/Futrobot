#include "mainwindow.h"
#include <QtWidgets>
#include <stdio.h>

#define BUTWIDTH 75

MainWindow::MainWindow(int te, QWidget *parent)
    : QWidget(parent),
    my_team (te),
    vant(true),
    smeid(0),
    pt (NULL)
{
    setWindowIcon(QIcon(":poti.ico"));

    playButton = new QPushButton(tr("PLAY"));
    playButton->setEnabled(true);
    playButton->setFixedWidth(BUTWIDTH);
    pauseButton = new QPushButton(tr("PAUSE"));
    pauseButton->setEnabled(true);
    pauseButton->setFixedWidth(BUTWIDTH);
    penaltyButton = new QPushButton(tr("Penalty"));
    penaltyButton->setEnabled(true);
    penaltyButton->setFixedWidth(BUTWIDTH);
    freekickButton = new QPushButton(tr("Freekick"));
    freekickButton->setEnabled(true);
    freekickButton->setFixedWidth(BUTWIDTH);
    goalkickButton = new QPushButton(tr("Goalkick"));
    goalkickButton->setEnabled(true);
    goalkickButton->setFixedWidth(BUTWIDTH);
    freeballButton = new QPushButton(tr("Freeball"));
    freeballButton->setEnabled(true);
    freeballButton->setFixedWidth(BUTWIDTH);
    inicialPositionButton = new QPushButton(tr("Ini Pos"));
    inicialPositionButton->setEnabled(true);
    inicialPositionButton->setFixedWidth(BUTWIDTH);

    advGroup = new QGroupBox("Advantage:");
    grpadv = new QButtonGroup();
    vantagem = new QRadioButton("Adv");
    desvantagem = new QRadioButton("Dadv");
    vantagem->setChecked(true);
    grpadv->addButton(vantagem, 1);
    grpadv->addButton(desvantagem, 2);

    fundoGroup = new QGroupBox("Fundo:");
    grpsd = new QButtonGroup();
    grpsd->setExclusive(false);
    imagem = new QCheckBox("Camera");

    grpsd->addButton(imagem, 1);

    itemGroup = new QGroupBox("Itens:");
    grpitem = new QButtonGroup();
    grpitem->setExclusive(false);
    linhasCheck = new QCheckBox("Linhas");
    linhasCheck->setChecked(true);
    meusCheck = new QCheckBox("Meus robos");
    meusCheck->setChecked(true);
    advsCheck = new QCheckBox("Adversarios");
    advsCheck->setChecked(true);
    rotuCheck = new QCheckBox("Rotulos");
    rotuCheck->setChecked(true);
    refsCheck = new QCheckBox("Referencias");
    refsCheck->setChecked(true);
    futureBallCheck = new QCheckBox("Bola Futura");
    futureBallCheck->setChecked(true);
    grpitem->addButton(linhasCheck, 1);
    grpitem->addButton(meusCheck, 2);
    grpitem->addButton(advsCheck, 3);
    grpitem->addButton(rotuCheck, 4);
    grpitem->addButton(refsCheck, 5);
    grpitem->addButton(futureBallCheck, 6);

    //STATUS BAR
    sb = new QStatusBar(this);
    sb->showMessage("STATUS BAR");

    //TESTANDO PROGRESS BAR
    //só aceita valores inteiros
    //portanto perguntar fator de multiplicação e transformaçao


    tempor = new QTimer(this);
    tempor->start(60);

    frame = new Animacao (my_team, this);
    frame ->show();

    //LAYOUTS
    QGridLayout * buttonLayout = new QGridLayout;
         buttonLayout->addWidget(playButton,0,0);
         buttonLayout->addWidget(pauseButton,1,0);
         buttonLayout->addWidget(penaltyButton,0,1);
         buttonLayout->addWidget(freekickButton,1,1);
         buttonLayout->addWidget(goalkickButton,0,2);
         buttonLayout->addWidget(freeballButton,1,2);
         buttonLayout->addWidget(inicialPositionButton,0,3);
         //teste PROGRESS BAR
         //buttonLayout->addWidget(pb,2,3);

        QVBoxLayout * advantageLayout = new QVBoxLayout;
            advantageLayout->addWidget(vantagem);
            advantageLayout->addWidget(desvantagem);

            advGroup->setLayout(advantageLayout);

         buttonLayout->addWidget(advGroup,0,4,2,1);

    datatable = new FutDataTable(my_team, this);

    QHBoxLayout * flagsLayout = new QHBoxLayout;
        QGridLayout * backgrLayout = new QGridLayout;
            backgrLayout->addWidget(imagem, 0, 0, Qt::AlignTop);
           // backgrLayout->addWidget(preto, 1, 0, Qt::AlignTop);
          //  backgrLayout->addWidget(regioes, 1, 1, Qt::AlignTop);
            fundoGroup->setLayout(backgrLayout);
        flagsLayout->addWidget(fundoGroup);

        QGridLayout * itensLayout = new QGridLayout;
            itensLayout->addWidget(linhasCheck, 0, 0, Qt::AlignTop);
            itensLayout->addWidget(meusCheck, 1, 0, Qt::AlignTop);
            itensLayout->addWidget(advsCheck, 0, 1, Qt::AlignTop);
            itensLayout->addWidget(rotuCheck, 1, 1, Qt::AlignTop);
            itensLayout->addWidget(refsCheck, 0, 2, Qt::AlignTop);
            itensLayout->addWidget(futureBallCheck, 1, 2, Qt::AlignTop);
            itemGroup->setLayout(itensLayout);
        flagsLayout->addWidget(itemGroup);

    QGridLayout *mainLayout = new QGridLayout;
        mainLayout->addWidget(datatable, 0, 0,Qt::AlignTop);
        mainLayout->addLayout(buttonLayout, 1, 0,Qt::AlignTop);
        mainLayout->addWidget(frame, 0, 1, Qt::AlignTop);
        mainLayout->addLayout(flagsLayout, 1, 1, Qt::AlignTop);
        mainLayout->addWidget(sb, 2,0, 1, -1, Qt::AlignBottom);
    setLayout(mainLayout);

    setWindowTitle("FutRobot - POTI/UFRN");

         //CONNECTIONS
    connect(playButton, SIGNAL (clicked()), this, SLOT(setPlay()));
    connect(pauseButton, SIGNAL (clicked()), this, SLOT(setPause()));
    connect(vantagem, SIGNAL (clicked()), this, SLOT(changeAdv()));
    connect(desvantagem, SIGNAL (clicked()), this, SLOT(changeAdv()));
    connect(penaltyButton, SIGNAL (clicked()), this, SLOT(setPenalty()));
    connect(freekickButton, SIGNAL (clicked()), this, SLOT(setFreekick()));
    connect(goalkickButton, SIGNAL (clicked()), this, SLOT(setGoalkick()));
    connect(freeballButton, SIGNAL (clicked()), this, SLOT(setFreeball()));
    connect(inicialPositionButton, SIGNAL (clicked()), this, SLOT(setInicialPosition()));
    connect(tempor, SIGNAL (timeout()), this, SLOT(setUpdate()));

    //TODO verificar datatable_timerID
    //Import
    //datatable_timerID = datatable->startTimer(200); //inicia o timer da tabela. Executa timerEvent() a cada 500ms.
    //SEMAFORO
    semdo.sem_num = 0;
    semdo.sem_flg = SEM_UNDO;

    semid = semget ((my_team==1)? KEY_SEM_YELLOW : KEY_SEM_BLUE, 1, 0666/*|IPC_EXCL|*/);
    //	printf("\n%d\n", semid);
    if (semid < 0) {
      //nao conseguiu criar semaforo
      printf ("[Import]:Nao foi possivel encontrar o semaforo!\n");
      exit(-1);
    }

    //definindo valor inicial do semaforo
    semdat.val = 1;
    semctl (semid, 0, SETVAL, semdat);

    //MEMORIA
    smeid = shmget ((my_team==1)? KEY_MEM_YELLOW : KEY_MEM_BLUE, sizeof(PACKAGEDAT), 0666);
    if (smeid <= 0) {
      //nao conseguiu criar memoria
      printf("[Import]:Nao foi possivel encontrar a mem. comp.!\n");
      exit(-1);
    }

    //Attach
    pt = (PACKAGEDAT *) shmat (smeid, (void *) 0, SHM_RND);

}

MainWindow::~MainWindow(){
    //sem.~Semaphore();
     semctl (semid, 0, IPC_RMID);

    // sme.~Sharedmem();
     shmdt (pt);
     shmctl (smeid, IPC_RMID, 0);
}

void MainWindow::setPlay(){
    semdo.sem_op = -1;
    semop (semid, &semdo, 1);

    //PACKAGEDAT pass = *pt;
//    pt->ret_game_state = PLAY_STATE
    pt->ret_game = PLAY_STATE;
    //FINAL REGIAO CRITICA
    semdo.sem_op = 1;
    semop (semid, &semdo, 1);
}

void MainWindow::setPause(){
    semdo.sem_op = -1;
    semop (semid, &semdo, 1);

    //PACKAGEDAT pass = *pt;
    pt->ret_game = PAUSE_STATE;

    //FINAL REGIAO CRITICA
    semdo.sem_op = 1;
    semop (semid, &semdo, 1);

}

/*void MainWindow::changeSide(){
}*/

void MainWindow::changeAdv(){
    semdo.sem_op = -1;
    semop (semid, &semdo, 1);

    if(vantagem->isChecked()){
        pt->adv = true;
    } else {
        pt->adv = false;
    }

    //FINAL REGIAO CRITICA
    semdo.sem_op = 1;
    semop (semid, &semdo, 1);
}


void MainWindow::setPenalty(){
    semdo.sem_op = -1;
    semop (semid, &semdo, 1);

    //PACKAGEDAT pass = *pt;
    pt->ret_game = PENALTY_STATE;

    //FINAL REGIAO CRITICA
    semdo.sem_op = 1;
    semop (semid, &semdo, 1);
}

void MainWindow::setFreekick(){
    semdo.sem_op = -1;
    semop (semid, &semdo, 1);

    //PACKAGEDAT pass = *pt;
    pt->ret_game = FREEKICK_STATE;

    //FINAL REGIAO CRITICA
    semdo.sem_op = 1;
    semop (semid, &semdo, 1);
}

void MainWindow::setGoalkick(){
    semdo.sem_op = -1;
    semop (semid, &semdo, 1);

    //PACKAGEDAT pass = *pt;
    pt->ret_game = GOALKICK_STATE;

    //FINAL REGIAO CRITICA
    semdo.sem_op = 1;
    semop (semid, &semdo, 1);
}

void MainWindow::setFreeball(){
    semdo.sem_op = -1;
    semop (semid, &semdo, 1);

    //PACKAGEDAT pass = *pt;
    pt->ret_game = FREEBALL_STATE;

    //FINAL REGIAO CRITICA
    semdo.sem_op = 1;
    semop (semid, &semdo, 1);
}

void MainWindow::setInicialPosition(){
    semdo.sem_op = -1;
    semop (semid, &semdo, 1);

    //PACKAGEDAT pass = *pt;
    pt->ret_game = INICIALPOSITION_STATE;

    //FINAL REGIAO CRITICA
    semdo.sem_op = 1;
    semop (semid, &semdo, 1);
}

void MainWindow::setUpdate(){

    //INICIO REGIAO CRITICA
    semdo.sem_op = -1;
    semop (semid, &semdo, 1);

    PACKAGEDAT pass = *pt;


    //FINAL REGIAO CRITICA
    semdo.sem_op = 1;
    semop (semid, &semdo, 1);

    datatable->updateMeus(0, pass.pos.me[0]);
    datatable->updateMeus(1, pass.pos.me[1]);
    datatable->updateMeus(2, pass.pos.me[2]);
    datatable->updateAdvs(0, pass.pos.op[0]);
    datatable->updateAdvs(1, pass.pos.op[1]);
    datatable->updateAdvs(2, pass.pos.op[2]);

    datatable->updateRefs(0, pass.ref.me[0]);
    datatable->updateRefs(1, pass.ref.me[1]);
    datatable->updateRefs(2, pass.ref.me[2]);

    datatable->updatePwms(0, pass.pwm.me[0]);
    datatable->updatePwms(1, pass.pwm.me[1]);
    datatable->updatePwms(2, pass.pwm.me[2]);

    datatable->updateBola(pass.pos.ball, pass.vel_ball);


    datatable->updateRols(0, pass.roles.me[0]);
    datatable->updateRols(1, pass.roles.me[1]);
    datatable->updateRols(2, pass.roles.me[2]);

    datatable->updateBlck(pass.block0, pass.block1, pass.block2);

    frame->setMeusPos(pass.pos.me[0], 0);
    frame->setMeusPos(pass.pos.me[1], 1);
    frame->setMeusPos(pass.pos.me[2], 2);
    frame->setAdvsPos(pass.pos.op[0], 0);
    frame->setAdvsPos(pass.pos.op[1], 1);
    frame->setAdvsPos(pass.pos.op[2], 2);

    frame->setrefPos(pass.ref.me[0], 0);
    frame->setrefPos(pass.ref.me[1], 1);
    frame->setrefPos(pass.ref.me[2], 2);

    frame->setFunction(pass.roles.me[0].funcao,
                       pass.roles.me[1].funcao,
                       pass.roles.me[2].funcao);

    frame->setballPos(pass.pos.ball, pass.vel_ball);

    frame->setVarControl(grpsd->checkedId(), linhasCheck->isChecked(), meusCheck->isChecked(),
                         advsCheck->isChecked(), rotuCheck->isChecked(), refsCheck->isChecked(),
                         futureBallCheck->isChecked());



    //PxRGB * ptemp= &(pass.imagem[0][0]);
    //frame->setimg(ptemp);
    if (imagem->isChecked()){
        // frame->setimg((const PxRGB *) pass.imagem);
    }
    frame->repaint();
}

void MainWindow::cleanFutRobot(){
    //killTimer(datatable_timerID);
    QCoreApplication::quit();
    //sem_destroy(&sem);
    std::cerr<<"Interface finalizada."<<std::endl;
}
