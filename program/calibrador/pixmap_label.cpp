#include "pixmap_label.h"
//Added by qt3to4:
#include <QPaintEvent>
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>

#include <iostream>
using namespace std;

pixmap_label::pixmap_label( QWidget * parent, const char * name, Qt::WFlags f )
    : QLabel( parent, name, f ),	
    PxMap(width(),height())
{
    setScaledContents(true);
    setAlignment(Qt::AlignCenter);
}

void pixmap_label::mouseMoveEvent (QMouseEvent *m){
//    if(pixmap()==NULL){
    if(PxMap.isNull()){	
      mousePos.setX(-1);
      mousePos.setY(-1);
    }
    else{
      if((m->x()>=0)&&(m->x()<=640))
        mousePos.setX(m->x());
      if((m->y()>=0)&&(m->y()<=480))
        mousePos.setY(m->y());
    }
    emit mouseMoved(mousePos);
}

void pixmap_label::mousePressEvent (QMouseEvent *m){
//    if(pixmap()==NULL){
    if(PxMap.isNull()){
      mousePos.setX(-1);
      mousePos.setY(-1);
    }
    else{
      if((m->x()>=0)&&(m->x()<=640))
        mousePos.setX(m->x());
      if((m->y()>=0)&&(m->y()<=480))
        mousePos.setY(m->y());
    }
    emit mousePressed(mousePos);
}

void pixmap_label::mouseReleaseEvent (QMouseEvent *m){
//    if(pixmap()==NULL){
    if(PxMap.isNull()){
      mousePos.setX(-1);
      mousePos.setY(-1);
    }
    else{
      if((m->x()>=0)&&(m->x()<=640))
        mousePos.setX(m->x());
      if((m->y()>=0)&&(m->y()<=480))
        mousePos.setY(m->y());
    }
    emit mouseReleased(mousePos);
}

void pixmap_label::redesenhe ()
{
  // bitBlt(this,0,0,
  // 	 &PxMap,0,0,
  // 	 PxMap.width(),PxMap.height(),
  // 	 Qt::CopyROP,TRUE);
  
  //  this->repaint();
  this->update();
  

  // QPainter painter(this);
  // painter.drawPixmap(0,0,PxMap.width(),PxMap.height(),PxMap);
}

void pixmap_label::paintEvent ( QPaintEvent *p )
{
  //  redesenhe();
  QPainter painter(this);
  painter.drawPixmap(0,0,PxMap.width(),PxMap.height(),PxMap);
}
