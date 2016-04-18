#ifndef CALIBRADOR_H
#define CALIBRADOR_H

#include <qvariant.h>


#include <Qt3Support/Q3MainWindow>
#include <Qt3Support/Q3WidgetStack>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <iostream>
#include <Qt3Support/Q3FileDialog>
#include <QtGui/QApplication>
#include <QtGui/QPixmap>
#include <QtCore/QTimer>
#include "calibratorprocessor.h"
#include "pixmap_label.h"
#include "variaveisglobais.h"

QT_BEGIN_NAMESPACE

class Ui_calibrador
{
public:
    QAction *fileNewAction;
    QAction *fileOpenAction;
    QAction *fileSaveAction;
    QAction *fileSaveAsAction;
    QAction *fileExitAction;
    QAction *imageLoadAction;
    QAction *imageSnapshotAction;
    QAction *imageSaveAction;
    QAction *cameraLoad_ParametersAction;
    QAction *cameraSave_ParametersAction;
    QAction *cameraSave_Parameters_asAction;
    QAction *cameraNewAction;
    QActionGroup *screenActionGroup;
    QAction *screenSelectOneAction;
    QAction *screenSelectTwoAction;
    QWidget *widget;
    QWidget *layout55;
    QHBoxLayout *hboxLayout;
    QHBoxLayout *hboxLayout1;
    QVBoxLayout *vboxLayout;
    QSlider *sliderVOffset;
    QSpacerItem *spacer25;
    QVBoxLayout *vboxLayout1;
    pixmap_label *pixmap_label1;
    QHBoxLayout *hboxLayout2;
    QSlider *sliderUOffset;
    QCheckBox *checkShooting;
    Q3WidgetStack *widgetStack1;
    QWidget *tela1;
    QWidget *layout45;
    QVBoxLayout *vboxLayout2;
    QVBoxLayout *vboxLayout3;
    QHBoxLayout *hboxLayout3;
    QLabel *labelRGB;
    QLCDNumber *lcdRGB_R;
    QLCDNumber *lcdRGB_G;
    QLCDNumber *lcdRGB_B;
    QHBoxLayout *hboxLayout4;
    QLabel *labelHGP;
    QLCDNumber *lcdHGP_H;
    QLCDNumber *lcdHGP_P;
    QLCDNumber *lcdHGP_G;
    QHBoxLayout *hboxLayout5;
    QLabel *labelX;
    QLCDNumber *lcdX;
    QSpacerItem *spacer3;
    QHBoxLayout *hboxLayout6;
    QLabel *labelY;
    QLCDNumber *lcdY;
    QSpacerItem *spacer4;
    QSpacerItem *spacer9;
    QHBoxLayout *hboxLayout7;
    QVBoxLayout *vboxLayout4;
    QCheckBox *checkExibirGrade;
    QCheckBox *checkExibirImagemProcessada;
    QSpacerItem *spacer8;
    QSpacerItem *spacer9_2;
    QVBoxLayout *vboxLayout5;
    QHBoxLayout *hboxLayout8;
    QLabel *labelLimiarPInf;
    QSpacerItem *spacer5_2;
    QHBoxLayout *hboxLayout9;
    QSpinBox *spinLimiarPInf;
    QSlider *sliderLimiarPInf;
    QVBoxLayout *vboxLayout6;
    QHBoxLayout *hboxLayout10;
    QLabel *labelLimiarPSup;
    QSpacerItem *spacer5;
    QHBoxLayout *hboxLayout11;
    QSpinBox *spinLimiarPSup;
    QSlider *sliderLimiarPSup;
    QSpacerItem *spacer11;
    QHBoxLayout *hboxLayout12;
    QPushButton *pushVoltarTela1;
    QPushButton *pushAvancarTela1;
    QWidget *tela2;
    QWidget *layout44;
    QVBoxLayout *vboxLayout7;
    QHBoxLayout *hboxLayout13;
    QSpacerItem *spacer41;
    QHBoxLayout *hboxLayout14;
    QLabel *textLabel1;
    QSpacerItem *spacer32;
    QComboBox *comboExibicao1;
    QHBoxLayout *hboxLayout15;
    QSpacerItem *spacer42;
    QHBoxLayout *hboxLayout16;
    QLabel *labelCor;
    QSpacerItem *spacer33;
    QComboBox *comboCores;
    QVBoxLayout *vboxLayout8;
    QHBoxLayout *hboxLayout17;
    QLabel *labelHMin;
    QSpacerItem *spacer34;
    QHBoxLayout *hboxLayout18;
    QSpinBox *spinHGP_Hmin;
    QSlider *sliderHGP_Hmin;
    QHBoxLayout *hboxLayout19;
    QLabel *labelHMax;
    QSpacerItem *spacer36;
    QHBoxLayout *hboxLayout20;
    QSpinBox *spinHGP_Hmax;
    QSlider *sliderHGP_Hmax;
    QHBoxLayout *hboxLayout21;
    QLabel *labelPMin;
    QSpacerItem *spacer39;
    QHBoxLayout *hboxLayout22;
    QSpinBox *spinHGP_Pmin;
    QSlider *sliderHGP_Pmin;
    QHBoxLayout *hboxLayout23;
    QLabel *labelPMax;
    QSpacerItem *spacer40;
    QHBoxLayout *hboxLayout24;
    QSpinBox *spinHGP_Pmax;
    QSlider *sliderHGP_Pmax;
    QHBoxLayout *hboxLayout25;
    QLabel *labelGMin;
    QSpacerItem *spacer37;
    QHBoxLayout *hboxLayout26;
    QSpinBox *spinHGP_Gmin;
    QSlider *sliderHGP_Gmin;
    QHBoxLayout *hboxLayout27;
    QLabel *labelGMax;
    QSpacerItem *spacer38;
    QHBoxLayout *hboxLayout28;
    QSpinBox *spinHGP_Gmax;
    QSlider *sliderHGP_Gmax;
    QSpacerItem *spacer45;
    QHBoxLayout *hboxLayout29;
    QHBoxLayout *hboxLayout30;
    QLabel *labelX2;
    QLCDNumber *lcdX2;
    QSpacerItem *spacer43;
    QHBoxLayout *hboxLayout31;
    QLabel *labelRGB2;
    QLCDNumber *lcdRGB_R2;
    QLCDNumber *lcdRGB_G2;
    QLCDNumber *lcdRGB_B2;
    QHBoxLayout *hboxLayout32;
    QHBoxLayout *hboxLayout33;
    QLabel *labelY2;
    QLCDNumber *lcdY2;
    QSpacerItem *spacer44;
    QHBoxLayout *hboxLayout34;
    QLabel *labelHGP2;
    QLCDNumber *lcdHGP_H2;
    QLCDNumber *lcdHGP_P2;
    QLCDNumber *lcdHGP_G2;
    QSpacerItem *spacer48;
    QHBoxLayout *hboxLayout35;
    QPushButton *pushVoltarTela2;
    QPushButton *pushFim;
    QWidget *tela0;
    QSpinBox *spinHue;
    QLabel *textLabel1_2_2_2_2_2;
    QSlider *sliderGamma;
    QSlider *sliderBrightness;
    QSpinBox *spinGamma;
    QSpinBox *spinContrast;
    QLabel *textLabel1_2_2_2_2_2_2;
    QLabel *textLabel1_2_2_2_2;
    QSlider *sliderContrast;
    QLabel *textLabel1_2;
    QLabel *textLabel1_2_2_2_2_2_4;
    QSpinBox *spinGain;
    QSlider *sliderGain;
    QSlider *sliderSaturation;
    QSlider *sliderHue;
    QSpinBox *spinShutter;
    QSpinBox *spinSaturation;
    QSpinBox *spinBrightness;
    QLabel *textLabel1_2_2;
    QLabel *textLabel1_2_2_2_2_2_3;
    QLabel *textLabel3;
    QSlider *sliderShutter;
    QPushButton *pushAvancarTela0;
    QMenuBar *MenuBar;
    QMenu *File;
    QMenu *Image;
    QMenu *Camera;

    void setupUi(Q3MainWindow *calibrador)
    {
        if (calibrador->objectName().isEmpty())
            calibrador->setObjectName(QString::fromUtf8("calibrador"));
        calibrador->resize(1056, 587);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(5));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(calibrador->sizePolicy().hasHeightForWidth());
        calibrador->setSizePolicy(sizePolicy);
        calibrador->setMinimumSize(QSize(21, 138));
        fileNewAction = new QAction(calibrador);
        fileNewAction->setObjectName(QString::fromUtf8("fileNewAction"));
        fileNewAction->setName("fileNewAction");
        const QIcon icon = QIcon(QString::fromUtf8("filenew"));
        fileNewAction->setIcon(icon);
        fileOpenAction = new QAction(calibrador);
        fileOpenAction->setObjectName(QString::fromUtf8("fileOpenAction"));
        fileOpenAction->setName("fileOpenAction");
        const QIcon icon1 = QIcon(QString::fromUtf8("fileopen"));
        fileOpenAction->setIcon(icon1);
        fileSaveAction = new QAction(calibrador);
        fileSaveAction->setObjectName(QString::fromUtf8("fileSaveAction"));
        fileSaveAction->setName("fileSaveAction");
        const QIcon icon2 = QIcon(QString::fromUtf8("filesave"));
        fileSaveAction->setIcon(icon2);
        fileSaveAsAction = new QAction(calibrador);
        fileSaveAsAction->setObjectName(QString::fromUtf8("fileSaveAsAction"));
        fileSaveAsAction->setName("fileSaveAsAction");
        fileExitAction = new QAction(calibrador);
        fileExitAction->setObjectName(QString::fromUtf8("fileExitAction"));
        fileExitAction->setName("fileExitAction");
        imageLoadAction = new QAction(calibrador);
        imageLoadAction->setObjectName(QString::fromUtf8("imageLoadAction"));
        imageLoadAction->setName("imageLoadAction");
        imageSnapshotAction = new QAction(calibrador);
        imageSnapshotAction->setObjectName(QString::fromUtf8("imageSnapshotAction"));
        imageSnapshotAction->setName("imageSnapshotAction");
        imageSaveAction = new QAction(calibrador);
        imageSaveAction->setObjectName(QString::fromUtf8("imageSaveAction"));
        imageSaveAction->setName("imageSaveAction");
        imageSaveAction->setEnabled(true);
        cameraLoad_ParametersAction = new QAction(calibrador);
        cameraLoad_ParametersAction->setObjectName(QString::fromUtf8("cameraLoad_ParametersAction"));
        cameraLoad_ParametersAction->setName("cameraLoad_ParametersAction");
        cameraSave_ParametersAction = new QAction(calibrador);
        cameraSave_ParametersAction->setObjectName(QString::fromUtf8("cameraSave_ParametersAction"));
        cameraSave_ParametersAction->setName("cameraSave_ParametersAction");
        cameraSave_Parameters_asAction = new QAction(calibrador);
        cameraSave_Parameters_asAction->setObjectName(QString::fromUtf8("cameraSave_Parameters_asAction"));
        cameraSave_Parameters_asAction->setName("cameraSave_Parameters_asAction");
        cameraNewAction = new QAction(calibrador);
        cameraNewAction->setObjectName(QString::fromUtf8("cameraNewAction"));
        cameraNewAction->setName("cameraNewAction");
        screenActionGroup = new QActionGroup(calibrador);
        screenActionGroup->setObjectName(QString::fromUtf8("screenActionGroup"));
        screenActionGroup->setName("screenActionGroup");
        screenSelectOneAction = new QAction(screenActionGroup);
        screenSelectOneAction->setObjectName(QString::fromUtf8("screenSelectOneAction"));
        screenSelectOneAction->setName("screenSelectOneAction");
        screenSelectOneAction->setCheckable(true);
        screenSelectOneAction->setChecked(true);
        screenSelectTwoAction = new QAction(screenActionGroup);
        screenSelectTwoAction->setObjectName(QString::fromUtf8("screenSelectTwoAction"));
        screenSelectTwoAction->setName("screenSelectTwoAction");
        screenSelectTwoAction->setCheckable(true);
        screenSelectTwoAction->setEnabled(false);
        widget = new QWidget(calibrador);
        widget->setObjectName(QString::fromUtf8("widget"));
        layout55 = new QWidget(widget);
        layout55->setObjectName(QString::fromUtf8("layout55"));
        layout55->setGeometry(QRect(4, 10, 1042, 518));
        hboxLayout = new QHBoxLayout(layout55);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(11, 11, 11, 11);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        sliderVOffset = new QSlider(layout55);
        sliderVOffset->setObjectName(QString::fromUtf8("sliderVOffset"));
        QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sliderVOffset->sizePolicy().hasHeightForWidth());
        sliderVOffset->setSizePolicy(sizePolicy1);
        sliderVOffset->setMinimumSize(QSize(0, 480));
        sliderVOffset->setMaximumSize(QSize(32767, 480));
        sliderVOffset->setOrientation(Qt::Vertical);

        vboxLayout->addWidget(sliderVOffset);

        spacer25 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacer25);


        hboxLayout1->addLayout(vboxLayout);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(6);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        pixmap_label1 = new pixmap_label(layout55);
        pixmap_label1->setObjectName(QString::fromUtf8("pixmap_label1"));
        QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pixmap_label1->sizePolicy().hasHeightForWidth());
        pixmap_label1->setSizePolicy(sizePolicy2);
        pixmap_label1->setMinimumSize(QSize(640, 480));
        pixmap_label1->setMaximumSize(QSize(640, 480));
        pixmap_label1->setMouseTracking(true);

        vboxLayout1->addWidget(pixmap_label1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        sliderUOffset = new QSlider(layout55);
        sliderUOffset->setObjectName(QString::fromUtf8("sliderUOffset"));
        sliderUOffset->setOrientation(Qt::Horizontal);

        hboxLayout2->addWidget(sliderUOffset);

        checkShooting = new QCheckBox(layout55);
        checkShooting->setObjectName(QString::fromUtf8("checkShooting"));

        hboxLayout2->addWidget(checkShooting);


        vboxLayout1->addLayout(hboxLayout2);


        hboxLayout1->addLayout(vboxLayout1);


        hboxLayout->addLayout(hboxLayout1);

        widgetStack1 = new Q3WidgetStack(layout55);
        widgetStack1->setObjectName(QString::fromUtf8("widgetStack1"));
        widgetStack1->setMinimumSize(QSize(362, 516));
        widgetStack1->setMaximumSize(QSize(362, 516));
        tela1 = new QWidget(widgetStack1);
        tela1->setObjectName(QString::fromUtf8("tela1"));
        layout45 = new QWidget(tela1);
        layout45->setObjectName(QString::fromUtf8("layout45"));
        layout45->setGeometry(QRect(10, 10, 340, 490));
        vboxLayout2 = new QVBoxLayout(layout45);
        vboxLayout2->setSpacing(6);
        vboxLayout2->setContentsMargins(11, 11, 11, 11);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        vboxLayout2->setContentsMargins(0, 0, 0, 0);
        vboxLayout3 = new QVBoxLayout();
        vboxLayout3->setSpacing(6);
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        labelRGB = new QLabel(layout45);
        labelRGB->setObjectName(QString::fromUtf8("labelRGB"));
        labelRGB->setWordWrap(false);

        hboxLayout3->addWidget(labelRGB);

        lcdRGB_R = new QLCDNumber(layout45);
        lcdRGB_R->setObjectName(QString::fromUtf8("lcdRGB_R"));

        hboxLayout3->addWidget(lcdRGB_R);

        lcdRGB_G = new QLCDNumber(layout45);
        lcdRGB_G->setObjectName(QString::fromUtf8("lcdRGB_G"));

        hboxLayout3->addWidget(lcdRGB_G);

        lcdRGB_B = new QLCDNumber(layout45);
        lcdRGB_B->setObjectName(QString::fromUtf8("lcdRGB_B"));

        hboxLayout3->addWidget(lcdRGB_B);


        vboxLayout3->addLayout(hboxLayout3);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setSpacing(6);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        labelHGP = new QLabel(layout45);
        labelHGP->setObjectName(QString::fromUtf8("labelHGP"));
        labelHGP->setWordWrap(false);

        hboxLayout4->addWidget(labelHGP);

        lcdHGP_H = new QLCDNumber(layout45);
        lcdHGP_H->setObjectName(QString::fromUtf8("lcdHGP_H"));

        hboxLayout4->addWidget(lcdHGP_H);

        lcdHGP_P = new QLCDNumber(layout45);
        lcdHGP_P->setObjectName(QString::fromUtf8("lcdHGP_P"));

        hboxLayout4->addWidget(lcdHGP_P);

        lcdHGP_G = new QLCDNumber(layout45);
        lcdHGP_G->setObjectName(QString::fromUtf8("lcdHGP_G"));

        hboxLayout4->addWidget(lcdHGP_G);


        vboxLayout3->addLayout(hboxLayout4);

        hboxLayout5 = new QHBoxLayout();
        hboxLayout5->setSpacing(6);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        labelX = new QLabel(layout45);
        labelX->setObjectName(QString::fromUtf8("labelX"));
        labelX->setWordWrap(false);

        hboxLayout5->addWidget(labelX);

        lcdX = new QLCDNumber(layout45);
        lcdX->setObjectName(QString::fromUtf8("lcdX"));

        hboxLayout5->addWidget(lcdX);

        spacer3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout5->addItem(spacer3);


        vboxLayout3->addLayout(hboxLayout5);

        hboxLayout6 = new QHBoxLayout();
        hboxLayout6->setSpacing(6);
        hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
        labelY = new QLabel(layout45);
        labelY->setObjectName(QString::fromUtf8("labelY"));
        labelY->setWordWrap(false);

        hboxLayout6->addWidget(labelY);

        lcdY = new QLCDNumber(layout45);
        lcdY->setObjectName(QString::fromUtf8("lcdY"));

        hboxLayout6->addWidget(lcdY);

        spacer4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout6->addItem(spacer4);


        vboxLayout3->addLayout(hboxLayout6);


        vboxLayout2->addLayout(vboxLayout3);

        spacer9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout2->addItem(spacer9);

        hboxLayout7 = new QHBoxLayout();
        hboxLayout7->setSpacing(6);
        hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
        vboxLayout4 = new QVBoxLayout();
        vboxLayout4->setSpacing(6);
        vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
        checkExibirGrade = new QCheckBox(layout45);
        checkExibirGrade->setObjectName(QString::fromUtf8("checkExibirGrade"));
        checkExibirGrade->setChecked(true);

        vboxLayout4->addWidget(checkExibirGrade);

        checkExibirImagemProcessada = new QCheckBox(layout45);
        checkExibirImagemProcessada->setObjectName(QString::fromUtf8("checkExibirImagemProcessada"));

        vboxLayout4->addWidget(checkExibirImagemProcessada);


        hboxLayout7->addLayout(vboxLayout4);

        spacer8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout7->addItem(spacer8);


        vboxLayout2->addLayout(hboxLayout7);

        spacer9_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout2->addItem(spacer9_2);

        vboxLayout5 = new QVBoxLayout();
        vboxLayout5->setSpacing(6);
        vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
        hboxLayout8 = new QHBoxLayout();
        hboxLayout8->setSpacing(6);
        hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
        labelLimiarPInf = new QLabel(layout45);
        labelLimiarPInf->setObjectName(QString::fromUtf8("labelLimiarPInf"));
        labelLimiarPInf->setWordWrap(false);

        hboxLayout8->addWidget(labelLimiarPInf);

        spacer5_2 = new QSpacerItem(160, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout8->addItem(spacer5_2);


        vboxLayout5->addLayout(hboxLayout8);

        hboxLayout9 = new QHBoxLayout();
        hboxLayout9->setSpacing(6);
        hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
        spinLimiarPInf = new QSpinBox(layout45);
        spinLimiarPInf->setObjectName(QString::fromUtf8("spinLimiarPInf"));
        spinLimiarPInf->setMaximum(99);
        spinLimiarPInf->setValue(0);

        hboxLayout9->addWidget(spinLimiarPInf);

        sliderLimiarPInf = new QSlider(layout45);
        sliderLimiarPInf->setObjectName(QString::fromUtf8("sliderLimiarPInf"));
        sliderLimiarPInf->setMaximum(99);
        sliderLimiarPInf->setValue(0);
        sliderLimiarPInf->setOrientation(Qt::Horizontal);

        hboxLayout9->addWidget(sliderLimiarPInf);


        vboxLayout5->addLayout(hboxLayout9);


        vboxLayout2->addLayout(vboxLayout5);

        vboxLayout6 = new QVBoxLayout();
        vboxLayout6->setSpacing(6);
        vboxLayout6->setObjectName(QString::fromUtf8("vboxLayout6"));
        hboxLayout10 = new QHBoxLayout();
        hboxLayout10->setSpacing(6);
        hboxLayout10->setObjectName(QString::fromUtf8("hboxLayout10"));
        labelLimiarPSup = new QLabel(layout45);
        labelLimiarPSup->setObjectName(QString::fromUtf8("labelLimiarPSup"));
        labelLimiarPSup->setWordWrap(false);

        hboxLayout10->addWidget(labelLimiarPSup);

        spacer5 = new QSpacerItem(160, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout10->addItem(spacer5);


        vboxLayout6->addLayout(hboxLayout10);

        hboxLayout11 = new QHBoxLayout();
        hboxLayout11->setSpacing(6);
        hboxLayout11->setObjectName(QString::fromUtf8("hboxLayout11"));
        spinLimiarPSup = new QSpinBox(layout45);
        spinLimiarPSup->setObjectName(QString::fromUtf8("spinLimiarPSup"));
        spinLimiarPSup->setMaximum(100);
        spinLimiarPSup->setMinimum(1);
        spinLimiarPSup->setValue(100);

        hboxLayout11->addWidget(spinLimiarPSup);

        sliderLimiarPSup = new QSlider(layout45);
        sliderLimiarPSup->setObjectName(QString::fromUtf8("sliderLimiarPSup"));
        sliderLimiarPSup->setMinimum(1);
        sliderLimiarPSup->setMaximum(100);
        sliderLimiarPSup->setValue(100);
        sliderLimiarPSup->setOrientation(Qt::Horizontal);

        hboxLayout11->addWidget(sliderLimiarPSup);


        vboxLayout6->addLayout(hboxLayout11);


        vboxLayout2->addLayout(vboxLayout6);

        spacer11 = new QSpacerItem(20, 38, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout2->addItem(spacer11);

        hboxLayout12 = new QHBoxLayout();
        hboxLayout12->setSpacing(6);
        hboxLayout12->setObjectName(QString::fromUtf8("hboxLayout12"));
        pushVoltarTela1 = new QPushButton(layout45);
        pushVoltarTela1->setObjectName(QString::fromUtf8("pushVoltarTela1"));

        hboxLayout12->addWidget(pushVoltarTela1);

        pushAvancarTela1 = new QPushButton(layout45);
        pushAvancarTela1->setObjectName(QString::fromUtf8("pushAvancarTela1"));

        hboxLayout12->addWidget(pushAvancarTela1);


        vboxLayout2->addLayout(hboxLayout12);

        widgetStack1->addWidget(tela1, 0);
        tela2 = new QWidget(widgetStack1);
        tela2->setObjectName(QString::fromUtf8("tela2"));
        layout44 = new QWidget(tela2);
        layout44->setObjectName(QString::fromUtf8("layout44"));
        layout44->setGeometry(QRect(10, 10, 339, 486));
        vboxLayout7 = new QVBoxLayout(layout44);
        vboxLayout7->setSpacing(6);
        vboxLayout7->setContentsMargins(11, 11, 11, 11);
        vboxLayout7->setObjectName(QString::fromUtf8("vboxLayout7"));
        vboxLayout7->setContentsMargins(0, 0, 0, 0);
        hboxLayout13 = new QHBoxLayout();
        hboxLayout13->setSpacing(6);
        hboxLayout13->setObjectName(QString::fromUtf8("hboxLayout13"));
        spacer41 = new QSpacerItem(34, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout13->addItem(spacer41);

        hboxLayout14 = new QHBoxLayout();
        hboxLayout14->setSpacing(6);
        hboxLayout14->setObjectName(QString::fromUtf8("hboxLayout14"));
        textLabel1 = new QLabel(layout44);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(false);

        hboxLayout14->addWidget(textLabel1);

        spacer32 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout14->addItem(spacer32);

        comboExibicao1 = new QComboBox(layout44);
        comboExibicao1->setObjectName(QString::fromUtf8("comboExibicao1"));

        hboxLayout14->addWidget(comboExibicao1);


        hboxLayout13->addLayout(hboxLayout14);


        vboxLayout7->addLayout(hboxLayout13);

        hboxLayout15 = new QHBoxLayout();
        hboxLayout15->setSpacing(6);
        hboxLayout15->setObjectName(QString::fromUtf8("hboxLayout15"));
        spacer42 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout15->addItem(spacer42);

        hboxLayout16 = new QHBoxLayout();
        hboxLayout16->setSpacing(6);
        hboxLayout16->setObjectName(QString::fromUtf8("hboxLayout16"));
        labelCor = new QLabel(layout44);
        labelCor->setObjectName(QString::fromUtf8("labelCor"));
        labelCor->setWordWrap(false);

        hboxLayout16->addWidget(labelCor);

        spacer33 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout16->addItem(spacer33);

        comboCores = new QComboBox(layout44);
        comboCores->setObjectName(QString::fromUtf8("comboCores"));
        comboCores->setEditable(false);

        hboxLayout16->addWidget(comboCores);


        hboxLayout15->addLayout(hboxLayout16);


        vboxLayout7->addLayout(hboxLayout15);

        vboxLayout8 = new QVBoxLayout();
        vboxLayout8->setSpacing(6);
        vboxLayout8->setObjectName(QString::fromUtf8("vboxLayout8"));
        hboxLayout17 = new QHBoxLayout();
        hboxLayout17->setSpacing(6);
        hboxLayout17->setObjectName(QString::fromUtf8("hboxLayout17"));
        labelHMin = new QLabel(layout44);
        labelHMin->setObjectName(QString::fromUtf8("labelHMin"));
        labelHMin->setWordWrap(false);

        hboxLayout17->addWidget(labelHMin);

        spacer34 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout17->addItem(spacer34);

        hboxLayout18 = new QHBoxLayout();
        hboxLayout18->setSpacing(6);
        hboxLayout18->setObjectName(QString::fromUtf8("hboxLayout18"));
        spinHGP_Hmin = new QSpinBox(layout44);
        spinHGP_Hmin->setObjectName(QString::fromUtf8("spinHGP_Hmin"));
        spinHGP_Hmin->setMaximum(180);
        spinHGP_Hmin->setMinimum(-180);

        hboxLayout18->addWidget(spinHGP_Hmin);

        sliderHGP_Hmin = new QSlider(layout44);
        sliderHGP_Hmin->setObjectName(QString::fromUtf8("sliderHGP_Hmin"));
        sliderHGP_Hmin->setMinimumSize(QSize(160, 0));
        sliderHGP_Hmin->setMinimum(-180);
        sliderHGP_Hmin->setMaximum(180);
        sliderHGP_Hmin->setPageStep(5);
        sliderHGP_Hmin->setValue(-180);
        sliderHGP_Hmin->setOrientation(Qt::Horizontal);

        hboxLayout18->addWidget(sliderHGP_Hmin);


        hboxLayout17->addLayout(hboxLayout18);


        vboxLayout8->addLayout(hboxLayout17);

        hboxLayout19 = new QHBoxLayout();
        hboxLayout19->setSpacing(6);
        hboxLayout19->setObjectName(QString::fromUtf8("hboxLayout19"));
        labelHMax = new QLabel(layout44);
        labelHMax->setObjectName(QString::fromUtf8("labelHMax"));
        labelHMax->setWordWrap(false);

        hboxLayout19->addWidget(labelHMax);

        spacer36 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout19->addItem(spacer36);

        hboxLayout20 = new QHBoxLayout();
        hboxLayout20->setSpacing(6);
        hboxLayout20->setObjectName(QString::fromUtf8("hboxLayout20"));
        spinHGP_Hmax = new QSpinBox(layout44);
        spinHGP_Hmax->setObjectName(QString::fromUtf8("spinHGP_Hmax"));
        spinHGP_Hmax->setMaximum(180);
        spinHGP_Hmax->setMinimum(-180);
        spinHGP_Hmax->setValue(0);

        hboxLayout20->addWidget(spinHGP_Hmax);

        sliderHGP_Hmax = new QSlider(layout44);
        sliderHGP_Hmax->setObjectName(QString::fromUtf8("sliderHGP_Hmax"));
        sliderHGP_Hmax->setMinimumSize(QSize(160, 0));
        sliderHGP_Hmax->setMinimum(-180);
        sliderHGP_Hmax->setMaximum(180);
        sliderHGP_Hmax->setPageStep(5);
        sliderHGP_Hmax->setValue(180);
        sliderHGP_Hmax->setOrientation(Qt::Horizontal);

        hboxLayout20->addWidget(sliderHGP_Hmax);


        hboxLayout19->addLayout(hboxLayout20);


        vboxLayout8->addLayout(hboxLayout19);

        hboxLayout21 = new QHBoxLayout();
        hboxLayout21->setSpacing(6);
        hboxLayout21->setObjectName(QString::fromUtf8("hboxLayout21"));
        labelPMin = new QLabel(layout44);
        labelPMin->setObjectName(QString::fromUtf8("labelPMin"));
        labelPMin->setWordWrap(false);

        hboxLayout21->addWidget(labelPMin);

        spacer39 = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout21->addItem(spacer39);

        hboxLayout22 = new QHBoxLayout();
        hboxLayout22->setSpacing(6);
        hboxLayout22->setObjectName(QString::fromUtf8("hboxLayout22"));
        spinHGP_Pmin = new QSpinBox(layout44);
        spinHGP_Pmin->setObjectName(QString::fromUtf8("spinHGP_Pmin"));
        spinHGP_Pmin->setMaximum(100);

        hboxLayout22->addWidget(spinHGP_Pmin);

        sliderHGP_Pmin = new QSlider(layout44);
        sliderHGP_Pmin->setObjectName(QString::fromUtf8("sliderHGP_Pmin"));
        sliderHGP_Pmin->setMinimumSize(QSize(160, 0));
        sliderHGP_Pmin->setMaximum(100);
        sliderHGP_Pmin->setPageStep(5);
        sliderHGP_Pmin->setOrientation(Qt::Horizontal);

        hboxLayout22->addWidget(sliderHGP_Pmin);


        hboxLayout21->addLayout(hboxLayout22);


        vboxLayout8->addLayout(hboxLayout21);

        hboxLayout23 = new QHBoxLayout();
        hboxLayout23->setSpacing(6);
        hboxLayout23->setObjectName(QString::fromUtf8("hboxLayout23"));
        labelPMax = new QLabel(layout44);
        labelPMax->setObjectName(QString::fromUtf8("labelPMax"));
        labelPMax->setWordWrap(false);

        hboxLayout23->addWidget(labelPMax);

        spacer40 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout23->addItem(spacer40);

        hboxLayout24 = new QHBoxLayout();
        hboxLayout24->setSpacing(6);
        hboxLayout24->setObjectName(QString::fromUtf8("hboxLayout24"));
        spinHGP_Pmax = new QSpinBox(layout44);
        spinHGP_Pmax->setObjectName(QString::fromUtf8("spinHGP_Pmax"));
        spinHGP_Pmax->setMaximum(100);
        spinHGP_Pmax->setValue(0);

        hboxLayout24->addWidget(spinHGP_Pmax);

        sliderHGP_Pmax = new QSlider(layout44);
        sliderHGP_Pmax->setObjectName(QString::fromUtf8("sliderHGP_Pmax"));
        sliderHGP_Pmax->setMinimumSize(QSize(160, 0));
        sliderHGP_Pmax->setMaximum(100);
        sliderHGP_Pmax->setPageStep(5);
        sliderHGP_Pmax->setValue(0);
        sliderHGP_Pmax->setOrientation(Qt::Horizontal);

        hboxLayout24->addWidget(sliderHGP_Pmax);


        hboxLayout23->addLayout(hboxLayout24);


        vboxLayout8->addLayout(hboxLayout23);

        hboxLayout25 = new QHBoxLayout();
        hboxLayout25->setSpacing(6);
        hboxLayout25->setObjectName(QString::fromUtf8("hboxLayout25"));
        labelGMin = new QLabel(layout44);
        labelGMin->setObjectName(QString::fromUtf8("labelGMin"));
        labelGMin->setWordWrap(false);

        hboxLayout25->addWidget(labelGMin);

        spacer37 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout25->addItem(spacer37);

        hboxLayout26 = new QHBoxLayout();
        hboxLayout26->setSpacing(6);
        hboxLayout26->setObjectName(QString::fromUtf8("hboxLayout26"));
        spinHGP_Gmin = new QSpinBox(layout44);
        spinHGP_Gmin->setObjectName(QString::fromUtf8("spinHGP_Gmin"));
        spinHGP_Gmin->setMaximum(100);

        hboxLayout26->addWidget(spinHGP_Gmin);

        sliderHGP_Gmin = new QSlider(layout44);
        sliderHGP_Gmin->setObjectName(QString::fromUtf8("sliderHGP_Gmin"));
        sliderHGP_Gmin->setMinimumSize(QSize(160, 0));
        sliderHGP_Gmin->setMaximum(100);
        sliderHGP_Gmin->setPageStep(5);
        sliderHGP_Gmin->setOrientation(Qt::Horizontal);

        hboxLayout26->addWidget(sliderHGP_Gmin);


        hboxLayout25->addLayout(hboxLayout26);


        vboxLayout8->addLayout(hboxLayout25);

        hboxLayout27 = new QHBoxLayout();
        hboxLayout27->setSpacing(6);
        hboxLayout27->setObjectName(QString::fromUtf8("hboxLayout27"));
        labelGMax = new QLabel(layout44);
        labelGMax->setObjectName(QString::fromUtf8("labelGMax"));
        labelGMax->setWordWrap(false);

        hboxLayout27->addWidget(labelGMax);

        spacer38 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout27->addItem(spacer38);

        hboxLayout28 = new QHBoxLayout();
        hboxLayout28->setSpacing(6);
        hboxLayout28->setObjectName(QString::fromUtf8("hboxLayout28"));
        spinHGP_Gmax = new QSpinBox(layout44);
        spinHGP_Gmax->setObjectName(QString::fromUtf8("spinHGP_Gmax"));
        spinHGP_Gmax->setMaximum(100);
        spinHGP_Gmax->setValue(0);

        hboxLayout28->addWidget(spinHGP_Gmax);

        sliderHGP_Gmax = new QSlider(layout44);
        sliderHGP_Gmax->setObjectName(QString::fromUtf8("sliderHGP_Gmax"));
        sliderHGP_Gmax->setMinimumSize(QSize(160, 0));
        sliderHGP_Gmax->setMaximum(100);
        sliderHGP_Gmax->setPageStep(5);
        sliderHGP_Gmax->setValue(0);
        sliderHGP_Gmax->setOrientation(Qt::Horizontal);

        hboxLayout28->addWidget(sliderHGP_Gmax);


        hboxLayout27->addLayout(hboxLayout28);


        vboxLayout8->addLayout(hboxLayout27);


        vboxLayout7->addLayout(vboxLayout8);

        spacer45 = new QSpacerItem(20, 44, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout7->addItem(spacer45);

        hboxLayout29 = new QHBoxLayout();
        hboxLayout29->setSpacing(6);
        hboxLayout29->setObjectName(QString::fromUtf8("hboxLayout29"));
        hboxLayout30 = new QHBoxLayout();
        hboxLayout30->setSpacing(6);
        hboxLayout30->setObjectName(QString::fromUtf8("hboxLayout30"));
        labelX2 = new QLabel(layout44);
        labelX2->setObjectName(QString::fromUtf8("labelX2"));
        labelX2->setWordWrap(false);

        hboxLayout30->addWidget(labelX2);

        lcdX2 = new QLCDNumber(layout44);
        lcdX2->setObjectName(QString::fromUtf8("lcdX2"));
        lcdX2->setMaximumSize(QSize(45, 23));

        hboxLayout30->addWidget(lcdX2);


        hboxLayout29->addLayout(hboxLayout30);

        spacer43 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout29->addItem(spacer43);

        hboxLayout31 = new QHBoxLayout();
        hboxLayout31->setSpacing(6);
        hboxLayout31->setObjectName(QString::fromUtf8("hboxLayout31"));
        labelRGB2 = new QLabel(layout44);
        labelRGB2->setObjectName(QString::fromUtf8("labelRGB2"));
        labelRGB2->setWordWrap(false);

        hboxLayout31->addWidget(labelRGB2);

        lcdRGB_R2 = new QLCDNumber(layout44);
        lcdRGB_R2->setObjectName(QString::fromUtf8("lcdRGB_R2"));
        lcdRGB_R2->setMaximumSize(QSize(45, 23));

        hboxLayout31->addWidget(lcdRGB_R2);

        lcdRGB_G2 = new QLCDNumber(layout44);
        lcdRGB_G2->setObjectName(QString::fromUtf8("lcdRGB_G2"));
        lcdRGB_G2->setMaximumSize(QSize(45, 23));

        hboxLayout31->addWidget(lcdRGB_G2);

        lcdRGB_B2 = new QLCDNumber(layout44);
        lcdRGB_B2->setObjectName(QString::fromUtf8("lcdRGB_B2"));
        lcdRGB_B2->setMaximumSize(QSize(45, 23));

        hboxLayout31->addWidget(lcdRGB_B2);


        hboxLayout29->addLayout(hboxLayout31);


        vboxLayout7->addLayout(hboxLayout29);

        hboxLayout32 = new QHBoxLayout();
        hboxLayout32->setSpacing(6);
        hboxLayout32->setObjectName(QString::fromUtf8("hboxLayout32"));
        hboxLayout33 = new QHBoxLayout();
        hboxLayout33->setSpacing(6);
        hboxLayout33->setObjectName(QString::fromUtf8("hboxLayout33"));
        labelY2 = new QLabel(layout44);
        labelY2->setObjectName(QString::fromUtf8("labelY2"));
        labelY2->setWordWrap(false);

        hboxLayout33->addWidget(labelY2);

        lcdY2 = new QLCDNumber(layout44);
        lcdY2->setObjectName(QString::fromUtf8("lcdY2"));
        lcdY2->setMaximumSize(QSize(45, 23));

        hboxLayout33->addWidget(lcdY2);


        hboxLayout32->addLayout(hboxLayout33);

        spacer44 = new QSpacerItem(41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout32->addItem(spacer44);

        hboxLayout34 = new QHBoxLayout();
        hboxLayout34->setSpacing(6);
        hboxLayout34->setObjectName(QString::fromUtf8("hboxLayout34"));
        labelHGP2 = new QLabel(layout44);
        labelHGP2->setObjectName(QString::fromUtf8("labelHGP2"));
        labelHGP2->setWordWrap(false);

        hboxLayout34->addWidget(labelHGP2);

        lcdHGP_H2 = new QLCDNumber(layout44);
        lcdHGP_H2->setObjectName(QString::fromUtf8("lcdHGP_H2"));
        lcdHGP_H2->setMaximumSize(QSize(45, 23));

        hboxLayout34->addWidget(lcdHGP_H2);

        lcdHGP_P2 = new QLCDNumber(layout44);
        lcdHGP_P2->setObjectName(QString::fromUtf8("lcdHGP_P2"));
        lcdHGP_P2->setMaximumSize(QSize(45, 23));

        hboxLayout34->addWidget(lcdHGP_P2);

        lcdHGP_G2 = new QLCDNumber(layout44);
        lcdHGP_G2->setObjectName(QString::fromUtf8("lcdHGP_G2"));
        lcdHGP_G2->setMaximumSize(QSize(45, 23));

        hboxLayout34->addWidget(lcdHGP_G2);


        hboxLayout32->addLayout(hboxLayout34);


        vboxLayout7->addLayout(hboxLayout32);

        spacer48 = new QSpacerItem(20, 44, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout7->addItem(spacer48);

        hboxLayout35 = new QHBoxLayout();
        hboxLayout35->setSpacing(6);
        hboxLayout35->setObjectName(QString::fromUtf8("hboxLayout35"));
        pushVoltarTela2 = new QPushButton(layout44);
        pushVoltarTela2->setObjectName(QString::fromUtf8("pushVoltarTela2"));

        hboxLayout35->addWidget(pushVoltarTela2);

        pushFim = new QPushButton(layout44);
        pushFim->setObjectName(QString::fromUtf8("pushFim"));

        hboxLayout35->addWidget(pushFim);


        vboxLayout7->addLayout(hboxLayout35);

        widgetStack1->addWidget(tela2, 1);
        tela0 = new QWidget(widgetStack1);
        tela0->setObjectName(QString::fromUtf8("tela0"));
        spinHue = new QSpinBox(tela0);
        spinHue->setObjectName(QString::fromUtf8("spinHue"));
        spinHue->setGeometry(QRect(300, 146, 53, 25));
        spinHue->setMaximum(0);
        textLabel1_2_2_2_2_2 = new QLabel(tela0);
        textLabel1_2_2_2_2_2->setObjectName(QString::fromUtf8("textLabel1_2_2_2_2_2"));
        textLabel1_2_2_2_2_2->setGeometry(QRect(13, 177, 96, 25));
        textLabel1_2_2_2_2_2->setWordWrap(false);
        sliderGamma = new QSlider(tela0);
        sliderGamma->setObjectName(QString::fromUtf8("sliderGamma"));
        sliderGamma->setGeometry(QRect(150, 212, 144, 16));
        sliderGamma->setMinimum(0);
        sliderGamma->setMaximum(0);
        sliderGamma->setOrientation(Qt::Horizontal);
        sliderBrightness = new QSlider(tela0);
        sliderBrightness->setObjectName(QString::fromUtf8("sliderBrightness"));
        sliderBrightness->setGeometry(QRect(115, 88, 179, 16));
        sliderBrightness->setMinimum(-64);
        sliderBrightness->setMaximum(64);
        sliderBrightness->setOrientation(Qt::Horizontal);
        spinGamma = new QSpinBox(tela0);
        spinGamma->setObjectName(QString::fromUtf8("spinGamma"));
        spinGamma->setGeometry(QRect(300, 208, 53, 25));
        spinGamma->setMaximum(0);
        spinGamma->setMinimum(0);
        spinContrast = new QSpinBox(tela0);
        spinContrast->setObjectName(QString::fromUtf8("spinContrast"));
        spinContrast->setGeometry(QRect(300, 115, 53, 25));
        spinContrast->setMaximum(64);
        textLabel1_2_2_2_2_2_2 = new QLabel(tela0);
        textLabel1_2_2_2_2_2_2->setObjectName(QString::fromUtf8("textLabel1_2_2_2_2_2_2"));
        textLabel1_2_2_2_2_2_2->setGeometry(QRect(13, 208, 130, 25));
        textLabel1_2_2_2_2_2_2->setWordWrap(false);
        textLabel1_2_2_2_2 = new QLabel(tela0);
        textLabel1_2_2_2_2->setObjectName(QString::fromUtf8("textLabel1_2_2_2_2"));
        textLabel1_2_2_2_2->setGeometry(QRect(13, 146, 130, 25));
        textLabel1_2_2_2_2->setWordWrap(false);
        sliderContrast = new QSlider(tela0);
        sliderContrast->setObjectName(QString::fromUtf8("sliderContrast"));
        sliderContrast->setGeometry(QRect(115, 119, 179, 16));
        sliderContrast->setMaximum(64);
        sliderContrast->setOrientation(Qt::Horizontal);
        textLabel1_2 = new QLabel(tela0);
        textLabel1_2->setObjectName(QString::fromUtf8("textLabel1_2"));
        textLabel1_2->setGeometry(QRect(13, 84, 96, 25));
        textLabel1_2->setWordWrap(false);
        textLabel1_2_2_2_2_2_4 = new QLabel(tela0);
        textLabel1_2_2_2_2_2_4->setObjectName(QString::fromUtf8("textLabel1_2_2_2_2_2_4"));
        textLabel1_2_2_2_2_2_4->setGeometry(QRect(13, 270, 120, 25));
        textLabel1_2_2_2_2_2_4->setWordWrap(false);
        spinGain = new QSpinBox(tela0);
        spinGain->setObjectName(QString::fromUtf8("spinGain"));
        spinGain->setGeometry(QRect(300, 270, 53, 25));
        spinGain->setMaximum(0);
        spinGain->setMinimum(0);
        sliderGain = new QSlider(tela0);
        sliderGain->setObjectName(QString::fromUtf8("sliderGain"));
        sliderGain->setGeometry(QRect(150, 274, 144, 16));
        sliderGain->setMinimum(0);
        sliderGain->setMaximum(0);
        sliderGain->setOrientation(Qt::Horizontal);
        sliderSaturation = new QSlider(tela0);
        sliderSaturation->setObjectName(QString::fromUtf8("sliderSaturation"));
        sliderSaturation->setGeometry(QRect(115, 181, 179, 16));
        sliderSaturation->setMaximum(128);
        sliderSaturation->setOrientation(Qt::Horizontal);
        sliderHue = new QSlider(tela0);
        sliderHue->setObjectName(QString::fromUtf8("sliderHue"));
        sliderHue->setGeometry(QRect(150, 150, 144, 16));
        sliderHue->setMaximum(0);
        sliderHue->setOrientation(Qt::Horizontal);
        spinShutter = new QSpinBox(tela0);
        spinShutter->setObjectName(QString::fromUtf8("spinShutter"));
        spinShutter->setGeometry(QRect(300, 239, 53, 25));
        spinShutter->setMaximum(0);
        spinSaturation = new QSpinBox(tela0);
        spinSaturation->setObjectName(QString::fromUtf8("spinSaturation"));
        spinSaturation->setGeometry(QRect(300, 177, 53, 25));
        spinSaturation->setMaximum(128);
        spinBrightness = new QSpinBox(tela0);
        spinBrightness->setObjectName(QString::fromUtf8("spinBrightness"));
        spinBrightness->setGeometry(QRect(300, 84, 53, 25));
        spinBrightness->setMinimum(-64);
        spinBrightness->setMaximum(64);
        textLabel1_2_2 = new QLabel(tela0);
        textLabel1_2_2->setObjectName(QString::fromUtf8("textLabel1_2_2"));
        textLabel1_2_2->setGeometry(QRect(13, 115, 96, 25));
        textLabel1_2_2->setWordWrap(false);
        textLabel1_2_2_2_2_2_3 = new QLabel(tela0);
        textLabel1_2_2_2_2_2_3->setObjectName(QString::fromUtf8("textLabel1_2_2_2_2_2_3"));
        textLabel1_2_2_2_2_2_3->setGeometry(QRect(13, 239, 130, 25));
        textLabel1_2_2_2_2_2_3->setWordWrap(false);
        textLabel3 = new QLabel(tela0);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setGeometry(QRect(11, 11, 340, 50));
        textLabel3->setWordWrap(false);
        sliderShutter = new QSlider(tela0);
        sliderShutter->setObjectName(QString::fromUtf8("sliderShutter"));
        sliderShutter->setGeometry(QRect(150, 243, 144, 16));
        sliderShutter->setMaximum(0);
        sliderShutter->setOrientation(Qt::Horizontal);
        pushAvancarTela0 = new QPushButton(tela0);
        pushAvancarTela0->setObjectName(QString::fromUtf8("pushAvancarTela0"));
        pushAvancarTela0->setGeometry(QRect(187, 465, 160, 30));
        widgetStack1->addWidget(tela0, 2);

        hboxLayout->addWidget(widgetStack1);

        calibrador->setCentralWidget(widget);
        MenuBar = new QMenuBar(calibrador);
        MenuBar->setObjectName(QString::fromUtf8("MenuBar"));
        File = new QMenu(MenuBar);
        File->setObjectName(QString::fromUtf8("File"));
        Image = new QMenu(MenuBar);
        Image->setObjectName(QString::fromUtf8("Image"));
        Camera = new QMenu(MenuBar);
        Camera->setObjectName(QString::fromUtf8("Camera"));

        MenuBar->addAction(File->menuAction());
        MenuBar->addAction(Image->menuAction());
        MenuBar->addAction(Camera->menuAction());
        File->addAction(fileNewAction);
        File->addAction(fileOpenAction);
        File->addAction(fileSaveAction);
        File->addAction(fileSaveAsAction);
        File->addSeparator();
        File->addAction(fileExitAction);
        Image->addAction(imageSaveAction);
        Image->addAction(imageLoadAction);
        Image->addSeparator();
        Image->addAction(imageSnapshotAction);
        Camera->addAction(cameraNewAction);
        Camera->addAction(cameraLoad_ParametersAction);
        Camera->addAction(cameraSave_ParametersAction);
        Camera->addAction(cameraSave_Parameters_asAction);

        retranslateUi(calibrador);
        QObject::connect(cameraLoad_ParametersAction, SIGNAL(activated()), calibrador, SLOT(cameraLoadParam()));
        QObject::connect(cameraNewAction, SIGNAL(activated()), calibrador, SLOT(cameraNew()));
        QObject::connect(cameraSave_ParametersAction, SIGNAL(activated()), calibrador, SLOT(cameraSaveParam()));
        QObject::connect(cameraSave_Parameters_asAction, SIGNAL(activated()), calibrador, SLOT(cameraSaveAsParam()));
        QObject::connect(checkExibirGrade, SIGNAL(stateChanged(int)), calibrador, SLOT(setarModo()));
        QObject::connect(checkExibirGrade, SIGNAL(clicked()), calibrador, SLOT(redesenharImagem()));
        QObject::connect(checkExibirImagemProcessada, SIGNAL(stateChanged(int)), calibrador, SLOT(setarModo()));
        QObject::connect(checkExibirImagemProcessada, SIGNAL(clicked()), calibrador, SLOT(redesenharImagem()));
        QObject::connect(checkShooting, SIGNAL(toggled(bool)), calibrador, SLOT(ShootingChanged(bool)));
        QObject::connect(comboCores, SIGNAL(activated(int)), calibrador, SLOT(redesenharImagem()));
        QObject::connect(comboCores, SIGNAL(activated(int)), calibrador, SLOT(atualizarLimitesHGP(int)));
        QObject::connect(comboExibicao1, SIGNAL(activated(int)), calibrador, SLOT(setarModo()));
        QObject::connect(comboExibicao1, SIGNAL(activated(int)), calibrador, SLOT(redesenharImagem()));
        QObject::connect(fileExitAction, SIGNAL(activated()), calibrador, SLOT(fileExit()));
        QObject::connect(fileNewAction, SIGNAL(activated()), calibrador, SLOT(redesenharImagem()));
        QObject::connect(fileNewAction, SIGNAL(activated()), calibrador, SLOT(fileNew()));
        QObject::connect(fileOpenAction, SIGNAL(activated()), calibrador, SLOT(redesenharImagem()));
        QObject::connect(fileOpenAction, SIGNAL(activated()), calibrador, SLOT(fileOpen()));
        QObject::connect(fileSaveAction, SIGNAL(activated()), calibrador, SLOT(fileSave()));
        QObject::connect(fileSaveAsAction, SIGNAL(activated()), calibrador, SLOT(fileSaveAs()));
        QObject::connect(imageLoadAction, SIGNAL(activated()), calibrador, SLOT(redesenharImagem()));
        QObject::connect(imageLoadAction, SIGNAL(activated()), calibrador, SLOT(imageOpen()));
        QObject::connect(imageSaveAction, SIGNAL(activated()), calibrador, SLOT(imageSave()));
        QObject::connect(pixmap_label1, SIGNAL(mouseReleased(QPoint)), calibrador, SLOT(mouseRelease(QPoint)));
        QObject::connect(pixmap_label1, SIGNAL(mousePressed(QPoint)), calibrador, SLOT(mousePress(QPoint)));
        QObject::connect(pixmap_label1, SIGNAL(mouseMoved(QPoint)), calibrador, SLOT(mouseMove(QPoint)));
        QObject::connect(pushAvancarTela0, SIGNAL(clicked()), calibrador, SLOT(redesenharImagem()));
        QObject::connect(pushAvancarTela0, SIGNAL(clicked()), calibrador, SLOT(mostrarTela1()));
        QObject::connect(pushAvancarTela1, SIGNAL(clicked()), calibrador, SLOT(redesenharImagem()));
        QObject::connect(pushAvancarTela1, SIGNAL(clicked()), calibrador, SLOT(mostrarTela2()));
        QObject::connect(pushFim, SIGNAL(clicked()), calibrador, SLOT(fileExit()));
        QObject::connect(pushVoltarTela1, SIGNAL(clicked()), calibrador, SLOT(redesenharImagem()));
        QObject::connect(pushVoltarTela1, SIGNAL(clicked()), calibrador, SLOT(mostrarTela0()));
        QObject::connect(pushVoltarTela2, SIGNAL(clicked()), calibrador, SLOT(redesenharImagem()));
        QObject::connect(pushVoltarTela2, SIGNAL(clicked()), calibrador, SLOT(mostrarTela1()));
        QObject::connect(sliderBrightness, SIGNAL(valueChanged(int)), calibrador, SLOT(BrightnessValueChanged(int)));
        QObject::connect(sliderContrast, SIGNAL(valueChanged(int)), calibrador, SLOT(ExposureValueChanged(int)));
        QObject::connect(sliderGain, SIGNAL(valueChanged(int)), calibrador, SLOT(GainValueChanged(int)));
        QObject::connect(sliderGamma, SIGNAL(valueChanged(int)), calibrador, SLOT(GammaValueChanged(int)));
        QObject::connect(sliderHGP_Gmax, SIGNAL(valueChanged(int)), calibrador, SLOT(GMaxValueChanged(int)));
        QObject::connect(sliderHGP_Gmin, SIGNAL(valueChanged(int)), calibrador, SLOT(GMinValueChanged(int)));
        QObject::connect(sliderHGP_Hmax, SIGNAL(valueChanged(int)), calibrador, SLOT(HMaxValueChanged(int)));
        QObject::connect(sliderHGP_Hmin, SIGNAL(valueChanged(int)), calibrador, SLOT(HMinValueChanged(int)));
        QObject::connect(sliderHGP_Pmax, SIGNAL(valueChanged(int)), calibrador, SLOT(PMaxValueChanged(int)));
        QObject::connect(sliderHGP_Pmin, SIGNAL(valueChanged(int)), calibrador, SLOT(PMinValueChanged(int)));
        QObject::connect(sliderHue, SIGNAL(valueChanged(int)), calibrador, SLOT(HueValueChanged(int)));
        QObject::connect(sliderLimiarPInf, SIGNAL(valueChanged(int)), calibrador, SLOT(sliderLimiarPInfValueChanged(int)));
        QObject::connect(sliderLimiarPSup, SIGNAL(valueChanged(int)), calibrador, SLOT(sliderLimiarPSupValueChanged(int)));
        QObject::connect(sliderSaturation, SIGNAL(valueChanged(int)), calibrador, SLOT(SaturationValueChanged(int)));
        QObject::connect(sliderShutter, SIGNAL(valueChanged(int)), calibrador, SLOT(ShutterValueChanged(int)));
        QObject::connect(sliderUOffset, SIGNAL(valueChanged(int)), calibrador, SLOT(redesenharImagem()));
        QObject::connect(sliderUOffset, SIGNAL(valueChanged(int)), calibrador, SLOT(changeUOffset(int)));
        QObject::connect(sliderVOffset, SIGNAL(valueChanged(int)), calibrador, SLOT(redesenharImagem()));
        QObject::connect(sliderVOffset, SIGNAL(valueChanged(int)), calibrador, SLOT(changeVOffset(int)));
        QObject::connect(spinBrightness, SIGNAL(valueChanged(int)), calibrador, SLOT(atualizarCamera()));
        QObject::connect(spinBrightness, SIGNAL(valueChanged(int)), calibrador, SLOT(BrightnessValueChanged(int)));
        QObject::connect(spinContrast, SIGNAL(valueChanged(int)), calibrador, SLOT(atualizarCamera()));
        QObject::connect(spinContrast, SIGNAL(valueChanged(int)), calibrador, SLOT(ExposureValueChanged(int)));
        QObject::connect(spinGain, SIGNAL(valueChanged(int)), calibrador, SLOT(atualizarCamera()));
        QObject::connect(spinGain, SIGNAL(valueChanged(int)), calibrador, SLOT(GainValueChanged(int)));
        QObject::connect(spinGamma, SIGNAL(valueChanged(int)), calibrador, SLOT(atualizarCamera()));
        QObject::connect(spinGamma, SIGNAL(valueChanged(int)), calibrador, SLOT(GammaValueChanged(int)));
        QObject::connect(spinHGP_Gmax, SIGNAL(valueChanged(int)), calibrador, SLOT(redesenharImagem()));
        QObject::connect(spinHGP_Gmax, SIGNAL(valueChanged(int)), calibrador, SLOT(GMaxValueChanged(int)));
        QObject::connect(spinHGP_Gmin, SIGNAL(valueChanged(int)), calibrador, SLOT(redesenharImagem()));
        QObject::connect(spinHGP_Gmin, SIGNAL(valueChanged(int)), calibrador, SLOT(GMinValueChanged(int)));
        QObject::connect(spinHGP_Hmax, SIGNAL(valueChanged(int)), calibrador, SLOT(redesenharImagem()));
        QObject::connect(spinHGP_Hmax, SIGNAL(valueChanged(int)), calibrador, SLOT(HMaxValueChanged(int)));
        QObject::connect(spinHGP_Hmin, SIGNAL(valueChanged(int)), calibrador, SLOT(redesenharImagem()));
        QObject::connect(spinHGP_Hmin, SIGNAL(valueChanged(int)), calibrador, SLOT(HMinValueChanged(int)));
        QObject::connect(spinHGP_Pmax, SIGNAL(valueChanged(int)), calibrador, SLOT(redesenharImagem()));
        QObject::connect(spinHGP_Pmax, SIGNAL(valueChanged(int)), calibrador, SLOT(PMaxValueChanged(int)));
        QObject::connect(spinHGP_Pmin, SIGNAL(valueChanged(int)), calibrador, SLOT(redesenharImagem()));
        QObject::connect(spinHGP_Pmin, SIGNAL(valueChanged(int)), calibrador, SLOT(PMinValueChanged(int)));
        QObject::connect(spinHue, SIGNAL(valueChanged(int)), calibrador, SLOT(atualizarCamera()));
        QObject::connect(spinHue, SIGNAL(valueChanged(int)), calibrador, SLOT(HueValueChanged(int)));
        QObject::connect(spinLimiarPInf, SIGNAL(valueChanged(int)), calibrador, SLOT(spinLimiarPInfValueChanged(int)));
        QObject::connect(spinLimiarPInf, SIGNAL(valueChanged(int)), calibrador, SLOT(redesenharImagem()));
        QObject::connect(spinLimiarPSup, SIGNAL(valueChanged(int)), calibrador, SLOT(spinLimiarPSupValueChanged(int)));
        QObject::connect(spinLimiarPSup, SIGNAL(valueChanged(int)), calibrador, SLOT(redesenharImagem()));
        QObject::connect(spinSaturation, SIGNAL(valueChanged(int)), calibrador, SLOT(atualizarCamera()));
        QObject::connect(spinSaturation, SIGNAL(valueChanged(int)), calibrador, SLOT(SaturationValueChanged(int)));
        QObject::connect(spinShutter, SIGNAL(valueChanged(int)), calibrador, SLOT(atualizarCamera()));
        QObject::connect(spinShutter, SIGNAL(valueChanged(int)), calibrador, SLOT(ShutterValueChanged(int)));

        QMetaObject::connectSlotsByName(calibrador);
    } // setupUi

    void retranslateUi(Q3MainWindow *calibrador)
    {
        calibrador->setWindowTitle(QApplication::translate("calibrador", "Calibrator", 0, QApplication::UnicodeUTF8));
        fileNewAction->setIconText(QApplication::translate("calibrador", "New", 0, QApplication::UnicodeUTF8));
        fileNewAction->setText(QApplication::translate("calibrador", "&New", 0, QApplication::UnicodeUTF8));
        fileNewAction->setShortcut(QApplication::translate("calibrador", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        fileOpenAction->setIconText(QApplication::translate("calibrador", "Open", 0, QApplication::UnicodeUTF8));
        fileOpenAction->setText(QApplication::translate("calibrador", "&Open", 0, QApplication::UnicodeUTF8));
        fileOpenAction->setShortcut(QApplication::translate("calibrador", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setIconText(QApplication::translate("calibrador", "Save", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setText(QApplication::translate("calibrador", "&Save", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setShortcut(QApplication::translate("calibrador", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        fileSaveAsAction->setIconText(QApplication::translate("calibrador", "Save As ...", 0, QApplication::UnicodeUTF8));
        fileSaveAsAction->setText(QApplication::translate("calibrador", "Save As ...", 0, QApplication::UnicodeUTF8));
        fileSaveAsAction->setShortcut(QString());
        fileExitAction->setIconText(QApplication::translate("calibrador", "Close", 0, QApplication::UnicodeUTF8));
        fileExitAction->setText(QApplication::translate("calibrador", "&Close", 0, QApplication::UnicodeUTF8));
        fileExitAction->setShortcut(QString());
        imageLoadAction->setIconText(QApplication::translate("calibrador", "Load", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        imageLoadAction->setStatusTip(QApplication::translate("calibrador", "Carrega imagem do arquivo.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        imageSnapshotAction->setIconText(QApplication::translate("calibrador", "Snapshot", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        imageSnapshotAction->setStatusTip(QApplication::translate("calibrador", "Carrega imagem da c\303\242mera.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        imageSaveAction->setIconText(QApplication::translate("calibrador", "Save", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        imageSaveAction->setStatusTip(QApplication::translate("calibrador", "Salva imagem em arquivo.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        cameraLoad_ParametersAction->setIconText(QApplication::translate("calibrador", "Load Parameters", 0, QApplication::UnicodeUTF8));
        cameraLoad_ParametersAction->setText(QApplication::translate("calibrador", "Load Parameters", 0, QApplication::UnicodeUTF8));
        cameraSave_ParametersAction->setIconText(QApplication::translate("calibrador", "Save Parameters", 0, QApplication::UnicodeUTF8));
        cameraSave_ParametersAction->setText(QApplication::translate("calibrador", "Save Parameters", 0, QApplication::UnicodeUTF8));
        cameraSave_Parameters_asAction->setIconText(QApplication::translate("calibrador", "Save Parameters As...", 0, QApplication::UnicodeUTF8));
        cameraSave_Parameters_asAction->setText(QApplication::translate("calibrador", "Save Parameters As...", 0, QApplication::UnicodeUTF8));
        cameraNewAction->setIconText(QApplication::translate("calibrador", "New", 0, QApplication::UnicodeUTF8));
        cameraNewAction->setText(QApplication::translate("calibrador", "New", 0, QApplication::UnicodeUTF8));
        screenSelectOneAction->setIconText(QApplication::translate("calibrador", "Tela 1", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        screenSelectOneAction->setStatusTip(QApplication::translate("calibrador", "Exibe a primeira tela de calibra\303\247\303\243o.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        screenSelectTwoAction->setIconText(QApplication::translate("calibrador", "Tela 2", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        screenSelectTwoAction->setStatusTip(QApplication::translate("calibrador", "Exibe a segunda tela de calibra\303\247\303\243o.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        checkShooting->setText(QApplication::translate("calibrador", "Shooting", 0, QApplication::UnicodeUTF8));
        labelRGB->setText(QApplication::translate("calibrador", "RGB (%):", 0, QApplication::UnicodeUTF8));
        labelHGP->setText(QApplication::translate("calibrador", "HPG (%):", 0, QApplication::UnicodeUTF8));
        labelX->setText(QApplication::translate("calibrador", "X:", 0, QApplication::UnicodeUTF8));
        labelY->setText(QApplication::translate("calibrador", "Y:", 0, QApplication::UnicodeUTF8));
        checkExibirGrade->setText(QApplication::translate("calibrador", "View Field Lines", 0, QApplication::UnicodeUTF8));
        checkExibirImagemProcessada->setText(QApplication::translate("calibrador", "View Process Image", 0, QApplication::UnicodeUTF8));
        labelLimiarPInf->setText(QApplication::translate("calibrador", "Lower P Threshold:", 0, QApplication::UnicodeUTF8));
        labelLimiarPSup->setText(QApplication::translate("calibrador", "Upper P Threshold:", 0, QApplication::UnicodeUTF8));
        pushVoltarTela1->setText(QApplication::translate("calibrador", "Back", 0, QApplication::UnicodeUTF8));
        pushAvancarTela1->setText(QApplication::translate("calibrador", "Next", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("calibrador", "View Mode:", 0, QApplication::UnicodeUTF8));
        comboExibicao1->clear();
        comboExibicao1->insertItems(0, QStringList()
         << QApplication::translate("calibrador", "Real Image", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("calibrador", "Current Color Image", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("calibrador", "Tagged Image", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("calibrador", "Soft Tagged Image", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("calibrador", "Error Image", 0, QApplication::UnicodeUTF8)
        );
        labelCor->setText(QApplication::translate("calibrador", "Color:", 0, QApplication::UnicodeUTF8));
        labelHMin->setText(QApplication::translate("calibrador", "H Min:", 0, QApplication::UnicodeUTF8));
        labelHMax->setText(QApplication::translate("calibrador", "H Max:", 0, QApplication::UnicodeUTF8));
        labelPMin->setText(QApplication::translate("calibrador", "P Min:", 0, QApplication::UnicodeUTF8));
        labelPMax->setText(QApplication::translate("calibrador", "P Max:", 0, QApplication::UnicodeUTF8));
        labelGMin->setText(QApplication::translate("calibrador", "G Min:", 0, QApplication::UnicodeUTF8));
        labelGMax->setText(QApplication::translate("calibrador", "G Max:", 0, QApplication::UnicodeUTF8));
        labelX2->setText(QApplication::translate("calibrador", "X:", 0, QApplication::UnicodeUTF8));
        labelRGB2->setText(QApplication::translate("calibrador", "RGB (%):", 0, QApplication::UnicodeUTF8));
        labelY2->setText(QApplication::translate("calibrador", "Y:", 0, QApplication::UnicodeUTF8));
        labelHGP2->setText(QApplication::translate("calibrador", "HPG (%):", 0, QApplication::UnicodeUTF8));
        pushVoltarTela2->setText(QApplication::translate("calibrador", "Back", 0, QApplication::UnicodeUTF8));
        pushFim->setText(QApplication::translate("calibrador", "Finish", 0, QApplication::UnicodeUTF8));
        textLabel1_2_2_2_2_2->setText(QApplication::translate("calibrador", "Saturation", 0, QApplication::UnicodeUTF8));
        textLabel1_2_2_2_2_2_2->setText(QApplication::translate("calibrador", "Gamma(Not Used)", 0, QApplication::UnicodeUTF8));
        textLabel1_2_2_2_2->setText(QApplication::translate("calibrador", "Hue(Not Used)", 0, QApplication::UnicodeUTF8));
        textLabel1_2->setText(QApplication::translate("calibrador", "Brightness", 0, QApplication::UnicodeUTF8));
        textLabel1_2_2_2_2_2_4->setText(QApplication::translate("calibrador", "Gain(Not Used)", 0, QApplication::UnicodeUTF8));
        textLabel1_2_2->setText(QApplication::translate("calibrador", "Contrast", 0, QApplication::UnicodeUTF8));
        textLabel1_2_2_2_2_2_3->setText(QApplication::translate("calibrador", "Shutter(Not Used)", 0, QApplication::UnicodeUTF8));
        textLabel3->setText(QApplication::translate("calibrador", "<b><font size=\"+2\">Parametros da Camera</font></b>", 0, QApplication::UnicodeUTF8));
        pushAvancarTela0->setText(QApplication::translate("calibrador", "Proximo", 0, QApplication::UnicodeUTF8));
        File->setTitle(QApplication::translate("calibrador", "File", 0, QApplication::UnicodeUTF8));
        Image->setTitle(QApplication::translate("calibrador", "Image", 0, QApplication::UnicodeUTF8));
        Camera->setTitle(QApplication::translate("calibrador", "Camera", 0, QApplication::UnicodeUTF8));
    } // retranslateUi


protected:
    enum IconID
    {
        image0_ID,
        unknown_ID
    };
    static QPixmap qt_get_icon(IconID id)
    {
    static const unsigned char image0_data[] = { 
    0x78, 0x9c, 0x53, 0x4e, 0x49, 0x4d, 0xcb, 0xcc, 0x4b, 0x55, 0x48, 0x29,
    0xcd, 0xcd, 0xad, 0x8c, 0x2f, 0xcf, 0x4c, 0x29, 0xc9, 0x50, 0x30, 0xe0,
    0x52, 0x46, 0x11, 0xcd, 0x48, 0xcd, 0x4c, 0xcf, 0x28, 0x01, 0x0a, 0x17,
    0x97, 0x24, 0x96, 0x64, 0x26, 0x2b, 0x24, 0x67, 0x24, 0x16, 0x41, 0xa5,
    0x92, 0x32, 0x4b, 0x8a, 0xa3, 0x63, 0x15, 0x6c, 0x15, 0xaa, 0xb9, 0x14,
    0x14, 0x6a, 0xad, 0xb9, 0x00, 0x67, 0x11, 0x1b, 0x1f
};

    switch (id) {
        case image0_ID:  { QImage img; img.loadFromData(image0_data, sizeof(image0_data), "XBM.GZ"); return QPixmap::fromImage(img); }
        default: return QPixmap();
    } // switch
    } // icon

};

namespace Ui {
    class calibrador: public Ui_calibrador {};
} // namespace Ui

QT_END_NAMESPACE

class calibrador : public Q3MainWindow, public Ui::calibrador
{
    Q_OBJECT

public:
    calibrador(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~calibrador();

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

    void init();
    void destroy();
    virtual void atualizarDisplays();
    virtual void atualizarLimitesP();

};

#endif // CALIBRADOR_H
