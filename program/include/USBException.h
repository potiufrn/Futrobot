/**
 * \file	USBException.h
 * \brief	Conjunto de classes de tratamento de erros.
 * \author  Djalma Neto
 * \version 1.0
 * \date  	Criação: 	06/abr/2009
 * \date  	Alteração: 	06/abr/2009
 */

#ifndef USBEXCEPTION_H
#define USBEXCEPTION_H

#define DIR_OUT	0
#define DIR_IN	1


//! Classe base para o tratamento das exceções USB
/*!
 *	Essa classe permite capturar todas as exceções lançadas pela USB.
 *	Sua principal funcionalidade é informar que tipo de erro ocorreu.
 */
class USBException
{
	public:
		//! Construtor padrão
		USBException(const char* = "Erro na porta USB");
		//! Imprime uma string que informa o tipo de erro
		/*!
		 *	Função principal da classe. Seu objetivo é retornar uma
		 *	string que ajude a identificar o tipo do erro.
		 *	\return string com o nome do erro.
 		 */
		const char* what() const {return message;}
		
	private:
		const char *message;	//!< nome do erro.
};

//! Classe base para o tratamento de erros de abertura
/*!
 *	Essa classe permite a captura de todas as exceções de abertura
 */
class OpenException : public USBException
{
	public:
		//! Construtor padrão
		/*!
		 * \param msg	mensagem
		 */
		OpenException(const char* = "Erro ao abrir dispositivo");
};


//! Classe para tratamento de erros no arquivo hiddev
class HiddevException : public OpenException
{
	public:
		//! Construtor padrão.
		/*!
		 *	\param p	path
		 *	\param err	tipo de erro
		 */
		HiddevException(const char*, const int err);

		//! Path do hiddev
		const char* getPath() {return path;}
		//! Valor do errno do SO.
		const int getError() {return error;}
		//! String com o nome do erro.
		const char* strError();
		
	private:
		const char* path;	//!< Caminho do arquivo
		const int error;	//!< Equivale ao errno do sistema
};

//! Classe para tratamento de erros no arquivo hidraw
class HidrawException : public OpenException
{
	public:
		//! Construtor padrão.
		/*!
		 *	\param p	path
		 *	\param err	tipo de erro
		 */
		HidrawException(const char*, const int err);

		//! Path do hidraw
		const char* getPath() {return path;}
		//! Valor do errno do SO.
		const int getError() {return error;}
		//! String com o nome do erro.
		const char* strError();
		
	private:
		const char* path;	//!< Caminho do arquivo
		const int error;	//!< Equivale ao errno do sistema
};


//! Classe para tratamento de dispositivo não suportado
class UnsuportedDevice : public OpenException
{
	public:
		//! Construtor padrão.
		/*!
		 * 	\param v 	id do fabricante
		 *	\param p 	id do produto
		 *	\param a	aplicação
		 */
		UnsuportedDevice(const unsigned, const unsigned, const unsigned);

		//! Retorna a id do fabricante
		const unsigned getVendor() {return vendor;}
		//! Retorna a id do produto
		const unsigned getProduct() {return product;}
		//! Retorna a aplicação
		const unsigned getApplication() {return app;}
	private:
		const unsigned vendor;		//!< Id do vendedor = Microchip
		const unsigned product;		//!< Id do produto
		const unsigned app;			//!< Id da application collection : definido pelo programador do firmware
};

//! Classe para tratamento de erro de report
class ReportException : public OpenException
{
	public:
		//! Construtor padrão
		/*!
		 *	\param id		identificador do report
		 *	\param type		tipo do report
		 *	\param fields	número de campos
		 *	\param dir		direção do report
		 *	\param err		valor do errno
		 *	\param isfld	se o erro é de campo
		 */
		ReportException(const unsigned, const unsigned, const unsigned, const unsigned, const unsigned, const bool);

		//! Retorna a id do report
		const unsigned getId() {return reportId;}
		//! Retorna o tipo do report
		const unsigned getType() {return reportType;}
		//! Retorna o número de campos do report
		const unsigned getFields() {return reportFields;}
		//! Verifica se foi erro de campo
		const bool isFieldError() {return fieldError;}
		//! Retorna o valor do errno
		const unsigned getError() {return error;}
		//! String que identifica o errno
		const char* strError();

	private:
		const unsigned reportId;		//!< Id do report
		const unsigned reportType;		//!< Tipo do report
		const unsigned reportFields;	//!< Campos
		const unsigned direction;		//!< Direção
		const unsigned error;			//!< equivale ao errno do sistema
		const bool fieldError;			//!< Identifica erro de campo
};


//! Classe para tratamento de exceção de dispositivo fechado
/*!
 *	Classe responsável por todo tratamento de erros de fechamento de descritor.
 */
class CloseException : public USBException
{
	public:
		//! Construtor padrão
		/*!
		 * \param msg	mensagem
		 */
		CloseException(const char* = "Erro ao fechar dispositivo");
};

//! Classe para tratamento de erro do hiddev
class HiddevClose : public CloseException
{
	public:
		//! Construtor padrão.
		/*!
		 *	\param e	valor do errrno
		 */
		HiddevClose(const int);

		//! Retorna o valor do errno do SO.
		const int getError() {return error;}
		//! String que identifica o errno.
		const char* strError();
	private:
		const int error;	//!< equivale ao errno do sistema.
};

//! Classe para tratamento de erro do hidraw
class HidrawClose : public CloseException
{
	public:
		//! Construtor padrão.
		/*!
		 *	\param e	valor do errrno
		 */
		HidrawClose(const int);

		//! Retorna o valor do errno do SO.
		const int getError() {return error;}
		//! String que identifica o errno.
		const char* strError();
	private:
		const int error;	//!< equivale ao errno do sistema
};

//! Classe para o tratamento de erro de Escrita
/*!
 *	Essa classe permite a captura de todas as exceções de escrita.
 */
class WriteException : public USBException
{
	public:
		//! Construtor padrão
		/*!
		 * \param msg	mensagem
		 */
		WriteException(const char* = "Erro ao escrever dispositivo");
};

//! Classe para o tratamento de erro de escrita arquivo fechado.
class WriteClosed : public WriteException
{
	public:
		//! Construtor padrão
		WriteClosed();
};

//! Classe para o tratamento de erro de escrita no usage.
class WriteUsage : public WriteException
{
	public:
		//! Construtor padrão.
		/*!
		 *	\param err	valor do errrno
		 */
		WriteUsage(const int);

		//! Retorna o valor do errno do SO.
		const int getError() {return error;}
		//! String que identifica o errno.
		const char* strError();
	private:
		const int error;	//!< equivale ao errno do sistema
};

//! Classe para o tratamento de erro ao enviar dado.
class WriteSend : public WriteException
{
	public:
		//! Construtor padrão.
		/*!
		 *	\param err	valor do errrno
		 */
		WriteSend(const int);

		//! Retorna o valor do errno do SO.
		const int getError() {return error;}
		//! String que identifica o errno.
		const char* strError();
	private:
		const int error;	//!< equivale ao errno do sistema

};

//! Classe para o tratamento de erro de escrita tamanho errado.
class WriteWrongSize : public WriteException
{
	public:
		//! Construtor padrão.
		/*!
		 *	\param s	tamanho
		 */
		WriteWrongSize(const int);

		//! Retorna o tamanho passado pelo usuário
		const int getSize() {return size;}
	private:
		const int size;		//!< tamanho
};


//! Classe para o tratamento de erros de Leitura
/*!
 *	Essa classe permite a captura de todas as exceções de leitura
 */
class ReadException : public USBException
{
	public:
		//! Construtor padrão
		/*!
		 * \param msg	mensagem
		 */
		ReadException(const char* = "Erro na leitura");
};

//! Classe para o tratamento de erro de dispositivo fechado
class ReadClosed : public ReadException
{
	public:
		//! Construtor padrão
		ReadClosed();
};

//! Classe para o tratamento usage não funcional
class ReadUsage : public ReadException
{
	public:
		//! Construtor padrão.
		/*!
		 *	\param err	valor do errrno
		 */
		ReadUsage(const int);

		//! Retorna o valor do errno do SO.
		const int getError() {return error;}
		//! String que identifica o errno.
		const char* strError();
	private:
		const int error;	//!< equivale ao errno do sistema
};

//! Classe para o tratamento de tamanho de buffer inadequado
class ReadWrongSize : public ReadException
{
	public:
		//! Construtor padrão.
		/*!
		 *	\param s	tamanho
		 */
		ReadWrongSize(const int);

		//! Retorna o tamanho passado pelo usuário
		const int getSize() {return size;}
	private:
		const int size;		//!< tamanho
};


//! Classe para o tratamento de erros de select.
class SelectException : public ReadException
{
	public:
		//! Construtor padrão.
		/*!
		 *	\param err	valor do errrno
		 */
		SelectException(const int);

		//! Retorna o valor do errno do SO.
		const int getError() {return error;}
		//! String que identifica o errno.
		const char* strError();
	private:
		const int error;	//!< equivale ao errno do sistema
};

//! Classe para o tratamento de erros de leitura bloqueante.
class ReadBlockingError : public ReadException
{
	public:
		//! Construtor padrão.
		/*!
		 *	\param err	valor do errrno
		 */
		ReadBlockingError(const int);

		//! Retorna o valor do errno do SO.
		const int getError() {return error;}
		//! String que identifica o errno.
		const char* strError();
	private:
		const int error;	//!< equivale ao errno do sistema
};

#endif

//---------------------------------------------------------------
// EOF
