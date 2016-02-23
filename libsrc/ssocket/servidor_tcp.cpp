#include <cstdlib>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <signal.h>

#include "ssocket.h"
#include "teste.h"

using namespace std;

#define NUM_MAX_CONEX 3
#define TEMPO_MAXIMO 20  // Tempo máximo de espera em segundos

// Normalmente, a biblioteca de socket para o programa sempre que
// acontece um erro. Para evitar este comportamento, vamos redefinir
// a função tratadora de erros por uma que apenas imprime a mensagem
// de erro, mas não encerra o programa. O usuário é que deve analisar
// os erros retornados pelas funções e decidir o que fazer.

static void imprime_msg(SOCKET_STATUS err, const char *msg)
{
  //std::cout << "Erro no programa servidor: " << msg << endl;
}

// Os sockets dos clientes
tcpSocketServidor c;
tcpSocket s[NUM_MAX_CONEX];
int nconex=0;
bool fim = false;

// Função auxiliar para desconectar um cliente
void desconecte(int i)
{
  cout << "Cliente " << i << " desconectado\n";
  s[i].close();
  for (int j=i; j<nconex-1; j++) {
    s[j]=s[j+1];
  }
  nconex--;
}

// Função auxiliar para enviar mensagem para todos os clientes
void envie_msg(int orig, const char *msg)
{
  char buffer[100];

  fim = (strcasecmp(msg,"fim")==0);
  if (orig>=0) {
    sprintf(buffer, "De %d -> ", orig);
  }
  else {
    if (!fim) sprintf(buffer, "Servidor: ");
    else buffer[0] = '\0';
  }
  strcat(buffer,msg);
  int n = strlen(buffer)+1;
  for (int i=0; i<nconex; i++) if (s[i].connected()) {
    if (s[i].write(&n,sizeof(n)) != SOCKET_OK ||
	s[i].write(buffer,n) != SOCKET_OK ) {
      cout << "Problema ao enviar para o cliente " << i << endl;
      desconecte(i);
    }
    else {
      cout << "Mensagem enviada para o cliente " << i << endl;
    }
  }
}

// Thread que efetivamente desempenhas as tarefas do servidor
void *servidor(void *x)
{
  tcpSocket t;
  fsocket f;
  SOCKET_STATUS result;

  char buffer[100];
  int i,n;
  
  while (!fim) {
    f.clean();
    if (!(fim = !c.accepting())) {
      f.include(c);
      for (i=0; i<nconex; i++) {
	if (s[i].connected()) {
	  f.include(s[i]);
	}
	//else desconecte(i);
      }
    }

    //cout << "Esperando alguma atividade...\n";
    result = f.wait_read(TEMPO_MAXIMO*1000);
    if (result!=SOCKET_OK && result!=SOCKET_TIMEOUT) {
      if (!fim) cout << "Erro na espera por alguma atividade\n";
      fim = true;
    }
    if (!fim) {
      if (result==SOCKET_OK) {
	// Testa em qual socket houve atividade
	for (i=0; i<nconex; i++) {
	  if (s[i].connected() && f.had_activity(s[i])) {
	    if (s[i].read(&n,sizeof(n)) != SOCKET_OK ||
		s[i].read(buffer,n) != SOCKET_OK ) {
	      desconecte(i);
	    }
	    else {
	      cout << "Mensagem recebida do cliente " << i << ": "
		   << buffer << endl;
	      if (strcasecmp(buffer,"fim")==0) {
		desconecte(i);
	      }
	      else {
		envie_msg(i,buffer);
	      }
	    }
	  }
	}
	if (f.had_activity(c)) {
	  if (c.accept(t) != SOCKET_OK) {
	    cout << "Não foi possível estabelecer uma conexão\n";
	    fim = true;
	  }
	  if (!fim) {
	    if (nconex >= NUM_MAX_CONEX) {
	      cout << "Número máximo de conexões atingido: conexão rejeitada\n";
	      t.close();
	    }
	    else {
	      // Aloca próximo socket livre para o novo cliente
	      s[nconex] = t;
	      cout << "Cliente " << i << " conectado no socket " 
		   << s[nconex] << endl;
	      nconex++;
	    }
	  }
	}
      }
      else {
	if (nconex == 0) {
	  cout << "Servidor inativo há muito tempo. Encerrando...\n";
	  fim = true;
	}
      }
    }
  }
  for (i=0; i<nconex; i++) if (s[i].connected()) {
    desconecte(i);
  }
  return(NULL);
}

int main(void)
{
  char buffer[100];
  socket_error = imprime_msg;

  if (c.listen(PORTA_TESTE,NUM_MAX_CONEX) != SOCKET_OK) {
    cout << "Não foi possível abrir o socket de controle\n";
    exit(1);
  }

  pthread_t tid;
  // Cria a thread que escreve as mensagens recebidas
  pthread_create(&tid,NULL,servidor,NULL);

  while (!fim) {
    cout << "Mensagem para os clientes: ";
    cin >> buffer;
    envie_msg(-1,buffer);
  }

  c.close();
  sleep(1);
  pthread_kill(tid,SIGTERM);
  pthread_join(tid,NULL);
}
