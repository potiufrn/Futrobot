/****************************************************************************
** Form interface generated from reading ui file 'calibrador.ui'
**
** Created by User Interface Compiler
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CALIBRADOR_H
#define CALIBRADOR_H
#include <qmessagebox.h>
#include <qvariant.h>
#include <qpixmap.h>
#include <qmainwindow.h>
#include <qfiledialog.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <iostream>
#include <qapplication.h>
#include "pixmap_label.h"
#include "calibratorprocessor.h"
#include "variaveisglobais.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class pixmap_label;
class QSlider;
class QCheckBox;
class QWidgetStack;
class QWidget;
class QLabel;
class QLCDNumber;
class QSpinBox;
class QPushButton;
class QComboBox;


class calibrador : public QMainWindow
{
    Q_OBJECT

public:

  calibrador( const char* name = 0, QWidget* parent = 0,  Qt::WindowFlags fl = Qt::WindowFlags());
    ~calibrador();

    QSlider* sliderVOffset;
    pixmap_label* pixmap_label1;
    QSlider* sliderUOffset;
    QCheckBox* checkShooting;
    QWidgetStack* widgetStack1;
    QWidget* tela1;
    QLabel* labelRGB;
    QLCDNumber* lcdRGB_R;
    QLCDNumber* lcdRGB_G;
    QLCDNumber* lcdRGB_B;
    QLabel* labelHGP;
    QLCDNumber* lcdHGP_H;
    QLCDNumber* lcdHGP_P;
    QLCDNumber* lcdHGP_G;
    QLabel* labelX;
    QLCDNumber* lcdX;
    QLabel* labelY;
    QLCDNumber* lcdY;
    QCheckBox* checkExibirGrade;
    QCheckBox* checkExibirImagemProcessada;
    QLabel* labelLimiarPInf;
    QSpinBox* spinLimiarPInf;
    QSlider* sliderLimiarPInf;
    QLabel* labelLimiarPSup;
    QSpinBox* spinLimiarPSup;
    QSlider* sliderLimiarPSup;
    QPushButton* pushVoltarTela1;
    QPushButton* pushAvancarTela1;
    QWidget* tela2;
    QLabel* textLabel1;
    QComboBox* comboExibicao1;
    QLabel* labelCor;
    QComboBox* comboCores;
    QLabel* labelHMin;
    QSpinBox* spinHGP_Hmin;
    QSlider* sliderHGP_Hmin;
    QLabel* labelHMax;
    QSpinBox* spinHGP_Hmax;
    QSlider* sliderHGP_Hmax;
    QLabel* labelPMin;
    QSpinBox* spinHGP_Pmin;
    QSlider* sliderHGP_Pmin;
    QLabel* labelPMax;
    QSpinBox* spinHGP_Pmax;
    QSlider* sliderHGP_Pmax;
    QLabel* labelGMin;
    QSpinBox* spinHGP_Gmin;
    QSlider* sliderHGP_Gmin;
    QLabel* labelGMax;
    QSpinBox* spinHGP_Gmax;
    QSlider* sliderHGP_Gmax;
    QLabel* labelX2;
    QLCDNumber* lcdX2;
    QLabel* labelRGB2;
    QLCDNumber* lcdRGB_R2;
    QLCDNumber* lcdRGB_G2;
    QLCDNumber* lcdRGB_B2;
    QLabel* labelY2;
    QLCDNumber* lcdY2;
    QLabel* labelHGP2;
    QLCDNumber* lcdHGP_H2;
    QLCDNumber* lcdHGP_P2;
    QLCDNumber* lcdHGP_G2;
    QPushButton* pushVoltarTela2;
    QPushButton* pushFim;
    QWidget* tela0;
    QSpinBox* spinHue;
    QLabel* textLabel1_2_2_2_2_2;
    QSlider* sliderGamma;
    QSlider* sliderBrightness;
    QSpinBox* spinGamma;
    QSpinBox* spinContrast;
    QLabel* textLabel1_2_2_2_2_2_2;
    QLabel* textLabel1_2_2_2_2;
    QSlider* sliderContrast;
    QLabel* textLabel1_2;
    QLabel* textLabel1_2_2_2_2_2_4;
    QSpinBox* spinGain;
    QSlider* sliderGain;
    QSlider* sliderSaturation;
    QSlider* sliderHue;

    QLabel*  labelExposure;
    QSlider* sliderExposure;
    QSpinBox* spinExposure;

    QSpinBox* spinShutter;
    QSpinBox* spinSaturation;
    QSpinBox* spinBrightness;
    QLabel* textLabel1_2_2;
    QLabel* textLabel1_2_2_2_2_2_3;
    QLabel* textLabel3;
    QSlider* sliderShutter;
    QPushButton* pushAvancarTela0;
    QMenuBar *MenuBar;
    QPopupMenu *File;
    QPopupMenu *Image;
    QPopupMenu *Camera;
    QAction* fileNewAction;
    QAction* fileOpenAction;
    QAction* fileSaveAction;
    QAction* fileSaveAsAction;
    QAction* fileExitAction;
    QAction* imageLoadAction;
    QAction* imageSnapshotAction;
    QAction* imageSaveAction;
    QActionGroup* screenActionGroup;
    QAction* screenSelectOneAction;
    QAction* screenSelectTwoAction;
    QAction* cameraLoad_ParametersAction;
    QAction* cameraSave_ParametersAction;
    QAction* cameraSave_Parameters_asAction;
    QAction* cameraNewAction;

    virtual bool carregaInterface();

public slots:
    virtual void fileNew();
    virtual void fileOpen();
    virtual void fileSave();
    virtual void fileSaveAs();
    virtual void fileExit();
    virtual void cameraLoadParam();
    virtual void cameraNew();
    virtual void cameraSaveParam();
    virtual void cameraSaveAsParam();
    virtual void mouseMove( QPoint Point );
    virtual void mousePress( QPoint Point );
    virtual void mouseRelease( QPoint Point );
    virtual void imageOpen();
    virtual void imageSave();
    virtual void sliderLimiarPInfValueChanged( int valor );
    virtual void spinLimiarPInfValueChanged( int valor );
    virtual void sliderLimiarPSupValueChanged( int valor );
    virtual void spinLimiarPSupValueChanged( int valor );
    virtual void BrightnessValueChanged( int valor );
    virtual void ExposureValueChanged( int valor );
    // virtual void ExposureAbsValueChanged( int valor );

    virtual void HueValueChanged( int valor );
    virtual void SaturationValueChanged( int valor );
    virtual void GammaValueChanged( int valor );
    virtual void ShutterValueChanged( int valor );
    virtual void GainValueChanged( int valor );
    virtual void HMinValueChanged( int valor );
    virtual void HMaxValueChanged( int valor );
    virtual void GMinValueChanged( int valor );
    virtual void GMaxValueChanged( int valor );
    virtual void PMinValueChanged( int valor );
    virtual void PMaxValueChanged( int valor );
    virtual void atualizarLimitesHGP( int item );
    virtual void atualizarCameraParam();
    virtual void mostrarTela0();
    virtual void mostrarTela1();
    virtual void mostrarTela2();
    virtual void novosParametrosCamera();
    virtual void processarImagem();
    virtual void setarModo();
    virtual void ShootingChanged( bool value );
    virtual void redesenharImagem();
    virtual void atualizarParametrosCamera();
    virtual void atualizarCamera();
    virtual void changeUOffset( int new_offset );
    virtual void changeVOffset( int new_offset );

protected:
    QHBoxLayout* layout55;
    QHBoxLayout* layout54;
    QVBoxLayout* layout53;
    QSpacerItem* spacer25;
    QVBoxLayout* layout48;
    QHBoxLayout* layout47;
    QVBoxLayout* layout45;
    QSpacerItem* spacer9;
    QSpacerItem* spacer9_2;
    QSpacerItem* spacer11;
    QVBoxLayout* layout9;
    QHBoxLayout* layout1;
    QHBoxLayout* layout2;
    QHBoxLayout* layout7;
    QSpacerItem* spacer3;
    QHBoxLayout* layout8;
    QSpacerItem* spacer4;
    QHBoxLayout* layout31;
    QSpacerItem* spacer8;
    QVBoxLayout* layout10;
    QVBoxLayout* layout27;
    QHBoxLayout* layout26;
    QSpacerItem* spacer5_2;
    QHBoxLayout* layout14;
    QVBoxLayout* layout29;
    QHBoxLayout* layout28;
    QSpacerItem* spacer5;
    QHBoxLayout* layout15;
    QHBoxLayout* layout43;
    QVBoxLayout* layout44;
    QSpacerItem* spacer45;
    QSpacerItem* spacer48;
    QHBoxLayout* layout43_2;
    QSpacerItem* spacer41;
    QHBoxLayout* layout55_2;
    QSpacerItem* spacer32;
    QHBoxLayout* layout67;
    QSpacerItem* spacer42;
    QHBoxLayout* layout56;
    QSpacerItem* spacer33;
    QVBoxLayout* layout123;
    QHBoxLayout* layout58;
    QSpacerItem* spacer34;
    QHBoxLayout* layout106;
    QHBoxLayout* layout60;
    QSpacerItem* spacer36;
    QHBoxLayout* layout105;
    QHBoxLayout* layout122;
    QSpacerItem* spacer39;
    QHBoxLayout* layout121;
    QHBoxLayout* layout64;
    QSpacerItem* spacer40;
    QHBoxLayout* layout98;
    QHBoxLayout* layout61;
    QSpacerItem* spacer37;
    QHBoxLayout* layout104;
    QHBoxLayout* layout62;
    QSpacerItem* spacer38;
    QHBoxLayout* layout103;
    QHBoxLayout* layout73;
    QSpacerItem* spacer43;
    QHBoxLayout* layout69;
    QHBoxLayout* layout71;
    QHBoxLayout* layout42;
    QSpacerItem* spacer44;
    QHBoxLayout* layout70;
    QHBoxLayout* layout41;
    QHBoxLayout* layout76;

protected slots:
    virtual void languageChange();

private:
    bool novosParametros, novosLimites;
    int ponto_selecionado;
    QString arquivo, arquivo_cameraParam;
    int MouseX, MouseY;
    PxRGB pixel;
    int telaAtual;
    QTimer *Timer;
    bool ponto_dragged;
    int R, G1, B, H, P, G2;

    QPixmap image0;

    void init();
    void destroy();
    virtual void atualizarDisplays();
    virtual void atualizarLimitesP();

};

#endif // CALIBRADOR_H
