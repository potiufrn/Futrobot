#include <QtWidgets>
#include "animacao.h"
#include <cmath>
#include <iostream>

using namespace std;

#define EXB_IMAGE_BORDER 10
#define EXB_IMAGE_WIDTH 520
#define EXB_IMAGE_HEIGHT 400


#define gray 168
#define dark_gray 118
#define light_gray 228

Animacao::Animacao(int n_my_team, QWidget *parent)
     : QWidget(parent),
    flag_fundo(1),
    flag_lin(true),
    flag_meu(true),
    flag_adv(true),
    flag_fun(false),
    flag_ref(false),
    flag_bal_fut(false),
    my_team(n_my_team),
    RDistortion(IMAGE_WIDTH, IMAGE_HEIGHT)
{
    PixPorMet = min((EXB_IMAGE_WIDTH-2*EXB_IMAGE_BORDER)/(FIELD_WIDTH+2*GOAL_WIDTH),
                    (EXB_IMAGE_HEIGHT-2*EXB_IMAGE_BORDER)/(FIELD_HEIGHT));

    centerImg.u = EXB_IMAGE_WIDTH/2;
    centerImg.v = EXB_IMAGE_HEIGHT/2;

    imag  = new QImage(EXB_IMAGE_WIDTH, EXB_IMAGE_HEIGHT, QImage::Format_RGB888);
    setFixedSize(EXB_IMAGE_WIDTH, EXB_IMAGE_HEIGHT);

   PARAMETROS_CALIBRACAO calibracaoParam;

   if(calibracaoParam.read("/futrobot/etc/calibracao.cal")){
     cerr << "Error Loading Calibration Parameters!\n";
     return;
   }

   //testa se o arquivo lido tem as dimensoes corretas de pontos
   if(calibracaoParam.nPontosNotaveis != NUM_POINTS){
       cerr << "Calibration file with wrong number of points\n";
     return;
   }


   unsigned i,
   MinU = IMAGE_WIDTH,
   MaxU = 0,
   MinV = IMAGE_HEIGHT,
   MaxV = 0;

   for(i = 0; i < calibracaoParam.nPontosNotaveis; i++){
     if(calibracaoParam.pontosImagem[i].u() < MinU)
       MinU = (unsigned int)floor(calibracaoParam.pontosImagem[i].u());
     if(calibracaoParam.pontosImagem[i].u() > MaxU)
       MaxU = (unsigned int)ceil(calibracaoParam.pontosImagem[i].u());
     if(calibracaoParam.pontosImagem[i].v() < MinV)
       MinV = (unsigned int)floor(calibracaoParam.pontosImagem[i].v());
     if(calibracaoParam.pontosImagem[i].v() > MaxV)
       MaxV = (unsigned int)ceil(calibracaoParam.pontosImagem[i].v());
   }

   //CALCULO DE DISTORCAO RADIAL E HOMOGRAFIA

   // Retas horizontais
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[2],
                                  calibracaoParam.pontosImagem[1],
                                  calibracaoParam.pontosImagem[0]) ) {
     cerr << "Reta superior foi desprezada\n";
   }
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[4],
                                  calibracaoParam.pontosImagem[22],
                                  calibracaoParam.pontosImagem[20]) ) {
     cerr << "Reta area-superior1 foi desprezada\n";
   }
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[4],
                                  calibracaoParam.pontosImagem[26],
                                  calibracaoParam.pontosImagem[20]) ) {
     cerr << "Reta area-superior2 foi desprezada\n";
   }
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[5],
                                  calibracaoParam.pontosImagem[24],
                                  calibracaoParam.pontosImagem[19]) ) {
     cerr << "Reta centro-superior foi desprezada\n";
   }
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[8],
                                  calibracaoParam.pontosImagem[25],
                                  calibracaoParam.pontosImagem[16]) ) {
     cerr << "Reta centro-inferior foi desprezada\n";
   }
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[9],
                                  calibracaoParam.pontosImagem[23],
                                  calibracaoParam.pontosImagem[15]) ) {
     cerr << "Reta area-inferior1 foi desprezada\n";
   }
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[9],
                                  calibracaoParam.pontosImagem[27],
                                  calibracaoParam.pontosImagem[15]) ) {
     cerr << "Reta area-inferior2 foi desprezada\n";
   }
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[11],
                                  calibracaoParam.pontosImagem[12],
                                  calibracaoParam.pontosImagem[13]) ) {
     cerr << "Reta inferior foi desprezada\n";
   }
   // Retas verticais
   //ATENCAO: ESTA RETA ESTAVA COMENTADA
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[3],
                                  calibracaoParam.pontosImagem[4],
                                  calibracaoParam.pontosImagem[10]) ) {
     cerr << "Reta esquerda1 foi desprezada\n";
   }
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[3],
                                  calibracaoParam.pontosImagem[5],
                                  calibracaoParam.pontosImagem[10]) ) {
     cerr << "Reta esquerda2 foi desprezada\n";
   }
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[3],
                                  calibracaoParam.pontosImagem[8],
                                  calibracaoParam.pontosImagem[10]) ) {
     cerr << "Reta esquerda3 foi desprezada\n";
   }
   //ATENCAO: ESTA RETA ESTAVA COMENTADA
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[3],
                                  calibracaoParam.pontosImagem[9],
                                  calibracaoParam.pontosImagem[10]) ) {
     cerr << "Reta esquerda4 foi desprezada\n";
   }
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[1],
                                  calibracaoParam.pontosImagem[24],
                                  calibracaoParam.pontosImagem[12]) ) {
     cerr << "Reta central1 foi desprezada\n";
   }
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[1],
                                  calibracaoParam.pontosImagem[25],
                                  calibracaoParam.pontosImagem[12]) ) {
     cerr << "Reta central2 foi desprezada\n";
   }
   //ATENCAO: ESTA RETA ESTAVA COMENTADA
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[21],
                                  calibracaoParam.pontosImagem[20],
                                  calibracaoParam.pontosImagem[14]) ) {
     cerr << "Reta direita1 foi desprezada\n";
   }
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[21],
                                  calibracaoParam.pontosImagem[19],
                                  calibracaoParam.pontosImagem[14]) ) {
     cerr << "Reta direita2 foi desprezada\n";
   }
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[21],
                                  calibracaoParam.pontosImagem[16],
                                  calibracaoParam.pontosImagem[14]) ) {
     cerr << "Reta direita3 foi desprezada\n";
   }
   //ATENCAO: ESTA RETA ESTAVA COMENTADA
   if ( RDistortion.incluirPontos(calibracaoParam.pontosImagem[21],
                                  calibracaoParam.pontosImagem[15],
                                  calibracaoParam.pontosImagem[14]) ) {
     cerr << "Reta direita4 foi desprezada\n";
   }

   RDistortion.calibrar();
   RDistortion.ajustar(false);

   Coord2 PImgCorrig[calibracaoParam.nPontosNotaveis];
   for (i=0; i<calibracaoParam.nPontosNotaveis; i++) {
     PImgCorrig[i] = RDistortion.corrigir(calibracaoParam.pontosImagem[i]);
     if(isnan(PImgCorrig[i].x()) ||
        isnan(PImgCorrig[i].y()) ){
       cerr<<"CORRECAO DE DISTORCAO RADIAL GERANDO NANs!!! "
           <<__FILE__<<" "<<__LINE__ <<endl;
     }
   }

   for (i=0; i<calibracaoParam.nPontosNotaveis; i++) {
     Homography.incluirPonto(calibracaoParam.pontosReais[i],PImgCorrig[i]);
   }

   Homography.calibrar();
}

void Animacao::desenhaLinhas() {

    QPainter painterLinhas(this);
    //QPainter *painter = new QPainter (this);

    painterLinhas.setRenderHint(QPainter::Antialiasing, true);
    painterLinhas.setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap));
    painterLinhas.setBrush(QBrush(Qt::transparent));

    //desenho do campo futrobot
    CoordI2 p1, p2;
    Coord2 c1, c2;

    //desenho da linhas começa pela linha superior, em seguida
    //adotamos a linha da esquerda, e assim por diante
    //Linha superior ---
    c1.x() = -(FIELD_WIDTH/2.0-CORNER_DIMENSION);
    c1.y() = FIELD_HEIGHT/2.0;
    c2.x() = (FIELD_WIDTH/2.0-CORNER_DIMENSION);
    c2.y() = FIELD_HEIGHT/2.0;

    p1 = mundo2img(c1);
    p2 = mundo2img(c2);

    painterLinhas.drawLine(p1.u, p1.v,
                      p2.u, p2.v);

    //linha de quina superior esquerda /<-
    c2.x() = -FIELD_WIDTH/2.0;
    c2.y() = FIELD_HEIGHT/2.0-CORNER_DIMENSION;
    p2 = mundo2img(c2);

    painterLinhas.drawLine(p1.u, p1.v,
                     p2.u, p2.v);

    //linha vertical esquerda |<-
    c1.x() = -FIELD_WIDTH/2.0;
    c1.y() = -(FIELD_HEIGHT/2.0 - CORNER_DIMENSION);
    p1 = mundo2img(c1);
    painterLinhas.drawLine(p1.u, p1.v,
                     p2.u, p2.v);

    //linha esquerda inclinada inferior \<-
    c2.x() = -(FIELD_WIDTH/2.0 - CORNER_DIMENSION);
    c2.y() = -FIELD_HEIGHT/2.0;
    p2 = mundo2img(c2);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    //linha inferior ___
    c1.x() = FIELD_WIDTH/2.0 - CORNER_DIMENSION;
    c1.y() = -(FIELD_HEIGHT/2.0);
    p1 = mundo2img(c1);
    painterLinhas.drawLine(p1.u, p1.v,
                     p2.u, p2.v);

    //linha direita inferior inclinada ./
    c2.x() = FIELD_WIDTH/2.0 ;
    c2.y() = -(FIELD_HEIGHT/2.0 - CORNER_DIMENSION);
    p2 = mundo2img(c2);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    //linha direita vertical .|
    c1.x() = FIELD_WIDTH/2.0;
    c1.y() = FIELD_HEIGHT/2.0 - CORNER_DIMENSION;
    p1 = mundo2img(c1);
    painterLinhas.drawLine(p1.u, p1.v,
                     p2.u, p2.v);

    //linha direita superior inclinada
    c2.x() = FIELD_WIDTH/2.0 - CORNER_DIMENSION;
    c2.y() = FIELD_HEIGHT/2.0;
    p2 = mundo2img(c2);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    //linha meio campo
    c1.x() = 0.0;
    c1.y() = FIELD_HEIGHT/2.0;
    c2.x() = 0.0;
    c2.y() = -FIELD_HEIGHT/2.0;
    p1 = mundo2img(c1);
    p2 = mundo2img(c2);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    // linha superior da area esquerda
    c1.x() = -(FIELD_WIDTH/2.0);
    c1.y() = GOAL_FIELD_HEIGHT/2.0;
    c2.x() = -(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH);
    c2.y() = GOAL_FIELD_HEIGHT/2.0;
    p1 = mundo2img(c1);
    p2 = mundo2img(c2);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    // linha frontal da area esquerda
    c1.x() = -(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH);
    c1.y() = -GOAL_FIELD_HEIGHT/2.0;
    p1 = mundo2img(c1);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    //linha inferior da area esquerda
    c2.x() = -FIELD_WIDTH/2.0;
    c2.y() = -GOAL_FIELD_HEIGHT/2.0;
    p2 = mundo2img(c2);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    // linha superior da area direita
    c1.x() = (FIELD_WIDTH/2.0);
    c1.y() = GOAL_FIELD_HEIGHT/2.0;
    c2.x() = (FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH);
    c2.y() = GOAL_FIELD_HEIGHT/2.0;
    p1 = mundo2img(c1);
    p2 = mundo2img(c2);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    // linha frontal da area direita
    c1.x() = (FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH);
    c1.y() = -GOAL_FIELD_HEIGHT/2.0;
    p1 = mundo2img(c1);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    //linha inferior da area direita
    c2.x() = FIELD_WIDTH/2.0;
    c2.y() = -GOAL_FIELD_HEIGHT/2.0;
    p2 = mundo2img(c2);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    //linha superior do gol esquerdo
    c1.x() = -(FIELD_WIDTH/2.0);
    c1.y() = GOAL_HEIGHT/2.0;
    c2.x() = -(FIELD_WIDTH/2.0 + GOAL_WIDTH);
    c2.y() = GOAL_HEIGHT/2.0;
    p1 = mundo2img(c1);
    p2 = mundo2img(c2);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    //linha atras do gol esquerdo
    c1.x() = -(FIELD_WIDTH/2.0 + GOAL_WIDTH);
    c1.y() = -GOAL_HEIGHT/2.0;
    p1 = mundo2img(c1);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    //linha inferior do gol esquerdo
    c2.x() = -FIELD_WIDTH/2.0;
    c2.y() = -GOAL_HEIGHT/2.0;
    p2 = mundo2img(c2);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    //linha superior do gol direito
    c1.x() = (FIELD_WIDTH/2.0);
    c1.y() = GOAL_HEIGHT/2.0;
    c2.x() = (FIELD_WIDTH/2.0 + GOAL_WIDTH);
    c2.y() = GOAL_HEIGHT/2.0;
    p1 = mundo2img(c1);
    p2 = mundo2img(c2);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    //linha atras do gol direito
    c1.x() = (FIELD_WIDTH/2.0 + GOAL_WIDTH);
    c1.y() = -GOAL_HEIGHT/2.0;
    p1 = mundo2img(c1);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    //linha inferior do gol direito
    c2.x() = FIELD_WIDTH/2.0;
    c2.y() = -GOAL_HEIGHT/2.0;
    p2 = mundo2img(c2);
    painterLinhas.drawLine(p1.u, p1.v,
                    p2.u, p2.v);

    //meia lua direita
    c1.x() = FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH+ARC_APOTHEM-ARC_RADIUS;
    c1.y() = ARC_RADIUS;
    p1 = mundo2img(c1);
    int angIniDraw = 16*rad2deg(atan2(ARC_HEIGHT/2.0, -ARC_APOTHEM));
    int angSpanDraw = 32*rad2deg(atan2(ARC_HEIGHT/2.0, ARC_APOTHEM));
    painterLinhas.drawArc (p1.u, p1.v, mundo2img(2*ARC_RADIUS),
                     mundo2img(2*ARC_RADIUS), angIniDraw, angSpanDraw);

    //meia lua esquerda
    c1.x() = -(FIELD_WIDTH/2.0-GOAL_FIELD_WIDTH+ARC_APOTHEM+ARC_RADIUS);
    c1.y() = ARC_RADIUS;
    p1 = mundo2img(c1);
    angIniDraw = -16*rad2deg(atan2(ARC_HEIGHT/2.0, ARC_APOTHEM));
    angSpanDraw = -2*angIniDraw;
    painterLinhas.drawArc (p1.u, p1.v, mundo2img(2*ARC_RADIUS),
                     mundo2img(2*ARC_RADIUS), angIniDraw, angSpanDraw);

    //linha circular meio campo
    c1.x() = -CIRCLE_RADIUS;
    c1.y() = CIRCLE_RADIUS;
    p1 = mundo2img(c1);
    painterLinhas.drawEllipse(p1.u, p1.v,
                         mundo2img(2*CIRCLE_RADIUS),mundo2img(2*CIRCLE_RADIUS));

    //painterAdvsRobos.~QPainter();

}

void Animacao::desenhaRef(int id){
    QPainter painterRef(this);
    Coord2 center; double theta;
    switch(id){
        case 2:
            center.x() = ref2.x();
            center.y() = ref2.y();
            theta = ref2.theta();
            break;
        case 1:
            center.x() = ref1.x();
            center.y() = ref1.y();
            theta = ref1.theta();
            break;
        default:
            center.x() = ref0.x();
            center.y() = ref0.y();
            theta = ref0.theta();
            break;
    }
    CoordI2 PixRef = mundo2img(center);
    double degreeRef = -rad2deg(theta);

    if (theta==POSITION_UNDEFINED){

        painterRef.save();
        painterRef.setPen(QPen(Qt::red,5, Qt::SolidLine, Qt::RoundCap));
        painterRef.drawPoint(PixRef.u,PixRef.v);
        painterRef.restore();
        return;
    }

    //QPainter *painter = new QPainter(this);

    painterRef.save();
    painterRef.setPen(QPen(Qt::red,2, Qt::SolidLine, Qt::RoundCap));
    painterRef.translate(PixRef.u,PixRef.v);
    painterRef.rotate(degreeRef);
    painterRef.drawLine(mundo2img(ROBOT_EDGE),0,0,0);
    painterRef.rotate(90.0);
    painterRef.drawLine(mundo2img(ROBOT_EDGE/2.0),0,-mundo2img(ROBOT_EDGE/2.0),0);
    painterRef.restore();

   // painter->~QPainter();
}



void Animacao::desenhaLinhaRef(int id){
    QPainter painterLinhaRef(this);
    Coord2 point1, point2;
    switch(id){
        case 2:
            point1.x()= ref2.x();
            point2.x()= meu2.x();
            point1.y()= ref2.y();
            point2.y()= meu2.y();
            break;
        case 1:
            point1.x()= ref1.x();
            point2.x()= meu1.x();
            point1.y()= ref1.y();
            point2.y()= meu1.y();
            break;
        default:
            point1.x()= ref0.x();
            point2.x()= meu0.x();
            point1.y()= ref0.y();
            point2.y()= meu0.y();
            break;
    }
    //conversoes metros -> pixels

    CoordI2 Pix1, Pix2;
    Pix1 = mundo2img(point1);
    Pix2 = mundo2img(point2);

    //QPainter *painter = new QPainter(this);

    painterLinhaRef.save();
    painterLinhaRef.setPen(QPen(Qt::yellow,2, Qt::DotLine, Qt::FlatCap));
    painterLinhaRef.drawLine(Pix1.u,Pix1.v,
                      Pix2.u,Pix2.v);
    painterLinhaRef.restore();

    //painter->~QPainter();
}

void Animacao::desenhaBola(){
    QPainter painterBola(this);

    CoordI2 posBola = mundo2img(ball);


    //ponto central da bola
    QPoint pointCircle(posBola.u, posBola.v);

    painterBola.save();
    painterBola.setPen(QPen(QColor(255,69,0),3));
    painterBola.drawEllipse(pointCircle,mundo2img(BALL_RADIUS),mundo2img(BALL_RADIUS));
    painterBola.restore();

    if (flag_bal_fut){
        Coord2 fball;
        fball.y() = ball.y()+vel_ball.mod*sin(vel_ball.ang);
        fball.x() = ball.x()+vel_ball.mod*cos(vel_ball.ang);

        CoordI2 imgfball = mundo2img(fball);

      //  velballimg = mundo2img(vel_ball.mod);
     //   angballimg = rad2Deg(vel_ball.ang);

        //CoordI2 posFutureBola = mundo2img(fut_ball);

        painterBola.save();
        painterBola.setPen(QPen(QColor(255,69,0),3));
        //painterBola.translate(posBola.u, posBola.v);
        painterBola.drawLine(posBola.u, posBola.v,
                             imgfball.u,imgfball.v);
       // painterBola.drawEllipse(pointCircle[1],mundo2img(BALL_RADIUS),mundo2img(BALL_RADIUS));
        painterBola.restore();
    }


}

void Animacao::desenhaMeusRobos(int id){
    QPainter painterMeusRobos(this);
    Coord2 center;
    double theta;

    switch(id){
        case 2:
            center.x() = meu2.x();
            center.y() = meu2.y();
            theta = meu2.theta();
            break;
        case 1:
            center.x() = meu1.x();
            center.y() = meu1.y();
            theta = meu1.theta();
            break;
        default:
            center.x() = meu0.x();
            center.y() = meu0.y();
            theta = meu0.theta();
            break;
        }

    CoordI2 Pix = mundo2img(center);
    double degree = -rad2deg(theta);


    //QPainter *painter = new QPainter(this);

     if (my_team == 1){
        painterMeusRobos.setPen(QPen(Qt::yellow,2, Qt::SolidLine, Qt::RoundCap));
    } else {
        painterMeusRobos.setPen(QPen(Qt::blue,2, Qt::SolidLine, Qt::RoundCap));
    }
    if(id == 0){

        painterMeusRobos.save();
        if (flag_fun) {
            if (fun0 == GOLEIRO){
                painterMeusRobos.setBrush(QColor(dark_gray, dark_gray, dark_gray));
            } else if (fun0 == COM_BOLA) {
                painterMeusRobos.setBrush(QColor(gray,gray,gray));
            } else if (fun0 == SEM_BOLA){
                painterMeusRobos.setBrush(QColor(light_gray,light_gray,light_gray));
            }
        } else {
            painterMeusRobos.setBrush(Qt::NoBrush);
        }
        painterMeusRobos.translate((double) Pix.u, (double) Pix.v);
        painterMeusRobos.rotate(degree);
        painterMeusRobos.drawEllipse(QPoint(0,0), mundo2img(ROBOT_EDGE/2.0), mundo2img(ROBOT_EDGE/2.0));
        painterMeusRobos.drawLine(mundo2img(ROBOT_EDGE),0,0,0);
        painterMeusRobos.rotate(90.0);
        painterMeusRobos.drawLine(mundo2img(ROBOT_EDGE/2.0),0,-mundo2img(ROBOT_EDGE/2.0),0);
        painterMeusRobos.restore();

    }

    if (id == 1){
        painterMeusRobos.save();
        if (flag_fun) {
            if (fun1 == GOLEIRO){
                painterMeusRobos.setBrush(QColor(dark_gray, dark_gray, dark_gray));
            } else if (fun1 == COM_BOLA) {
                painterMeusRobos.setBrush(QColor(gray,gray,gray));
            } else if (fun1 == SEM_BOLA){
                painterMeusRobos.setBrush(QColor(light_gray,light_gray,light_gray));
            }
        } else {
            painterMeusRobos.setBrush(Qt::NoBrush);
        }
        painterMeusRobos.translate((double) Pix.u, (double) Pix.v);
        painterMeusRobos.rotate(degree);
        painterMeusRobos.drawEllipse(QPoint(0,0), mundo2img(ROBOT_EDGE/2.0), mundo2img(ROBOT_EDGE/2.0));
        painterMeusRobos.drawLine(mundo2img(ROBOT_EDGE),0,0,0);
        painterMeusRobos.rotate(90.0);
        painterMeusRobos.drawLine(mundo2img(ROBOT_EDGE/2.0),0,-mundo2img(ROBOT_EDGE/2.0),0);
        painterMeusRobos.restore();

    }

    if (id == 2){
        painterMeusRobos.save();
        if (flag_fun) {
            if (fun2 == GOLEIRO){
                painterMeusRobos.setBrush(QColor(dark_gray, dark_gray, dark_gray));
            } else if (fun2 == COM_BOLA) {
                painterMeusRobos.setBrush(QColor(gray,gray,gray));
            } else if (fun2 == SEM_BOLA){
                painterMeusRobos.setBrush(QColor(light_gray,light_gray,light_gray));
            }
        } else {
            painterMeusRobos.setBrush(Qt::NoBrush);
        }
        painterMeusRobos.translate((double) (Pix.u), (double) (Pix.v));
        painterMeusRobos.rotate(degree);
        painterMeusRobos.drawEllipse(QPoint(0,0), mundo2img(ROBOT_EDGE/2.0), mundo2img(ROBOT_EDGE/2.0));
        painterMeusRobos.drawLine(mundo2img(ROBOT_EDGE),0,0,0);
        painterMeusRobos.rotate(90.0);
        painterMeusRobos.drawLine(mundo2img(ROBOT_EDGE/2.0),0,-mundo2img(ROBOT_EDGE/2.0),0);
        painterMeusRobos.restore();

    }
  //painter->~QPainter();
}

void Animacao::desenhaAdvsRobos(int id){
    QPainter painterAdvsRobos(this);
    Coord2 center;
 //   double theta;

    switch(id){
        case 2:
            center.x() = adv2.x();
            center.y() = adv2.y();
 //           theta = adv2.theta();
            break;
        case 1:
            center.x() = adv1.x();
            center.y() = adv1.y();
//            theta = adv1.theta();
            break;
        default:
            center.x() = adv0.x();
            center.y() = adv0.y();
//            theta = adv0.theta();

            break;
        }

    CoordI2 Pix = mundo2img(center);
//    double degree = -rad2deg(theta);

    //QPainter *painter = new QPainter(this);

    //ponto central do robo/circulo
    QPoint pointCircle(Pix.u, Pix.v);

    painterAdvsRobos.save();
    if (my_team == 1){
        painterAdvsRobos.setPen(QPen(Qt::blue,2, Qt::SolidLine, Qt::RoundCap));
    } else {
        painterAdvsRobos.setPen(QPen(Qt::yellow,2, Qt::SolidLine, Qt::RoundCap));
    }
    painterAdvsRobos.setBrush(Qt::NoBrush);
    //painterAdvsRobos.translate((double) Pix.u,(double) Pix.v);
    //painterAdvsRobos.rotate(degree);
    painterAdvsRobos.drawEllipse(pointCircle, mundo2img(ROBOT_EDGE/2.0), mundo2img(ROBOT_EDGE/2.0));
    painterAdvsRobos.restore();

    //painter->~QPainter();
}

void Animacao::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //QPainter * painter = new QPainter(this);
    switch(flag_fundo) {
    case 1:
        painter.drawImage(0,0, *imag);
        break;
    default:
        //fundo preto
        painter.save();
        painter.setBrush(Qt::black);
        painter.drawRect(0,0,EXB_IMAGE_WIDTH, EXB_IMAGE_HEIGHT);
        painter.restore();
        break;
    }


    if (flag_lin) {
       desenhaLinhas();
    }

    if (flag_meu) {
        desenhaMeusRobos(0);
        desenhaMeusRobos(1);
        desenhaMeusRobos(2);
    }

    if (flag_adv) {
        desenhaAdvsRobos(0);
        desenhaAdvsRobos(1);
        desenhaAdvsRobos(2);
    }


    //linha dos amarelos
    if (flag_ref && flag_meu) {
        desenhaLinhaRef(0);
        desenhaLinhaRef(1);
        desenhaLinhaRef(2);


        desenhaRef(0);
        desenhaRef(1);
        desenhaRef(2);
    }

    desenhaBola();

}

void Animacao::setrefPos(POS_ROBO pos, int id) {
    switch (id) {
    case 2: ref2 = pos; break;
    case 1: ref1 = pos; break;
    default: ref0 = pos; break;
    }
}


void Animacao::setMeusPos(POS_ROBO pos, int id) {
    switch (id) {
    case 2: meu2 = pos; break;
    case 1: meu1 = pos; break;
    default: meu0 = pos; break;
    }
}

void Animacao::setAdvsPos(POS_ROBO pos, int id) {
    switch (id) {
    case 2: adv2 = pos; break;
    case 1: adv1 = pos; break;
    default: adv0 = pos; break;
    }
}

void Animacao::setballPos(POS_BOLA pos, VELOCITY vel){
    ball = pos;
    vel_ball = vel;
}

void Animacao::setimg (const PxRGB * img){
    QRgb value;
//   imag->loadFromData((uchar *) pass,
    Coord2 ccampo, cimg_n_dist, cimg_dist;
    unsigned k;
    CoordI2 pix;
    for (int j=0; j < EXB_IMAGE_WIDTH; j++){
        for(int i=0; i < EXB_IMAGE_HEIGHT; i++){
            //CONVERTER DE  i, j na imagem de exibicao para
            // x, y no campo
            //ccampo.x()= (j-260)/real2Pix;
            //ccampo.y()= -(i-200)/real2Pix;
            pix.u= j; pix.v=i;
            ccampo = img2mundo(pix);
            cimg_n_dist = Homography*ccampo;
            cimg_dist = RDistortion.distorcer(cimg_n_dist);
            k = (unsigned) (round (cimg_dist.v())*IMAGE_WIDTH+round(cimg_dist.u()));
            value = qRgb(img[k].r, img[k].g, img[k].b);
            imag->setPixel(j, i, value);
        }
    }
}

void Animacao::setVarControl(int n_fundo, bool n_lin, bool n_mro, bool n_adv, bool n_rtr,
                             bool n_ref, bool n_bal_fut){
   flag_fundo = n_fundo;
   flag_lin = n_lin;
   flag_meu = n_mro;
   flag_adv = n_adv;
   flag_fun = n_rtr;
   flag_ref = n_ref;
   flag_bal_fut = n_bal_fut;

}

void Animacao::setFunction(FUNCTION n0, FUNCTION n1, FUNCTION n2){
    fun0 = n0;
    fun1 = n1;
    fun2 = n2;
}

Coord2 Animacao::img2mundo(CoordI2 P){
    Coord2 temp;
    temp.x() = (P.u-centerImg.u)/PixPorMet;
    temp.y() = -(P.v-centerImg.v)/PixPorMet;
    return temp;
}

CoordI2 Animacao::mundo2img(Coord2 P){
    CoordI2 temp;
    temp.u = (int) round (PixPorMet*P.x())+centerImg.u;
    temp.v = (int) round (-PixPorMet*P.y())+centerImg.v;
    return temp;
}
