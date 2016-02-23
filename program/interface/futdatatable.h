#ifndef FUTDATATABLE_H
#define FUTDATATABLE_H

#include <QWidget>
#include <QLabel>
#include <QLCDNumber>
#include <QLineEdit>
#include <QProgressBar>
#include "../data.h"
//#include "../futrobot.h"

class FutDataTable : public QWidget
{
    Q_OBJECT

private:
  //Futrobot *fut;
    int var_pwmE, var_pwmD;


    QLabel * labelMeus;
    QLabel * labelAdv;
    QLabel * labelBola;

    QLabel * labelMeusCian;
    QLabel * labelMeusRosa;
    QLabel * labelMeusVerd;
    QLabel * labelAdvCian;
    QLabel * labelAdvRosa;
    QLabel * labelAdvVerd;

    QLabel * labelPosX;
    QLabel * labelPosY;
    QLabel * labelPosTh;

    QLabel * labelRefX;
    QLabel * labelRefY;
    QLabel * labelRefTh;

    QLabel * labelPwmDir;
    QLabel * labelPwmEsq;

    QLabel * labelFunction;

    QLabel * labelBlock;

    QLineEdit * LEMeusCianPosX;
    QLineEdit * LEMeusCianPosY;
    QLineEdit * LEMeusCianPosTh;
    QLineEdit * LEMeusRosaPosX;
    QLineEdit * LEMeusRosaPosY;
    QLineEdit * LEMeusRosaPosTh;
    QLineEdit * LEMeusVerdPosX;
    QLineEdit * LEMeusVerdPosY;
    QLineEdit * LEMeusVerdPosTh;

    QLineEdit * LEAdvCianPosX;
    QLineEdit * LEAdvCianPosY;
    QLineEdit * LEAdvCianPosTh;
    QLineEdit * LEAdvRosaPosX;
    QLineEdit * LEAdvRosaPosY;
    QLineEdit * LEAdvRosaPosTh;
    QLineEdit * LEAdvVerdPosX;
    QLineEdit * LEAdvVerdPosY;
    QLineEdit * LEAdvVerdPosTh;

    QLineEdit * LEBolaPosX;
    QLineEdit * LEBolaPosY;
    QLineEdit * LEBolaPosTh;


    QLineEdit * LEMeusCianRefX;
    QLineEdit * LEMeusCianRefY;
    QLineEdit * LEMeusCianRefTh;
    QLineEdit * LEMeusRosaRefX;
    QLineEdit * LEMeusRosaRefY;
    QLineEdit * LEMeusRosaRefTh;
    QLineEdit * LEMeusVerdRefX;
    QLineEdit * LEMeusVerdRefY;
    QLineEdit * LEMeusVerdRefTh;

    QLineEdit * LEMeusCianPwmDir;
    QLineEdit * LEMeusCianPwmEsq;
    QLineEdit * LEMeusRosaPwmDir;
    QLineEdit * LEMeusRosaPwmEsq;
    QLineEdit * LEMeusVerdPwmDir;
    QLineEdit * LEMeusVerdPwmEsq;

    QLabel * labelMeusCianFunction;
    QLabel * labelMeusRosaFunction;
    QLabel * labelMeusVerdFunction;

    QLabel * labelAcoes;
    QLineEdit * LEAcaoCian;
    QLineEdit * LEAcaoRosa;
    QLineEdit * LEAcaoVerd;

    QProgressBar *pwmE0;
    QProgressBar *pwmD0;

    QProgressBar *pwmE1;
    QProgressBar *pwmD1;
    QProgressBar *pwmE2;
    QProgressBar *pwmD2;

    QLabel * labelBlock0;
    QLabel * labelBlock1;
    QLabel * labelBlock2;

protected:
   // void timerEvent(QTimerEvent * e);

public:
    FutDataTable(int new_team, QWidget * parent = 0);
void refreshPos(const CONFIG &npos);
void updateMeus(unsigned i, POS_ROBO robo);
void updateAdvs(unsigned i, POS_ROBO robo);
void updateRefs(unsigned i, POS_ROBO robo);
void updatePwms(unsigned i, PWM_WHEEL robo);
void updateBola(POS_BOLA bola, VELOCITY vel_ball);
void updateRols(unsigned i, ROLE_JOGADOR role);
void updateBlck(bool bloq0, bool bloq1, bool bloq2);
//void updateBola(P)
};

#endif // FUTDATATABLE_H
