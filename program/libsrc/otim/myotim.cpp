#include <stdlib.h>
#include <iostream>

#include "otim.h"
#include "myotim.h"

// A função default de tratamento de erros

static void otim_msg(OTIM_STATUS err, const char *msg)
{
  std::cout << "otim: " << msg << '\n';
  // Eliminar o exit se não quiser que pare nos erros...
  exit(err);
}

// A função que é chamada pelos métodos da classe otim quando há um erro
// Esta função chama a função contida na variável "otim_error"

OTIM_STATUS otim_error_int(OTIM_STATUS err)
{
  const char *msgerr;

  switch(err)
    {
    case OTIM_OK:
      return err;
    case OTIM_ALOCACAO_MEMORIA:
      msgerr = "Erro de alocacao de memoria";
      break;
    case OTIM_PARAMETRO_INVALIDO:
      msgerr = "Parametro invalido";
      break;
    case OTIM_DIMENSAO_INVALIDA:
      msgerr = "Dimensao invalida";
      break;
    case OTIM_INDICE_INVALIDO:
      msgerr = "Indice fora de faixa";
      break;
    case OTIM_NAO_CONVERGIU:
      msgerr = "Metodo nao convergiu para um minimo";
      break;
    default:
      msgerr = "Erro indefinido";
      break;
    }
  otim_error(err,msgerr);
  return(err);
}

// Variável que contém a função tratadora de erros
// Se o usuário quiser mudar a maneira de tratar os erros, deve definir
// uma nova função que receba os mesmos parâmetros ("err" e "msg") e
// em seguida atribuir esta função à variável "otim_error"

void (*otim_error)(OTIM_STATUS err, const char *msg) =
otim_msg;
