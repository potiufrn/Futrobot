#include "calibrador.h"
#include "ui_calibrador.h"
#include "pixmap_label.h"

#include <qimage.h>
#include <qpixmap.h>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QRect>
#include <QSizePolicy>
#include <QPoint>

#include <iostream>

using namespace std;

calibrador::calibrador(QWidget *parent, /* const char* name,*/ Qt::WindowFlags fl)
    : QMainWindow(parent, fl), ui(new Ui::calibrador)
{

    //Perguntar sobre o indice da camera aqui

    // chama a interface
    ui->setupUi(this);

    ui->verticalLayout->setGeometry(QRect(30, 20, 640, 480));
    pixmap_label1 = new pixmap_label();
    pixmap_label1->setMinimumSize(QSize(640, 480));
    pixmap_label1->setMaximumSize(QSize(640, 480));
    pixmap_label1->setMouseTracking(true);
    ui->verticalLayout->addWidget(pixmap_label1);

    // inicia a pilha sempre no indice 0
    ui->stackedWidget->setCurrentIndex(0);
    //habilita o lançamento de imagens na tela
    ui->check_shooting->setChecked(true);
    // linhas visiveis
    ui->check_viewlines->setChecked(true);

    // seta os limites inferiores e superiores
    ui->sliderLimiarPInf->setRange(PG_MIN_VALUE, PG_MAX_VALUE - 1);
    ui->sliderLimiarPSup->setRange(PG_MIN_VALUE + 1, PG_MAX_VALUE);
    ui->sliderLimiarPInf->setValue(PG_MIN_VALUE);
    ui->sliderLimiarPSup->setValue(PG_MAX_VALUE);

    ui->spinLimiarPInf->setRange(PG_MIN_VALUE, PG_MAX_VALUE - 1);
    ui->spinLimiarPSup->setRange(PG_MIN_VALUE + 1, PG_MAX_VALUE);
    ui->spinLimiarPInf->setValue(PG_MIN_VALUE);
    ui->spinLimiarPSup->setValue(PG_MAX_VALUE);

    // seta os limites dos componentes HPG
    ui->slider_hmin->setRange(H_MIN_VALUE, H_MAX_VALUE);
    ui->slider_hmax->setRange(H_MIN_VALUE, H_MAX_VALUE);
    ui->slider_pmin->setRange(PG_MIN_VALUE, PG_MAX_VALUE);
    ui->slider_pmax->setRange(PG_MIN_VALUE, PG_MAX_VALUE);
    ui->slider_gmin->setRange(PG_MIN_VALUE, PG_MAX_VALUE);
    ui->slider_gmax->setRange(PG_MIN_VALUE, PG_MAX_VALUE);

    ui->spin_hmin->setRange(H_MIN_VALUE, H_MAX_VALUE);
    ui->spin_hmax->setRange(H_MIN_VALUE, H_MAX_VALUE);
    ui->spin_pmin->setRange(PG_MIN_VALUE, PG_MAX_VALUE);
    ui->spin_pmax->setRange(PG_MIN_VALUE, PG_MAX_VALUE);
    ui->spin_gmin->setRange(PG_MIN_VALUE, PG_MAX_VALUE);
    ui->spin_gmax->setRange(PG_MIN_VALUE, PG_MAX_VALUE);

    ui->combo_viewmode->clear();
    ui->combo_viewmode->addItem("Real Image");
    ui->combo_viewmode->addItem("Current Color Image");
    ui->combo_viewmode->addItem("Tagged Image");
    ui->combo_viewmode->addItem("Error Image");

    connect(ui->bt_next_0, SIGNAL(clicked()), this, SLOT(redesenharImagem()));
    connect(ui->bt_next_0, SIGNAL(clicked()), this, SLOT(showPage1()));

    connect(ui->bt_back_1, SIGNAL(clicked()), this, SLOT(redesenharImagem()));
    connect(ui->bt_back_1, SIGNAL(clicked()), this, SLOT(showPage0()));

    connect(ui->bt_next_1, SIGNAL(clicked()), this, SLOT(redesenharImagem()));
    connect(ui->bt_next_1, SIGNAL(clicked()), this, SLOT(showPage2()));

    connect(ui->bt_back_2, SIGNAL(clicked()), this, SLOT(redesenharImagem()));
    connect(ui->bt_back_2, SIGNAL(clicked()), this, SLOT(showPage1()));
    connect(ui->bt_finish, SIGNAL(clicked()), this, SLOT(fileExit()));

    connect(ui->actionCamera_loadParameters, SIGNAL(triggered()), this, SLOT(cameraLoadParam()));
    connect(ui->actionCamera_new, SIGNAL(triggered()), this, SLOT(cameraNew()));
    connect(ui->actionCamera_saveParameters, SIGNAL(triggered()), this, SLOT(cameraSaveParam()));
    connect(ui->actionCamera_saveParametersAs, SIGNAL(triggered()), this, SLOT(cameraSaveAsParam()));

    connect(ui->check_viewlines, SIGNAL(stateChanged(int)), this, SLOT(setMode()));
    connect(ui->check_viewlines, SIGNAL(clicked()), this, SLOT(redesenharImagem()));

    connect(ui->check_viewimage, SIGNAL(stateChanged(int)), this, SLOT(setMode()));
    connect(ui->check_viewimage, SIGNAL(clicked()), this, SLOT(redesenharImagem()));

    connect(ui->check_shooting, SIGNAL(toggled(bool)), this, SLOT(shootingChanged(bool)));

    connect(ui->combo_color, SIGNAL(activated(int)), this, SLOT(redesenharImagem()));
    connect(ui->combo_color, SIGNAL(activated(int)), this, SLOT(atualizarLimitesHGP(int)));

    connect(ui->combo_viewmode, SIGNAL(activated(int)), this, SLOT(setMode()));
    connect(ui->combo_viewmode, SIGNAL(activated(int)), this, SLOT(redesenharImagem()));

    connect(ui->actionFile_new, SIGNAL(triggered()), this, SLOT(redesenharImagem()));
    connect(ui->actionFile_new, SIGNAL(triggered()), this, SLOT(fileNew()));
    connect(ui->actionFile_open, SIGNAL(triggered()), this, SLOT(redesenharImagem()));
    connect(ui->actionFile_open, SIGNAL(triggered()), this, SLOT(fileOpen()));
    connect(ui->actionFile_save, SIGNAL(triggered()), this, SLOT(fileSave()));
    connect(ui->actionFile_saveAs, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    connect(ui->actionFile_exit, SIGNAL(triggered()), this, SLOT(fileExit()));

    connect(ui->actionImage_load, SIGNAL(triggered()), this, SLOT(redesenharImagem()));
    connect(ui->actionImage_load, SIGNAL(triggered()), this, SLOT(imageOpen()));
    connect(ui->actionImage_save, SIGNAL(triggered()), this, SLOT(imageSave()));

    connect(pixmap_label1, SIGNAL(mouseReleased(QPoint)), this, SLOT(mouseRelease(QPoint)));
    connect(pixmap_label1, SIGNAL(mousePressed(QPoint)), this, SLOT(mousePress(QPoint)));
    connect(pixmap_label1, SIGNAL(mouseMoved(QPoint)), this, SLOT(mouseMove(QPoint)));

    connect(ui->slider_brightness, SIGNAL(valueChanged(int)), this, SLOT(BrightnessValueChanged(int)));
    connect(ui->slider_contrast, SIGNAL(valueChanged(int)), this, SLOT(ContrastValueChanged(int)));
    connect(ui->slider_gain, SIGNAL(valueChanged(int)), this, SLOT(GainValueChanged(int)));
    connect(ui->slider_gamma, SIGNAL(valueChanged(int)), this, SLOT(GammaValueChanged(int)));
    connect(ui->slider_hue, SIGNAL(valueChanged(int)), this, SLOT(HueValueChanged(int)));
    connect(ui->slider_saturation, SIGNAL(valueChanged(int)), this, SLOT(SaturationValueChanged(int)));
    connect(ui->slider_exposureAbs, SIGNAL(valueChanged(int)), this, SLOT(ExposureAbsValueChanged(int)));

    connect(ui->sliderLimiarPInf, SIGNAL(valueChanged(int)), this, SLOT(sliderLimiarPInfValueChanged(int)));
    connect(ui->sliderLimiarPSup, SIGNAL(valueChanged(int)), this, SLOT(sliderLimiarPSupValueChanged(int)));

    //    connect( sliderUOffset, SIGNAL( valueChanged(int) ), this, SLOT( redesenharImagem() ) );
    //    connect( sliderUOffset, SIGNAL( valueChanged(int) ), this, SLOT( changeUOffset(int) ) );
    //    connect( sliderVOffset, SIGNAL( valueChanged(int) ), this, SLOT( redesenharImagem() ) );
    //    connect( sliderVOffset, SIGNAL( valueChanged(int) ), this, SLOT( changeVOffset(int) ) );

    connect(ui->spin_brightness, SIGNAL(valueChanged(int)), this, SLOT(atualizarCamera()));
    connect(ui->spin_brightness, SIGNAL(valueChanged(int)), this, SLOT(BrightnessValueChanged(int)));

    //        // implementar
    //    connect(ui->spin_exposure, SIGNAL( valueChanged(int) ), this, SLOT( atualizarCamera() ) );
    //    connect(ui->spin_exposure, SIGNAL( valueChanged(int) ), this, SLOT( ExposureValueChanged(int) ) );

    connect(ui->spin_gain, SIGNAL(valueChanged(int)), this, SLOT(atualizarCamera()));
    connect(ui->spin_gain, SIGNAL(valueChanged(int)), this, SLOT(GainValueChanged(int)));
    connect(ui->spin_gamma, SIGNAL(valueChanged(int)), this, SLOT(atualizarCamera()));
    connect(ui->spin_gamma, SIGNAL(valueChanged(int)), this, SLOT(GammaValueChanged(int)));

    connect(ui->spin_hue, SIGNAL(valueChanged(int)), this, SLOT(atualizarCamera()));
    connect(ui->spin_hue, SIGNAL(valueChanged(int)), this, SLOT(HueValueChanged(int)));
    connect(ui->spin_saturation, SIGNAL(valueChanged(int)), this, SLOT(atualizarCamera()));
    connect(ui->spin_saturation, SIGNAL(valueChanged(int)), this, SLOT(SaturationValueChanged(int)));
    connect(ui->spin_exposureAbs, SIGNAL(valueChanged(int)), this, SLOT(atualizarCamera()));
    connect(ui->spin_exposureAbs, SIGNAL(valueChanged(int)), this, SLOT(ExposureAbsValueChanged(int)));

    connect(ui->spinLimiarPInf, SIGNAL(valueChanged(int)), this, SLOT(spinLimiarPInfValueChanged(int)));
    connect(ui->spinLimiarPInf, SIGNAL(valueChanged(int)), this, SLOT(redesenharImagem()));
    connect(ui->spinLimiarPSup, SIGNAL(valueChanged(int)), this, SLOT(spinLimiarPSupValueChanged(int)));
    connect(ui->spinLimiarPSup, SIGNAL(valueChanged(int)), this, SLOT(redesenharImagem()));

    connect(ui->slider_gmax, SIGNAL(valueChanged(int)), this, SLOT(GMaxValueChanged(int)));
    connect(ui->slider_gmin, SIGNAL(valueChanged(int)), this, SLOT(GMinValueChanged(int)));
    connect(ui->slider_hmax, SIGNAL(valueChanged(int)), this, SLOT(HMaxValueChanged(int)));
    connect(ui->slider_hmin, SIGNAL(valueChanged(int)), this, SLOT(HMinValueChanged(int)));
    connect(ui->slider_pmax, SIGNAL(valueChanged(int)), this, SLOT(PMaxValueChanged(int)));
    connect(ui->slider_pmin, SIGNAL(valueChanged(int)), this, SLOT(PMinValueChanged(int)));

    connect(ui->spin_gmax, SIGNAL(valueChanged(int)), this, SLOT(redesenharImagem()));
    connect(ui->spin_gmax, SIGNAL(valueChanged(int)), this, SLOT(GMaxValueChanged(int)));
    connect(ui->spin_gmin, SIGNAL(valueChanged(int)), this, SLOT(redesenharImagem()));
    connect(ui->spin_gmin, SIGNAL(valueChanged(int)), this, SLOT(GMinValueChanged(int)));
    connect(ui->spin_hmax, SIGNAL(valueChanged(int)), this, SLOT(redesenharImagem()));
    connect(ui->spin_hmax, SIGNAL(valueChanged(int)), this, SLOT(HMaxValueChanged(int)));
    connect(ui->spin_hmin, SIGNAL(valueChanged(int)), this, SLOT(redesenharImagem()));
    connect(ui->spin_hmin, SIGNAL(valueChanged(int)), this, SLOT(HMinValueChanged(int)));
    connect(ui->spin_pmax, SIGNAL(valueChanged(int)), this, SLOT(redesenharImagem()));
    connect(ui->spin_pmax, SIGNAL(valueChanged(int)), this, SLOT(PMaxValueChanged(int)));
    connect(ui->spin_pmin, SIGNAL(valueChanged(int)), this, SLOT(redesenharImagem()));
    connect(ui->spin_pmin, SIGNAL(valueChanged(int)), this, SLOT(PMinValueChanged(int)));

    init();
}

calibrador::~calibrador()
{
    destroy();
    delete ui;
}

void calibrador::init()
{
    if (carregaInterface())
    {
        cerr << "Erro na leitura do arquivo de Interface" << endl;
        exit(1);
    }

    //inicializa posição do mouse.
    MouseX = 0;
    MouseY = 0;

    //inicializacoes do mouse
    ponto_dragged = false;
    ponto_selecionado = -1;

    //inicializacao das flags para redesenhar a imagem
    novosLimites = true;
    novosParametros = true;

    // inicializarComValoresPadroes();
    arquivo = QString();
    arquivo_cameraParam = QString();

    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(novosParametrosCamera()));
    connect(Timer, SIGNAL(timeout()), this, SLOT(processarImagem()));
    Timer->start(60);
}

bool calibrador::carregaInterface()
{
    //gambiarra, nunca vai retornar true para dizer se vai dar erro.

    QString ColorName;

    for (unsigned i = 0; i < X.getNumCores(); i++)
    {
        ColorName = X.getNomeCor(i);
        //cout << ColorName.toStdString() << endl;
        ui->combo_color->addItem(ColorName);
    }

    // Voltar aqui
    //    sliderUOffset->setMaxValue(X.getCamWidth() - (pixmap_label1->size()).width());
    //    sliderVOffset->setMaxValue(X.getCamHeight() - (pixmap_label1->size()).height());

    atualizarLimitesHGP(ui->combo_color->currentIndex());

    atualizarCameraParam();
    return false;
}

void calibrador::destroy()
{
    delete Timer;
    delete pixmap_label1;
}

void calibrador::setCamera()
{
    // Onde colocamos a camera no combobox
    cout << "setCamera()" << endl;
}

void calibrador::showPage0()
{
    ui->stackedWidget->setCurrentIndex(0);
    setMode();
}

//TODO: Ajustar a captura do campo vazio
void calibrador::showPage1()
{
    ui->stackedWidget->setCurrentIndex(1);
    setMode();

    // Captura do campo vazio
    QString infoText;
    QMessageBox msgBox;

    if (!X.campoVazioCapturado())
    {
        infoText = QString("Operacao obrigatoria, certifique-se de que o programa ira capturar a imagem do campo vazio.\n \n\
      Deseja realizar a captura ?");
    }
    else
    {
        infoText = QString("Deseja realizar uma nova captura do campo Vazio ?");
        // setarModo();
        // return;
    }

    msgBox.setText("Captura do Campo Vazio");
    msgBox.setInformativeText(infoText);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes)
    {
        X.calImgMedia(NUM_AMOSTRAS);
        QMessageBox msg;
        msg.setText("Captura da imagem media realizada");
        msg.exec();
    }

    if (X.campoVazioCapturado())
    {
        ui->bt_next_1->setEnabled(true);
        ui->bt_next_1->setText("Next");
        ui->check_viewimage->setEnabled(true);
    }

    //setMode();
}

void calibrador::showPage2()
{
    ui->stackedWidget->setCurrentIndex(2);
    setMode();
}

void calibrador::fileNew()
{
    // Adicionar um reset limiares P
    X.resetHPG();
    X.resetPixelsNotaveis();
    novosLimites = true;
    atualizarLimitesHGP(ui->combo_color->currentIndex());
    arquivo = QString();
}

void calibrador::fileOpen()
{
    cout << "---> Entrou em fileOpen()." << endl;

    QString text;
    QFileDialog *fd = new QFileDialog(this);
    fd->setDirectory("../../etc");
    fd->setFileMode(QFileDialog::ExistingFile);
    fd->show();
    if (fd->exec() == QDialog::Accepted)
    {
        text = fd->selectedFiles().at(0);
        if (!text.isEmpty() && !X.fileOpen(text.toStdString().c_str()))
        {
            arquivo = text;
            atualizarCameraParam();
            atualizarLimitesP();
            atualizarLimitesHGP(ui->combo_color->currentIndex());
        }
        else
        {
            //insira seu pop-up de erro aki
        }
    }
}

void calibrador::fileSave()
{
    if (arquivo.isEmpty())
    {
        fileSaveAs();
    }
    else if (X.fileSave(arquivo.toStdString().c_str()))
    {
        QMessageBox::warning(this,
                             tr(""),
                             tr("Nem todos os parametros foram configurados"));
    }
}

void calibrador::fileSaveAs()
{
    QString text;
    QFileDialog *fd = new QFileDialog(this);
    fd->setDirectory("../../etc");
    fd->setFileMode(QFileDialog::AnyFile);
    fd->show();
    if (fd->exec() == QDialog::Accepted)
    {
        text = fd->selectedFiles().at(0);
        if (!text.isEmpty())
        {
            arquivo = text;
            fileSave();
        }
        else
        {
            //TODO: exibir pop-up de erro.
        }
    }
}

void calibrador::fileExit()
{
    //TODO: Exibir pop-up aqui, caso os parametros nao estejam salvos ainda.

    fileSaveAs();
    //    X.terminar();
    //    close();
    QCoreApplication::quit();
    //quitProgram();
}

void calibrador::quitProgram()
{
    X.terminar();
    close();
}

void calibrador::setMode()
{
    int currentpage = ui->stackedWidget->currentIndex();

    if (currentpage == 0)
    {
        X.setMode(CALIBRATOR_IMAGEM_REAL);
    }
    else if (currentpage == 1)
    {
        if (ui->check_viewimage->isChecked() && ui->check_viewlines->isChecked())
        {
            X.setMode(CALIBRATOR_LIMITES_P_E_PONTOS);
        }
        else if (ui->check_viewimage->isChecked())
        {
            X.setMode(CALIBRATOR_LIMITES_P);
        }
        else if (ui->check_viewlines->isChecked())
        {
            X.setMode(CALIBRATOR_PONTOS);
        }
        else
        {
            X.setMode(CALIBRATOR_IMAGEM_REAL);
        }
    }
    else if (currentpage == 2)
    {
        switch (ui->combo_viewmode->currentIndex())
        {
        case 0: //Real Image
            X.setMode(CALIBRATOR_IMAGEM_REAL);
            break;
        case 1: //Current Color
            X.setMode(CALIBRATOR_COR_ATUAL);
            break;
        case 2: //Tagget Image
            X.setMode(CALIBRATOR_COR_ETIQUETADA);
            break;
        case 3: //SOFT Tagget Image
            X.setMode(CALIBRATOR_COR_ETIQUETADA_SOFT);
            break;
        case 4: //Error Image
            X.setMode(CALIBRATOR_IMAGEM_ERROS);
            break;
        default:
            cerr << "Modo inexistente." << endl;
            //exit(1);
            break;
        }
    }
    else
    {
        cerr << "Página inexistente." << endl;
        //TODO: popup de erro aki.
        exit(1);
    }
}

void calibrador::cameraLoadParam()
{
    QString text;
    QFileDialog *fd = new QFileDialog(this);
    fd->setDirectory("../../etc");
    fd->setFileMode(QFileDialog::ExistingFile);
    fd->show();
    if (fd->exec() == QDialog::Accepted)
    {
        text = fd->selectedFiles().at(0);
        if (!text.isEmpty() && !X.loadCameraParam(text.toStdString().c_str()))
        {
            arquivo_cameraParam = text;
            atualizarCameraParam();
            novosLimites = true;
        }
        else
        {
            //insira seu pop-up aki
        }
    }
}

void calibrador::cameraNew()
{
    arquivo_cameraParam = QString();
}

void calibrador::cameraSaveParam()
{
    if (arquivo_cameraParam.isEmpty())
    {
        cameraSaveAsParam();
    }
    else if (X.saveCameraParam(arquivo_cameraParam.toStdString().c_str()))
    {
        //insira seu pop-up de erro aki
    }
}

void calibrador::cameraSaveAsParam()
{
    QString text;
    QFileDialog *fd = new QFileDialog(this);
    fd->setDirectory("../../etc");
    fd->setFileMode(QFileDialog::AnyFile);
    fd->show();
    if (fd->exec() == QDialog::Accepted)
    {
        text = fd->selectedFiles().at(0);
        if (!text.isEmpty() && !X.saveCameraParam(text.toStdString().c_str()))
        {
            arquivo_cameraParam = text;
        }
        else
        {
            //insira seu pop-up aki
        }
    }
}

void calibrador::imageOpen()
{
    QString text;
    QFileDialog *fd = new QFileDialog(this);
    fd->setFilter(QDir::Dirs);
    fd->show();
    if (fd->exec() == QDialog::Accepted)
    {
        text = fd->selectedFiles().at(0);
        if (X.loadImage(text.toStdString().c_str()))
        {
            //pop-up de erro!
        }
    }
}

//TODO: Abrir dialog com caixa de texto.
void calibrador::imageSave()
{
    QString text;
    QFileDialog *fd = new QFileDialog(this);
    fd->setFileMode(QFileDialog::AnyFile);
    fd->show();
    if (fd->exec() == QDialog::Accepted)
    {
        text = fd->selectedFiles().at(0);
        if (!text.isEmpty())
        {
            X.saveImage(text.toStdString().c_str());
        }
    }
}

//TODO: Verificar sliderLimiarPInfValueChanged().
void calibrador::sliderLimiarPInfValueChanged(int valor)
{
    if (valor != ui->spinLimiarPInf->value())
    {
        if (valor >= ui->sliderLimiarPSup->value())
            ui->sliderLimiarPSup->setValue(valor + 1);
        ui->spinLimiarPInf->setValue(valor);
        X.setPinf(valor);

        // X.setConstField(valor);
        //novosLimites = true;
    }
}

//TODO: Verificar spinLimiarPInfValueChanged().
void calibrador::spinLimiarPInfValueChanged(int valor)
{
    if (valor != ui->sliderLimiarPInf->value())
    {
        if (valor >= ui->spinLimiarPSup->value())
        {
            ui->spinLimiarPSup->setValue(valor + 1);
        }
        ui->sliderLimiarPInf->setValue(valor);
        X.setPinf(valor);

        // X.setConstField(valor);
        //spinLimiarPSup->setMinValue(valor);
        //novosLimites = true;
    }
}

//TODO: Verificar sliderLimiarPSupValueChanged().
void calibrador::sliderLimiarPSupValueChanged(int valor)
{
    if (valor != ui->spinLimiarPSup->value())
    {
        if (valor <= ui->sliderLimiarPInf->value())
        {
            ui->sliderLimiarPInf->setValue(valor - 1);
        }
        ui->spinLimiarPSup->setValue(valor);
        // X.setConstObject(valor);
        X.setPsup(valor);
        //novosLimites = true;
    }
}

//TODO: Verificar spinLimiarPSupValueChanged().
void calibrador::spinLimiarPSupValueChanged(int valor)
{
    if (valor != ui->sliderLimiarPSup->value())
    {
        if (valor <= ui->spinLimiarPInf->value())
        {
            ui->spinLimiarPInf->setValue(valor - 1);
        }
        ui->sliderLimiarPSup->setValue(valor);
        // X.setConstObject(valor);
        X.setPsup(valor);
    }
}

void calibrador::BrightnessValueChanged(int valor)
{
    if (valor != ui->spin_brightness->value())
    {
        ui->spin_brightness->setValue(valor);
        X.setBrightness(valor);
        //novosLimites = true;
    }
    if (valor != ui->slider_brightness->value())
    {
        ui->slider_brightness->setValue(valor);
        X.setBrightness(valor);
    }
}

//TODO: Implementar ExposureValueChanged() corretamente.
void calibrador::ExposureValueChanged(int valor)
{
    cerr << "---> Implementar ExposureValueChanged(): " << valor << endl;

    //    if(valor != ui->spin_contrast->value() ){
    //        //if(valor != spinExposure->value() ){
    //        ui->spin_contrast->setValue(valor);
    //        //spinExposure->setValue(valor);
    //        X.setExposureAbs(valor);
    //        //novosLimites = true;
    //    }
    //    if(valor != ui->slider_contrast->value() ){
    //        // if(valor != sliderExposure->value() ){
    //        ui->slider_contrast->setValue(valor);
    //        //sliderExposure->setValue(valor);
    //        X.setExposureAbs(valor);
    //        //novosLimites = true;
    //    }
}

//TODO: Verificar comentários.
void calibrador::ContrastValueChanged(int valor)
{
    if (valor != ui->spin_contrast->value())
    {
        //if(valor != spinExposure->value() ){
        ui->spin_contrast->setValue(valor);
        //spinExposure->setValue(valor);
        X.setConstrast(valor);
        //novosLimites = true;
    }
    if (valor != ui->slider_contrast->value())
    {
        // if(valor != sliderExposure->value() ){
        ui->slider_contrast->setValue(valor);
        //sliderExposure->setValue(valor);
        X.setConstrast(valor);
        //novosLimites = true;
    }
}

void calibrador::HueValueChanged(int valor)
{
    if (valor != ui->spin_hue->value())
    {
        ui->spin_hue->setValue(valor);
        X.setHue(valor);
        //novosLimites = true;
    }
    if (valor != ui->slider_hue->value())
    {
        ui->slider_hue->setValue(valor);
        X.setHue(valor);
        //novosLimites = true;
    }
}

void calibrador::SaturationValueChanged(int valor)
{
    if (valor != ui->spin_saturation->value())
    {
        ui->spin_saturation->setValue(valor);
        X.setSaturation(valor);
        //novosLimites = true;
    }
    if (valor != ui->slider_saturation->value())
    {
        ui->slider_saturation->setValue(valor);
        X.setSaturation(valor);
        //novosLimites = true;
    }
}

void calibrador::GammaValueChanged(int valor)
{
    if (valor != ui->spin_gamma->value())
    {
        ui->spin_gamma->setValue(valor);
        X.setGamma(valor);
        //novosLimites = true;
    }
    if (valor != ui->slider_gamma->value())
    {
        ui->slider_gamma->setValue(valor);
        X.setGamma(valor);
        //novosLimites = true;
    }
}

void calibrador::ExposureAbsValueChanged(int valor)
{
    if (valor != ui->spin_exposureAbs->value())
    {
        ui->spin_exposureAbs->setValue(valor);
        X.setExposureAbs(valor);
        //novosLimites = true;
    }
    if (valor != ui->slider_exposureAbs->value())
    {
        ui->slider_exposureAbs->setValue(valor);
        X.setExposureAbs(valor);
        //novosLimites = true;
    }
}

void calibrador::GainValueChanged(int valor)
{
    if (valor != ui->spin_gain->value())
    {
        ui->spin_gain->setValue(valor);
        X.setGain(valor);
        //novosLimites = true;
    }
    if (valor != ui->slider_gain->value())
    {
        ui->slider_gain->setValue(valor);
        X.setGain(valor);
        //novosLimites = true;
    }
}

void calibrador::HMinValueChanged(int valor)
{
    //limitesHPG[comboCores->currentItem()][0] = valor;
    if (valor != ui->spin_hmin->value())
    {
        ui->spin_hmin->setValue(valor);
        X.setHmin(ui->combo_color->currentIndex(), valor);
        //novosLimites = true;
    }
    if (valor != ui->slider_hmin->value())
    {
        ui->slider_hmin->setValue(valor);
        X.setHmin(ui->combo_color->currentIndex(), valor);
        //novosLimites = true;
    }
}

void calibrador::HMaxValueChanged(int valor)
{
    //limitesHPG[comboCores->currentItem()][1] = valor;
    if (valor != ui->spin_hmax->value())
    {
        ui->spin_hmax->setValue(valor);
        X.setHmax(ui->combo_color->currentIndex(), valor);
        //novosLimites = true;
    }
    if (valor != ui->slider_hmax->value())
    {
        ui->slider_hmax->setValue(valor);
        X.setHmax(ui->combo_color->currentIndex(), valor);
        //novosLimites = true;
    }
}

void calibrador::GMinValueChanged(int valor)
{
    //    limitesHPG[comboCores->currentItem()][4] = valor;
    if (valor != ui->spin_gmin->value())
    {
        ui->spin_gmin->setValue(valor);
        X.setGmin(ui->combo_color->currentIndex(), valor);
        //novosLimites = true;
    }
    if (valor != ui->slider_gmin->value())
    {
        ui->slider_gmin->setValue(valor);
        X.setGmin(ui->combo_color->currentIndex(), valor);
        //novosLimites = true;
    }
}

void calibrador::GMaxValueChanged(int valor)
{
    //    limitesHPG[comboCores->currentItem()][5] = valor;
    if (valor != ui->spin_gmax->value())
    {
        ui->spin_gmax->setValue(valor);
        X.setGmax(ui->combo_color->currentIndex(), valor);
        //novosLimites = true;
    }
    if (valor != ui->slider_gmax->value())
    {
        ui->slider_gmax->setValue(valor);
        X.setGmax(ui->combo_color->currentIndex(), valor);
        //novosLimites = true;
    }
}

void calibrador::PMinValueChanged(int valor)
{
    //    limitesHPG[comboCores->currentItem()][2] = valor;
    if (valor != ui->spin_pmin->value())
    {
        ui->spin_pmin->setValue(valor);
        X.setPmin(ui->combo_color->currentIndex(), valor);
        //novosLimites = true;
    }
    if (valor != ui->slider_pmin->value())
    {
        ui->slider_pmin->setValue(valor);
        X.setPmin(ui->combo_color->currentIndex(), valor);
        //novosLimites = true;
    }
}

void calibrador::PMaxValueChanged(int valor)
{
    //    limitesHPG[comboCores->currentItem()][3] = valor;
    if (valor != ui->spin_pmax->value())
    {
        ui->spin_pmax->setValue(valor);
        X.setPmax(ui->combo_color->currentIndex(), valor);
        //novosLimites = true;
    }
    if (valor != ui->slider_pmax->value())
    {
        ui->slider_pmax->setValue(valor);
        X.setPmax(ui->combo_color->currentIndex(), valor);
        //novosLimites = true;
    }
}

void calibrador::atualizarLimitesHGP(int item)
{
    X.setColor(item);
    limitesHPG prov = X.getLimHPG(item);
    ui->spin_hmin->setValue(prov.H.min);
    ui->spin_hmax->setValue(prov.H.max);
    ui->spin_pmin->setValue(prov.P.min);
    ui->spin_pmax->setValue(prov.P.max);
    ui->spin_gmin->setValue(prov.G.min);
    ui->spin_gmax->setValue(prov.G.max);
}

void calibrador::atualizarCameraParam()
{
    struct controler ctrl;

    if (!X.queryBrightness(ctrl))
    {
        ui->slider_brightness->setEnabled(false);
        ui->spin_brightness->setEnabled(false);
    }
    else
    {
        ui->slider_brightness->setRange(ctrl.min, ctrl.max);
        ui->spin_brightness->setRange(ctrl.min, ctrl.max);

        ui->slider_brightness->setValue(X.getBrightness());
        ui->spin_brightness->setValue(X.getBrightness());
    }

    if (!X.queryContrast(ctrl))
    {
        ui->slider_contrast->setEnabled(false);
        ui->spin_contrast->setEnabled(false);
    }
    else
    {
        ui->slider_contrast->setRange(ctrl.min, ctrl.max);
        ui->spin_contrast->setRange(ctrl.min, ctrl.max);

        ui->slider_contrast->setValue(X.getContrast());
        ui->spin_contrast->setValue(X.getContrast());
    }

    if (!X.queryHue(ctrl))
    {
        ui->slider_hue->setEnabled(false);
        ui->spin_hue->setEnabled(false);
    }
    else
    {
        ui->slider_hue->setRange(ctrl.min, ctrl.max);
        ui->spin_hue->setRange(ctrl.min, ctrl.max);

        ui->slider_hue->setValue(X.getHue());
        ui->spin_hue->setValue(X.getHue());
    }

    if (!X.querySaturation(ctrl))
    {
        ui->slider_saturation->setEnabled(false);
        ui->spin_saturation->setEnabled(false);
    }
    else
    {
        ui->slider_saturation->setRange(ctrl.min, ctrl.max);
        ui->spin_saturation->setRange(ctrl.min, ctrl.max);

        ui->slider_saturation->setValue(X.getSaturation());
        ui->spin_saturation->setValue(X.getSaturation());
    }

    if (!X.queryGamma(ctrl))
    {
        ui->slider_gamma->setEnabled(false);
        ui->spin_gamma->setEnabled(false);
    }
    else
    {
        ui->slider_gamma->setRange(ctrl.min, ctrl.max);
        ui->spin_gamma->setRange(ctrl.min, ctrl.max);

        ui->slider_gamma->setValue(X.getGamma());
        ui->spin_gamma->setValue(X.getGamma());
    }

    if (!X.queryGain(ctrl))
    {
        ui->slider_gain->setEnabled(false);
        ui->spin_gain->setEnabled(false);
    }
    else
    {
        ui->slider_gain->setRange(ctrl.min, ctrl.max);
        ui->spin_gain->setRange(ctrl.min, ctrl.max);

        ui->slider_gain->setValue(X.getGain());
        ui->spin_gain->setValue(X.getGain());
    }

    if (!X.queryExposureAbs(ctrl))
    {
        ui->slider_exposureAbs->setEnabled(false);
        ui->spin_exposureAbs->setEnabled(false);
    }
    else
    {
        ui->slider_exposureAbs->setRange(ctrl.min, ctrl.max);
        ui->spin_exposureAbs->setRange(ctrl.min, ctrl.max);

        ui->slider_exposureAbs->setValue(X.getExposureAbs());
        ui->spin_exposureAbs->setValue(X.getExposureAbs());
    }
}

void calibrador::atualizarLimitesP()
{
    ui->spinLimiarPInf->setValue(X.getPinf());
    ui->spinLimiarPSup->setValue(X.getPsup());

    //Verificar esse teste...
    // spinLimiarPInf->setValue(X.getConstField());
    // spinLimiarPSup->setValue(X.getConstObject());
}

void calibrador::shootingChanged(bool value)
{
    X.modoCaptura(value);
}

void calibrador::redesenharImagem()
{
    novosLimites = true;
}

void calibrador::atualizarParametrosCamera()
{
    novosParametros = true;
}

void calibrador::atualizarCamera()
{
    novosParametros = true;
}

void calibrador::novosParametrosCamera()
{
    if (novosParametros)
    {
        // X.setParameters();
        novosParametros = false;
    }
}

void calibrador::processarImagem()
{
    if (ui->stackedWidget->currentIndex() == 0 || ui->check_shooting->isChecked() || novosLimites)
    {
        X.processImage();
        pixmap_label1->loadFromData((uchar *)X.getPNMdata(), X.getPNMsize(), "PPM");
        pixmap_label1->redesenhe();
        novosLimites = false;
    }
}

void calibrador::mouseMove(QPoint Point)
{
    if (X.posicaoValida((unsigned int)Point.x(), (unsigned int)Point.y()))
    {
        //	ImPosicaoValida(Point.x(),Point.y())) {
        MouseX = Point.x();
        MouseY = Point.y();
        atualizarDisplays();
        if (ponto_dragged)
        {
            X.moverPonto(ponto_selecionado, MouseX, MouseY);
            novosLimites = true;
            //	    pixelsNotaveis[ponto_selecionado].u() = MouseX;
            //	    pixelsNotaveis[ponto_selecionado].v() = MouseY;
            //	    desenharCampo(	);
        }
    }
}

void calibrador::mousePress(QPoint Point)
{
    if (ui->stackedWidget->currentIndex() == 1 && ui->check_viewlines->isChecked())
    {
        MouseX = Point.x();
        MouseY = Point.y();
        int selec = X.pontoSelecionado(Point.x(), Point.y());
        if (selec != -1)
        {
            ponto_selecionado = selec;
            ponto_dragged = true;
            X.moverPonto(ponto_selecionado, MouseX, MouseY);
            novosLimites = true;
        }
    }
    else if (ui->stackedWidget->currentIndex() == 2)
    {
        MouseX = Point.x();
        MouseY = Point.y();

        X.getPxValor(MouseX, MouseY, R, G1, B, H, P, G2);

        X.setHmin(ui->combo_color->currentIndex(), H - 20);
        X.setHmax(ui->combo_color->currentIndex(), H + 20);
        atualizarLimitesHGP(ui->combo_color->currentIndex());
    }
}

void calibrador::mouseRelease(QPoint Point)
{
    if (ponto_dragged)
    {
        X.moverPonto(ponto_selecionado, Point.x(), Point.y());
        ponto_selecionado = -1;
        ponto_dragged = false;
        novosLimites = true;
    }
}

void calibrador::atualizarDisplays()
{

    //tela 1
    X.getPxValor(MouseX, MouseY, R, G1, B, H, P, G2);
    ui->lcd_x->display(MouseX);
    ui->lcd_y->display(MouseY);

    ui->lcd_rgb_r->display(R);
    ui->lcd_rgb_g->display(G1);
    ui->lcd_rgb_b->display(B);

    ui->lcd_hpg_h->display(H);
    ui->lcd_hpg_p->display(P);
    ui->lcd_hpg_g->display(G2);

    //Tela 2
    //    lcdX2->display(MouseX);
    //    lcdY2->display(MouseY);

    //    lcdRGB_R2->display(R);
    //    lcdRGB_G2->display(G1);
    //    lcdRGB_B2->display(B);

    //    lcdHGP_H2->display(H);
    //    lcdHGP_P2->display(P);
    //    lcdHGP_G2->display(G2);
}
