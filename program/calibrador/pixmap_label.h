#ifndef PIXMAP_LABEL_H
#define PIXMAP_LABEL_H

#include <qlabel.h>
#include <qpixmap.h>
//Added by qt3to4:
#include <QPaintEvent>
#include <QMouseEvent>

class pixmap_label : public QLabel{
  Q_OBJECT;
  private:
    QPoint mousePos;
    QPixmap PxMap;
  public:
    pixmap_label(QWidget * parent = 0, const char * name = 0, Qt::WFlags f = 0);
    ~pixmap_label() {}
    void redesenhe();
    inline void loadFromData( const uchar * buf, uint len, const char * format){
	     PxMap.loadFromData(buf,len,format);
    }
  signals:
    void mouseMoved(QPoint);
    void mousePressed(QPoint);
    void mouseReleased(QPoint);
  protected:
    virtual void mouseMoveEvent(QMouseEvent *m);
    virtual void mousePressEvent(QMouseEvent *m);
    virtual void mouseReleaseEvent(QMouseEvent *m);
    virtual void paintEvent ( QPaintEvent *p );
};

#endif
