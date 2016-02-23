#ifndef ANIMACAO_H
#define ANIMACAO_H
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include "../data.h"
#include "../parameters.h"
#include "../dados_calibracao.h"
#include <imagem.h>

class CoordI2 {
    public:
        int u, v;
};

class Animacao : public QWidget
{
    Q_OBJECT

public:
    Animacao(int n_my_team, QWidget *parent = 0);


protected:
    void paintEvent(QPaintEvent *event);

private:
    double PixPorMet;
    CoordI2 centerImg;

    //variaveis de controle
    int flag_fundo;
    bool flag_lin;
    bool flag_meu;
    bool flag_adv;
    bool flag_fun;
    bool flag_ref;
    bool flag_bal_fut;


    int my_team;

    POS_ROBO meu0;
    POS_ROBO meu1;
    POS_ROBO meu2;
    POS_ROBO adv0;
    POS_ROBO adv1;
    POS_ROBO adv2;
    POS_ROBO ref0;
    POS_ROBO ref1;
    POS_ROBO ref2;
    FUNCTION fun0;
    FUNCTION fun1;
    FUNCTION fun2;

    POS_BOLA ball;
    VELOCITY vel_ball;

    QImage * imag;

    QPainter painter;
    QPainter painterLinhas;
    QPainter painterRef;
    QPainter painterLinhasRef;
    QPainter painterMeusRobos;
    QPainter painterAdvsRobos;
    QPainter painterBola;

    Homografia Homography;
    DistRadial RDistortion;

    void desenhaLinhas();
    void desenhaRef(int id);
    void desenhaLinhaRef(int id);
    void desenhaMeusRobos(int id);
    void desenhaAdvsRobos(int id);
    void desenhaBola();

    Coord2 img2mundo(CoordI2 P);
    CoordI2 mundo2img(Coord2 P);

    inline int mundo2img (double x) {return (int) round (x*PixPorMet);}
    inline int rad2deg (double ang) {return (int) round ((180.0/M_PI)*ang);}


public:
    void setVarControl(int n_fundo, bool n_lin, bool n_mro, bool n_adv, bool n_rtr, bool n_ref,
                       bool n_bal_fut);
    void setMeusPos (POS_ROBO pos,  int id);
    void setAdvsPos (POS_ROBO pos , int id);
    void setrefPos(POS_ROBO pos, int id);
    void setballPos (POS_BOLA pos, VELOCITY vel);
    void setimg (const PxRGB * img);
    void setFunction(FUNCTION, FUNCTION, FUNCTION);
};
#endif // ANIMACAO_H
