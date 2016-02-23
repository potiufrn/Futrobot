#ifndef HISTOGRAMA_H
#define HISTOGRAMA_H

class Histograma : public QWidget
{
  Q_OBJECT

 public:
  Histograma(QWidget *parent = 0, const char *name = 0);
  QSize sizeHint() const;
 signals:
  void limiarMinChanged(int); //
  void limiarMaxChanged(int);
  
 protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void paintEvent(QPaintEvent *event);

 private:
  

  QImage imagem; //Armazena a imagem a ser feita o histograma
  int *PixelValues; //armazena valores do histograma para exibição
  double Pmin, Pmax; //armazenam limiares de P
  int modo;  //Indica o que exibir no histograma (H ou G)
  
};

#endif
