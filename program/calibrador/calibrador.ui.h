/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.5
*****************************************************************************/

#define SPIN_MIN_VALUE 0
#define SPIN_MAX_VALUE 100
#define NUM_AMOSTRAS 100 //amostrar para calcular a média do campo vazio
//#define LARGURA_EXIBICAO 640
//#define ALTURA_EXIBICAO 480
//#define METADE_LARGURA_EXIBICAO 320
//#define METADE_ALTURA_EXIBICAO 240
#include <qmessagebox.h>

using namespace std;

//extern QApplication *qApp;

void calibrador::fileNew()
{
    X.resetHPG();
    X.resetPixelsNotaveis();
    novosLimites = true;
    atualizarLimitesHGP( comboCores->currentItem() );
    arquivo = QString::null;
}

void calibrador::fileOpen()
{
  QString text;
  Q3FileDialog* fd = new Q3FileDialog(this);
  fd->setDir("../../etc");
  fd->setMode( Q3FileDialog::ExistingFile );
  fd->show();
  if(fd->exec()==QDialog::Accepted){
    text = fd->selectedFile();
    if( !text.isEmpty() && !X.fileOpen(text)){
        arquivo = text;
        atualizarCameraParam();
        atualizarLimitesP();
        atualizarLimitesHGP(comboCores->currentItem());
	     }else{
	    //insira seu pop-up de erro aki
	   }
  }
}



void calibrador::fileSave()
{


    if(arquivo.isEmpty()){
	fileSaveAs();
    }else if(X.fileSave(arquivo)){
      QMessageBox::warning(this,
                          tr(""),
                          tr("Nem todos os parametros foram configurados"));
    }
}


void calibrador::fileSaveAs()
{


    QString text;
    Q3FileDialog* fd = new Q3FileDialog( this );
    fd->setDir("../../etc");
    fd->setMode( Q3FileDialog::AnyFile );
    fd->show();
    if(fd->exec()==QDialog::Accepted){
	text=fd->selectedFile();
	if( !text.isEmpty() ){
	    arquivo = text;
	    fileSave();
	}else{
	    //TODO: exibir pop-up de erro.
	}
    }
}



void calibrador::fileExit()
{
    //TODO: Exibir pop-up aqui, caso os parametros nao estejam salvos ainda.

    fileSaveAs();
    X.terminar();
    close();
}

void calibrador::cameraLoadParam(){
    QString text;
    Q3FileDialog* fd = new Q3FileDialog( this );
    fd->setDir("../../etc");
    fd->setMode( Q3FileDialog::ExistingFile );
    fd->show();
    if(fd->exec()==QDialog::Accepted){
	text=fd->selectedFile();
	if( !text.isEmpty() && !X.loadCameraParam(text)){
	    arquivo_cameraParam = text;
	    atualizarCameraParam();
	    novosLimites = true;
	}else{
	    //insira seu pop-up aki
	}
    }
}

void calibrador::cameraNew()
{
    arquivo_cameraParam = QString::null;
}

void calibrador::cameraSaveParam()
{
    if(arquivo_cameraParam.isEmpty()){
	cameraSaveAsParam();
    }else if(X.saveCameraParam(arquivo_cameraParam)) {
	//insira seu pop-up de erro aki
    }
}

void calibrador::cameraSaveAsParam()
{
    QString text;
    Q3FileDialog* fd = new Q3FileDialog( this );
    fd->setDir("../../etc");
    fd->setMode( Q3FileDialog::AnyFile );
    fd->show();
    if(fd->exec()==QDialog::Accepted){
	text=fd->selectedFile();
	if( !text.isEmpty() && !X.saveCameraParam(text)){
	    arquivo_cameraParam = text;
	}else{
	    //insira seu pop-up aki
	}
    }
}


void calibrador::init()
{
    textLabel1_2_2_2_2_2_3 -> setText("Exposure Absolute");
    textLabel1_2_2_2_2_2_4 -> setText("Gain");
    textLabel1_2_2_2_2 -> setText("Hue");
    if (carregaInterface()){
    	cerr<<"Erro na leitura do arquivo de Interface"<<endl;
    	exit(1);
    }
    lcdHGP_P->setSegmentStyle(QLCDNumber::Flat);
    lcdHGP_G->setSegmentStyle(QLCDNumber::Flat);
    lcdHGP_H->setSegmentStyle(QLCDNumber::Flat);

    lcdHGP_H2->setSegmentStyle(QLCDNumber::Flat);
    lcdHGP_P2->setSegmentStyle(QLCDNumber::Flat);
    lcdHGP_G2->setSegmentStyle(QLCDNumber::Flat);

    checkExibirImagemProcessada->setText("View field average");
    checkExibirImagemProcessada->setEnabled(false);
    pushAvancarTela1->setText("Next(falta a imagem Media)");
    pushAvancarTela1->setEnabled(false);
    checkShooting->setChecked(true);
    //inicializa posição do mouse.
    MouseX = 0;
    MouseY = 0;

    //WARNING apagar daqui
    labelLimiarPInf->setText("Const Field");
    labelLimiarPSup->setText("Const Object");
    sliderLimiarPInf->setMaxValue(10);
    sliderLimiarPInf->setMinValue(1);
    sliderLimiarPSup->setMaxValue(10);
    sliderLimiarPSup->setMinValue(1);

    spinLimiarPInf->setMaxValue(10);
    spinLimiarPInf->setMinValue(1);

    spinLimiarPSup->setMaxValue(10);
    spinLimiarPSup->setMinValue(1);

    sliderLimiarPSup->setValue(X.getConstObject());//usado para alterar const object
    sliderLimiarPInf->setValue(X.getConstField());

    X.setPinf(0);
    X.setPsup(100);

    // std::cerr << "const object: "<<X.getConstObject() << '\n';
    // std::cerr << "const Field: "<<X.getConstField() << '\n';

    //TODO ate aqui

    //exibe a tela de calibracao de pontos.
    mostrarTela0();

    //inicializacoes do mouse
    ponto_dragged = false;
    ponto_selecionado = -1;

    //inicializacao das flags para redesenhar a imagem
    novosLimites = true;
    novosParametros = true;

    //inicializarComValoresPadroes();
    arquivo = QString::null;
    arquivo_cameraParam = QString::null;

    Timer = new QTimer(this,"Timer");
    connect( Timer, SIGNAL(timeout()), this, SLOT(novosParametrosCamera()) );
    connect( Timer, SIGNAL(timeout()), this, SLOT(processarImagem()) );
    //connect( Timer, SIGNAL(timeout()), myObject, SLOT(timerDone()) );
    Timer->start(200,false);


}

bool calibrador::carregaInterface(){

    QString ColorName;
    for(unsigned i=0; i < X.getNumCores(); i++){
	ColorName = X.getNomeCor(i);
	comboCores->insertItem(ColorName);
   }
    sliderUOffset->setMaxValue(X.getCamWidth() - (pixmap_label1->size()).width());
    sliderVOffset->setMaxValue(X.getCamHeight() - (pixmap_label1->size()).height());

    atualizarLimitesHGP(comboCores->currentItem());
    atualizarCameraParam();
    return false;
}

void calibrador::destroy(){
  delete Timer;
}

void calibrador::mouseMove( QPoint Point)
{
      if( X.posicaoValida((unsigned int)Point.x(),(unsigned int)Point.y() )){
//	ImPosicaoValida(Point.x(),Point.y())) {
	MouseX = Point.x();
	MouseY = Point.y();
	atualizarDisplays();
	if(ponto_dragged){
	  X.moverPonto(ponto_selecionado,MouseX,MouseY);
	  novosLimites = true;
	  //	    pixelsNotaveis[ponto_selecionado].u() = MouseX;
	  //	    pixelsNotaveis[ponto_selecionado].v() = MouseY;
	  //	    desenharCampo(	);
	}
    }
}

void calibrador::mousePress( QPoint Point)
{
  if(telaAtual == 1 && checkExibirGrade->isChecked()){
    MouseX = Point.x();
    MouseY = Point.y();
    int selec = X.pontoSelecionado(Point.x(),Point.y());
    if(selec != -1){
      ponto_selecionado = selec;
      ponto_dragged = true;
      X.moverPonto(ponto_selecionado,MouseX,MouseY);
      novosLimites = true;
    }
  }else if(telaAtual == 2){
    MouseX = Point.x();
    MouseY = Point.y();

    X.getPxValor(MouseX, MouseY, R, G1, B, H, P, G2);

    X.setHmin(comboCores->currentItem(), H-20);
    X.setHmax(comboCores->currentItem(), H+20);
    atualizarLimitesHGP(comboCores->currentItem());
  }
}


void calibrador::mouseRelease(QPoint Point)
{
  if(ponto_dragged){
    X.moverPonto(ponto_selecionado,Point.x(),Point.y());
    ponto_selecionado = -1;
    ponto_dragged = false;
    novosLimites = true;
  }
}

void calibrador::imageOpen()
{
    QString text;
    Q3FileDialog* fd = new Q3FileDialog( this );
    fd->addFilter( "Images (*.png *.jpg *.xpm *.bmp *.ppm)" );
    fd->show();
    if(fd->exec()==QDialog::Accepted){
	text=fd->selectedFile();
	if (X.loadImage(text)){
	    //pop-up de erro!
	}
    }
}

void calibrador::imageSave()
{
    QString text;
    Q3FileDialog* fd = new Q3FileDialog( this );
    fd->setMode( Q3FileDialog::AnyFile );
    fd->show();
    if(fd->exec()==QDialog::Accepted){
	text=fd->selectedFile();
	if( !text.isEmpty() ){
	    X.saveImage(text);
	}
    }
}




void calibrador::atualizarDisplays()
{

    //tela 1
    X.getPxValor(MouseX,MouseY,R,G1,B,H,P,G2);
    lcdX->display(MouseX);
    lcdY->display(MouseY);

    lcdRGB_R->display(R);
    lcdRGB_G->display(G1);
    lcdRGB_B->display(B);

    lcdHGP_H->display(H);
    lcdHGP_P->display(P);
    lcdHGP_G->display(G2);

    //Tela 2
    lcdX2->display(MouseX);
    lcdY2->display(MouseY);

    lcdRGB_R2->display(R);
    lcdRGB_G2->display(G1);
    lcdRGB_B2->display(B);

    lcdHGP_H2->display(H);
    lcdHGP_P2->display(P);
    lcdHGP_G2->display(G2);
}

void calibrador::sliderLimiarPInfValueChanged( int valor )
{
  if(valor != spinLimiarPInf->value() ){
  	if(valor >= sliderLimiarPSup->value() )
  	    sliderLimiarPSup->setValue(valor+1);
  	spinLimiarPInf->setValue(valor);
  	// X.setPinf(valor);

    X.setConstField(valor);
  	//novosLimites = true;
  }
}

void calibrador::spinLimiarPInfValueChanged( int valor )
{
    if(valor != sliderLimiarPInf->value() ){
	if(valor >= spinLimiarPSup->value() ){
	    spinLimiarPSup->setValue(valor+1);
	}
	sliderLimiarPInf->setValue(valor);
	// X.setPinf(valor);

  X.setConstField(valor);
	//spinLimiarPSup->setMinValue(valor);
	//novosLimites = true;
    }
}

void calibrador::sliderLimiarPSupValueChanged( int valor )
{
    if(valor != spinLimiarPSup->value() ){
	if(valor <= sliderLimiarPInf->value() ){
	    sliderLimiarPInf->setValue(valor-1);
	}
	spinLimiarPSup->setValue(valor);
	X.setConstObject(valor);
	//novosLimites = true;
    }
}

void calibrador::spinLimiarPSupValueChanged( int valor )
{
  if(valor != sliderLimiarPSup->value() ){
  	if(valor <= spinLimiarPInf->value() ){
  	    spinLimiarPInf->setValue(valor-1);
  	}
    sliderLimiarPSup->setValue(valor);
    X.setConstObject(valor);
  }
}


void calibrador::BrightnessValueChanged(int valor)
{
  if(valor != spinBrightness->value() ){
  	spinBrightness->setValue(valor);
  	X.setBrightness(valor);
  	//novosLimites = true;
  }
  if(valor != sliderBrightness->value() ){
  	sliderBrightness->setValue(valor);
  	X.setBrightness(valor);
  }
}

void calibrador::ExposureValueChanged(int valor)
{
    if(valor != spinContrast->value() ){
    //if(valor != spinExposure->value() ){
	spinContrast->setValue(valor);
	//spinExposure->setValue(valor);
	X.setExposureAbs(valor);
	//novosLimites = true;
    }
    if(valor != sliderContrast->value() ){
   // if(valor != sliderExposure->value() ){
	sliderContrast->setValue(valor);
	//sliderExposure->setValue(valor);
	X.setExposureAbs(valor);
	//novosLimites = true;
    }
}



void calibrador::HueValueChanged(int valor)
{
  if(valor != spinHue->value() ){
  	spinHue->setValue(valor);
  	X.setHue(valor);
  	//novosLimites = true;
  }
  if(valor != sliderHue->value() ){
  	sliderHue->setValue(valor);
  	X.setHue(valor);
  	//novosLimites = true;
  }
}

void calibrador::SaturationValueChanged(int valor)
{
    if(valor != spinSaturation->value() ){
    	spinSaturation->setValue(valor);
    	X.setSaturation(valor);
    	//novosLimites = true;
    }
    if(valor != sliderSaturation->value() ){
    	sliderSaturation->setValue(valor);
    	X.setSaturation(valor);
    	//novosLimites = true;
    }
}

void calibrador::GammaValueChanged(int valor)
{
    if(valor != spinGamma->value() ){
	spinGamma->setValue(valor);
	X.setGamma(valor);
	//novosLimites = true;
    }
    if(valor != sliderGamma->value() ){
	sliderGamma->setValue(valor);
	X.setGamma(valor);
	//novosLimites = true;
    }
}

void calibrador::ShutterValueChanged(int valor)
{
    if(valor != spinShutter->value() ){
	spinShutter->setValue(valor);
	X.setShutter(valor);
	//novosLimites = true;
    }
    if(valor != sliderShutter->value() ){
	sliderShutter->setValue(valor);
	X.setShutter(valor);
	//novosLimites = true;
    }
}

void calibrador::GainValueChanged(int valor)
{
    if(valor != spinGain->value() ){
    	spinGain->setValue(valor);
    	X.setGain(valor);
	//novosLimites = true;
    }
    if(valor != sliderGain->value() ){
	     sliderGain->setValue(valor);
	     X.setGain(valor);
	//novosLimites = true;
    }
}


void calibrador::HMinValueChanged(int valor)
{
    //limitesHPG[comboCores->currentItem()][0] = valor;
    if(valor != spinHGP_Hmin->value() ){
	spinHGP_Hmin->setValue(valor);
	X.setHmin(comboCores->currentItem(),valor);
	//novosLimites = true;
    }
    if(valor != sliderHGP_Hmin->value() ){
	sliderHGP_Hmin->setValue(valor);
	X.setHmin(comboCores->currentItem(),valor);
	//novosLimites = true;
    }
}


void calibrador::HMaxValueChanged(int valor)
{
    //limitesHPG[comboCores->currentItem()][1] = valor;
    if(valor != spinHGP_Hmax->value() ){
	spinHGP_Hmax->setValue(valor);
	X.setHmax(comboCores->currentItem(),valor);
	//novosLimites = true;
    }
    if(valor != sliderHGP_Hmax->value() ){
	sliderHGP_Hmax->setValue(valor);
	X.setHmax(comboCores->currentItem(),valor);
	//novosLimites = true;
    }
}


void calibrador::GMinValueChanged(int valor)
{
    //    limitesHPG[comboCores->currentItem()][4] = valor;
    if(valor != spinHGP_Gmin->value() ){
	spinHGP_Gmin->setValue(valor);
	X.setGmin(comboCores->currentItem(),valor);
	//novosLimites = true;
    }
    if(valor != sliderHGP_Gmin->value() ){
	sliderHGP_Gmin->setValue(valor);
	X.setGmin(comboCores->currentItem(),valor);
	//novosLimites = true;
    }
}

void calibrador::GMaxValueChanged(int valor)
{
    //    limitesHPG[comboCores->currentItem()][5] = valor;
    if(valor != spinHGP_Gmax->value() ){
	spinHGP_Gmax->setValue(valor);
	X.setGmax(comboCores->currentItem(),valor);
	//novosLimites = true;
    }
    if(valor != sliderHGP_Gmax->value() ){
	sliderHGP_Gmax->setValue(valor);
	X.setGmax(comboCores->currentItem(),valor);
	//novosLimites = true;
    }
}


void calibrador::PMinValueChanged(int valor)
{

    //    limitesHPG[comboCores->currentItem()][2] = valor;
    if(valor != spinHGP_Pmin->value() ){
	spinHGP_Pmin->setValue(valor);
	X.setPmin(comboCores->currentItem(),valor);
    	//novosLimites = true;
    }
    if(valor != sliderHGP_Pmin->value() ){
	sliderHGP_Pmin->setValue(valor);
	X.setPmin(comboCores->currentItem(),valor);
	//novosLimites = true;
    }
}


void calibrador::PMaxValueChanged(int valor)
{
    //    limitesHPG[comboCores->currentItem()][3] = valor;
    if(valor != spinHGP_Pmax->value() ){
	spinHGP_Pmax->setValue(valor);
	X.setPmax(comboCores->currentItem(),valor);
	//novosLimites = true;
    }
    if(valor != sliderHGP_Pmax->value() ){
	sliderHGP_Pmax->setValue(valor);
	X.setPmax(comboCores->currentItem(),valor);
	//novosLimites = true;
    }
}

void calibrador::atualizarLimitesHGP( int item )
{
    X.setColor(item);
    limitesHPG prov = X.getLimHPG(item);
    spinHGP_Hmin->setValue( prov.H.min );
    spinHGP_Hmax->setValue( prov.H.max );
    spinHGP_Pmin->setValue( prov.P.min );
    spinHGP_Pmax->setValue( prov.P.max );
    spinHGP_Gmin->setValue( prov.G.min );
    spinHGP_Gmax->setValue( prov.G.max );
}

void calibrador::atualizarCameraParam(){

    struct controler ctrl;
    if(!X.queryBrightness(ctrl)){
      spinBrightness->setEnabled(false);
      sliderBrightness->setEnabled(false);
    }else{
      spinBrightness->setMaxValue(ctrl.max);
      spinBrightness->setMinValue(ctrl.min);

      sliderBrightness->setMaxValue(ctrl.max);
      sliderBrightness->setMinValue(ctrl.min);

      sliderBrightness->setValue(X.getBrightness());
      spinBrightness->setValue(X.getBrightness());
    }

    if(!X.queryContrast(ctrl)){
      spinContrast->setEnabled(false);
      sliderContrast->setEnabled(false);
    }else{
      spinContrast->setMaxValue(ctrl.max);
      spinContrast->setMinValue(ctrl.min);

      sliderContrast->setMaxValue(ctrl.max);
      sliderContrast->setMinValue(ctrl.min);

      sliderContrast->setValue(X.getContrast());
      spinContrast->setValue(X.getContrast());
    }


    if(!X.queryHue(ctrl)){
      spinHue->setEnabled(false);
      sliderHue->setEnabled(false);
    }else{
      spinHue->setMaxValue(ctrl.max);
      spinHue->setMinValue(ctrl.min);
      spinHue->setValue(X.getHue());

      sliderHue->setMaxValue(ctrl.max);
      sliderHue->setMinValue(ctrl.min);
      sliderHue->setValue(X.getHue());
    }

    if(!X.querySaturation(ctrl))
    {
      spinSaturation->setEnabled(false);
      sliderSaturation->setEnabled(false);
    }else{
      spinSaturation->setMaxValue(ctrl.max);
      spinSaturation->setMinValue(ctrl.min);
      spinSaturation->setValue(X.getSaturation());

      sliderSaturation->setMaxValue(ctrl.max);
      sliderSaturation->setMinValue(ctrl.min);
      sliderSaturation->setValue(X.getSaturation());
    }

    if(!X.queryGamma(ctrl))
    {
      spinGamma->setEnabled(false);
      sliderGamma->setEnabled(false);
    }else{
      spinGamma->setMaxValue(ctrl.max);
      spinGamma->setMinValue(ctrl.min);
      spinGamma->setValue(X.getGamma());

      sliderGamma->setMaxValue(ctrl.max);
      sliderGamma->setMinValue(ctrl.min);
      sliderGamma->setValue(X.getGamma());
    }

    if(!X.queryGain(ctrl)){
        spinGain->setEnabled(false);
        sliderGain->setEnabled(false);
    }else {
      spinGain->setMaxValue(ctrl.max);
      spinGain->setMinValue(ctrl.min);
      sliderGain->setMinValue(ctrl.min);
      sliderGain->setMaxValue(ctrl.max);
      spinGain -> setValue(X.getGain());
      sliderGain-> setValue(X.getGain());
    }

    //OBS: SHUTTER eh tratado como Exposure Absolute
    if(!X.queryExposureAbs(ctrl)){
        spinShutter->setEnabled(false);
        sliderShutter->setEnabled(false);
    }else {
      spinShutter->setMaxValue(ctrl.max);
      spinShutter->setMinValue(ctrl.min);
      sliderShutter->setMinValue(ctrl.min);
      sliderShutter->setMaxValue(ctrl.max);
      spinShutter -> setValue(X.getExposureAbs());
      sliderShutter-> setValue(X.getExposureAbs());
    }
}


void calibrador::atualizarLimitesP()
{
    spinLimiarPInf->setValue(X.getPinf());
    spinLimiarPSup->setValue(X.getPsup());
}



void calibrador::mostrarTela0()
{
    widgetStack1->raiseWidget(tela0);
    telaAtual = 0;
    //novosLimites = true;
    setarModo();
}


void calibrador::mostrarTela1()
{
    widgetStack1->raiseWidget(tela1);
    telaAtual = 1;
    QString infoText;
    QMessageBox msgBox;

    if(!X.campoVazioCapturado()){
      infoText = QString("Operacao obrigatoria, certifique-se de que o programa ira capturar a imagem do campo vazio.\n \n\\
      Deseja realizar a captura ?");
    }else{
      infoText = QString("Deseja realizar uma nova captura do campo Vazio ?");
      // setarModo();
      // return;
    }


    msgBox.setText("Captura do Campo Vazio");
    msgBox.setInformativeText(infoText);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Yes){
      X.calImgMedia(NUM_AMOSTRAS);
      QMessageBox msg;
      msg.setText("Captura da imagem media realizada");
      msg.exec();

      pushAvancarTela1->setEnabled(true);
      pushAvancarTela1->setText("Next");
      checkExibirImagemProcessada->setEnabled(true);
    }

    setarModo();
}


void calibrador::mostrarTela2()
{
    widgetStack1->raiseWidget(tela2);
    telaAtual = 2;
    //novosLimites = true;
    setarModo();
}

void calibrador::novosParametrosCamera(){
    if(novosParametros){
	// X.setParameters();
	novosParametros = false;
    }
}

void calibrador::processarImagem()
{
  if(telaAtual == 0 || checkShooting->isChecked() || novosLimites){
  	X.processImage();
  	pixmap_label1->loadFromData((uchar*)X.getPNMdata(),
    X.getPNMsize(),"PPM");

  	pixmap_label1->redesenhe();
  	novosLimites = false;
  }
}


void calibrador::setarModo()
{
    if(telaAtual == 0){
	X.setMode(CALIBRATOR_IMAGEM_REAL);
    }else if(telaAtual == 1){
	if(checkExibirImagemProcessada->isChecked() &&
	   checkExibirGrade->isChecked()){
	    X.setMode(CALIBRATOR_LIMITES_P_E_PONTOS);
	}else if(checkExibirImagemProcessada->isChecked()){
	    X.setMode(CALIBRATOR_LIMITES_P);
	}else if(checkExibirGrade->isChecked()){
	    X.setMode(CALIBRATOR_PONTOS);
	}else{
	    X.setMode(CALIBRATOR_IMAGEM_REAL);
	}
    }else if(telaAtual == 2){
	switch(comboExibicao1->currentItem()){
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
	    X.setMode(    CALIBRATOR_IMAGEM_ERROS);
	    break;
	default:
	    //cerr<<"Qual modo vc quer que eu use?"<<endl;
	    exit(1);
	    break;
	}

    }else{
	//	cerr<<"Qual tela vc está exibindo?!\n";
	//TODO: popup de erro aki.
	exit(1);
    }

}

void calibrador::ShootingChanged( bool value )
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
    // spinHue->setValue(X.getHue());
    // spinGain->setValue(X.getGain());
    // spinGamma->setValue(X.getGamma());
    // spinShutter->setValue(X.getExposureAbs());
    // spinExposure->setValue(X.getExposure());
    // spinContrast->setValue(X.getContrast());
    // spinBrightness->setValue(X.getBrightness());
    // spinSaturation->setValue(X.getSaturation());
}


void calibrador::atualizarCamera()
{
    novosParametros = true;
}

void calibrador::changeUOffset( int new_offset )
{
    X.setOffsetU(new_offset);
}


void calibrador::changeVOffset( int new_offset )
{
    X.setOffsetV(new_offset);
}
