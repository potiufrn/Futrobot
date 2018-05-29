/****************************************************************************
** Form implementation generated from reading ui file 'calibrador.ui'
**
** Created by User Interface Compiler
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "calibrador.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qcheckbox.h>
#include <qwidgetstack.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qlcdnumber.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>

#include "pixmap_label.h"
#include "calibrador.ui.h"
/*
 *  Constructs a calibrador as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */

calibrador::calibrador( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
  cout<<"Estou no construtor de calibrator "<<endl;
    (void)statusBar();
    if ( !name )
	setName( "calibrador" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 21, 154 ) );
    setCentralWidget( new QWidget( this, "qt_central_widget" ) );

    QWidget* privateLayoutWidget = new QWidget( centralWidget(), "layout55" );
    privateLayoutWidget->setGeometry( QRect( 4, 10, 1042, 518 ) );
    layout55 = new QHBoxLayout( privateLayoutWidget, 11, 6, "layout55");

    layout54 = new QHBoxLayout( 0, 0, 6, "layout54");

    layout53 = new QVBoxLayout( 0, 0, 6, "layout53");

    sliderVOffset = new QSlider( privateLayoutWidget, "sliderVOffset" );
    sliderVOffset->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, sliderVOffset->sizePolicy().hasHeightForWidth() ) );
    sliderVOffset->setMinimumSize( QSize( 0, 480 ) );
    sliderVOffset->setMaximumSize( QSize( 32767, 480 ) );
    sliderVOffset->setOrientation( QSlider::Vertical );
    layout53->addWidget( sliderVOffset );
    spacer25 = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout53->addItem( spacer25 );
    layout54->addLayout( layout53 );

    layout48 = new QVBoxLayout( 0, 0, 6, "layout48");

    pixmap_label1 = new pixmap_label( privateLayoutWidget, "pixmap_label1" );
    pixmap_label1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, pixmap_label1->sizePolicy().hasHeightForWidth() ) );
    pixmap_label1->setMinimumSize( QSize( 640, 480 ) );
    pixmap_label1->setMaximumSize( QSize( 640, 480 ) );
    pixmap_label1->setMouseTracking( TRUE );
    layout48->addWidget( pixmap_label1 );

    layout47 = new QHBoxLayout( 0, 0, 6, "layout47");

    sliderUOffset = new QSlider( privateLayoutWidget, "sliderUOffset" );
    sliderUOffset->setOrientation( QSlider::Horizontal );
    layout47->addWidget( sliderUOffset );

    checkShooting = new QCheckBox( privateLayoutWidget, "checkShooting" );
    layout47->addWidget( checkShooting );
    layout48->addLayout( layout47 );
    layout54->addLayout( layout48 );
    layout55->addLayout( layout54 );

    widgetStack1 = new QWidgetStack( privateLayoutWidget, "widgetStack1" );
    widgetStack1->setMinimumSize( QSize( 362, 516 ) );
    widgetStack1->setMaximumSize( QSize( 362, 516 ) );

    tela1 = new QWidget( widgetStack1, "tela1" );

    QWidget* privateLayoutWidget_2 = new QWidget( tela1, "layout45" );
    privateLayoutWidget_2->setGeometry( QRect( 10, 10, 340, 490 ) );
    layout45 = new QVBoxLayout( privateLayoutWidget_2, 11, 6, "layout45");

    layout9 = new QVBoxLayout( 0, 0, 6, "layout9");

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1");

    labelRGB = new QLabel( privateLayoutWidget_2, "labelRGB" );
    layout1->addWidget( labelRGB );

    lcdRGB_R = new QLCDNumber( privateLayoutWidget_2, "lcdRGB_R" );
    lcdRGB_R->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout1->addWidget( lcdRGB_R );

    lcdRGB_G = new QLCDNumber( privateLayoutWidget_2, "lcdRGB_G" );
    lcdRGB_G->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout1->addWidget( lcdRGB_G );

    lcdRGB_B = new QLCDNumber( privateLayoutWidget_2, "lcdRGB_B" );
    lcdRGB_B->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout1->addWidget( lcdRGB_B );
    layout9->addLayout( layout1 );

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2");

    labelHGP = new QLabel( privateLayoutWidget_2, "labelHGP" );
    layout2->addWidget( labelHGP );

    lcdHGP_H = new QLCDNumber( privateLayoutWidget_2, "lcdHGP_H" );
    lcdHGP_H->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout2->addWidget( lcdHGP_H );

    lcdHGP_P = new QLCDNumber( privateLayoutWidget_2, "lcdHGP_P" );
    lcdHGP_P->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout2->addWidget( lcdHGP_P );

    lcdHGP_G = new QLCDNumber( privateLayoutWidget_2, "lcdHGP_G" );
    lcdHGP_G->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout2->addWidget( lcdHGP_G );
    layout9->addLayout( layout2 );

    layout7 = new QHBoxLayout( 0, 0, 6, "layout7");

    labelX = new QLabel( privateLayoutWidget_2, "labelX" );
    layout7->addWidget( labelX );

    lcdX = new QLCDNumber( privateLayoutWidget_2, "lcdX" );
    lcdX->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout7->addWidget( lcdX );
    spacer3 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout7->addItem( spacer3 );
    layout9->addLayout( layout7 );

    layout8 = new QHBoxLayout( 0, 0, 6, "layout8");

    labelY = new QLabel( privateLayoutWidget_2, "labelY" );
    layout8->addWidget( labelY );

    lcdY = new QLCDNumber( privateLayoutWidget_2, "lcdY" );
    lcdY->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout8->addWidget( lcdY );
    spacer4 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout8->addItem( spacer4 );
    layout9->addLayout( layout8 );
    layout45->addLayout( layout9 );
    spacer9 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout45->addItem( spacer9 );

    layout31 = new QHBoxLayout( 0, 0, 6, "layout31");

    layout10 = new QVBoxLayout( 0, 0, 6, "layout10");

    checkExibirGrade = new QCheckBox( privateLayoutWidget_2, "checkExibirGrade" );
    checkExibirGrade->setChecked( TRUE );
    layout10->addWidget( checkExibirGrade );

    checkExibirImagemProcessada = new QCheckBox( privateLayoutWidget_2, "checkExibirImagemProcessada" );
    layout10->addWidget( checkExibirImagemProcessada );
    layout31->addLayout( layout10 );
    spacer8 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout31->addItem( spacer8 );
    layout45->addLayout( layout31 );
    spacer9_2 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout45->addItem( spacer9_2 );

    layout27 = new QVBoxLayout( 0, 0, 6, "layout27");

    layout26 = new QHBoxLayout( 0, 0, 6, "layout26");

    labelLimiarPInf = new QLabel( privateLayoutWidget_2, "labelLimiarPInf" );
    layout26->addWidget( labelLimiarPInf );
    spacer5_2 = new QSpacerItem( 160, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout26->addItem( spacer5_2 );
    layout27->addLayout( layout26 );

    layout14 = new QHBoxLayout( 0, 0, 6, "layout14");

    spinLimiarPInf = new QSpinBox( privateLayoutWidget_2, "spinLimiarPInf" );
    spinLimiarPInf->setMaxValue( 99 );
    spinLimiarPInf->setValue( 0 );
    layout14->addWidget( spinLimiarPInf );

    sliderLimiarPInf = new QSlider( privateLayoutWidget_2, "sliderLimiarPInf" );
    sliderLimiarPInf->setMaxValue( 99 );
    sliderLimiarPInf->setValue( 0 );
    sliderLimiarPInf->setOrientation( QSlider::Horizontal );
    layout14->addWidget( sliderLimiarPInf );
    layout27->addLayout( layout14 );
    layout45->addLayout( layout27 );

    layout29 = new QVBoxLayout( 0, 0, 6, "layout29");

    layout28 = new QHBoxLayout( 0, 0, 6, "layout28");

    labelLimiarPSup = new QLabel( privateLayoutWidget_2, "labelLimiarPSup" );
    layout28->addWidget( labelLimiarPSup );
    spacer5 = new QSpacerItem( 160, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout28->addItem( spacer5 );
    layout29->addLayout( layout28 );

    layout15 = new QHBoxLayout( 0, 0, 6, "layout15");

    spinLimiarPSup = new QSpinBox( privateLayoutWidget_2, "spinLimiarPSup" );
    spinLimiarPSup->setMaxValue( 100 );
    spinLimiarPSup->setMinValue( 1 );
    spinLimiarPSup->setValue( 100 );
    layout15->addWidget( spinLimiarPSup );

    sliderLimiarPSup = new QSlider( privateLayoutWidget_2, "sliderLimiarPSup" );
    sliderLimiarPSup->setMinValue( 1 );
    sliderLimiarPSup->setMaxValue( 100 );
    sliderLimiarPSup->setValue( 100 );
    sliderLimiarPSup->setOrientation( QSlider::Horizontal );
    layout15->addWidget( sliderLimiarPSup );
    layout29->addLayout( layout15 );
    layout45->addLayout( layout29 );
    spacer11 = new QSpacerItem( 20, 38, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout45->addItem( spacer11 );

    layout43 = new QHBoxLayout( 0, 0, 6, "layout43");

    pushVoltarTela1 = new QPushButton( privateLayoutWidget_2, "pushVoltarTela1" );
    layout43->addWidget( pushVoltarTela1 );

    pushAvancarTela1 = new QPushButton( privateLayoutWidget_2, "pushAvancarTela1" );
    layout43->addWidget( pushAvancarTela1 );
    layout45->addLayout( layout43 );
    widgetStack1->addWidget( tela1, 0 );

    tela2 = new QWidget( widgetStack1, "tela2" );

    QWidget* privateLayoutWidget_3 = new QWidget( tela2, "layout44" );
    privateLayoutWidget_3->setGeometry( QRect( 10, 10, 339, 486 ) );
    layout44 = new QVBoxLayout( privateLayoutWidget_3, 11, 6, "layout44");

    layout43_2 = new QHBoxLayout( 0, 0, 6, "layout43_2");
    spacer41 = new QSpacerItem( 34, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout43_2->addItem( spacer41 );

    layout55_2 = new QHBoxLayout( 0, 0, 6, "layout55_2");

    textLabel1 = new QLabel( privateLayoutWidget_3, "textLabel1" );
    layout55_2->addWidget( textLabel1 );
    spacer32 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout55_2->addItem( spacer32 );

    comboExibicao1 = new QComboBox( FALSE, privateLayoutWidget_3, "comboExibicao1" );
    layout55_2->addWidget( comboExibicao1 );
    layout43_2->addLayout( layout55_2 );
    layout44->addLayout( layout43_2 );

    layout67 = new QHBoxLayout( 0, 0, 6, "layout67");
    spacer42 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout67->addItem( spacer42 );

    layout56 = new QHBoxLayout( 0, 0, 6, "layout56");

    labelCor = new QLabel( privateLayoutWidget_3, "labelCor" );
    layout56->addWidget( labelCor );
    spacer33 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout56->addItem( spacer33 );

    comboCores = new QComboBox( FALSE, privateLayoutWidget_3, "comboCores" );
    comboCores->setEditable( FALSE );
    layout56->addWidget( comboCores );
    layout67->addLayout( layout56 );
    layout44->addLayout( layout67 );

    layout123 = new QVBoxLayout( 0, 0, 6, "layout123");

    layout58 = new QHBoxLayout( 0, 0, 6, "layout58");

    labelHMin = new QLabel( privateLayoutWidget_3, "labelHMin" );
    layout58->addWidget( labelHMin );
    spacer34 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout58->addItem( spacer34 );

    layout106 = new QHBoxLayout( 0, 0, 6, "layout106");

    spinHGP_Hmin = new QSpinBox( privateLayoutWidget_3, "spinHGP_Hmin" );
    spinHGP_Hmin->setMaxValue( 180 );
    spinHGP_Hmin->setMinValue( -180 );
    layout106->addWidget( spinHGP_Hmin );

    sliderHGP_Hmin = new QSlider( privateLayoutWidget_3, "sliderHGP_Hmin" );
    sliderHGP_Hmin->setMinimumSize( QSize( 160, 0 ) );
    sliderHGP_Hmin->setMinValue( -180 );
    sliderHGP_Hmin->setMaxValue( 180 );
    sliderHGP_Hmin->setPageStep( 5 );
    sliderHGP_Hmin->setValue( -180 );
    sliderHGP_Hmin->setOrientation( QSlider::Horizontal );
    layout106->addWidget( sliderHGP_Hmin );
    layout58->addLayout( layout106 );
    layout123->addLayout( layout58 );

    layout60 = new QHBoxLayout( 0, 0, 6, "layout60");

    labelHMax = new QLabel( privateLayoutWidget_3, "labelHMax" );
    layout60->addWidget( labelHMax );
    spacer36 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout60->addItem( spacer36 );

    layout105 = new QHBoxLayout( 0, 0, 6, "layout105");

    spinHGP_Hmax = new QSpinBox( privateLayoutWidget_3, "spinHGP_Hmax" );
    spinHGP_Hmax->setMaxValue( 180 );
    spinHGP_Hmax->setMinValue( -180 );
    spinHGP_Hmax->setValue( 0 );
    layout105->addWidget( spinHGP_Hmax );

    sliderHGP_Hmax = new QSlider( privateLayoutWidget_3, "sliderHGP_Hmax" );
    sliderHGP_Hmax->setMinimumSize( QSize( 160, 0 ) );
    sliderHGP_Hmax->setMinValue( -180 );
    sliderHGP_Hmax->setMaxValue( 180 );
    sliderHGP_Hmax->setPageStep( 5 );
    sliderHGP_Hmax->setValue( 180 );
    sliderHGP_Hmax->setOrientation( QSlider::Horizontal );
    layout105->addWidget( sliderHGP_Hmax );
    layout60->addLayout( layout105 );
    layout123->addLayout( layout60 );

    layout122 = new QHBoxLayout( 0, 0, 6, "layout122");

    labelPMin = new QLabel( privateLayoutWidget_3, "labelPMin" );
    layout122->addWidget( labelPMin );
    spacer39 = new QSpacerItem( 60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout122->addItem( spacer39 );

    layout121 = new QHBoxLayout( 0, 0, 6, "layout121");

    spinHGP_Pmin = new QSpinBox( privateLayoutWidget_3, "spinHGP_Pmin" );
    spinHGP_Pmin->setMaxValue( 100 );
    layout121->addWidget( spinHGP_Pmin );

    sliderHGP_Pmin = new QSlider( privateLayoutWidget_3, "sliderHGP_Pmin" );
    sliderHGP_Pmin->setMinimumSize( QSize( 160, 0 ) );
    sliderHGP_Pmin->setMaxValue( 100 );
    sliderHGP_Pmin->setPageStep( 5 );
    sliderHGP_Pmin->setOrientation( QSlider::Horizontal );
    layout121->addWidget( sliderHGP_Pmin );
    layout122->addLayout( layout121 );
    layout123->addLayout( layout122 );

    layout64 = new QHBoxLayout( 0, 0, 6, "layout64");

    labelPMax = new QLabel( privateLayoutWidget_3, "labelPMax" );
    layout64->addWidget( labelPMax );
    spacer40 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout64->addItem( spacer40 );

    layout98 = new QHBoxLayout( 0, 0, 6, "layout98");

    spinHGP_Pmax = new QSpinBox( privateLayoutWidget_3, "spinHGP_Pmax" );
    spinHGP_Pmax->setMaxValue( 100 );
    spinHGP_Pmax->setValue( 0 );
    layout98->addWidget( spinHGP_Pmax );

    sliderHGP_Pmax = new QSlider( privateLayoutWidget_3, "sliderHGP_Pmax" );
    sliderHGP_Pmax->setMinimumSize( QSize( 160, 0 ) );
    sliderHGP_Pmax->setMaxValue( 100 );
    sliderHGP_Pmax->setPageStep( 5 );
    sliderHGP_Pmax->setValue( 0 );
    sliderHGP_Pmax->setOrientation( QSlider::Horizontal );
    layout98->addWidget( sliderHGP_Pmax );
    layout64->addLayout( layout98 );
    layout123->addLayout( layout64 );

    layout61 = new QHBoxLayout( 0, 0, 6, "layout61");

    labelGMin = new QLabel( privateLayoutWidget_3, "labelGMin" );
    layout61->addWidget( labelGMin );
    spacer37 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout61->addItem( spacer37 );

    layout104 = new QHBoxLayout( 0, 0, 6, "layout104");

    spinHGP_Gmin = new QSpinBox( privateLayoutWidget_3, "spinHGP_Gmin" );
    spinHGP_Gmin->setMaxValue( 100 );
    layout104->addWidget( spinHGP_Gmin );

    sliderHGP_Gmin = new QSlider( privateLayoutWidget_3, "sliderHGP_Gmin" );
    sliderHGP_Gmin->setMinimumSize( QSize( 160, 0 ) );
    sliderHGP_Gmin->setMaxValue( 100 );
    sliderHGP_Gmin->setPageStep( 5 );
    sliderHGP_Gmin->setOrientation( QSlider::Horizontal );
    layout104->addWidget( sliderHGP_Gmin );
    layout61->addLayout( layout104 );
    layout123->addLayout( layout61 );

    layout62 = new QHBoxLayout( 0, 0, 6, "layout62");

    labelGMax = new QLabel( privateLayoutWidget_3, "labelGMax" );
    layout62->addWidget( labelGMax );
    spacer38 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout62->addItem( spacer38 );

    layout103 = new QHBoxLayout( 0, 0, 6, "layout103");

    spinHGP_Gmax = new QSpinBox( privateLayoutWidget_3, "spinHGP_Gmax" );
    spinHGP_Gmax->setMaxValue( 100 );
    spinHGP_Gmax->setValue( 0 );
    layout103->addWidget( spinHGP_Gmax );

    sliderHGP_Gmax = new QSlider( privateLayoutWidget_3, "sliderHGP_Gmax" );
    sliderHGP_Gmax->setMinimumSize( QSize( 160, 0 ) );
    sliderHGP_Gmax->setMaxValue( 100 );
    sliderHGP_Gmax->setPageStep( 5 );
    sliderHGP_Gmax->setValue( 0 );
    sliderHGP_Gmax->setOrientation( QSlider::Horizontal );
    layout103->addWidget( sliderHGP_Gmax );
    layout62->addLayout( layout103 );
    layout123->addLayout( layout62 );
    layout44->addLayout( layout123 );
    spacer45 = new QSpacerItem( 20, 44, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout44->addItem( spacer45 );

    layout73 = new QHBoxLayout( 0, 0, 6, "layout73");

    layout69 = new QHBoxLayout( 0, 0, 6, "layout69");

    labelX2 = new QLabel( privateLayoutWidget_3, "labelX2" );
    layout69->addWidget( labelX2 );

    lcdX2 = new QLCDNumber( privateLayoutWidget_3, "lcdX2" );
    lcdX2->setMaximumSize( QSize( 45, 23 ) );
    lcdX2->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout69->addWidget( lcdX2 );
    layout73->addLayout( layout69 );
    spacer43 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout73->addItem( spacer43 );

    layout71 = new QHBoxLayout( 0, 0, 6, "layout71");

    labelRGB2 = new QLabel( privateLayoutWidget_3, "labelRGB2" );
    layout71->addWidget( labelRGB2 );

    lcdRGB_R2 = new QLCDNumber( privateLayoutWidget_3, "lcdRGB_R2" );
    lcdRGB_R2->setMaximumSize( QSize( 45, 23 ) );
    lcdRGB_R2->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout71->addWidget( lcdRGB_R2 );

    lcdRGB_G2 = new QLCDNumber( privateLayoutWidget_3, "lcdRGB_G2" );
    lcdRGB_G2->setMaximumSize( QSize( 45, 23 ) );
    lcdRGB_G2->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout71->addWidget( lcdRGB_G2 );

    lcdRGB_B2 = new QLCDNumber( privateLayoutWidget_3, "lcdRGB_B2" );
    lcdRGB_B2->setMaximumSize( QSize( 45, 23 ) );
    lcdRGB_B2->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout71->addWidget( lcdRGB_B2 );
    layout73->addLayout( layout71 );
    layout44->addLayout( layout73 );

    layout42 = new QHBoxLayout( 0, 0, 6, "layout42");

    layout70 = new QHBoxLayout( 0, 0, 6, "layout70");

    labelY2 = new QLabel( privateLayoutWidget_3, "labelY2" );
    layout70->addWidget( labelY2 );

    lcdY2 = new QLCDNumber( privateLayoutWidget_3, "lcdY2" );
    lcdY2->setMaximumSize( QSize( 45, 23 ) );
    lcdY2->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout70->addWidget( lcdY2 );
    layout42->addLayout( layout70 );
    spacer44 = new QSpacerItem( 41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout42->addItem( spacer44 );

    layout41 = new QHBoxLayout( 0, 0, 6, "layout41");

    labelHGP2 = new QLabel( privateLayoutWidget_3, "labelHGP2" );
    layout41->addWidget( labelHGP2 );

    lcdHGP_H2 = new QLCDNumber( privateLayoutWidget_3, "lcdHGP_H2" );
    lcdHGP_H2->setMaximumSize( QSize( 45, 23 ) );
    lcdHGP_H2->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout41->addWidget( lcdHGP_H2 );

    lcdHGP_P2 = new QLCDNumber( privateLayoutWidget_3, "lcdHGP_P2" );
    lcdHGP_P2->setMaximumSize( QSize( 45, 23 ) );
    lcdHGP_P2->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout41->addWidget( lcdHGP_P2 );

    lcdHGP_G2 = new QLCDNumber( privateLayoutWidget_3, "lcdHGP_G2" );
    lcdHGP_G2->setMaximumSize( QSize( 45, 23 ) );
    lcdHGP_G2->setPaletteBackgroundColor( QColor( 129, 129, 129 ) );
    layout41->addWidget( lcdHGP_G2 );
    layout42->addLayout( layout41 );
    layout44->addLayout( layout42 );
    spacer48 = new QSpacerItem( 20, 44, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout44->addItem( spacer48 );

    layout76 = new QHBoxLayout( 0, 0, 6, "layout76");

    pushVoltarTela2 = new QPushButton( privateLayoutWidget_3, "pushVoltarTela2" );
    layout76->addWidget( pushVoltarTela2 );

    pushFim = new QPushButton( privateLayoutWidget_3, "pushFim" );
    layout76->addWidget( pushFim );
    layout44->addLayout( layout76 );
    widgetStack1->addWidget( tela2, 1 );

    tela0 = new QWidget( widgetStack1, "tela0" );

    spinHue = new QSpinBox( tela0, "spinHue" );
    spinHue->setGeometry( QRect( 300, 146, 53, 25 ) );
    spinHue->setMaxValue( 0 );

    textLabel1_2_2_2_2_2 = new QLabel( tela0, "textLabel1_2_2_2_2_2" );
    textLabel1_2_2_2_2_2->setGeometry( QRect( 13, 177, 96, 25 ) );


    sliderGamma = new QSlider( tela0, "sliderGamma" );
    sliderGamma->setGeometry( QRect( 115, 212, 179, 16 ) );
    sliderGamma->setOrientation( QSlider::Horizontal );

    spinGamma = new QSpinBox( tela0, "spinGamma" );
    spinGamma->setGeometry( QRect( 300, 208, 53, 25 ) );


    sliderBrightness = new QSlider( tela0, "sliderBrightness" );
    sliderBrightness->setGeometry( QRect( 115, 88, 179, 16 ) );

    sliderBrightness->setOrientation( QSlider::Horizontal );

    spinBrightness = new QSpinBox( tela0, "spinBrightness" );
    spinBrightness->setGeometry( QRect( 300, 84, 53, 25 ) );

    sliderExposure = new QSlider( tela0, "sliderExposure" );
    sliderExposure->setGeometry( QRect( 150, 274, 144, 16 ) );
    sliderExposure->setOrientation( QSlider::Horizontal );

    spinContrast = new QSpinBox( tela0, "spinContrast" );
    spinContrast->setGeometry( QRect( 300, 115, 53, 25 ) );

    sliderContrast = new QSlider( tela0, "sliderContrast" );
    sliderContrast->setGeometry( QRect( 115, 119, 179, 16 ) );
    sliderContrast->setOrientation( QSlider::Horizontal );

    textLabel1_2_2_2_2_2_2 = new QLabel( tela0, "textLabel1_2_2_2_2_2_2" );
    textLabel1_2_2_2_2_2_2->setGeometry( QRect( 13, 208, 96, 25 ) );

    textLabel1_2_2_2_2 = new QLabel( tela0, "textLabel1_2_2_2_2" );
    textLabel1_2_2_2_2->setGeometry( QRect( 13, 146, 96, 25 ) );


    textLabel1_2 = new QLabel( tela0, "textLabel1_2" );
    textLabel1_2->setGeometry( QRect( 13, 84, 96, 25 ) );

    textLabel1_2_2_2_2_2_4 = new QLabel( tela0, "textLabel1_2_2_2_2_2_4" );
    textLabel1_2_2_2_2_2_4->setGeometry( QRect( 13, 270, 96, 25 ) );


    spinGain = new QSpinBox( tela0, "spinGain" );
    spinGain->setGeometry( QRect( 300, 270, 53, 25 ) );

    // Setado para zero pois nao é usado nesse tipo de camera
    sliderGain = new QSlider( tela0, "sliderGain" );
    sliderGain->setGeometry( QRect( 150, 274, 144, 16 ) );
    sliderGain->setOrientation( QSlider::Horizontal );


    sliderExposure = new QSlider( tela0, "sliderExposure" );
    //sliderGain->setGeometry( QRect( 115, 274, 179, 16 ) );
    sliderExposure->setGeometry( QRect( 150, 274, 144, 16 ) );
    sliderExposure->setOrientation( QSlider::Horizontal );


    sliderSaturation = new QSlider( tela0, "sliderSaturation" );
    sliderSaturation->setGeometry( QRect( 115, 181, 179, 16 ) );
    sliderSaturation->setOrientation( QSlider::Horizontal );

    spinSaturation = new QSpinBox( tela0, "spinSaturation" );
    spinSaturation->setGeometry( QRect( 300, 177, 53, 25 ) );

    sliderHue = new QSlider( tela0, "sliderHue" );
    sliderHue->setGeometry( QRect( 115, 150, 179, 16 ) );

    sliderHue->setOrientation( QSlider::Horizontal );

    spinShutter = new QSpinBox( tela0, "spinShutter" );
    spinShutter->setGeometry( QRect( 300, 239, 53, 25 ) );




    textLabel1_2_2 = new QLabel( tela0, "textLabel1_2_2" );
    textLabel1_2_2->setGeometry( QRect( 13, 115, 96, 25 ) );

    textLabel1_2_2_2_2_2_3 = new QLabel( tela0, "textLabel1_2_2_2_2_2_3" );
    textLabel1_2_2_2_2_2_3->setGeometry( QRect( 13, 239, 96, 25 ) );

    textLabel3 = new QLabel( tela0, "textLabel3" );
    textLabel3->setGeometry( QRect( 11, 11, 340, 50 ) );

    sliderShutter = new QSlider( tela0, "sliderShutter" );
    sliderShutter->setGeometry( QRect( 115, 243, 179, 16 ) );
    sliderShutter->setMaxValue( 0 );
    sliderShutter->setOrientation( QSlider::Horizontal );

    pushAvancarTela0 = new QPushButton( tela0, "pushAvancarTela0" );
    pushAvancarTela0->setGeometry( QRect( 187, 465, 160, 30 ) );
    widgetStack1->addWidget( tela0, 2 );
    layout55->addWidget( widgetStack1 );

    // actions
    fileNewAction = new QAction( this, "fileNewAction" );
    fileNewAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "filenew" ) ) );
    fileOpenAction = new QAction( this, "fileOpenAction" );
    fileOpenAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "fileopen" ) ) );
    fileSaveAction = new QAction( this, "fileSaveAction" );
    fileSaveAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "filesave" ) ) );
    fileSaveAsAction = new QAction( this, "fileSaveAsAction" );
    fileExitAction = new QAction( this, "fileExitAction" );
    imageLoadAction = new QAction( this, "imageLoadAction" );
    imageSnapshotAction = new QAction( this, "imageSnapshotAction" );
    imageSaveAction = new QAction( this, "imageSaveAction" );
    imageSaveAction->setEnabled( TRUE );
    screenActionGroup = new QActionGroup( this, "screenActionGroup" );
    screenActionGroup->setOn( FALSE );
    screenActionGroup->setUsesDropDown( FALSE );
    screenSelectOneAction = new QAction( screenActionGroup, "screenSelectOneAction" );
    screenSelectOneAction->setToggleAction( TRUE );
    screenSelectOneAction->setOn( TRUE );
    screenSelectTwoAction = new QAction( screenActionGroup, "screenSelectTwoAction" );
    screenSelectTwoAction->setToggleAction( TRUE );
    screenSelectTwoAction->setEnabled( FALSE );
    cameraLoad_ParametersAction = new QAction( this, "cameraLoad_ParametersAction" );
    cameraSave_ParametersAction = new QAction( this, "cameraSave_ParametersAction" );
    cameraSave_Parameters_asAction = new QAction( this, "cameraSave_Parameters_asAction" );
    cameraNewAction = new QAction( this, "cameraNewAction" );


    // toolbars


    // menubar
    MenuBar = new QMenuBar( this, "MenuBar" );


    File = new QPopupMenu( this );
    fileNewAction->addTo( File );
    fileOpenAction->addTo( File );
    fileSaveAction->addTo( File );
    fileSaveAsAction->addTo( File );
    File->insertSeparator();
    fileExitAction->addTo( File );
    MenuBar->insertItem( QString(""), File, 1 );

    Image = new QPopupMenu( this );
    imageSaveAction->addTo( Image );
    imageLoadAction->addTo( Image );
    Image->insertSeparator();
    imageSnapshotAction->addTo( Image );
    MenuBar->insertItem( QString(""), Image, 2 );

    Camera = new QPopupMenu( this );
    cameraNewAction->addTo( Camera );
    cameraLoad_ParametersAction->addTo( Camera );
    cameraSave_ParametersAction->addTo( Camera );
    cameraSave_Parameters_asAction->addTo( Camera );
    MenuBar->insertItem( QString(""), Camera, 3 );

    languageChange();
    resize( QSize(1056, 587).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( cameraLoad_ParametersAction, SIGNAL( activated() ), this, SLOT( cameraLoadParam() ) );
    connect( cameraNewAction, SIGNAL( activated() ), this, SLOT( cameraNew() ) );
    connect( cameraSave_ParametersAction, SIGNAL( activated() ), this, SLOT( cameraSaveParam() ) );
    connect( cameraSave_Parameters_asAction, SIGNAL( activated() ), this, SLOT( cameraSaveAsParam() ) );
    connect( checkExibirGrade, SIGNAL( stateChanged(int) ), this, SLOT( setarModo() ) );
    connect( checkExibirGrade, SIGNAL( clicked() ), this, SLOT( redesenharImagem() ) );
    connect( checkExibirImagemProcessada, SIGNAL( stateChanged(int) ), this, SLOT( setarModo() ) );
    connect( checkExibirImagemProcessada, SIGNAL( clicked() ), this, SLOT( redesenharImagem() ) );
    connect( checkShooting, SIGNAL( toggled(bool) ), this, SLOT( ShootingChanged(bool) ) );
    connect( comboCores, SIGNAL( activated(int) ), this, SLOT( redesenharImagem() ) );
    connect( comboCores, SIGNAL( activated(int) ), this, SLOT( atualizarLimitesHGP(int) ) );
    connect( comboExibicao1, SIGNAL( activated(int) ), this, SLOT( setarModo() ) );
    connect( comboExibicao1, SIGNAL( activated(int) ), this, SLOT( redesenharImagem() ) );
    connect( fileExitAction, SIGNAL( activated() ), this, SLOT( fileExit() ) );
    connect( fileNewAction, SIGNAL( activated() ), this, SLOT( redesenharImagem() ) );
    connect( fileNewAction, SIGNAL( activated() ), this, SLOT( fileNew() ) );
    connect( fileOpenAction, SIGNAL( activated() ), this, SLOT( redesenharImagem() ) );
    connect( fileOpenAction, SIGNAL( activated() ), this, SLOT( fileOpen() ) );
    connect( fileSaveAction, SIGNAL( activated() ), this, SLOT( fileSave() ) );
    connect( fileSaveAsAction, SIGNAL( activated() ), this, SLOT( fileSaveAs() ) );
    connect( imageLoadAction, SIGNAL( activated() ), this, SLOT( redesenharImagem() ) );
    connect( imageLoadAction, SIGNAL( activated() ), this, SLOT( imageOpen() ) );
    connect( imageSaveAction, SIGNAL( activated() ), this, SLOT( imageSave() ) );
    connect( pixmap_label1, SIGNAL( mouseReleased(QPoint) ), this, SLOT( mouseRelease(QPoint) ) );
    connect( pixmap_label1, SIGNAL( mousePressed(QPoint) ), this, SLOT( mousePress(QPoint) ) );
    connect( pixmap_label1, SIGNAL( mouseMoved(QPoint) ), this, SLOT( mouseMove(QPoint) ) );
    connect( pushAvancarTela0, SIGNAL( clicked() ), this, SLOT( redesenharImagem() ) );
    connect( pushAvancarTela0, SIGNAL( clicked() ), this, SLOT( mostrarTela1() ) );
    connect( pushAvancarTela1, SIGNAL( clicked() ), this, SLOT( redesenharImagem() ) );
    connect( pushAvancarTela1, SIGNAL( clicked() ), this, SLOT( mostrarTela2() ) );
    connect( pushFim, SIGNAL( clicked() ), this, SLOT( fileExit() ) );
    connect( pushVoltarTela1, SIGNAL( clicked() ), this, SLOT( redesenharImagem() ) );
    connect( pushVoltarTela1, SIGNAL( clicked() ), this, SLOT( mostrarTela0() ) );
    connect( pushVoltarTela2, SIGNAL( clicked() ), this, SLOT( redesenharImagem() ) );
    connect( pushVoltarTela2, SIGNAL( clicked() ), this, SLOT( mostrarTela1() ) );
    connect( sliderBrightness, SIGNAL( valueChanged(int) ), this, SLOT( BrightnessValueChanged(int) ) );
    connect( sliderContrast, SIGNAL( valueChanged(int) ), this, SLOT( ExposureValueChanged(int) ) );
    connect( sliderGain, SIGNAL( valueChanged(int) ), this, SLOT( GainValueChanged(int) ) );
    connect( sliderGamma, SIGNAL( valueChanged(int) ), this, SLOT( GammaValueChanged(int) ) );
    connect( sliderHGP_Gmax, SIGNAL( valueChanged(int) ), this, SLOT( GMaxValueChanged(int) ) );
    connect( sliderHGP_Gmin, SIGNAL( valueChanged(int) ), this, SLOT( GMinValueChanged(int) ) );
    connect( sliderHGP_Hmax, SIGNAL( valueChanged(int) ), this, SLOT( HMaxValueChanged(int) ) );
    connect( sliderHGP_Hmin, SIGNAL( valueChanged(int) ), this, SLOT( HMinValueChanged(int) ) );
    connect( sliderHGP_Pmax, SIGNAL( valueChanged(int) ), this, SLOT( PMaxValueChanged(int) ) );
    connect( sliderHGP_Pmin, SIGNAL( valueChanged(int) ), this, SLOT( PMinValueChanged(int) ) );
    connect( sliderHue, SIGNAL( valueChanged(int) ), this, SLOT( HueValueChanged(int) ) );
    connect( sliderLimiarPInf, SIGNAL( valueChanged(int) ), this, SLOT( sliderLimiarPInfValueChanged(int) ) );
    connect( sliderLimiarPSup, SIGNAL( valueChanged(int) ), this, SLOT( sliderLimiarPSupValueChanged(int) ) );
    connect( sliderSaturation, SIGNAL( valueChanged(int) ), this, SLOT( SaturationValueChanged(int) ) );
    connect( sliderShutter, SIGNAL( valueChanged(int) ), this, SLOT( ShutterValueChanged(int) ) );
    connect( sliderUOffset, SIGNAL( valueChanged(int) ), this, SLOT( redesenharImagem() ) );
    connect( sliderUOffset, SIGNAL( valueChanged(int) ), this, SLOT( changeUOffset(int) ) );
    connect( sliderVOffset, SIGNAL( valueChanged(int) ), this, SLOT( redesenharImagem() ) );
    connect( sliderVOffset, SIGNAL( valueChanged(int) ), this, SLOT( changeVOffset(int) ) );
    connect( spinBrightness, SIGNAL( valueChanged(int) ), this, SLOT( atualizarCamera() ) );
    connect( spinBrightness, SIGNAL( valueChanged(int) ), this, SLOT( BrightnessValueChanged(int) ) );
    connect( spinExposure, SIGNAL( valueChanged(int) ), this, SLOT( atualizarCamera() ) );
    connect( spinExposure, SIGNAL( valueChanged(int) ), this, SLOT( ExposureValueChanged(int) ) );
    connect( spinGain, SIGNAL( valueChanged(int) ), this, SLOT( atualizarCamera() ) );
    connect( spinGain, SIGNAL( valueChanged(int) ), this, SLOT( GainValueChanged(int) ) );
    connect( spinGamma, SIGNAL( valueChanged(int) ), this, SLOT( atualizarCamera() ) );
    connect( spinGamma, SIGNAL( valueChanged(int) ), this, SLOT( GammaValueChanged(int) ) );
    connect( spinHGP_Gmax, SIGNAL( valueChanged(int) ), this, SLOT( redesenharImagem() ) );
    connect( spinHGP_Gmax, SIGNAL( valueChanged(int) ), this, SLOT( GMaxValueChanged(int) ) );
    connect( spinHGP_Gmin, SIGNAL( valueChanged(int) ), this, SLOT( redesenharImagem() ) );
    connect( spinHGP_Gmin, SIGNAL( valueChanged(int) ), this, SLOT( GMinValueChanged(int) ) );
    connect( spinHGP_Hmax, SIGNAL( valueChanged(int) ), this, SLOT( redesenharImagem() ) );
    connect( spinHGP_Hmax, SIGNAL( valueChanged(int) ), this, SLOT( HMaxValueChanged(int) ) );
    connect( spinHGP_Hmin, SIGNAL( valueChanged(int) ), this, SLOT( redesenharImagem() ) );
    connect( spinHGP_Hmin, SIGNAL( valueChanged(int) ), this, SLOT( HMinValueChanged(int) ) );
    connect( spinHGP_Pmax, SIGNAL( valueChanged(int) ), this, SLOT( redesenharImagem() ) );
    connect( spinHGP_Pmax, SIGNAL( valueChanged(int) ), this, SLOT( PMaxValueChanged(int) ) );
    connect( spinHGP_Pmin, SIGNAL( valueChanged(int) ), this, SLOT( redesenharImagem() ) );
    connect( spinHGP_Pmin, SIGNAL( valueChanged(int) ), this, SLOT( PMinValueChanged(int) ) );
    connect( spinHue, SIGNAL( valueChanged(int) ), this, SLOT( atualizarCamera() ) );
    connect( spinHue, SIGNAL( valueChanged(int) ), this, SLOT( HueValueChanged(int) ) );
    connect( spinLimiarPInf, SIGNAL( valueChanged(int) ), this, SLOT( spinLimiarPInfValueChanged(int) ) );
    connect( spinLimiarPInf, SIGNAL( valueChanged(int) ), this, SLOT( redesenharImagem() ) );
    connect( spinLimiarPSup, SIGNAL( valueChanged(int) ), this, SLOT( spinLimiarPSupValueChanged(int) ) );
    connect( spinLimiarPSup, SIGNAL( valueChanged(int) ), this, SLOT( redesenharImagem() ) );
    connect( spinSaturation, SIGNAL( valueChanged(int) ), this, SLOT( atualizarCamera() ) );
    connect( spinSaturation, SIGNAL( valueChanged(int) ), this, SLOT( SaturationValueChanged(int) ) );
    connect( spinShutter, SIGNAL( valueChanged(int) ), this, SLOT( atualizarCamera() ) );
    connect( spinShutter, SIGNAL( valueChanged(int) ), this, SLOT( ShutterValueChanged(int) ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
calibrador::~calibrador()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void calibrador::languageChange()
{
    setCaption( tr( "Calibrator" ) );
    checkShooting->setText( tr( "Shooting" ) );
    labelRGB->setText( tr( "RGB (%):" ) );
    labelHGP->setText( tr( "HPG (%):" ) );
    labelX->setText( tr( "X:" ) );
    labelY->setText( tr( "Y:" ) );
    checkExibirGrade->setText( tr( "View Field Lines" ) );
    checkExibirImagemProcessada->setText( tr( "View Process Image" ) );
    labelLimiarPInf->setText( tr( "Lower P Threshold:" ) );
    labelLimiarPSup->setText( tr( "Upper P Threshold:" ) );
    pushVoltarTela1->setText( tr( "Back" ) );
    pushAvancarTela1->setText( tr( "Next" ) );
    textLabel1->setText( tr( "View Mode:" ) );
    comboExibicao1->clear();
    comboExibicao1->insertItem( tr( "Real Image" ) );
    comboExibicao1->insertItem( tr( "Current Color Image" ) );
    comboExibicao1->insertItem( tr( "Tagged Image" ) );
    comboExibicao1->insertItem( tr( "Error Image" ) );
    labelCor->setText( tr( "Color:" ) );
    labelHMin->setText( tr( "H Min:" ) );
    labelHMax->setText( tr( "H Max:" ) );
    labelPMin->setText( tr( "P Min:" ) );
    labelPMax->setText( tr( "P Max:" ) );
    labelGMin->setText( tr( "G Min:" ) );
    labelGMax->setText( tr( "G Max:" ) );
    labelX2->setText( tr( "X:" ) );
    labelRGB2->setText( tr( "RGB (%):" ) );
    labelY2->setText( tr( "Y:" ) );
    labelHGP2->setText( tr( "HPG (%):" ) );
    pushVoltarTela2->setText( tr( "Back" ) );
    pushFim->setText( tr( "Finish" ) );
    textLabel1_2_2_2_2_2->setText( tr( "Saturation" ) );
    textLabel1_2_2_2_2_2_2->setText( tr( "Gamma" ) );
    textLabel1_2_2_2_2->setText( tr( "Hue" ) );
    textLabel1_2->setText( tr( "Brightness" ) );
    textLabel1_2_2_2_2_2_4->setText( tr( "Gain" ) );
    textLabel1_2_2->setText( tr( "Contrast" ) );
    textLabel1_2_2_2_2_2_3->setText( tr( "Shutter" ) );
    textLabel3->setText( tr( "<b><font size=\"+2\">Parametros da Camera</font></b>" ) );
    pushAvancarTela0->setText( tr( "Proximo" ) );
    fileNewAction->setText( tr( "New" ) );
    fileNewAction->setMenuText( tr( "&New" ) );
    fileNewAction->setAccel( tr( "Ctrl+N" ) );
    fileOpenAction->setText( tr( "Open" ) );
    fileOpenAction->setMenuText( tr( "&Open" ) );
    fileOpenAction->setAccel( tr( "Ctrl+O" ) );
    fileSaveAction->setText( tr( "Save" ) );
    fileSaveAction->setMenuText( tr( "&Save" ) );
    fileSaveAction->setAccel( tr( "Ctrl+S" ) );
    fileSaveAsAction->setText( tr( "Save As ..." ) );
    fileSaveAsAction->setMenuText( tr( "Save As ..." ) );
    fileSaveAsAction->setAccel( QString::null );
    fileExitAction->setText( tr( "Close" ) );
    fileExitAction->setMenuText( tr( "&Close" ) );
    fileExitAction->setAccel( QString::null );
    imageLoadAction->setText( tr( "Load" ) );
    imageLoadAction->setStatusTip( tr( "Carrega imagem do arquivo." ) );
    imageSnapshotAction->setText( tr( "Snapshot" ) );
    imageSnapshotAction->setStatusTip( trUtf8( "\x43\x61\x72\x72\x65\x67\x61\x20\x69\x6d\x61\x67\x65\x6d\x20\x64\x61\x20\x63\xc3\xa2"
    "\x6d\x65\x72\x61\x2e" ) );
    imageSaveAction->setText( tr( "Save" ) );
    imageSaveAction->setStatusTip( tr( "Salva imagem em arquivo." ) );
    screenActionGroup->setText( tr( "ActionGroup" ) );
    screenSelectOneAction->setText( tr( "Tela 1" ) );
    screenSelectOneAction->setStatusTip( trUtf8( "\x45\x78\x69\x62\x65\x20\x61\x20\x70\x72\x69\x6d\x65\x69\x72\x61\x20\x74\x65\x6c\x61"
    "\x20\x64\x65\x20\x63\x61\x6c\x69\x62\x72\x61\xc3\xa7\xc3\xa3\x6f\x2e" ) );
    screenSelectTwoAction->setText( tr( "Tela 2" ) );
    screenSelectTwoAction->setStatusTip( trUtf8( "\x45\x78\x69\x62\x65\x20\x61\x20\x73\x65\x67\x75\x6e\x64\x61\x20\x74\x65\x6c\x61\x20"
    "\x64\x65\x20\x63\x61\x6c\x69\x62\x72\x61\xc3\xa7\xc3\xa3\x6f\x2e" ) );
    cameraLoad_ParametersAction->setText( tr( "Load Parameters" ) );
    cameraLoad_ParametersAction->setMenuText( tr( "Load Parameters" ) );
    cameraSave_ParametersAction->setText( tr( "Save Parameters" ) );
    cameraSave_ParametersAction->setMenuText( tr( "Save Parameters" ) );
    cameraSave_Parameters_asAction->setText( tr( "Save Parameters As..." ) );
    cameraSave_Parameters_asAction->setMenuText( tr( "Save Parameters As..." ) );
    cameraNewAction->setText( tr( "New" ) );
    cameraNewAction->setMenuText( tr( "New" ) );
    if (MenuBar->findItem(1))
        MenuBar->findItem(1)->setText( tr( "File" ) );
    if (MenuBar->findItem(2))
        MenuBar->findItem(2)->setText( tr( "Image" ) );
    if (MenuBar->findItem(3))
        MenuBar->findItem(3)->setText( tr( "Camera" ) );
}
