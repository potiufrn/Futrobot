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
//#define LARGURA_EXIBICAO 640
//#define ALTURA_EXIBICAO 480
//#define METADE_LARGURA_EXIBICAO 320
//#define METADE_ALTURA_EXIBICAO 240
#include <QMessageBox>

typedef double *pdouble;

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
    Q3FileDialog* fd = new Q3FileDialog( this );
    fd->setMode( Q3FileDialog::ExistingFile );
    fd->show();
    if(fd->exec()==QDialog::Accepted){
	text=fd->selectedFile();
	if( !text.isEmpty() && !X.fileOpen(text)){
	    arquivo = text;
	    atualizarLimitesP();
	    atualizarLimitesHGP(comboCores->currentItem());
	}else{
	    //insira seu pop-up aki
	}
    }
}



void calibrador::fileSave()
{
    if(arquivo.isEmpty()){
	fileSaveAs();
    }else if(X.fileSave(arquivo)){
	//insira seu pop-up de erro aki
    }
}


void calibrador::fileSaveAs()
{
    QString text;
    Q3FileDialog* fd = new Q3FileDialog( this );
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
    //terminar = true;

	QMessageBox msgBox;
	msgBox.setWindowTitle("Fechando Janela");
	msgBox.setText("Tem Certeza que deseja fechar");
	msgBox.setStandardButtons(QMessageBox::Yes);
	msgBox.addButton(QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	if(msgBox.exec() == QMessageBox::Yes){
		X.terminar();
    		close();
	}else {
		
	}
}

void calibrador::cameraLoadParam(){
    QString text;
    Q3FileDialog* fd = new Q3FileDialog( this );
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
    X.resetCameraParam();
    atualizarCameraParam();
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
    if (carregaInterface()){
	cerr<<"Erro na leitura do arquivo de Interface"<<endl;
	exit(1);
    }
    //inicializa posição do mouse.
    MouseX = 0;
    MouseY = 0;
    
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



void calibrador::destroy()
{
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
    MouseX = Point.x();
    MouseY = Point.y();
    int selec = X.pontoSelecionado(Point.x(),Point.y());
    if(selec != -1){
	ponto_selecionado = selec;
	ponto_dragged = true;	
	X.moverPonto(ponto_selecionado,MouseX,MouseY);
	novosLimites = true;
    }
}


void calibrador::mouseRelease(QPoint Point)
{
    if(ponto_dragged){
	X.moverPonto(ponto_selecionado,Point.x(),Point.y());
	ponto_selecionado = -1;
	ponto_dragged = false;
	novosLimites = true;
	//desenharCampo();
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
    //    cout << "MOUSE!" << endl;
    
    //tela 1
    //X.getPxValor(MouseX,MouseY,R,G1,B,H,P,G2);
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
    

    /*
    QRgb ponto;
    if(checkExibirImagemProcessada->isChecked() ){
	ponto = imagem_processada.pixel(MouseX,MouseY);
    }else{
	ponto = imagem.pixel(MouseX,MouseY);
    }
    
    lcdRGB_R->display((int)round((qRed(ponto)/255.0)*100));
    lcdRGB_G->display((int)round((qGreen(ponto)/255.0)*100));
    lcdRGB_B->display((int)round((qBlue(ponto)/255.0)*100));
    pixel=PxRGB(qRed(ponto),qGreen(ponto),qBlue(ponto));
    
//	pixel.set( ((double)qRed(ponto))/255.0,
//		   ((double)qGreen(ponto))/255.0,
//		   ((double)qBlue(ponto))/255.0 );
    
    pixel.getHPG(H, P_, G_);
    if(isnan(H)){
	lcdHGP_H->display(999);   
    }
    else{
	lcdHGP_H->display((int)round(H*100.0));
    }
    lcdHGP_P->display((int)round(P_*100.0));
    if(isnan(G_)){
	lcdHGP_G->display(999);    
    }
    else{
	lcdHGP_G->display((int)round(G_*100.0));    
    }
    lcdX->display(MouseX);
    lcdY->display(MouseY);
   */ 
}

void calibrador::sliderLimiarPInfValueChanged( int valor )
{
    if(valor != spinLimiarPInf->value() ){
	if(valor >= sliderLimiarPSup->value() ){
	    sliderLimiarPSup->setValue(valor+1);
	}
	spinLimiarPInf->setValue(valor);
	X.setPinf(valor);		
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
	X.setPinf(valor);	
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
	X.setPsup(valor);
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
	X.setPsup(valor);	
//	novosParametros = true;
	//novosLimites = true;
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
	//novosLimites = true;
    }
}

void calibrador::ExposureValueChanged(int valor)
{
    if(valor != spinContrast->value() ){
    //if(valor != spinExposure->value() ){
	spinContrast->setValue(valor);
	//spinExposure->setValue(valor);
	X.setExposure(valor);
	//novosLimites = true;
    }
    if(valor != sliderContrast->value() ){
   // if(valor != sliderExposure->value() ){
	sliderContrast->setValue(valor);
	//sliderExposure->setValue(valor);
	X.setExposure(valor);
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
    PARAMETROS_CAMERA prov2 = X.getCameraParam();
    spinBrightness->setValue(prov2.brightness);
    spinContrast->setValue(prov2.exposure);
    //spinExposure->setValue(prov2.exposure);
    spinHue->setValue(prov2.hue);
    spinSaturation->setValue(prov2.saturation);
    spinGamma->setValue(prov2.gamma);
    spinShutter->setValue(prov2.shutter);
    spinGain->setValue(prov2.gain);
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
    //novosLimites = true;
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
	X.setParameters();
	novosParametros = false;
    }
}

void calibrador::processarImagem()
{
//    static ImagemRGB mimimi("imagem_clara.ppm");
    if(telaAtual == 0 || checkShooting->isChecked() || novosLimites){
	X.processImage();
	pixmap_label1->loadFromData((uchar*)X.getPNMdata(),
				    X.getPNMsize(),"PPM");

	pixmap_label1->redesenhe();
	novosLimites = false;
    }
  
/*    pixmap_label1->loadFromData((uchar*)mimimi.getPNMData(),
				mimimi.getPNMSize(),"PPM");
    pixmap_label1->redesenhe();	
*/    
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

