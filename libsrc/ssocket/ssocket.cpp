#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "ssocket.h"

// A fun��o default de tratamento de erros

static void socket_msg(SOCKET_STATUS err, const char *msg)
{
  std::cout << "socket: " << msg << '\n';
  // Eliminar o exit se n�o quiser que pare nos erros...
  exit(err);
}

// A fun��o que � chamada pelos m�todos da classe socket quando h� um erro
// Esta fun��o chama a fun��o contida na vari�vel "socket_error"

static SOCKET_STATUS socket_error_int(SOCKET_STATUS err)
{
  const char *msgerr;

  switch (err)
    {
    case SOCKET_OK:
      return err;
    case SOCKET_JA_EM_USO:
      msgerr = "Socket ja em uso";
      break;
    case SOCKET_NAO_CRIADO:
      msgerr = "Erro na criacao";
      break;
    case SOCKET_NAO_FECHADO:
      msgerr = "Erro na destruicao";
      break;
    case SOCKET_HOST_INVALIDO:
      msgerr = "Nome (ou IP) invalido";
      break;
    case SOCKET_NAO_ACEITANDO:
      msgerr = "Nao esta aceitando";
      break;
    case SOCKET_NAO_CONECTADO:
      msgerr = "Nao esta conectado";
      break;
    case SOCKET_ERRO_BIND:
      msgerr = "Erro na atribuicao de nome";
      break;
    case SOCKET_ERRO_LISTEN:
      msgerr = "Erro na espera por conexao";
      break;
    case SOCKET_ERRO_ACCEPT:
      msgerr = "Erro no aceite da conexao";
      break;
    case SOCKET_ERRO_CONEXAO:
      msgerr = "Erro na conexao";
      break;
    case SOCKET_ERRO_ENVIO:
      msgerr = "Erro no envio";
      break;
    case SOCKET_ERRO_RECEPCAO:
      msgerr = "Erro na recepcao";
      break;
    case SOCKET_EM_USO:
      msgerr = "Ponto de comunicacao ja sendo utilizado";
      break;
    case SOCKET_FILA_VAZIA:
      msgerr = "Fila vazia";
      break;
    case SOCKET_ERRO_SELECT:
      msgerr = "Erro na espera por atividade";
      break;
    case SOCKET_DADO_VAZIO:
      msgerr = "Dado de comprimento nulo";
      break;
    case SOCKET_TIMEOUT:
      msgerr = "Tempo maximo atingido";
      break;
    default:
      msgerr = "Erro indeterminado";
      break;
    }
  socket_error(err,msgerr);
  return(err);
}

// Vari�vel que cont�m a fun��o tratadora de erros
// Se o usu�rio quiser mudar a maneira de tratar os erros, deve definir
// uma nova fun��o que receba os mesmos par�metros ("err" e "msg") e
// em seguida atribuir esta fun��o � vari�vel "simul_trata_erros"

void (*socket_error)(SOCKET_STATUS err, const char *msg) =
socket_msg;

/*********************************************
 A classe base ssocket
 *********************************************/

// Construtores e destrutores

ssocket::ssocket()
{
  state = SOCKET_IDLE;
  id = -1;
}

SOCKET_STATUS ssocket::close()
{
  state = SOCKET_IDLE;
  bool erro = false;
  if (id >= 0) {
    erro = (::close(id) == -1);
  }
  id = -1;
  if (erro) return(socket_error_int(SOCKET_NAO_FECHADO));
  return(SOCKET_OK);
}

// Impress�o

std::ostream& operator<<(std::ostream& os, const ssocket &s)
{
  os << s.id << '(';
  switch (s.state) {
  case SOCKET_IDLE:
    os << 'I';
    break;
  case SOCKET_ACCEPTING:
    os << 'A';
    break;
  case SOCKET_CONNECTED:
    os << 'C';
    break;
  }
  os << ')';
  return os;
}

/*********************************************
 Sockets orientados a conex�o (STREAM SOCKET)
 *********************************************/

// Sockets servidores

SOCKET_STATUS tcpSocketServidor::listen(uint16_t port, int nconex)
{
  if (id!=-1 || state!=SOCKET_IDLE) {
    return(socket_error_int(SOCKET_JA_EM_USO));
  }
  // Cria o socket
  id = ::socket(PF_INET, SOCK_STREAM, 0);
  if (id<0) {
    return(socket_error_int(SOCKET_NAO_CRIADO));
  }
  // Atribui��o do nome do socket
  struct sockaddr_in servidor;

  servidor.sin_family = AF_INET;
  servidor.sin_addr.s_addr = INADDR_ANY;
  servidor.sin_port = htons(port);
  if ( ::bind(id,(struct sockaddr *)&servidor,sizeof(servidor)) == -1 ) {
    return(socket_error_int(SOCKET_ERRO_BIND));
  }
  // Defini��o do n�mero de conex�es
  if ( ::listen(id,nconex) == -1) {
    return(socket_error_int(SOCKET_ERRO_LISTEN));
  }
  state = SOCKET_ACCEPTING;
  return(SOCKET_OK);
}

SOCKET_STATUS tcpSocketServidor::accept(tcpSocket &a) const
{
  if (!accepting()) {
    return(socket_error_int(SOCKET_NAO_ACEITANDO));
  }
  a.id = ::accept(id,NULL,0);
  if (a.id == -1) {
    return(socket_error_int(SOCKET_ERRO_ACCEPT));
  }
  a.state = SOCKET_CONNECTED;
  return(SOCKET_OK);
}

// Sockets clientes

SOCKET_STATUS tcpSocket::connect(const char *name, uint16_t port)
{
  if (id!=-1 || state!=SOCKET_IDLE) {
    return(socket_error_int(SOCKET_JA_EM_USO));
  }
  // Cria o socket
  id = ::socket(PF_INET, SOCK_STREAM, 0);
  if (id<0) {
    return(socket_error_int(SOCKET_NAO_CRIADO));
  }
  // Define o servidor
  struct sockaddr_in servidor;
  struct hostent *host;

  servidor.sin_family = AF_INET;
  servidor.sin_port   = htons(port);
  host = gethostbyname(name);
  if( host==NULL || host->h_addrtype!=AF_INET ||
      host->h_length!=sizeof(struct in_addr) || host->h_addr_list[0]==NULL) {
    return(socket_error_int(SOCKET_HOST_INVALIDO));
  }
  servidor.sin_addr = *(struct in_addr*)(host->h_addr_list[0]);
  // Conecta-se ao servidor
  if( ::connect(id,(struct sockaddr*)&servidor,sizeof(servidor)) == -1 ) {
    return(socket_error_int(SOCKET_ERRO_CONEXAO));
  }
  state = SOCKET_CONNECTED;
  return(SOCKET_OK);
}

SOCKET_STATUS tcpSocket::write(const void *dado, size_t len) const
{
  if (!connected()) {
    return(socket_error_int(SOCKET_NAO_CONECTADO));
  }
  if (len==0) {
    return(socket_error_int(SOCKET_DADO_VAZIO));
  }
  int enviados;
  do {
    enviados = ::send(id,dado,len,MSG_NOSIGNAL);
    if (enviados <= 0) {
      return(socket_error_int(SOCKET_ERRO_ENVIO));
    }
    len -= enviados;
    if (len > 0) {
      dado = (void*)((char*)dado+enviados);
    }
  } while (len>0);
  return(SOCKET_OK);
}

SOCKET_STATUS tcpSocket::read(void *dado, size_t len) const
{
  if (!connected()) {
    return(socket_error_int(SOCKET_NAO_CONECTADO));
  }
  if (len==0) {
    return(socket_error_int(SOCKET_DADO_VAZIO));
  }
  int recebidos;
  do {
    recebidos = ::recv(id,dado,len,0);
    if (recebidos <= 0) {
      return(socket_error_int(SOCKET_ERRO_RECEPCAO));
    }
    len -= recebidos;
    if (len > 0) {
      dado = (void*)((char*)dado+recebidos);
    }
  } while (len>0);
  return(SOCKET_OK);
}

/*********************************************
 Sockets baseados em datagrams (DGRAM SOCKET)
 *********************************************/

SOCKET_STATUS udpSocket::listen(uint16_t port)
{
  if (id!=-1 || state!=SOCKET_IDLE) {
    return(socket_error_int(SOCKET_JA_EM_USO));
  }
  // Cria o socket
  id = ::socket(PF_INET, SOCK_DGRAM, 0);
  if (id<0) {
    return(socket_error_int(SOCKET_NAO_CRIADO));
  }
  // Atribui��o do nome do socket
  struct sockaddr_in servidor;

  servidor.sin_family = AF_INET;
  servidor.sin_addr.s_addr = INADDR_ANY;
  servidor.sin_port = htons(port);
  if ( ::bind(id,(struct sockaddr *)&servidor,sizeof(servidor)) == -1 ) {
    return(socket_error_int(SOCKET_ERRO_BIND));
  }
  state = SOCKET_ACCEPTING;
  return(SOCKET_OK);
}

SOCKET_STATUS udpSocket::accept()
{
  if (!accepting()) {
    return(socket_error_int(SOCKET_NAO_ACEITANDO));
  }
  struct sockaddr_in cliente;
  int numbytes;
  socklen_t sizebuf=sizeof(struct sockaddr);
  char buf[20];  // 10 >= sizeof(MSG_INITIAL)
  // Recebe a mensagem inicial do cliente
  numbytes = recvfrom(id, buf, 20, 0, (struct sockaddr *)&cliente, &sizebuf);
  if (numbytes!=strlen(MSG_INITIAL)) {
    return(socket_error_int(SOCKET_ERRO_ACCEPT));
  }
  // Conecta-se ao cliente
  if( ::connect(id,(struct sockaddr*)&cliente,sizeof(cliente)) == -1 ) {
    return(socket_error_int(SOCKET_ERRO_CONEXAO));
  }
  state = SOCKET_CONNECTED;
  return(SOCKET_OK);
}

SOCKET_STATUS udpSocket::connect(const char *name, uint16_t port)
{
  if (id!=-1 || state!=SOCKET_IDLE) {
    return(socket_error_int(SOCKET_JA_EM_USO));
  }
  // Cria o socket
  id = ::socket(PF_INET, SOCK_DGRAM, 0);
  if (id<0) {
    return(socket_error_int(SOCKET_NAO_CRIADO));
  }
  
  // Define o servidor
  struct sockaddr_in servidor;
  struct hostent *host;
  

  servidor.sin_family = AF_INET;
  servidor.sin_port   = htons(port);
  host = gethostbyname(name);
  if( host==NULL || host->h_addrtype!=AF_INET ||
      host->h_length!=sizeof(struct in_addr) || host->h_addr_list[0]==NULL) {
    return(socket_error_int(SOCKET_HOST_INVALIDO));
  }
  servidor.sin_addr = *(struct in_addr*)(host->h_addr_list[0]);
  // Conecta-se ao servidor
  if( ::connect(id,(struct sockaddr*)&servidor,sizeof(servidor)) == -1 ) {
    return(socket_error_int(SOCKET_ERRO_CONEXAO));
  }

  state = SOCKET_CONNECTED;
  // Escreve mensagem inicial
  if (write(MSG_INITIAL, strlen(MSG_INITIAL)) != SOCKET_OK) {
    state = SOCKET_IDLE;
    return(socket_error_int(SOCKET_ERRO_CONEXAO));
  }
    

  return(SOCKET_OK);
}

SOCKET_STATUS udpSocket::joinMulticastGroup(const char* addr, uint16_t port)
{
  struct sockaddr_in address;
  struct ip_mreq mreq;  // endereco multicast

  if (id!=-1 || state!=SOCKET_IDLE) {
    return(socket_error_int(SOCKET_JA_EM_USO));
  }
  
  // Cria o socket
  id = ::socket(AF_INET, SOCK_DGRAM, 0);
  if (id<0) {
    return(socket_error_int(SOCKET_NAO_CRIADO));
  }

  // permite compartilhar porta multcast
  int reuse = 1;
  if(setsockopt(id, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse,sizeof(reuse))!=0) {
    fprintf(stderr,"ERROR WHEN SETTING SO_REUSEADDR ON UDP SOCKET\n");
    fflush(stderr);
  }

  int yes = 1;
  // allow packets to be received on this host
  if (setsockopt(id, IPPROTO_IP, IP_MULTICAST_LOOP, (const char*)&yes, sizeof(yes))!=0) {
      fprintf(stderr,"ERROR WHEN SETTING IP_MULTICAST_LOOP ON UDP SOCKET\n");
      fflush(stderr);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(port);

  if( bind(id, (struct sockaddr *) &address, sizeof(address)) < 0 )
  {
      return(socket_error_int(SOCKET_ERRO_BIND));
  }

  mreq.imr_multiaddr.s_addr=inet_addr(addr);
  mreq.imr_interface.s_addr=htonl(INADDR_ANY);
  if (setsockopt(id,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
      // perror("setsockopt");
      return(socket_error_int(SOCKET_ERRO_CONEXAO));
  }
  state = SOCKET_CONNECTED;
  return(SOCKET_OK);
}

SOCKET_STATUS udpSocket::sendTo(const void* data, size_t size, const char* addr, uint16_t port)
{
  struct sockaddr_in address;
  size_t sends = 0;
  //ainda não criado
  if(id == -1)
  { 
     // cria um novo socket
    if ( (id = socket(AF_INET, SOCK_DGRAM, 0) )  < 0  )
    {
        return(socket_error_int(SOCKET_NAO_CRIADO));
    }
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(addr);
  address.sin_port = htons(port);
  sends = sendto(id, (void*)data, size, 0, (const sockaddr*)&address, sizeof(address));

  if(sends != size){
    return(socket_error_int(SOCKET_ERRO_ENVIO));
  }

  return(SOCKET_OK);
}

int udpSocket::recvFrom(void* data, size_t size, const char* addr, uint16_t port, bool nonblocking)
{
  struct sockaddr_in address;
  int r = -1;
  if (id == -1 || state != SOCKET_STATE::SOCKET_CONNECTED) {
    socket_error_int(SOCKET_NAO_CRIADO);
    return r;
  }
  memset(&address, 0, sizeof(sockaddr_in));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(addr);
  address.sin_port = htons(port);
  socklen_t addr_size = sizeof(address);
  
  r = recvfrom(id, data, size, (nonblocking)?MSG_DONTWAIT:0, (struct sockaddr*)&address, &addr_size);

  return r;
}

SOCKET_STATUS udpSocket::write(const void *dado, size_t len) const
{
  if (!connected()) {
    return(socket_error_int(SOCKET_NAO_CONECTADO));
  }
  if (len==0) {
    return(socket_error_int(SOCKET_DADO_VAZIO));
  }
  int enviados;
  do {
    enviados = ::send(id,dado,len,MSG_NOSIGNAL);
    if (enviados <= 0) {
      return(socket_error_int(SOCKET_ERRO_ENVIO));
    }
    len -= enviados;
    if (len > 0) {
      dado = (void*)((char*)dado+enviados);
    }
  } while (len>0);
  return(SOCKET_OK);
}

int udpSocket::read(void *dado, size_t len, bool nonblocking) const
{
  if (!connected()) {
    return(socket_error_int(SOCKET_NAO_CONECTADO));
  }
  if (len==0) {
    return(socket_error_int(SOCKET_DADO_VAZIO));
  }
  int r;
  r = ::recv(id,dado,len,(nonblocking)?MSG_DONTWAIT:0);
  return r;  
  // if(recebidos >= 0);
  //   return(SOCKET_OK);
  // return(socket_error_int(SOCKET_ERRO_RECEPCAO));
  // do {
  //   recebidos = ::recv(id,dado,len,(nonblocking)?MSG_DONTWAIT:0);
  //   if (recebidos <= 0) {
  //     return(socket_error_int(SOCKET_ERRO_RECEPCAO));
  //   }
  //   len -= recebidos;
  //   if (len > 0) {
  //     dado = (void*)((char*)dado+recebidos);
  //   }
  // } while (len>0);
  // return(SOCKET_OK);
}

/*********************************************
A CLASSE fsocket (FILA DE SOCKETS)
*********************************************/

void fsocket::clean()
{
  FD_ZERO(&set);
  nmax = -1;
}

fsocket::~fsocket()
{
  FD_ZERO(&set);
}

SOCKET_STATUS fsocket::include(const ssocket &a)
{
  FD_SET(a.id,&set);
  if (a.id > nmax) nmax = a.id;
  return(SOCKET_OK);
}

SOCKET_STATUS fsocket::exclude(const ssocket &a)
{
  if (nmax < 0) {
    return(socket_error_int(SOCKET_FILA_VAZIA));
  }
  FD_CLR(a.id,&set);
  return(SOCKET_OK);
}

SOCKET_STATUS fsocket::wait_read(long milisec)
{
  if (nmax < 0) {
    return(socket_error_int(SOCKET_FILA_VAZIA));
  }
  int n;
  if (milisec >= 0) {
    struct timeval t;
    t.tv_sec = milisec/1000;
    t.tv_usec = 1000*(milisec - 1000*t.tv_sec);
    n = ::select(nmax+1, &set, NULL, NULL, &t);
  }
  else {
    n = ::select(nmax+1, &set, NULL, NULL, NULL);
  }
  if (n < 0) {
    return(socket_error_int(SOCKET_ERRO_SELECT));
  }
  if (n == 0) {
    return(socket_error_int(SOCKET_TIMEOUT));
  }
  return(SOCKET_OK);
}

SOCKET_STATUS fsocket::wait_write(long milisec)
{
  if (nmax < 0) {
      return(socket_error_int(SOCKET_FILA_VAZIA));
  }
  int n;
  if (milisec >= 0) {
    struct timeval t;
    t.tv_sec = milisec/1000;
    t.tv_usec = 1000*(milisec - 1000*t.tv_sec);
    n = ::select(nmax+1, NULL, &set, NULL, &t);
  }
  else {
    n = ::select(nmax+1, NULL, &set, NULL, NULL);
  }
  if (n < 0) {
      return(socket_error_int(SOCKET_ERRO_SELECT));
  }
  if (n == 0) {
      return(socket_error_int(SOCKET_TIMEOUT));
  }
  return(SOCKET_OK);
}

bool fsocket::had_activity(const ssocket &a)
{
  if (nmax < 0) {  // Fila vazia
    return(false);
  }
  return(FD_ISSET(a.id,&set));
}
