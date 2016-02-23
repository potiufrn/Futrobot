#ifndef _FIELD_H_
#define _FIELD_H_
#include <QWidget>
#include <QLCDNumber>
#include <QLabel>
#include <QString>

//QT_BEGIN_NAMESPACE
//class QPushButton;
//class QLabel;
//class QString;
//QT_END_NAMESPACE



class Field : public QWidget {
  Q_OBJECT
  private:
  QLCDNumber num;
  QLabel nam;
public:
  Field (const QString &text, uint ndig, QWidget * parent=0);
  ~Field();

public slots:
  void display (double f) {
        num.display(f);
  }

};












#endif //_FIELD_H_
