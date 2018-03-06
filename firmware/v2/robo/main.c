/**
 *   Firmware dos Robôs do time POTI da UFRN
 *   DCA - Departamento de Computação e Automação / UFRN
 *   30/005/2012
 *
 *   O código foi feito para utilizar o transceiver nRF24L01+, que se
 *   comunica com o pic através do protocolo SPI. O PIC vai configurar
 *   suas saidas, modulos, timers e depois configura o transceiver. O
 *   ID do robô é definido por um switch e lido UMA VEZ na
 *   inicialização. O ID do robô pode assumir valores 0, 1 ou 2. O
 *   switch permite setar o ID para 3, porém caso isso aconteça o robô
 *   não inicializará e seus 3 leds piscarão, indicando um erro. No
 *   futuro, o ID = 3 pode iniciar o robô em algum outro modo de
 *   operação, como por exemplo um modo de debug, mas atualmente isso
 *   não está implementado. A frequencia também é definida por um
 *   switch e é lida UMA VEZ na inicialização. Para mudar os valores
 *   de frequencia e ID deve-se ajustar os respectivos switchs e
 *   RESETAR o robô. O código utiliza um bootloader pela USB para
 *   realizar a regravação do PIC. Dados internos do robô podem ser
 *   lidos pela USB durante seu funcionamento, desde que o código seja
 *   alterado de acordo para enviar os dados a serem analizados. Essa
 *   função é extremamente importante na fase de debug do código.
 *
 *   TODO:
 *   - RecepÃ§Ã£o de apenas 2 pacotes.
 *   - Usar pacotes individuais para se comunicar com cada robô.
 *   - Mudar o pacote para receber 11 bits de informação por motor.
 *
 *   TODO Antigos:
 *      Desempacotamento dos dados
 *	ConfiguraÃ§Ã£o de Zona Morta
 *
 *   @author Ellon Paiva
 *   @version 1.1
 */

#ifndef MAIN_C
#define MAIN_C

/** INCLUDES *******************************************************/
#include "./USB/usb.h"
#include "HardwareProfile.h"
#include "./USB/usb_function_hid.h"

/* #include "./USB/usb.h" */
/* #include "HardwareProfile.h" */
/* #include "./USB/usb_function_hid.h" */
/* #include "GenericTypeDefs.h" */
/* #include "Compiler.h" */
/* #include "usb_config.h" */
/* #include "./USB/usb_device.h" */
	
#include "nRF24L01.h"
	
#include <timers.h>
#include <delays.h>
#include <usart.h>
#include <spi.h>
#include <portb.h>
#include <pwm.h>


/** DEFINES ******************************************************/
//Define as portas de saida que serao utilizadas pelo PIC e suas
//respectivas portas TRIS
#define CSN_TRIS TRISBbits.TRISB7 
#define CSN  PORTBbits.RB7
#define IRQ_TRIS TRISBbits.TRISB2 
#define IRQ  PORTBbits.RB2
#define CE_TRIS  TRISBbits.TRISB6 
#define CE   PORTBbits.RB6

//Define as portas que serao utilizadas como leds e seus respectivos TRIS.
#define LED1_TRIS TRISAbits.TRISA2
#define LED1 PORTAbits.RA2 
#define LED2_TRIS TRISCbits.TRISC0 
#define LED2 PORTCbits.RC0         
#define LED3_TRIS TRISCbits.TRISC6 
#define LED3 PORTCbits.RC6         

//Define os bits que serao lidos para definir ID e FREQ
#define SWCH_ID0_TRIS TRISAbits.TRISA4
#define SWCH_ID0 PORTAbits.RA4
#define SWCH_ID1_TRIS TRISAbits.TRISA5
#define SWCH_ID1 PORTAbits.RA5
#define SWCH_FREQ0_TRIS TRISBbits.TRISB3
#define SWCH_FREQ0 PORTBbits.RB3
#define SWCH_FREQ1_TRIS TRISBbits.TRISB5
#define SWCH_FREQ1 PORTBbits.RB5

//Define os bits de direcao dos motores direito e esquerdo
#define SENTIDO_MOTOR_DIR_TRIS TRISAbits.TRISA0
#define SENTIDO_MOTOR_DIR PORTAbits.RA0
#define SENTIDO_MOTOR_ESQ_TRIS TRISAbits.TRISA1
#define SENTIDO_MOTOR_ESQ PORTAbits.RA1

//Define as mascaras para extrair o bit de sentido e o valor do pwm.
#define MASK_SENTIDO_MOTOR 0b10000000
#define MASK_SENTIDO_MOTOR_11BITS 0b00000100

#define MASK_PWM_MOTOR     0b01111111
#define MASK_PWM_MOTOR_HIGH 0b00000011
 
//Define o valor maximo do Duty Cycle para um PWM de 10 bits
#define MAX_DUTY_CYCLE 1023

//PWM_PERIOD vai ser utilizado para iniciar o PWM pela funcao
//OpenPWM1 e OpenPWM2, e ela sera definida como sendo 255. Esse valor
//vai ser armazenado no registrador PR2 pela funcao OpenPWMx(). Essa
//configuracao define a frequencia do PWM como sendo 11.719kHz, e
//permite uma resolucao da PWM de 12 bits. Como o registrador do PWM
//tem somente 10 bits, a resolucao utilizada vai ser 10 bits. A
//frequencia do PWM sera calculada por PWMfreq = 1/PWMperiod; onde
//PWMperiod = [PWM_PERIOD+1]*4*Tosc*TIMER2_PRESCALER
#define PWM_PERIOD 255


/** CONFIGURATION **************************************************/
#if defined(PICDEM_FS_USB)      // Configuration bits for PICDEM FS USB Demo Board (based on PIC18F4550)
#pragma config PLLDIV   = 5         // (20 MHz crystal on PICDEM FS USB board)
#pragma config CPUDIV   = OSC1_PLL2   
#pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
#pragma config FOSC     = HSPLL_HS
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF
#pragma config PWRT     = ON
#pragma config BOR      = ON
#pragma config BORV     = 2
#pragma config VREGEN   = ON      //USB Voltage Regulator
#pragma config WDT      = OFF
#pragma config WDTPS    = 32768
#pragma config MCLRE    = ON
#pragma config LPT1OSC  = OFF
#pragma config PBADEN   = OFF
#pragma config CCP2MX   = ON
#pragma config STVREN   = ON
#pragma config LVP      = OFF
//      #pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming
#pragma config XINST    = OFF       // Extended Instruction Set
#pragma config CP0      = OFF
#pragma config CP1      = OFF
//      #pragma config CP2      = OFF
//      #pragma config CP3      = OFF
#pragma config CPB      = OFF
//      #pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
//      #pragma config WRT2     = OFF
//      #pragma config WRT3     = OFF
#pragma config WRTB     = OFF       // Boot Block Write Protection
#pragma config WRTC     = OFF
//      #pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
//      #pragma config EBTR2    = OFF
//      #pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF


#elif defined(PIC18F87J50_PIM)				// Configuration bits for PIC18F87J50 FS USB Plug-In Module board
#pragma config XINST    = OFF   	// Extended instruction set
#pragma config STVREN   = ON      	// Stack overflow reset
#pragma config PLLDIV   = 3         // (12 MHz crystal used on this board)
#pragma config WDTEN    = OFF      	// Watch Dog Timer (WDT)
#pragma config CP0      = OFF      	// Code protect
#pragma config CPUDIV   = OSC1      // OSC1 = divide by 1 mode
#pragma config IESO     = OFF      	// Internal External (clock) Switchover
#pragma config FCMEN    = OFF      	// Fail Safe Clock Monitor
#pragma config FOSC     = HSPLL     // Firmware must also set OSCTUNE<PLLEN> to start PLL!
#pragma config WDTPS    = 32768
//      #pragma config WAIT     = OFF      	// Commented choices are
//      #pragma config BW       = 16      	// only available on the
//      #pragma config MODE     = MM      	// 80 pin devices in the 
//      #pragma config EASHFT   = OFF      	// family.
#pragma config MSSPMSK  = MSK5
//      #pragma config PMPMX    = DEFAULT
//      #pragma config ECCPMX   = DEFAULT
#pragma config CCP2MX   = DEFAULT   

#elif defined(PIC18F46J50_PIM) || defined(PIC18F_STARTER_KIT_1) || defined(PIC18F47J53_PIM)
#pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
#pragma config PLLDIV = 3           //Divide by 3 (12 MHz oscillator input)
#pragma config STVREN = ON          //stack overflow/underflow reset enabled
#pragma config XINST = OFF          //Extended instruction set disabled
#pragma config CPUDIV = OSC1        //No CPU system clock divide
#pragma config CP0 = OFF            //Program memory is not code-protected
#pragma config OSC = HSPLL          //HS oscillator, PLL enabled, HSPLL used by USB
#pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF           //Two-Speed Start-up disabled
#pragma config WDTPS = 32768        //1:32768
#pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock
#pragma config RTCOSC = T1OSCREF    //RTCC uses T1OSC/T1CKI as clock
#pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep
#pragma config DSWDTEN = OFF        //Disabled
#pragma config DSWDTPS = 8192       //1:8,192 (8.5 seconds)
#pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
#pragma config MSSP7B_EN = MSK7     //7 Bit address masking
#pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
#pragma config WPEND = PAGE_0       //Start protection at page 0
#pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
#pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored 
#if defined(PIC18F47J53_PIM)
#pragma config CFGPLLEN = OFF
#else
#pragma config T1DIG = ON           //Sec Osc clock source may be selected
#pragma config LPT1OSC = OFF        //high power Timer1 mode
#endif
#elif defined(LOW_PIN_COUNT_USB_DEVELOPMENT_KIT)
#pragma config CPUDIV = NOCLKDIV
#pragma config USBDIV = OFF
#pragma config FOSC   = HS
#pragma config PLLEN  = ON
#pragma config FCMEN  = OFF
#pragma config IESO   = OFF
#pragma config PWRTEN = OFF
#pragma config BOREN  = OFF
#pragma config BORV   = 30
#pragma config WDTEN  = OFF
#pragma config WDTPS  = 32768
#pragma config MCLRE  = OFF
#pragma config HFOFST = OFF
#pragma config STVREN = ON
#pragma config LVP    = OFF
#pragma config XINST  = OFF
#pragma config BBSIZ  = OFF
#pragma config CP0    = OFF
#pragma config CP1    = OFF
#pragma config CPB    = OFF
#pragma config WRT0   = OFF
#pragma config WRT1   = OFF
#pragma config WRTB   = OFF
#pragma config WRTC   = OFF
#pragma config EBTR0  = OFF
#pragma config EBTR1  = OFF
#pragma config EBTRB  = OFF                                                  // CONFIG7H
        
#elif defined(EXPLORER_16)
#if defined(__PIC24FJ256GB110__)
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & FWDTEN_OFF & ICS_PGx2) 
_CONFIG2( PLL_96MHZ_ON & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV2 & IOL1WAY_ON)
#elif defined(PIC24FJ256GB210_PIM)
_CONFIG1(FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & IOL1WAY_ON & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
#elif defined(__PIC24FJ64GB004__)
_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_ON)
_CONFIG3(WPFP_WPFP0 & SOSCSEL_SOSC & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)
#elif defined(__32MX460F512L__) || defined(__32MX795F512L__)
#pragma config UPLLEN   = ON        // USB PLL Enabled
#pragma config FPLLMUL  = MUL_15        // PLL Multiplier
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = HS            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
#pragma config DEBUG    = ON            // Background Debugger Enable
#elif defined(__dsPIC33EP512MU810__) || defined (__PIC24EP512GU810__)
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
_FWDT(FWDTEN_OFF);
    
#else
#error No hardware board defined, see "HardwareProfile.h" and __FILE__
#endif
#elif defined(PIC24F_STARTER_KIT)
_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
_CONFIG2( 0xF7FF & IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV3 & IOL1WAY_ON)
#elif defined(PIC24FJ256DA210_DEV_BOARD)
_CONFIG1(FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & IOL1WAY_ON & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
#elif defined(PIC32_USB_STARTER_KIT)
#pragma config UPLLEN   = ON        // USB PLL Enabled
#pragma config FPLLMUL  = MUL_15        // PLL Multiplier
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = HS            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
#pragma config DEBUG    = ON            // Background Debugger Enable
#elif defined(DSPIC33E_USB_STARTER_KIT)
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
_FWDT(FWDTEN_OFF);
#else
#error No hardware board defined, see "HardwareProfile.h" and __FILE__
#endif

	

/** VARIABLES ******************************************************/
#pragma udata

#if defined(__18F14K50) || defined(__18F13K50) || defined(__18LF14K50) || defined(__18LF13K50) 
#pragma udata usbram2
#elif defined(__18F2455) || defined(__18F2550) || defined(__18F4455) || defined(__18F4550) \
  || defined(__18F2458) || defined(__18F2453) || defined(__18F4558) || defined(__18F4553)
#pragma udata USB_VARIABLES=0x500
#elif defined(__18F4450) || defined(__18F2450)
#pragma udata USB_VARIABLES=0x480
#else
#pragma udata
#endif

unsigned char ReceivedDataBuffer[64];
unsigned char ToSendDataBuffer[64];
#pragma udata

USB_HANDLE USBOutHandle = 0;
USB_HANDLE USBInHandle = 0;
BOOL blinkStatusValid = TRUE;

//Variaveis relativas a logica do robo
unsigned char id = 0;
unsigned int pwm_dir = 0;
unsigned int pwm_esq = 0;
unsigned int sentido_dir = 0;
unsigned int sentido_esq = 0;


//Variaveis relativas ao transceiver nRF24l01+
//int payload = 8; //8 bytes
int payload = 14; //14 bytes TODO: Descomentar essa linha quando utilizar 11 bits por motor.
unsigned int channel = 0;
unsigned char my_status = 0;
unsigned char fifo_status = 0;
//unsigned char dados[8];
unsigned char dados[14]; //TODO: Descomentar essa linha quando utilizar 11 bits por motor.
int hasNewData = 0;
int j;
int i;	
//addr_id_x[addr_byte]; Variaveis que vão manter os enderecos a serem
//utilizados pelos robos; O endereço setado no transceiver vai 
//depender do id lido na fase de inicialização. Atualmente, todos
//os endereços serão iguais, visto que é enviado somente um pacote para
//todos os robôs. Mas deverão ser mudados quando for implementado o 
//envio de um pacote por robô.
//ATENCAO: OS VALORES ABAIXO TEM QUE SER IGUAIS AO CONTIDO NO FIRMWARE
//DO TRANSMISSOR!
unsigned char addr_id_0[5] = {0xCE, 0xCE, 0xCE, 0xCE, 0xCE};
unsigned char addr_id_1[5] = {0xCE, 0xCE, 0xCE, 0xCE, 0xCE};
unsigned char addr_id_2[5] = {0xCE, 0xCE, 0xCE, 0xCE, 0xCE};

/** PRIVATE PROTOTYPES *********************************************/
void BlinkUSBStatus(void);
BOOL Switch2IsPressed(void);
BOOL Switch3IsPressed(void);
static void InitializeSystem(void);
void ProcessIO(void);
void UserInit(void);
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void USBCBSendResume(void);
WORD_VAL ReadPOT(void);

/** VECTOR REMAPPING ***********************************************/
#if defined(__18CXX)
//On PIC18 devices, addresses 0x00, 0x08, and 0x18 are used for
//the reset, high priority interrupt, and low priority interrupt
//vectors.  However, the current Microchip USB bootloader 
//examples are intended to occupy addresses 0x00-0x7FF or
//0x00-0xFFF depending on which bootloader is used.  Therefore,
//the bootloader code remaps these vectors to new locations
//as indicated below.  This remapping is only necessary if you
//wish to program the hex file generated from this project with
//the USB bootloader.  If no bootloader is used, edit the
//usb_config.h file and comment out the following defines:
//#define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER
//#define PROGRAMMABLE_WITH_USB_LEGACY_CUSTOM_CLASS_BOOTLOADER
	
#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)
#define REMAPPED_RESET_VECTOR_ADDRESS			0x1000
#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x1008
#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x1018
#elif defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)	
#define REMAPPED_RESET_VECTOR_ADDRESS			0x800
#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x808
#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x818
#else	
#define REMAPPED_RESET_VECTOR_ADDRESS			0x00
#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x08
#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x18
#endif
	
#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
extern void _startup (void);        // See c018i.c in your C18 compiler dir
#pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS
void _reset (void)
{
  _asm goto _startup _endasm
    }
#endif
#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS
void Remapped_High_ISR (void)
{
  _asm goto YourHighPriorityISRCode _endasm
    }
#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS
void Remapped_Low_ISR (void)
{
  _asm goto YourLowPriorityISRCode _endasm
    }
	
#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
//Note: If this project is built while one of the bootloaders has
//been defined, but then the output hex file is not programmed with
//the bootloader, addresses 0x08 and 0x18 would end up programmed with 0xFFFF.
//As a result, if an actual interrupt was enabled and occured, the PC would jump
//to 0x08 (or 0x18) and would begin executing "0xFFFF" (unprogrammed space).  This
//executes as nop instructions, but the PC would eventually reach the REMAPPED_RESET_VECTOR_ADDRESS
//(0x1000 or 0x800, depending upon bootloader), and would execute the "goto _startup".  This
//would effective reset the application.
	
//To fix this situation, we should always deliberately place a 
//"goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS" at address 0x08, and a
//"goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS" at address 0x18.  When the output
//hex file of this project is programmed with the bootloader, these sections do not
//get bootloaded (as they overlap the bootloader space).  If the output hex file is not
//programmed using the bootloader, then the below goto instructions do get programmed,
//and the hex file still works like normal.  The below section is only required to fix this
//scenario.
#pragma code HIGH_INTERRUPT_VECTOR = 0x08
void High_ISR (void)
{
  _asm goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS _endasm
    }
#pragma code LOW_INTERRUPT_VECTOR = 0x18
void Low_ISR (void)
{
  _asm goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS _endasm
    }
#endif	//end of "#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_LEGACY_CUSTOM_CLASS_BOOTLOADER)"

#pragma code
	
	
//These are your actual interrupt handling routines.
#pragma interrupt YourHighPriorityISRCode
void YourHighPriorityISRCode()
{
  //Check which interrupt flag caused the interrupt.
  //Service the interrupt
  //Clear the interrupt flag
  //Etc.

  if(INTCON3bits.INT2IF == 1)//Foi uma interrupcao do Transceiver.
    {

      INTCON3bits.INT2IF = 0; //limpa o flag de interrupÃ§Ã£o externa 2 (trans)
				
      //LÃª o status para verificar qual tipo de interrupÃ§Ã£o foi gerada
      CSN = 0;
      WriteSPI(R_REGISTER | STATUS);
      my_status = ReadSPI();
      CSN = 1;
      if((my_status & 0b01000000) != 0){ //Verifica se foi uma
					 //interrupcao devido a
					 //chegada de um novo pacote
	while(1){
	  //3) Checa se tem dados disponiveis no RX FIFO (lendo o FIFO_STATUS)
	  CSN = 0;
	  WriteSPI(R_REGISTER | FIFO_STATUS);
	  fifo_status = ReadSPI();
	  CSN = 1;
	  //4) Se nao tem novos dados, saia do laco.
	  if((fifo_status & 0b00000001) != 0){
	    break; //Nao tem novos dados em RX_FIFO
	  }	
						
	  //1) Le o pacote
	  CSN = 0;
	  WriteSPI(R_RX_PAYLOAD);
	  for(j=0;j<payload;j++){
	    dados[j] = ReadSPI();	
	  }	
	  CSN = 1;	
						
	  //2) Limpa o bit de interrupcao
	  my_status = my_status | (1<<RX_DR);
	  CSN = 0;
	  WriteSPI(W_REGISTER | STATUS);
	  WriteSPI(my_status);
	  CSN = 1;
	}//end while(1)
	hasNewData = 1;
	LED2 ^= 1; //pisca led para verificar que recebeu pacote
      }//end if((my_status & 0b01000000) != 0)
      if((my_status & 0b00010000) != 0){ //Verifica se a interrupcao foi devido ao maximo de retransmissoes.
	//Limpa o bit de interrupcao
	my_status = my_status | (1<<MAX_RT);
	CSN = 0;
	WriteSPI(W_REGISTER | STATUS);
	WriteSPI(my_status);
	CSN = 1;
      }//end if((my_status & 0b01000000) != 0)
    }//end if(INTCON3bits.INT2IF == 1)

#if defined(USB_INTERRUPT)
  USBDeviceTasks();
#endif
	
}	//This return will be a "retfie fast", since this is in a #pragma interrupt section 
#pragma interruptlow YourLowPriorityISRCode
void YourLowPriorityISRCode()
{
  //Check which interrupt flag caused the interrupt.
  //Service the interrupt
  //Clear the interrupt flag
  //Etc.
	
}	//This return will be a "retfie", since this is in a #pragma interruptlow section 
#endif




/** DECLARATIONS ***************************************************/
#pragma code

/********************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *******************************************************************/
#if defined(__18CXX)
void main(void)
#else
  int main(void)
#endif
{   
  InitializeSystem();

#if defined(USB_INTERRUPT)
  USBDeviceAttach();
#endif

  while(1)
    {
#if defined(USB_POLLING)
      // Check bus status and service USB interrupts.
      USBDeviceTasks(); // Interrupt or polling method.  If using polling, must call
      // this function periodically.  This function will take care
      // of processing and responding to SETUP transactions 
      // (such as during the enumeration process when you first
      // plug in).  USB hosts require that USB devices should accept
      // and process SETUP packets in a timely fashion.  Therefore,
      // when using polling, this function should be called 
      // regularly (such as once every 1.8ms or faster** [see 
      // inline code comments in usb_device.c for explanation when
      // "or faster" applies])  In most cases, the USBDeviceTasks() 
      // function does not take very long to execute (ex: <100 
      // instruction cycles) before it returns.
#endif
    				  

      // Application-specific tasks.
      // Application related code may be added here, or in the ProcessIO() function.
      ProcessIO();        
    }//end while
}//end main


/********************************************************************
 * Function:        static void InitializeSystem(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        InitializeSystem is a centralize initialization
 *                  routine. All required USB initialization routines
 *                  are called from here.
 *
 *                  User application initialization routine should
 *                  also be called from here.                  
 *
 * Note:            None
 *******************************************************************/
static void InitializeSystem(void)
{
#if (defined(__18CXX) & !defined(PIC18F87J50_PIM))
  ADCON1 |= 0x0F;                 // Default all pins to digital
#elif defined(__C30__)
#if defined(__PIC24FJ256DA210__) || defined(__PIC24FJ256GB210__)
  ANSA = 0x0000;
  ANSB = 0x0000;
  ANSC = 0x0000;
  ANSD = 0x0000;
  ANSE = 0x0000;
  ANSF = 0x0000;
  ANSG = 0x0000;
#elif defined(__dsPIC33EP512MU810__) || defined (__PIC24EP512GU810__)
  ANSELA = 0x0000;
  ANSELB = 0x0000;
  ANSELC = 0x0000;
  ANSELD = 0x0000;
  ANSELE = 0x0000;
  ANSELG = 0x0000;
            
  // The dsPIC33EP512MU810 features Peripheral Pin
  // select. The following statements map UART2 to 
  // device pins which would connect to the the 
  // RX232 transciever on the Explorer 16 board.

  RPINR19 = 0;
  RPINR19 = 0x64;
  RPOR9bits.RP101R = 0x3;

#else
  AD1PCFGL = 0xFFFF;
#endif        
#elif defined(__C32__)
  AD1PCFG = 0xFFFF;
#endif

#if defined(PIC18F87J50_PIM) || defined(PIC18F46J50_PIM) || defined(PIC18F_STARTER_KIT_1) || defined(PIC18F47J53_PIM)
  //On the PIC18F87J50 Family of USB microcontrollers, the PLL will not power up and be enabled
  //by default, even if a PLL enabled oscillator configuration is selected (such as HS+PLL).
  //This allows the device to power up at a lower initial operating frequency, which can be
  //advantageous when powered from a source which is not gauranteed to be adequate for 48MHz
  //operation.  On these devices, user firmware needs to manually set the OSCTUNE<PLLEN> bit to
  //power up the PLL.
  {
    unsigned int pll_startup_counter = 600;
    OSCTUNEbits.PLLEN = 1;  //Enable the PLL and wait 2+ms until the PLL locks before enabling USB module
    while(pll_startup_counter--);
  }
  //Device switches over automatically to PLL output after PLL is locked and ready.
#endif

#if defined(PIC18F87J50_PIM)
  //Configure all I/O pins to use digital input buffers.  The PIC18F87J50 Family devices
  //use the ANCONx registers to control this, which is different from other devices which
  //use the ADCON1 register for this purpose.
  WDTCONbits.ADSHR = 1;			// Select alternate SFR location to access ANCONx registers
  ANCON0 = 0xFF;                  // Default all pins to digital
  ANCON1 = 0xFF;                  // Default all pins to digital
  WDTCONbits.ADSHR = 0;			// Select normal SFR locations
#endif

#if defined(__dsPIC33EP512MU810__) || defined (__PIC24EP512GU810__)

  // Configure the device PLL to obtain 60 MIPS operation. The crystal
  // frequency is 8MHz. Divide 8MHz by 2, multiply by 60 and divide by
  // 2. This results in Fosc of 120MHz. The CPU clock frequency is
  // Fcy = Fosc/2 = 60MHz. Wait for the Primary PLL to lock and then
  // configure the auxilliary PLL to provide 48MHz needed for USB 
  // Operation.

  PLLFBD = 38;				/* M  = 60	*/
  CLKDIVbits.PLLPOST = 0;		/* N1 = 2	*/
  CLKDIVbits.PLLPRE = 0;		/* N2 = 2	*/
  OSCTUN = 0;			

  /*	Initiate Clock Switch to Primary
   *	Oscillator with PLL (NOSC= 0x3)*/
	
  __builtin_write_OSCCONH(0x03);		
  __builtin_write_OSCCONL(0x01);
  while (OSCCONbits.COSC != 0x3);       

  // Configuring the auxiliary PLL, since the primary
  // oscillator provides the source clock to the auxiliary
  // PLL, the auxiliary oscillator is disabled. Note that
  // the AUX PLL is enabled. The input 8MHz clock is divided
  // by 2, multiplied by 24 and then divided by 2. Wait till 
  // the AUX PLL locks.

  ACLKCON3 = 0x24C1;   
  ACLKDIV3 = 0x7;
  ACLKCON3bits.ENAPLL = 1;
  while(ACLKCON3bits.APLLCK != 1); 

#endif

#if defined(PIC18F46J50_PIM) || defined(PIC18F_STARTER_KIT_1) || defined(PIC18F47J53_PIM)
  //Configure all I/O pins to use digital input buffers.  The PIC18F87J50 Family devices
  //use the ANCONx registers to control this, which is different from other devices which
  //use the ADCON1 register for this purpose.
  ANCON0 = 0xFF;                  // Default all pins to digital
  ANCON1 = 0xFF;                  // Default all pins to digital
#endif
    
#if defined(PIC24FJ64GB004_PIM) || defined(PIC24FJ256DA210_DEV_BOARD)
  //On the PIC24FJ64GB004 Family of USB microcontrollers, the PLL will not power up and be enabled
  //by default, even if a PLL enabled oscillator configuration is selected (such as HS+PLL).
  //This allows the device to power up at a lower initial operating frequency, which can be
  //advantageous when powered from a source which is not gauranteed to be adequate for 32MHz
  //operation.  On these devices, user firmware needs to manually set the CLKDIV<PLLEN> bit to
  //power up the PLL.
  {
    unsigned int pll_startup_counter = 600;
    CLKDIVbits.PLLEN = 1;
    while(pll_startup_counter--);
  }

  //Device switches over automatically to PLL output after PLL is locked and ready.
#endif


  //	The USB specifications require that USB peripheral devices must never source
  //	current onto the Vbus pin.  Additionally, USB peripherals should not source
  //	current on D+ or D- when the host/hub is not actively powering the Vbus line.
  //	When designing a self powered (as opposed to bus powered) USB peripheral
  //	device, the firmware should make sure not to turn on the USB module and D+
  //	or D- pull up resistor unless Vbus is actively powered.  Therefore, the
  //	firmware needs some means to detect when Vbus is being powered by the host.
  //	A 5V tolerant I/O pin can be connected to Vbus (through a resistor), and
  // 	can be used to detect when Vbus is high (host actively powering), or low
  //	(host is shut down or otherwise not supplying power).  The USB firmware
  // 	can then periodically poll this I/O pin to know when it is okay to turn on
  //	the USB module/D+/D- pull up resistor.  When designing a purely bus powered
  //	peripheral device, it is not possible to source current on D+ or D- when the
  //	host is not actively providing power on Vbus. Therefore, implementing this
  //	bus sense feature is optional.  This firmware can be made to use this bus
  //	sense feature by making sure "USE_USB_BUS_SENSE_IO" has been defined in the
  //	HardwareProfile.h file.    
#if defined(USE_USB_BUS_SENSE_IO)
  tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
#endif
    
  //	If the host PC sends a GetStatus (device) request, the firmware must respond
  //	and let the host know if the USB peripheral device is currently bus powered
  //	or self powered.  See chapter 9 in the official USB specifications for details
  //	regarding this request.  If the peripheral device is capable of being both
  //	self and bus powered, it should not return a hard coded value for this request.
  //	Instead, firmware should check if it is currently self or bus powered, and
  //	respond accordingly.  If the hardware has been configured like demonstrated
  //	on the PICDEM FS USB Demo Board, an I/O pin can be polled to determine the
  //	currently selected power source.  On the PICDEM FS USB Demo Board, "RA2" 
  //	is used for	this purpose.  If using this feature, make sure "USE_SELF_POWER_SENSE_IO"
  //	has been defined in HardwareProfile.h, and that an appropriate I/O pin has been mapped
  //	to it in HardwareProfile.h.
#if defined(USE_SELF_POWER_SENSE_IO)
  tris_self_power = INPUT_PIN;	// See HardwareProfile.h
#endif

  UserInit();
    
  USBDeviceInit();	//usb_device.c.  Initializes USB module SFRs and firmware
  //variables to known states.
}//end InitializeSystem



/******************************************************************************
 * Function:        void UserInit(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine should take care of all of the demo code
 *                  initialization that is required.
 *
 * Note:            
 *
 *****************************************************************************/
void UserInit(void)
{

  /**************************************************************************
   * Inicializa parametros do microcontrolador
   **************************************************************************/
  //Define as entradas das chaves que definem o ID e a FREQ
  SWCH_ID0_TRIS = 1; 
  SWCH_ID1_TRIS = 1;
  SWCH_FREQ0_TRIS = 1;
  SWCH_FREQ1_TRIS = 1;

  //Pinos transceiver
  CSN_TRIS = 0; //Define CSN como saida.
  IRQ_TRIS = 1; //Define IRQ como entrada.
  CE_TRIS  = 0; //Define CE como saida.
		
  //Desabilita EUSART, necessario para que o RC6 funcione como saida
  //digital. (RC6 sera utilizado pelo CCP para gerar o PWM do motor
  //esquerdo.
  CloseUSART();

  //Definindo porta para led's
  LED1_TRIS = 0; 
  LED2_TRIS = 0; 
  LED3_TRIS = 0; 
  
  //Inicia com os led's desligados.
  LED1 = 0; 
  LED2 = 0; 
  LED3 = 0; 

  //Define os pinos que indicam o sentido de giro dos motores como
  //saidas digitais
  SENTIDO_MOTOR_DIR_TRIS = 0;
  SENTIDO_MOTOR_ESQ_TRIS = 0;

  //Define o valor inicial do sentido do motor como sentido direto.
  //0 = sentido direto 
  //1 = sentido reverso
  SENTIDO_MOTOR_DIR = 0;
  SENTIDO_MOTOR_ESQ = 0;

  //Define o valor inicial dos pinos de controle do tranceiver.
  CE = 0;
  CSN = 1;

  //Habilita interrupcao INT2, que fica no pino RB2.
  OpenRB2INT(PORTB_CHANGE_INT_ON & 
	     FALLING_EDGE_INT &
	     PORTB_PULLUPS_ON);
				   
  INTCON3bits.INT2IP = 1; //Seta INT2 como alta prioridade

  /*******************************************************************
   * Inicializa algumas variaveis utilizadas pelo USB (Codigo mantido
   * da versÃ£o anterior).
   *******************************************************************/
  //initialize the variable holding the handle for the last
  // transmission
  USBOutHandle = 0;
  USBInHandle = 0;

  //Esvazia buffer de recepcao e transmissao
  for(i=0; i<64; i++){
    ReceivedDataBuffer[i] = 0;
    ToSendDataBuffer[i] = 0;
  }

  /**************************************************************************
   * Ler os dados das chaves para setar o ID do robo e o canal de transmissao.
   **************************************************************************/
  id = (SWCH_ID1 << 1) | SWCH_ID0; //Ler o ID das portas conectadas a
				   //chave (0, 1 ou 2. 3 não é permitido.
  
  //O ID 3 não é válido. Caso ele seja selecionado o robô irá travar
  //na inicialização e piscará os 3 leds, indicando erro.
  if(id == 3){
    while(1){
      //Pisca os leds
      LED1 ^= 1;
      LED2 ^= 1;
      LED3 ^= 1;
      //Espera meio segundo.
      for(i=0; i<5;i++){
	Delay10KTCYx(120); //Delay de 0.1s
      }
    }
  }

  //Ler os bits que definem a frequencia a ser utilizada das
  //chaves. Esse valor vai ser de 0 a 3. Foi definido que
  //utilizariamos 4 tipos de frequencias: 2010MHz, 2020MHz, 2030MHz e
  //2040MHz. Utilizar essas frequencias significa utilizar os canais
  //10, 20, 30, e 40, respectivamente. Logo, para calcular o canal
  //correto, soma-se 1 ao valor lido das chaves e multiplica o
  //resultado por 10.
  //channel = (((SWCH_FREQ1 << 1) | SWCH_FREQ0)+1)*10; 
  //  channel = (((SWCH_FREQ1 << 1) | SWCH_FREQ0))*30 + 10;

	if(SWCH_FREQ1 == 0 && SWCH_FREQ0 == 0){
		channel = 10;	
	}else if(SWCH_FREQ1 == 0 && SWCH_FREQ0 == 1){
		channel = 40;		
	}else if(SWCH_FREQ1 == 1 && SWCH_FREQ0 == 0){
		channel = 70;		
	}else if(SWCH_FREQ1 == 1 && SWCH_FREQ0 == 1){
		channel = 100;		
	}


  /*************************************************************
   * Inicializa o mÃ³dulo SPI
   *************************************************************/
  OpenSPI(SPI_FOSC_4, MODE_00, SMPEND);

  /**************************************************************************
   * Configura o transceiver
   **************************************************************************/

  //QUADRO A SER RECEBIDO:
  // ________________________________________
  //|_CAB_|_E0_|_D0_|_E1_|_D1_|_E2_|_D2_|_FQ_|

  //Realiza um delay de 150ms, que Ã© o tempo necessario para fazer a
  //inicializaÃ§Ã£o do transceiver. NÃ£o se sabe ao certo quanto tempo ja
  //se passou desde o inicio da execucao do codigo, mas como o tempo
  //de inicializacao sera 100ms, o delay aqui aplicado sera > 100ms.
  Delay10KTCYx(180); //Pausa de 150ms

  //Coloca o transmissor no modo Power Down. Esse processo sera
  //necessario no caso do PIC estar sendo reiniciado apos um reset,
  //pois o transceiver teria sido configurado e estaria em modo
  //Power Up e os comandos de configuracao nao funcionariam. No caso
  //do pic estar sendo configurado a primeira vez apos ser alimentado,
  //esse comando nÃ£o causa problemas.
  CSN = 0;
  WriteSPI(W_REGISTER | CONFIG); //Comando de escrita em registrador
				 //combinado com endereco do
				 //registrador
  WriteSPI(STARTUP_CONFIG | (0<<PWR_UP));
  CSN = 1;

  //Configura transceiver em 2Mbps e usando a potencia de 0dBm
  CSN = 0;
  WriteSPI(W_REGISTER | RF_SETUP); 
  //WriteSPI((1 << RF_DR_HIGH)|(0b11 << RF_PWR)); // 2Mbps e usando a potência de 0dBm
  //WriteSPI((0 << RF_DR_HIGH)|(0b11 << RF_PWR)); // 1Mbps e usando a potência de 0dBm
  WriteSPI(((1 << RF_DR_LOW)|(0b11 << RF_PWR))&(~(1 << RF_DR_HIGH)));  // 250Kbps e usando a potência de 0dBm
  CSN = 1;
		
  //Configura frequencia do transceiver para a frequencia definida na
  //variavel 'channel'. Ela pode ser 2410MHz, 2440MHz, 2470MHz ou
  //2500MHz.
  CSN = 0;
  WriteSPI(W_REGISTER | RF_CH); 
  WriteSPI(channel);
  CSN = 1;

  //Muda o endereço do receptor para um endereço diferente do
  //default. Isso é feito para reduzir as chances de recebermos
  //pacotes de outros transceivers que estejam trabalhando no mesmo
  //canal e estejam utilizando o endereço default. Endereço Utilizado:
  //0xCECECECECE
  CSN = 0;
  WriteSPI(W_REGISTER | RX_ADDR_P0);
  for(i=0;i<5;i++){
    if(id == 0){
      WriteSPI(addr_id_0[i]);
    }
    if(id == 1){
      WriteSPI(addr_id_1[i]);
    }
    if(id == 2){
      WriteSPI(addr_id_2[i]);
    }
  }
  CSN = 1;
  
  //Define o payload a ser utilizado. O transceiver vai esperar
  //receber 'payload' bytes até informar ao PIC que chegou um novo
  //pacote.
  CSN = 0;
  WriteSPI(W_REGISTER | RX_PW_P0);
  WriteSPI(payload);
  CSN = 1;

  //Coloca o transceiver em modo Power Up, setando ele como
  //receptor. É recomendado realizar uma pequena pausa apos esse
  //comando para esperar o oscilador do transceiver ser inicializado.
  CSN = 0;
  WriteSPI(W_REGISTER | CONFIG); //Comando de escrita em registrador
				 //combinado com endereco do
				 //registrador
  WriteSPI(STARTUP_CONFIG | (1<<PWR_UP | 1<<PRIM_RX));
  CSN = 1;
		
  //Realiza uma pausa de 200us, que sera um pouco maior que o tempo de
  //start up do oscilador até o transceiver entrar no modo Standby-I.
  Delay100TCYx(24); //Delay de 200us
  		
  //Esvaziando buffer de recepcao
  CSN = 0;
  WriteSPI(FLUSH_RX);
  CSN = 1;
		
  //Esvaziando buffer de transmissao
  CSN = 0;
  WriteSPI(FLUSH_TX);
  CSN = 1;
		
  //Habilita o modo de recepcao.
  CE = 1;

  /*****************************************************************
   * Configura Timer e PWM
   *****************************************************************/
  
  //Desabilita os timers que nao serao utilizados
  CloseTimer0(); //Desabilitar o Timer0 tambem habilita a saida digital RC0
  CloseTimer1();
  CloseTimer3();
  
  //Habilita o timer2
  OpenTimer2(TIMER_INT_OFF & T2_PS_1_4 & T2_POST_1_1);

  OpenPWM1(PWM_PERIOD); //Inicia o PWM1, que vai acionar o motor direito
  OpenPWM2(PWM_PERIOD); //Inicia o PWM2, que vai acionar o motor esquerdo.

  //Aciona os motores com 50% da velocidade maxima. A funcao
  //SetDCPWMx(dutycycle) aceita valores de duty cycle entre 0 e 1023
  //(ou seja, são considerados os 10 bits menos significativos),
  //onde 0 significa nenhum duty cycle e 1023 significa duty cycle
  //cheio, ou seja, sinal completo.
  pwm_dir = 512;
  pwm_esq = 512;
  SetDCPWM1(pwm_dir);
  SetDCPWM2(pwm_esq);

  //Espera 2 segundos
  for(i=0;i<10;i++)
    Delay10KTCYx(240); //Delay de 0.2s
  
  //Desliga os motores.
  pwm_dir = 0;
  pwm_esq = 0;
  SetDCPWM1(pwm_dir);
  SetDCPWM2(pwm_esq);
	
}//end UserInit

/********************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user
 *                  routines. It is a mixture of both USB and
 *                  non-USB tasks.
 *
 * Note:            None
 *******************************************************************/
void ProcessIO(void)
{
  LED1 = 1;
  // User Application USB tasks
  //if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;
	    
  //  	if(!HIDRxHandleBusy(USBOutHandle)){
  //		//Re-arm the OUT endpoint for the next packet
  //	    USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
  //    }
	    
  //	csnLow();
  //	WriteSPI(W_TX_PAYLOAD);
  //	for(i = 7; i>=0 ; i--){
  //		WriteSPI(ReceivedDataBuffer[i]);			
  //	}
  //	csnHi();

  //Processa o novo dado e redefine o duty cycle do PWM se ele recebeu
  //algum na ultima interrupção.
  if(hasNewData == 1){
	  //Formato do pacote:
	  //|__0__|_01_|_02_|_03_|_04_|_05_|_06_|_07_|_08_|_09_|_10_|_11_|_12_|_13_|
	  //|_CAB_|_E0_|_E0_|_D0_|_D0_|_E1_|_E1_|_D1_|_D1_|_E2_|_E2_|_D2_|_D2_|_FQ_|
	  //
	  //Verifica se o cabeçalho e o fim de quadro estão corretos
//	  if(dados[0] == 0x80 && dados[7] == 0x7F)
//	  {
		if(dados[0] == 0x80 && dados[13] == 0x7F) //TODO: Descomentar essas linhas e eliminar as equivalentes atuais quando utilizar 11 bits por motor.
		{
		//Extrai o sentido de giro do motor direito e esquerdo 
		sentido_esq = (dados[4*id+1] & MASK_SENTIDO_MOTOR_11BITS) >> 2;
    	sentido_dir = (dados[4*id+3] & MASK_SENTIDO_MOTOR_11BITS) >> 2;
    
		//Extrai o valor de PWM dos dados recebidos. 
	    pwm_esq = (dados[4*id+1] & MASK_PWM_MOTOR_HIGH);
	    pwm_dir = (dados[4*id+3] & MASK_PWM_MOTOR_HIGH);
	    pwm_esq = pwm_esq << 8;
	    pwm_dir = pwm_dir << 8;
	    pwm_esq = pwm_esq | dados[4*id+2]; //TODO: Verificar se isso causa perda de dados
	    pwm_dir = pwm_dir | dados[4*id+4];
	
		
//    	//Extrai o sentido de giro do motor direito e esquerdo 
//    	sentido_esq = (dados[2*id+1] & MASK_SENTIDO_MOTOR) >> 7;
//    	sentido_dir = (dados[2*id+2] & MASK_SENTIDO_MOTOR) >> 7;
//    
//    	//Extrai o valor de PWM dos dados recebidos. O deslocamento Ã©
//    	//necessÃ¡rio pois o valor recebido para cada robÃ´ tem 7 bits de
//	    //dados e o PWM aceita 10 bits de duty cycle. Logo, a resoluÃ§Ã£o do
//	    //PWM Ã© menor do que a possivel.
//	    pwm_esq = (dados[2*id+1] & MASK_PWM_MOTOR);
//	    pwm_dir = (dados[2*id+2] & MASK_PWM_MOTOR);
//	    pwm_esq = pwm_esq << 3;
//	    pwm_dir = pwm_dir << 3;
//	
	    //Trata o PWM em funcao do sentido de giro desejado, ajustando o
	    //valor do PWM se necessario.
	    if(sentido_dir == 1){ //Motor direito girando no sentido reverso
	      pwm_dir = MAX_DUTY_CYCLE - pwm_dir; //Ajusta o valor de PWM
	    }
	    if(sentido_esq == 1){ //Motor esquerdo girando no sentido reverso
	      pwm_esq = MAX_DUTY_CYCLE - pwm_esq; //Ajusta o valor de PWM
	    }
	
	    //Aciona motor direito
	    SENTIDO_MOTOR_DIR = sentido_dir;
	    SetDCPWM1(pwm_dir);
	    
	    //Aciona motor esquerdo
	    SENTIDO_MOTOR_ESQ = sentido_esq;
	    SetDCPWM2(pwm_esq);
	
	    //TODO: Mudar o formato do pacote para enviar 10 bits de
	    //velocidade + 1 bit de sentido por motor, usando assim a
	    //resoluÃ§Ã£o mÃ¡xima permitida pelo PWM.
	
	    //TODO: Adicionar uma forma de proteÃ§Ã£o para o caso do robÃ´ passar
	    //muito tempo sem ouvir dados do transmissor. Provavelmente mandar
	    //ele parar (pwm_x = 0) possa ser a melhor opÃ§Ã£o, mas Ã© preciso
	    //analizar casos particulares, como o caso de testes atravÃ©s do
	    //programa 'simple'.
	    
	
	    //Pisca o led para saber que recebeu um dado novo e acionou os motores.
	    LED3 ^= 1;
	    
	    if(!HIDTxHandleBusy(USBInHandle))
	      {
			//		for(i = 0; i<payload; i++){
			//		  ToSendDataBuffer[i] = dados[i];
			//		}
				
			ToSendDataBuffer[0] = pwm_dir & 0xFF;
			ToSendDataBuffer[1] = (pwm_dir >> 8) & 0b00000011;
			ToSendDataBuffer[2] = pwm_esq & 0xFF;
			ToSendDataBuffer[3] = (pwm_esq >> 8) & 0b00000011;
				
			USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
		 }

	}
    //Processou o novo dado. Reseta a variavel.
    hasNewData = 0;

  }

  //	if(!HIDTxHandleBusy(USBInHandle))
  //    {
  //		USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
  //    }
  //    
  //	if(!HIDTxHandleBusy(USBInHandle))
  //    {
  //		USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&my_status,1);
  //    }
  //
	
}//end ProcessIO

/******************************************************************************
 * Function:        WORD_VAL ReadPOT(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          WORD_VAL - the 10-bit right justified POT value
 *
 * Side Effects:    ADC buffer value updated
 *
 * Overview:        This function reads the POT and leaves the value in the 
 *                  ADC buffer register
 *
 * Note:            None
 *****************************************************************************/
WORD_VAL ReadPOT(void)
{
  WORD_VAL w;

#if defined(__18CXX)
  mInitPOT();

  ADCON0bits.GO = 1;              // Start AD conversion
  while(ADCON0bits.NOT_DONE);     // Wait for conversion

  w.v[0] = ADRESL;
  w.v[1] = ADRESH;

#elif defined(__C30__) || defined(__C32__)
#if defined(PIC24FJ256GB110_PIM) ||		\
  defined(PIC24FJ256DA210_DEV_BOARD) ||		\
  defined(PIC24FJ256GB210_PIM)
  AD1CHS = 0x5;           //MUXA uses AN5

  // Get an ADC sample
  AD1CON1bits.SAMP = 1;           //Start sampling
  for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
  AD1CON1bits.SAMP = 0;           //Start sampling
  for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
  while(!AD1CON1bits.DONE);       //Wait for conversion to complete

#elif defined(DSPIC33EP512MU810_PIM) || defined (PIC24EP512GU810_PIM)
            
  // Routine to read the Explorer 16 potentiometer.
  // Get an ADC sample
  AD1CHS0bits.CH0SA = 5;\
  AD1CON1bits.SAMP = 1;           //Start sampling
  for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
  AD1CON1bits.SAMP = 0;           //Start sampling
  for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
  while(!AD1CON1bits.DONE);       //Wait for conversion to complete
        
#elif defined (DSPIC33E_USB_STARTER_KIT)
        
  w.Val = 0;
        
#elif defined(PIC24FJ64GB004_PIM)
  AD1CHS = 0x7;           //MUXA uses AN7

  // Get an ADC sample
  AD1CON1bits.SAMP = 1;           //Start sampling
  for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
  AD1CON1bits.SAMP = 0;           //Start sampling
  for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
  while(!AD1CON1bits.DONE);       //Wait for conversion to complete

#elif defined(PIC24F_STARTER_KIT)
  AD1CHS = 0x0;           //MUXA uses AN0

  // Get an ADC sample
  AD1CON1bits.SAMP = 1;           //Start sampling
  for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
  AD1CON1bits.SAMP = 0;           //Start sampling
  for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
  while(!AD1CON1bits.DONE);       //Wait for conversion to complete

#elif defined(PIC32MX460F512L_PIM) || defined(PIC32_USB_STARTER_KIT) || defined(PIC32MX795F512L_PIM)
  AD1PCFG = 0xFFFB; // PORTB = Digital; RB2 = analog
  AD1CON1 = 0x0000; // SAMP bit = 0 ends sampling ...
  // and starts converting
  AD1CHS = 0x00020000; // Connect RB2/AN2 as CH0 input ..
  // in this example RB2/AN2 is the input
  AD1CSSL = 0;
  AD1CON3 = 0x0002; // Manual Sample, Tad = internal 6 TPB
  AD1CON2 = 0;
  AD1CON1SET = 0x8000; // turn ADC ON

  AD1CON1SET = 0x0002; // start sampling ...
  for(w.Val=0;w.Val<1000;w.Val++); //Sample delay, conversion start automatically
  AD1CON1CLR = 0x0002; // start Converting
  while (!(AD1CON1 & 0x0001));// conversion done?
#else
#error
#endif

  w.Val = ADC1BUF0;

#endif

  return w;
}//end ReadPOT


/********************************************************************
 * Function:        void BlinkUSBStatus(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        BlinkUSBStatus turns on and off LEDs 
 *                  corresponding to the USB device state.
 *
 * Note:            mLED macros can be found in HardwareProfile.h
 *                  USBDeviceState is declared and updated in
 *                  usb_device.c.
 *******************************************************************/
void BlinkUSBStatus(void)
{
#if defined(PIC24FJ256DA210_DEV_BOARD)
  // No need to clear UIRbits.SOFIF to 0 here.
  // Callback caller is already doing that.
#define BLINK_INTERVAL 20000
#define BLANK_INTERVAL 200000

  static WORD blink_count=0;
  static DWORD loop_count = 0;
    
  if(loop_count == 0)
    {
      if(blink_count != 0)
        {
	  loop_count = BLINK_INTERVAL;
	  if(mGetLED_1())
            {
	      mLED_1_Off();
	      blink_count--;
            }
	  else
            {
	      mLED_1_On();
            }
        }
      else
        {
	  loop_count = BLANK_INTERVAL;
	  switch(USBDeviceState)
            {
	    case ATTACHED_STATE:
	      blink_count = 1;
	      break;
	    case POWERED_STATE:
	      blink_count = 2;
	      break;
	    case DEFAULT_STATE:
	      blink_count = 3;
	      break;
	    case ADR_PENDING_STATE:
	      blink_count = 4;
	      break;
	    case ADDRESS_STATE:
	      blink_count = 5;
	      break;
	    case CONFIGURED_STATE:
	      blink_count = 6;
	      break;
	    case DETACHED_STATE:
	      //fall through
	    default:
	      blink_count = 0;
	      break;
            }
        }
    }
  else
    {
      loop_count--;
    }

#else
  // No need to clear UIRbits.SOFIF to 0 here.
  // Callback caller is already doing that.
  static WORD led_count=0;
    
  if(led_count == 0)led_count = 10000U;
  led_count--;

#define mLED_Both_Off()         {mLED_1_Off();mLED_2_Off();}
#define mLED_Both_On()          {mLED_1_On();mLED_2_On();}
#define mLED_Only_1_On()        {mLED_1_On();mLED_2_Off();}
#define mLED_Only_2_On()        {mLED_1_Off();mLED_2_On();}

  if(USBSuspendControl == 1)
    {
      if(led_count==0)
        {
	  mLED_1_Toggle();
	  if(mGetLED_1())
            {
	      mLED_2_On();
            }
	  else
            {
	      mLED_2_Off();
            }
        }//end if
    }
  else
    {
      if(USBDeviceState == DETACHED_STATE)
        {
	  mLED_Both_Off();
        }
      else if(USBDeviceState == ATTACHED_STATE)
        {
	  mLED_Both_On();
        }
      else if(USBDeviceState == POWERED_STATE)
        {
	  mLED_Only_1_On();
        }
      else if(USBDeviceState == DEFAULT_STATE)
        {
	  mLED_Only_2_On();
        }
      else if(USBDeviceState == ADDRESS_STATE)
        {
	  if(led_count == 0)
            {
	      mLED_1_Toggle();
	      mLED_2_Off();
            }//end if
        }
      else if(USBDeviceState == CONFIGURED_STATE)
        {
	  if(led_count==0)
            {
	      mLED_1_Toggle();
	      if(mGetLED_1())
                {
		  mLED_2_Off();
                }
	      else
                {
		  mLED_2_On();
                }
            }//end if
        }
    }
#endif
}//end BlinkUSBStatus




// ******************************************************************************************************
// ************** USB Callback Functions ****************************************************************
// ******************************************************************************************************
// The USB firmware stack will call the callback functions USBCBxxx() in response to certain USB related
// events.  For example, if the host PC is powering down, it will stop sending out Start of Frame (SOF)
// packets to your device.  In response to this, all USB devices are supposed to decrease their power
// consumption from the USB Vbus to <2.5mA each.  The USB module detects this condition (which according
// to the USB specifications is 3+ms of no bus activity/SOF packets) and then calls the USBCBSuspend()
// function.  You should modify these callback functions to take appropriate actions for each of these
// conditions.  For example, in the USBCBSuspend(), you may wish to add code that will decrease power
// consumption from Vbus to <2.5mA (such as by clock switching, turning off LEDs, putting the
// microcontroller to sleep, etc.).  Then, in the USBCBWakeFromSuspend() function, you may then wish to
// add code that undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack will not automatically call this
// function.  This function is meant to be called from the application firmware instead.  See the
// additional comments near the function.

/******************************************************************************
 * Function:        void USBCBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Call back that is invoked when a USB suspend is detected
 *
 * Note:            None
 *****************************************************************************/
void USBCBSuspend(void)
{
  //Example power saving code.  Insert appropriate code here for the desired
  //application behavior.  If the microcontroller will be put to sleep, a
  //process similar to that shown below may be used:
	
  //ConfigureIOPinsForLowPower();
  //SaveStateOfAllInterruptEnableBits();
  //DisableAllInterruptEnableBits();
  //EnableOnlyTheInterruptsWhichWillBeUsedToWakeTheMicro();	//should enable at least USBActivityIF as a wake source
  //Sleep();
  //RestoreStateOfAllPreviouslySavedInterruptEnableBits();	//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.
  //RestoreIOPinsToNormal();									//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.

  //IMPORTANT NOTE: Do not clear the USBActivityIF (ACTVIF) bit here.  This bit is 
  //cleared inside the usb_device.c file.  Clearing USBActivityIF here will cause 
  //things to not work as intended.	
	

#if defined(__C30__)
  //This function requires that the _IPL level be something other than 0.
  //  We can set it here to something other than 
#ifndef DSPIC33E_USB_STARTER_KIT
  _IPL = 1;
  USBSleepOnSuspend();
#endif
#endif
}


/******************************************************************************
 * Function:        void _USB1Interrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the USB interrupt bit is set
 *					In this example the interrupt is only used when the device
 *					goes to sleep when it receives a USB suspend command
 *
 * Note:            None
 *****************************************************************************/
#if 0
void __attribute__ ((interrupt)) _USB1Interrupt(void)
{
#if !defined(self_powered)
  if(U1OTGIRbits.ACTVIF)
    {
      IEC5bits.USB1IE = 0;
      U1OTGIEbits.ACTVIE = 0;
      IFS5bits.USB1IF = 0;
        
      //USBClearInterruptFlag(USBActivityIFReg,USBActivityIFBitNum);
      USBClearInterruptFlag(USBIdleIFReg,USBIdleIFBitNum);
      //USBSuspendControl = 0;
    }
#endif
}
#endif

/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The host may put USB peripheral devices in low power
 *					suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *					mode, the host may wake the device back up by sending non-
 *					idle state signalling.
 *					
 *					This call back is invoked when a wakeup from USB suspend 
 *					is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
  // If clock switching or other power savings measures were taken when
  // executing the USBCBSuspend() function, now would be a good time to
  // switch back to normal full power run mode conditions.  The host allows
  // a few milliseconds of wakeup time, after which the device must be 
  // fully back to normal, and capable of receiving and processing USB
  // packets.  In order to do this, the USB module must receive proper
  // clocking (IE: 48MHz clock must be available to SIE for full speed USB
  // operation).
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void)
{
  // No need to clear UIRbits.SOFIF to 0 here.
  // Callback caller is already doing that.
}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void)
{
  // No need to clear UEIR to 0 here.
  // Callback caller is already doing that.

  // Typically, user firmware does not need to do anything special
  // if a USB error occurs.  For example, if the host sends an OUT
  // packet to your device, but the packet gets corrupted (ex:
  // because of a bad connection, or the user unplugs the
  // USB cable during the transmission) this will typically set
  // one or more USB error interrupt flags.  Nothing specific
  // needs to be done however, since the SIE will automatically
  // send a "NAK" packet to the host.  In response to this, the
  // host will normally retry to send the packet again, and no
  // data loss occurs.  The system will typically recover
  // automatically, without the need for application firmware
  // intervention.
	
  // Nevertheless, this callback function is provided, such as
  // for debugging purposes.
}


/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        When SETUP packets arrive from the host, some
 * 					firmware must process the request and respond
 *					appropriately to fulfill the request.  Some of
 *					the SETUP packets will be for standard
 *					USB "chapter 9" (as in, fulfilling chapter 9 of
 *					the official USB specifications) requests, while
 *					others may be specific to the USB device class
 *					that is being implemented.  For example, a HID
 *					class device needs to be able to respond to
 *					"GET REPORT" type of requests.  This
 *					is not a standard USB chapter 9 request, and 
 *					therefore not handled by usb_device.c.  Instead
 *					this request should be handled by class specific 
 *					firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *******************************************************************/
void USBCBCheckOtherReq(void)
{
  USBCheckHIDRequest();
}//end


/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is
 *					called when a SETUP, bRequest: SET_DESCRIPTOR request
 *					arrives.  Typically SET_DESCRIPTOR requests are
 *					not used in most applications, and it is
 *					optional to support this type of request.
 *
 * Note:            None
 *******************************************************************/
void USBCBStdSetDscHandler(void)
{
  // Must claim session ownership if supporting this request
}//end


/*******************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This 
 *					callback function should initialize the endpoints 
 *					for the device's usage according to the current 
 *					configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
  //enable the HID endpoint
  USBEnableEndpoint(HID_EP,USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
  //Re-arm the OUT endpoint for the next packet
  USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB specifications allow some types of USB
 * 					peripheral devices to wake up a host PC (such
 *					as if it is in a low power suspend to RAM state).
 *					This can be a very useful feature in some
 *					USB applications, such as an Infrared remote
 *					control	receiver.  If a user presses the "power"
 *					button on a remote control, it is nice that the
 *					IR receiver can detect this signalling, and then
 *					send a USB "command" to the PC to wake up.
 *					
 *					The USBCBSendResume() "callback" function is used
 *					to send this special USB signalling which wakes 
 *					up the PC.  This function may be called by
 *					application firmware to wake up the PC.  This
 *					function will only be able to wake up the host if
 *                  all of the below are true:
 *					
 *					1.  The USB driver used on the host PC supports
 *						the remote wakeup capability.
 *					2.  The USB configuration descriptor indicates
 *						the device is remote wakeup capable in the
 *						bmAttributes field.
 *					3.  The USB host PC is currently sleeping,
 *						and has previously sent your device a SET 
 *						FEATURE setup packet which "armed" the
 *						remote wakeup capability.   
 *
 *                  If the host has not armed the device to perform remote wakeup,
 *                  then this function will return without actually performing a
 *                  remote wakeup sequence.  This is the required behavior, 
 *                  as a USB device that has not been armed to perform remote 
 *                  wakeup must not drive remote wakeup signalling onto the bus;
 *                  doing so will cause USB compliance testing failure.
 *                  
 *					This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            This function does nothing and returns quickly, if the USB
 *                  bus and host are not in a suspended condition, or are 
 *                  otherwise not in a remote wakeup ready state.  Therefore, it
 *                  is safe to optionally call this function regularly, ex: 
 *                  anytime application stimulus occurs, as the function will
 *                  have no effect, until the bus really is in a state ready
 *                  to accept remote wakeup. 
 *
 *                  When this function executes, it may perform clock switching,
 *                  depending upon the application specific code in 
 *                  USBCBWakeFromSuspend().  This is needed, since the USB
 *                  bus will no longer be suspended by the time this function
 *                  returns.  Therefore, the USB module will need to be ready
 *                  to receive traffic from the host.
 *
 *                  The modifiable section in this routine may be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of ~3-15 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at least 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
void USBCBSendResume(void)
{
  static WORD delay_count;
    
  //First verify that the host has armed us to perform remote wakeup.
  //It does this by sending a SET_FEATURE request to enable remote wakeup,
  //usually just before the host goes to standby mode (note: it will only
  //send this SET_FEATURE request if the configuration descriptor declares
  //the device as remote wakeup capable, AND, if the feature is enabled
  //on the host (ex: on Windows based hosts, in the device manager 
  //properties page for the USB device, power management tab, the 
  //"Allow this device to bring the computer out of standby." checkbox 
  //should be checked).
  if(USBGetRemoteWakeupStatus() == TRUE) 
    {
      //Verify that the USB bus is in fact suspended, before we send
      //remote wakeup signalling.
      if(USBIsBusSuspended() == TRUE)
        {
	  USBMaskInterrupts();
            
	  //Clock switch to settings consistent with normal USB operation.
	  USBCBWakeFromSuspend();
	  USBSuspendControl = 0; 
	  USBBusIsSuspended = FALSE;  //So we don't execute this code again, 
	  //until a new suspend condition is detected.

	  //Section 7.1.7.7 of the USB 2.0 specifications indicates a USB
	  //device must continuously see 5ms+ of idle on the bus, before it sends
	  //remote wakeup signalling.  One way to be certain that this parameter
	  //gets met, is to add a 2ms+ blocking delay here (2ms plus at 
	  //least 3ms from bus idle to USBIsBusSuspended() == TRUE, yeilds
	  //5ms+ total delay since start of idle).
	  delay_count = 3600U;        
	  do
            {
	      delay_count--;
            }while(delay_count);
            
	  //Now drive the resume K-state signalling onto the USB bus.
	  USBResumeControl = 1;       // Start RESUME signaling
	  delay_count = 1800U;        // Set RESUME line for 1-13 ms
	  do
            {
	      delay_count--;
            }while(delay_count);
	  USBResumeControl = 0;       //Finished driving resume signalling

	  USBUnmaskInterrupts();
        }
    }
}


/*******************************************************************
 * Function:        BOOL USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, WORD size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  WORD size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
  switch(event)
    {
    case EVENT_TRANSFER:
      //Add application specific callback task or callback function here if desired.
      break;
    case EVENT_SOF:
      USBCB_SOF_Handler();
      break;
    case EVENT_SUSPEND:
      USBCBSuspend();
      break;
    case EVENT_RESUME:
      USBCBWakeFromSuspend();
      break;
    case EVENT_CONFIGURED: 
      USBCBInitEP();
      break;
    case EVENT_SET_DESCRIPTOR:
      USBCBStdSetDscHandler();
      break;
    case EVENT_EP0_REQUEST:
      USBCBCheckOtherReq();
      break;
    case EVENT_BUS_ERROR:
      USBCBErrorHandler();
      break;
    case EVENT_TRANSFER_TERMINATED:
      //Add application specific callback task or callback function here if desired.
      //The EVENT_TRANSFER_TERMINATED event occurs when the host performs a CLEAR
      //FEATURE (endpoint halt) request on an application endpoint which was 
      //previously armed (UOWN was = 1).  Here would be a good place to:
      //1.  Determine which endpoint the transaction that just got terminated was 
      //      on, by checking the handle value in the *pdata.
      //2.  Re-arm the endpoint if desired (typically would be the case for OUT 
      //      endpoints).
      break;
    default:
      break;
    }      
  return TRUE; 
}

/** EOF main.c *************************************************/
#endif
