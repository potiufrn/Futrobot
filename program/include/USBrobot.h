/**
 * \mainpage USBROBOT
 *
 *
 * \section intro_sec Introdução
 *
 *	Esta biblioteca tem por objetivo prover uma interface de
 *	acesso a dispositivos Usb. Para isso essa classe conta com
 *	uma interface básica: open, close, read e write. O usuário
 *	pode desenvolver seus próprios aplicativos ou classes
 *	utilizando essa API. Para isso basta ter um dispositivo
 *	que siga o padrão HID e utilize a USB 2.0.
 *
 * \section hist_sec Histórico
 *
 *	Essa biblioteca começou a ser desenvolvida em 4 de março
 *	de 2009.
 *
 *	A biblioteca conta atualmente com a interface de acesso
 *	padrão.
 *	\section rest_sec Restrições
 *
 *	\li Essa classe deve ser utilizada preferencialmente com
 *	micro-controladores PIC USB.
 *	\li O tempo de polling dos endpoints são definidos pelo
 *	dispositivo.
 *	\li Inicialmente utilizamos um descritor HID padrão. Esse
 *	define dois endpoints, um de entrada e outro de saída,
 *	cada um com 64-bytes.
 */

/**
 * \file  USBrobot.h
 * \brief  Classe para comunicação com dispositivos HID
 * \author  Djalma Neto
 * \version 1.0
 * \date  Criação: 06/abr/2009
 * \date  Alteração: 06/abr/2009
 * \todo 
 *  \li Implementar rotinas de análise dos descritores HID.
 */


#ifndef USBROBOT_H
#define USBROBOT_H

//---------------------------------------------------------------
// INCLUDE

#include <sys/ioctl.h>
#include <asm/types.h>
#include <fcntl.h>
#include <linux/hiddev.h>
#include <stdint.h>
#include<unistd.h>
#include "USBException.h"

//---------------------------------------------------------------
// DEFINE

#define VENDOR			0x04d8
#define PRODUCT			0x003f
#define APPLICATION		0xff000001

#define ENDPOINT_MAX	64

//!  Classe para acesso de dispositivos HID 
/*!
 * 	Essa classe faz parte da API básica desenvolvida pelo Laboratório de
 *	Robótica da UFRN para controle e interfaceamento de micro-controladores
 *	USB.	
 */
class USBrobot{
	public:

		//! Construtor da classe USBrobot.
		USBrobot(const char * = "/dev/usb/hiddev0", 
			 const char * = "/dev/hidraw0",
			 const int = VENDOR, 
			 const int = PRODUCT);

		//! Destrutor.
		~USBrobot();

		//! Abrir dispositivo.
		void openDevice(const char* hiddev_file, 
				const char* hidraw_file);
		//! Fechar dispositivo.
		void closeDevice();
		//! Escrita não bloqueante no dispositivo.
		void writeDevice(const uint8_t* buffer, const int size);
		//! Leitura não bloqueante no dispositivo.
		int readDevice(uint8_t* buffer, const int size);
		//! Leitura bloqueante no dispositivo.
		int readBlocking(uint8_t* buffer, const int size, const unsigned s, const unsigned us);

		//! Definir id de produto a ser verificada pela classe.
		bool setDeviceId( const int prod );
		//! Retornar id do produto.
		int getDeviceId();

		//! Definir id do vendedor a ser verificada pela classe.
		bool setVendorId(const int vend );
		//! Retornar id do vendedor.
		int getVendorId();

		//! Tamanho do endpoint de entrada.
		const int getInSize() { return IN_SIZE; }
		//! Tamanho do endpoint de saída.
		const int getOutSize() { return OUT_SIZE; }

	private:

		// Constantes
		int IN_SIZE;					//!< Tamanho do endpoint de entrada 
		int OUT_SIZE;					//!< Tamanho do endpoint de saída

		// Descritores
		int hiddev_device;				//!< Descritor hiddev
		int hidraw_device;				//!< Descritor hidraw
		
		bool opened;					//!< O dispositivo está aberto?
		int deviceId;					//!< Id do produto
		int vendorId;					//!< Id do fabricante

		// Estruturas do hiddev
		hiddev_devinfo device_info;		//!< Informações sobre o dispositivo
		hiddev_report_info reportIN;	//!< Report de entrada
		hiddev_report_info reportOUT;	//!< Report de saída
		hiddev_field_info fieldIN;		//!< Informações sobre os campos do REPORT OUT
		hiddev_field_info fieldOUT;		//!< Informações sobre os campos do REPORT IN
		hiddev_usage_ref onebyteIN;		//!< Usage de entrada, para recebimento de um byte
		hiddev_usage_ref onebyteOUT;	//!< Usage de saída, para envio de um byte
		hiddev_usage_ref_multi packIN;	//!< Muli-usage para entrada de dados
		hiddev_usage_ref_multi packOUT;	//!< Multi-usage para saída de dados

		fd_set	fdset;					//!< File descriptor set utilizado pelo readBlocking
		timeval	tv;						//!< Estrutura de temporização utilizada pelo readBlocking
};

#endif

//---------------------------------------------------------------
// EOF
