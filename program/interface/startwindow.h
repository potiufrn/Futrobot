#ifndef STARTWINDOW_H
#define STARTWINDOW_H
#include <QWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QButtonGroup>

class StartWindow : public QWidget {
    Q_OBJECT

   int * myteam;
 //  int * myside;

   QGroupBox * team;
   QButtonGroup * grpteam;
   QRadioButton * ylw;
   QRadioButton * ble;

 /*  QGroupBox * side;
   QButtonGroup * grpside;
   QRadioButton * lft;
   QRadioButton * rht;
*/
   QPushButton * ok;
   QPushButton * close;


public slots:
   void optionsselected();
   void optionschoosen();

public:
   StartWindow(int * newteam, /*int * newside,*/ QWidget * parent=0);
   ~StartWindow();

};

#endif // STARTWINDOW_H
