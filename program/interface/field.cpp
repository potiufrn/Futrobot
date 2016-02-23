#include "field.h"
#include <QHBoxLayout>

Field::Field(const QString &text, uint ndig, QWidget *parent):
        QWidget(parent),
  num (ndig, parent),
  nam (text, parent){
    num.setSegmentStyle(QLCDNumber::Flat);

    QHBoxLayout * lay = new QHBoxLayout;
    lay->addWidget(&nam);
    lay->addWidget(&num);
//lay->set
    setLayout(lay);
  }

Field::~Field(){
}
