#ifndef CALIBRADOR_H
#define CALIBRADOR_H

#include <QMainWindow>
#include <QFileDialog>
#include <iostream>
#include "calibratorprocessor.h"
#include "variaveisglobais.h"
#include "pixmap_label.h"

#define NUM_AMOSTRAS 100 //amostrar para calcular a média do campo vazio

QT_BEGIN_NAMESPACE
namespace Ui
{
    class calibrador;
}
QT_END_NAMESPACE

class calibrador : public QMainWindow
{
    Q_OBJECT

public:
    calibrador(QWidget *parent = nullptr, /*const char* name = 0,*/ Qt::WindowFlags fl = Qt::WindowFlags());
    ~calibrador();

    pixmap_label *pixmap_label1;
    bool carregaInterface();

private slots:

    void setCamera();

    void showPage0();
    void showPage1();
    void showPage2();

    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void fileExit();

    void quitProgram();

    void setMode();

    void cameraLoadParam();
    void cameraNew();
    void cameraSaveParam();
    void cameraSaveAsParam();

    void mouseMove(QPoint Point);
    void mousePress(QPoint Point);
    void mouseRelease(QPoint Point);
    void imageOpen();
    void imageSave();

    void sliderLimiarPInfValueChanged(int valor);
    void spinLimiarPInfValueChanged(int valor);
    void sliderLimiarPSupValueChanged(int valor);
    void spinLimiarPSupValueChanged(int valor);

    void BrightnessValueChanged(int valor);
    void ExposureValueChanged(int valor);
    void ContrastValueChanged(int valor);
    void HueValueChanged(int valor);
    void SaturationValueChanged(int valor);
    void GammaValueChanged(int valor);
    void ExposureAbsValueChanged(int valor);

    void GainValueChanged(int valor);
    void HMinValueChanged(int valor);
    void HMaxValueChanged(int valor);
    void GMinValueChanged(int valor);
    void GMaxValueChanged(int valor);
    void PMinValueChanged(int valor);
    void PMaxValueChanged(int valor);

    void atualizarLimitesHGP(int item);
    void atualizarCameraParam();

    void novosParametrosCamera();
    void processarImagem();

    void shootingChanged(bool value);
    void redesenharImagem();
    void atualizarParametrosCamera();
    void atualizarCamera();

    //void changeUOffset( int new_offset );
    //virtual void changeVOffset( int new_offset );

private:
    Ui::calibrador *ui;
    //CalibratorProcessor X; // Descomentar isso depois de retirar do main.cpp

    bool novosParametros, novosLimites;
    int ponto_selecionado;
    QString arquivo, arquivo_cameraParam;
    int MouseX, MouseY;
    PxRGB pixel;
    //int telaAtual;
    QTimer *Timer;
    bool ponto_dragged;
    int R, G1, B, H, P, G2;

    QPixmap image0;

    void init();
    void destroy();
    void atualizarDisplays();
    void atualizarLimitesP();
};
#endif // CALIBRADOR_H
