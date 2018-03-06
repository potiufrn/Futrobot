#include "futdatatable.h"
#include <QGridLayout>
#include <stdio.h>
#include <QVariant>
#include <iostream>

using namespace::std;

FutDataTable::FutDataTable(int new_team, QWidget * parent) :
QWidget(parent)
{
    if(new_team==1){
        labelMeus = new QLabel("<center><b><font color = yellow>MEU TIME</font></b></center>");
        labelAdv = new QLabel("<center><b><font color = black>ADVERSARIO</font></b></center>");
    }else{
        labelMeus = new QLabel("<center><b><font color = blue>MEU TIME</font></b></center>");
        labelAdv = new QLabel("<center><b><font color = black>ADVERSARIO</font></b></center>");
    }

    labelBola = new QLabel("<center><b><font color = orange>BOLA</font></b></center>");

    labelMeusCian = new QLabel("<b><font color = cyan>0 CIAN</font></b>");
    labelMeusRosa = new QLabel("<b><font color = violet>1 ROSA</font></b>");
    labelMeusVerd = new QLabel("<b><font color = green>2 VERD</font></b>");
    labelAdvCian = new QLabel("");
    labelAdvRosa = new QLabel("");
    labelAdvVerd = new QLabel("");

    labelPosX = new QLabel("<b>Pos x</b>");
    labelPosY = new QLabel("<b>Pos y</b>");
    labelPosTh = new QLabel("<b>Pos th</b>");

    labelRefX = new QLabel("<b>Ref x</b>");
    labelRefY = new QLabel("<b>Ref y</b>");
    labelRefTh = new QLabel("<b>Ref th</b>");

    labelPwmDir = new QLabel("<b>PWM d</b>");
    labelPwmEsq = new QLabel("<b>PWM e</b>");

    labelFunction = new QLabel("<b>Function</b>");

    labelBlock = new QLabel("<b>Block</b>");

    LEMeusCianPosX = new QLineEdit("0.0");
    LEMeusCianPosX->setFixedWidth(50);
    LEMeusCianPosX->setReadOnly(true);
    LEMeusCianPosY = new QLineEdit("0.0");
    LEMeusCianPosY->setFixedWidth(50);
    LEMeusCianPosY->setReadOnly(true);
    LEMeusCianPosTh = new QLineEdit("0.0");
    LEMeusCianPosTh->setFixedWidth(50);
    LEMeusCianPosTh->setReadOnly(true);
    LEMeusRosaPosX = new QLineEdit("0.0");
    LEMeusRosaPosX->setFixedWidth(50);
    LEMeusRosaPosX->setReadOnly(true);
    LEMeusRosaPosY = new QLineEdit("0.0");
    LEMeusRosaPosY->setFixedWidth(50);
    LEMeusRosaPosY->setReadOnly(true);
    LEMeusRosaPosTh = new QLineEdit("0.0");
    LEMeusRosaPosTh->setFixedWidth(50);
    LEMeusRosaPosTh->setReadOnly(true);
    LEMeusVerdPosX = new QLineEdit("0.0");
    LEMeusVerdPosX->setFixedWidth(50);
    LEMeusVerdPosX->setReadOnly(true);
    LEMeusVerdPosY = new QLineEdit("0.0");
    LEMeusVerdPosY->setFixedWidth(50);
    LEMeusVerdPosY->setReadOnly(true);
    LEMeusVerdPosTh = new QLineEdit("0.0");
    LEMeusVerdPosTh->setFixedWidth(50);
    LEMeusVerdPosTh->setReadOnly(true);

    LEAdvCianPosX = new QLineEdit("0.0");
    LEAdvCianPosX->setFixedWidth(50);
    LEAdvCianPosX->setReadOnly(true);

    LEAdvCianPosY = new QLineEdit("0.0");
    LEAdvCianPosY->setFixedWidth(50);
    LEAdvCianPosY->setReadOnly(true);
    LEAdvCianPosTh = new QLineEdit("0.0");
    LEAdvCianPosTh->setFixedWidth(50);
    LEAdvCianPosTh->setReadOnly(true);

    LEAdvRosaPosX = new QLineEdit("0.0");
    LEAdvRosaPosX->setFixedWidth(50);
    LEAdvRosaPosX->setReadOnly(true);

    LEAdvRosaPosY = new QLineEdit("0.0");
    LEAdvRosaPosY->setFixedWidth(50);
    LEAdvRosaPosY->setReadOnly(true);

    LEAdvRosaPosTh = new QLineEdit("0.0");
    LEAdvRosaPosTh->setFixedWidth(50);
    LEAdvRosaPosTh->setReadOnly(true);
    LEAdvVerdPosX = new QLineEdit("0.0");
    LEAdvVerdPosX->setFixedWidth(50);
    LEAdvVerdPosX->setReadOnly(true);
    LEAdvVerdPosY = new QLineEdit("0.0");
    LEAdvVerdPosY->setFixedWidth(50);
    LEAdvVerdPosY->setReadOnly(true);
    LEAdvVerdPosTh = new QLineEdit("0.0");
    LEAdvVerdPosTh->setFixedWidth(50);
    LEAdvVerdPosTh->setReadOnly(true);

    LEBolaPosX = new QLineEdit("0.0");
    LEBolaPosX->setFixedWidth(50);
    LEBolaPosX->setReadOnly(true);
    LEBolaPosY = new QLineEdit("0.0");
    LEBolaPosY->setFixedWidth(50);
    LEBolaPosY->setReadOnly(true);
    LEBolaPosTh = new QLineEdit("0.0");
    LEBolaPosTh->setFixedWidth(50);
    LEBolaPosTh->setReadOnly(true);

    LEMeusCianRefX = new QLineEdit("0.0");
    LEMeusCianRefX->setFixedWidth(50);
    LEMeusCianRefX->setReadOnly(true);

    LEMeusCianRefY = new QLineEdit("0.0");
    LEMeusCianRefY->setFixedWidth(50);
    LEMeusCianRefY->setReadOnly(true);

    LEMeusCianRefTh = new QLineEdit("0.0");
    LEMeusCianRefTh->setFixedWidth(50);
    LEMeusCianRefTh->setReadOnly(true);

    LEMeusRosaRefX = new QLineEdit("0.0");
    LEMeusRosaRefX->setFixedWidth(50);
    LEMeusRosaRefX->setReadOnly(true);

    LEMeusRosaRefY = new QLineEdit("0.0");
    LEMeusRosaRefY->setFixedWidth(50);
    LEMeusRosaRefY->setReadOnly(true);

    LEMeusRosaRefTh = new QLineEdit("0.0");
    LEMeusRosaRefTh->setFixedWidth(50);
    LEMeusRosaRefTh->setReadOnly(true);

    LEMeusVerdRefX = new QLineEdit("0.0");
    LEMeusVerdRefX->setFixedWidth(50);
    LEMeusVerdRefX->setReadOnly(true);

    LEMeusVerdRefY = new QLineEdit("0.0");
    LEMeusVerdRefY->setFixedWidth(50);
    LEMeusVerdRefY->setReadOnly(true);

    LEMeusVerdRefTh = new QLineEdit("0.0");
    LEMeusVerdRefTh->setFixedWidth(50);
    LEMeusVerdRefTh->setReadOnly(true);

    LEMeusCianPwmDir = new QLineEdit("0.0");
    LEMeusCianPwmDir->setFixedWidth(50);
    LEMeusCianPwmDir->setReadOnly(true);

    LEMeusCianPwmEsq = new QLineEdit("0.0");
    LEMeusCianPwmEsq->setFixedWidth(50);
    LEMeusCianPwmEsq->setReadOnly(true);

    LEMeusRosaPwmDir = new QLineEdit("0.0");
    LEMeusRosaPwmDir->setFixedWidth(50);
    LEMeusRosaPwmDir->setReadOnly(true);

    LEMeusRosaPwmEsq = new QLineEdit("0.0");
    LEMeusRosaPwmEsq->setFixedWidth(50);
    LEMeusRosaPwmEsq->setReadOnly(true);

    LEMeusVerdPwmDir = new QLineEdit("0.0");
    LEMeusVerdPwmDir->setFixedWidth(50);
    LEMeusVerdPwmDir->setReadOnly(true);

    LEMeusVerdPwmEsq = new QLineEdit("0.0");
    LEMeusVerdPwmEsq->setFixedWidth(50);
    LEMeusVerdPwmEsq->setReadOnly(true);

    labelMeusCianFunction = new QLabel("none");
    labelMeusCianFunction->setFixedWidth(50);
    labelMeusRosaFunction = new QLabel("none");
    labelMeusRosaFunction->setFixedWidth(50);
    labelMeusVerdFunction = new QLabel("none");
    labelMeusVerdFunction->setFixedWidth(50);

    labelAcoes = new QLabel(tr("<b>Actions</b>"));
    LEAcaoCian = new QLineEdit("none");
    LEAcaoCian->setFixedWidth(50);
    LEAcaoCian->setReadOnly(true);
    LEAcaoRosa =  new QLineEdit("none");
    LEAcaoRosa->setFixedWidth(50);
    LEAcaoRosa->setReadOnly(true);
    LEAcaoVerd =  new QLineEdit("none");
    LEAcaoVerd->setFixedWidth(50);
    LEAcaoVerd->setReadOnly(true);

    pwmE0 = new QProgressBar(this);
    pwmE0->setOrientation(Qt::Horizontal);
    pwmE0->setFormat("%v");
    pwmE0->setRange(-100,100);
    pwmE0->setFixedWidth(50);
    pwmE0->setValue(0);

    pwmD0 = new QProgressBar(this);
    pwmD0->setOrientation(Qt::Horizontal);
    pwmD0->setFormat("%v");
    pwmD0->setRange(-100,100);
    pwmD0->setFixedWidth(50);
    pwmD0->setValue(0);

    pwmE1 = new QProgressBar(this);
    pwmE1->setOrientation(Qt::Horizontal);
    pwmE1->setFormat("%v");
    pwmE1->setRange(-100,100);
    pwmE1->setFixedWidth(50);
    pwmE1->setValue(0);

    pwmD1 = new QProgressBar(this);
    pwmD1->setOrientation(Qt::Horizontal);
    pwmD1->setFormat("%v");
    pwmD1->setRange(-100,100);
    pwmD1->setFixedWidth(50);
    pwmD1->setValue(0);

    pwmE2 = new QProgressBar(this);
    pwmE2->setOrientation(Qt::Horizontal);
    pwmE2->setFormat("%v");
    pwmE2->setRange(-100,100);
    pwmE2->setFixedWidth(50);
    pwmE2->setValue(0);

    pwmD2 = new QProgressBar(this);
    pwmD2->setOrientation(Qt::Horizontal);
    pwmD2->setFormat("%v");
    pwmD2->setRange(-100,100);
    pwmD2->setFixedWidth(50);
    pwmD2->setValue(0);

    labelBlock0 = new QLabel("<b><font color = green><center>NO</center></font></b>");
    labelMeusVerdFunction->setFixedWidth(50);
    labelBlock1 = new QLabel("<b><font color = green><center>NO</center></font></b>");
    labelMeusVerdFunction->setFixedWidth(50);
    labelBlock2 = new QLabel("<b><font color = green><center>NO</center></font></b>");
    labelMeusVerdFunction->setFixedWidth(50);

    int minSize = 5;

    QGridLayout * maingrid = new QGridLayout();
    maingrid->setRowMinimumHeight(2,minSize);
    maingrid->addWidget(labelPosX,3, 0);
    maingrid->addWidget(labelPosY,4, 0);
    maingrid->addWidget(labelPosTh,5, 0);
    maingrid->setRowMinimumHeight(6,minSize);
    maingrid->addWidget(labelRefX,7, 0);
    maingrid->addWidget(labelRefY,8, 0);
    maingrid->addWidget(labelRefTh,9, 0);
    maingrid->setRowMinimumHeight(10,minSize);
    maingrid->addWidget(labelPwmEsq,11, 0);
    maingrid->addWidget(labelPwmDir,12, 0);
    maingrid->setRowMinimumHeight(13,minSize);
    maingrid->addWidget(labelAcoes,14, 0);
    maingrid->addWidget(labelFunction, 15, 0);
    maingrid->setRowMinimumHeight(16, minSize);
    maingrid->addWidget(labelBlock, 17, 0);

    maingrid->setColumnMinimumWidth(1,minSize);

    maingrid->addWidget(labelMeus,0,2,1,5);
    maingrid->addWidget(labelMeusCian,1,2);
    maingrid->setColumnMinimumWidth(3,minSize);
    maingrid->addWidget(labelMeusRosa,1,4);
    maingrid->setColumnMinimumWidth(5,minSize);
    maingrid->addWidget(labelMeusVerd,1,6);

    maingrid->addWidget(LEMeusCianPosX,3,2);
    maingrid->addWidget(LEMeusCianPosY,4,2);
    maingrid->addWidget(LEMeusCianPosTh,5,2);

    maingrid->addWidget(LEMeusRosaPosX,3,4);
    maingrid->addWidget(LEMeusRosaPosY,4,4);
    maingrid->addWidget(LEMeusRosaPosTh,5,4);

    maingrid->addWidget(LEMeusVerdPosX,3,6);
    maingrid->addWidget(LEMeusVerdPosY,4,6);
    maingrid->addWidget(LEMeusVerdPosTh,5,6);


    maingrid->addWidget(LEMeusCianRefX,7,2);
    maingrid->addWidget(LEMeusCianRefY,8,2);
    maingrid->addWidget(LEMeusCianRefTh,9,2);

    maingrid->addWidget(LEMeusRosaRefX,7,4);
    maingrid->addWidget(LEMeusRosaRefY,8,4);
    maingrid->addWidget(LEMeusRosaRefTh,9,4);

    maingrid->addWidget(LEMeusVerdRefX,7,6);
    maingrid->addWidget(LEMeusVerdRefY,8,6);
    maingrid->addWidget(LEMeusVerdRefTh,9,6);


    maingrid->addWidget(pwmE0,11,2);
    maingrid->addWidget(pwmD0,12,2);

    maingrid->addWidget(pwmE1,11,4);
    maingrid->addWidget(pwmD1,12,4);

    maingrid->addWidget(pwmE2,11,6);
    maingrid->addWidget(pwmD2,12,6);

    maingrid->addWidget(LEAcaoCian,14,2);

    maingrid->addWidget(LEAcaoRosa,14,4);

    maingrid->addWidget(LEAcaoVerd,14,6);

    maingrid->addWidget(labelMeusCianFunction, 15, 2);

    maingrid->addWidget(labelMeusRosaFunction, 15, 4);

    maingrid->addWidget(labelMeusVerdFunction, 15, 6);

    maingrid->addWidget(labelBlock0, 17, 2);

    maingrid->addWidget(labelBlock1, 17, 4);

    maingrid->addWidget(labelBlock2, 17, 6);

    maingrid->setColumnMinimumWidth(7,minSize);


    maingrid->addWidget(labelAdv,0,8,1,5);
    maingrid->addWidget(labelAdvCian,1,8);
    maingrid->setColumnMinimumWidth(9,minSize);
    maingrid->addWidget(labelAdvRosa,1,10);
    maingrid->setColumnMinimumWidth(11,minSize);
    maingrid->addWidget(labelAdvVerd,1,12);


    maingrid->addWidget(LEAdvCianPosX,3,8);
    maingrid->addWidget(LEAdvCianPosY,4,8);
    maingrid->addWidget(LEAdvCianPosTh,5,8);

    maingrid->addWidget(LEAdvRosaPosX,3,10);
    maingrid->addWidget(LEAdvRosaPosY,4,10);
    maingrid->addWidget(LEAdvRosaPosTh,5,10);

    maingrid->addWidget(LEAdvVerdPosX,3,12);
    maingrid->addWidget(LEAdvVerdPosY,4,12);
    maingrid->addWidget(LEAdvVerdPosTh,5,12);

    maingrid->setColumnMinimumWidth(13,minSize);

    maingrid->addWidget(labelBola,0,14,2,1);

    maingrid->addWidget(LEBolaPosX,3,14);
    maingrid->addWidget(LEBolaPosY,4,14);
    maingrid->addWidget(LEBolaPosTh,5,14);

    setLayout(maingrid);

}

void FutDataTable::updateMeus(unsigned i, POS_ROBO robo){
    static char buffer[10];

    switch(i) {
    case 2:
        sprintf(buffer, "%+5.2lf", robo.x());
        LEMeusVerdPosX->setText(buffer);
        sprintf(buffer, "%+5.2lf", robo.y());
        LEMeusVerdPosY->setText(buffer);
        sprintf(buffer, "%+5.2lf", robo.theta());
        LEMeusVerdPosTh->setText(buffer);
        break;

    case 1:
        sprintf(buffer, "%+5.2lf", robo.x());
        LEMeusRosaPosX->setText(buffer);
        sprintf(buffer, "%+5.2lf", robo.y());
        LEMeusRosaPosY->setText(buffer);
        sprintf(buffer, "%+5.2lf", robo.theta());
        LEMeusRosaPosTh->setText(buffer);
        break;

    default:
        sprintf(buffer, "%+5.2lf", robo.x());
        LEMeusCianPosX->setText(buffer);
        sprintf(buffer, "%+5.2lf", robo.y());
        LEMeusCianPosY->setText(buffer);
        sprintf(buffer, "%+5.2lf", robo.theta());
        LEMeusCianPosTh->setText(buffer);
        break;
    }

}

void FutDataTable::updateAdvs(unsigned i, POS_ROBO robo){
    static char buffer[10];

    switch(i) {
        case 2:
            sprintf(buffer, "%+5.2lf", robo.x());
            LEAdvVerdPosX->setText(buffer);
            sprintf(buffer, "%+5.2lf", robo.y());
            LEAdvVerdPosY->setText(buffer);
            sprintf(buffer, "%+5.2lf", robo.theta());
            LEAdvVerdPosTh->setText(buffer);
            break;

        case 1:
            sprintf(buffer, "%+5.2lf", robo.x());
            LEAdvRosaPosX->setText(buffer);
            sprintf(buffer, "%+5.2lf", robo.y());
            LEAdvRosaPosY->setText(buffer);
            sprintf(buffer, "%+5.2lf", robo.theta());
            LEAdvRosaPosTh->setText(buffer);
            break;

        default:
            sprintf(buffer, "%+5.2lf", robo.x());
            LEAdvCianPosX->setText(buffer);
            sprintf(buffer, "%+5.2lf", robo.y());
            LEAdvCianPosY->setText(buffer);
            sprintf(buffer, "%+5.2lf", robo.theta());
            LEAdvCianPosTh->setText(buffer);
            break;
       }
}

void FutDataTable::updateRefs(unsigned i, POS_ROBO robo) {
    static char buffer[10];
    switch(i) {
    case 2:
        sprintf(buffer, "%+5.2lf", robo.x());
        LEMeusVerdRefX->setText(buffer);
        sprintf(buffer, "%+5.2lf", robo.y());
        LEMeusVerdRefY->setText(buffer);
        sprintf(buffer, "%+5.2lf", robo.theta());
        LEMeusVerdRefTh->setText(buffer);
        break;

    case 1:
        sprintf(buffer, "%+5.2lf", robo.x());
        LEMeusRosaRefX->setText(buffer);
        sprintf(buffer, "%+5.2lf", robo.y());
        LEMeusRosaRefY->setText(buffer);
        sprintf(buffer, "%+5.2lf", robo.theta());
        LEMeusRosaRefTh->setText(buffer);
        break;

    default:
        sprintf(buffer, "%+5.2lf", robo.x());
        LEMeusCianRefX->setText(buffer);
        sprintf(buffer, "%+5.2lf", robo.y());
        LEMeusCianRefY->setText(buffer);
        sprintf(buffer, "%+5.2lf", robo.theta());
        LEMeusCianRefTh->setText(buffer);
        break;
    }
}

void FutDataTable::updatePwms(unsigned i, PWM_WHEEL robo) {

    int var_pwmE = (int)(robo.left*100);
    int var_pwmD = (int)(robo.right*100);

    switch(i) {
    case 2:
           pwmE2->setValue(var_pwmE);
           pwmD2->setValue(var_pwmD);
        break;

    case 1:
           pwmE1->setValue(var_pwmE);
           pwmD1->setValue(var_pwmD);
        break;

    default:
           pwmE0->setValue(var_pwmE);
           pwmD0->setValue(var_pwmD);
        break;
    }
}

void FutDataTable::updateBola(POS_BOLA bola, VELOCITY vel_ball){
    static char buffer[10];
    sprintf(buffer, "%+5.2lf", bola.x());
    LEBolaPosX->setText(buffer);
    sprintf(buffer, "%+5.2lf", bola.y());
    LEBolaPosY->setText(buffer);
    sprintf(buffer, "%+5.2lf", vel_ball.ang);
    LEBolaPosTh->setText(buffer);
}

void FutDataTable::updateRols(unsigned i, ROLE_JOGADOR role) {
    QLineEdit * t = (i==0)? LEAcaoCian:((i==1)?LEAcaoRosa:LEAcaoVerd);
    switch(role.acao){
        case ESTACIONAR: t->setText("estac"); break;
        case IR_MEIO_CAMPO: t->setText("ir_m_c"); break;
        case FORMAR_BARREIRA: t->setText("barrer"); break;
        case CIRCULO_CENTRAL: t->setText("c_centra"); break;
        case ISOLAR_BOLA: t->setText("isolar"); break;
        case IR_BOLA: t-> setText("ir_bola"); break;
        case LADO_AREA: t->setText("lado_a"); break;
        case G_DEFENDER : t->setText("defend"); break;
        case G_CENTRO_GOL: t->setText("c_gol"); break;
        case A_IR_MARCA: t->setText("ir_mar"); break;
	  //        case A_BATER_PEN: t->setText("bat_pen"); break;
        case A_FREE_BALL: t->setText("fr_bal"); break;
        case A_BOLA_CENTRO: t->setText("bol_cen"); break;
        case A_DESCOLAR: t->setText("descol"); break;
        case A_POSICIONAR_PARA_DESCOLAR: t->setText("pos_desc"); break;
        case A_POSICIONAR_FRENTE_AREA: t->setText("p_f_ar"); break;
        case A_CONTORNAR: t->setText("contor"); break;
        case A_ALINHAR_GOL: t->setText("al_com"); break;
        case A_CHUTAR_GOL: t->setText("chut_g"); break;
        case D_NAO_ATRAPALHAR: t->setText("n_atr"); break;
        case IMPOSSIVEL: t->setText("impos"); break;
        case COMEMORAR:t->setText("comem"); break;
        case A_CONTORNAR_POR_DENTRO: t->setText("cont_den"); break;
        case A_ALINHAR_SEM_ORIENTACAO: t->setText("al_sem"); break;
        case POS_PENALTY1: t->setText("pos_pen1"); break;
        case POS_PENALTY2: t->setText("pos_pen2"); break;
    //default:
        case NAO_DEFINIDO: t->setText("naodef"); break;
    }
    switch(i){
    case 2:
        if (role.funcao == GOLEIRO){
            labelMeusVerdFunction->setText("<font color = #505050>GOLEIRO</font>");
        }
        if (role.funcao == COM_BOLA){
            labelMeusVerdFunction->setText("<font color = #808080>COM BOLA</font>");
        }
        if (role.funcao == SEM_BOLA){
            labelMeusVerdFunction->setText("<font color = #B0B0B0>SEM BOLA</font>");
        }
        break;
    case 1: if (role.funcao == GOLEIRO){
            labelMeusRosaFunction->setText("<font color = #505050>GOLEIRO</font>");
        }
        if (role.funcao == COM_BOLA){
            labelMeusRosaFunction->setText("<font color = #808080>COM BOLA</font>");
        }
        if (role.funcao == SEM_BOLA){
            labelMeusRosaFunction->setText("<font color = #B0B0B0>SEM BOLA</font>");
        }
        break;
    default: if (role.funcao == GOLEIRO){
            labelMeusCianFunction->setText("<font color = #505050>GOLEIRO</font>");
        }
        if (role.funcao == COM_BOLA){
            labelMeusCianFunction->setText("<font color = #808080>COM BOLA</font>");
        }
        if (role.funcao == SEM_BOLA){
            labelMeusCianFunction->setText("<font color = #B0B0B0>SEM BOLA</font>");
        }
        break;

    }

}

void FutDataTable::updateBlck(bool bloq0, bool bloq1, bool bloq2){
    if (bloq0) {
        labelBlock0->setText("<b><font color = red><center>YES</center></font></b>");
    } else {
        labelBlock0->setText("<b><font color = green><center>NO</center></font></b>");
    }
    if (bloq1) {
        labelBlock1->setText("<b><font color = red><center>YES</center></font></b>");
    } else {
        labelBlock1->setText("<b><font color = green><center>NO</center></font></b>");
    }
    if (bloq2) {
        labelBlock2->setText("<b><font color = red><center>YES</center></font></b>");
    } else {
        labelBlock2->setText("<b><font color = green><center>NO</center></font></b>");
    }

}









