#include <iostream>
#include <complex>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "matrix.h"
#include "mymatrix.h"

// A função default de tratamento de erros

static void matrix_msg(MATRIX_STATUS err, const char *msg)
{
  std::cout << "matrix: " << msg << '\n';
  // Eliminar o exit se não quiser que pare nos erros...
  exit(err);
}

// A função que é chamada pelos métodos da classe matrix quando há um erro
// Esta função chama a função contida na variável "matrix_error"

MATRIX_STATUS matrix_error_int(MATRIX_STATUS err)
{
  const char *msgerr;

  switch(err) {
  case MATRIX_OK:
    return err;
  case MATRIX_MEMORY_ALLOCATION:
    msgerr = "Memory allocation error";
    break;
  case MATRIX_BAD_PARAMETER:
    msgerr = "Bad parameter";
    break;
  case MATRIX_WRONG_DIMENSION:
    msgerr = "Wrong dimension(s)";
    break;
  case MATRIX_OUT_OF_RANGE:
    msgerr = "Index out of range";
    break;
  case MATRIX_NO_CONVERGENCE:
    msgerr = "No convergence after iterations";
    break;
  case MATRIX_DIVISION_ZERO:
    msgerr = "Division by zero";
    break;
  case MATRIX_NOT_SQUARE:
    msgerr = "Not square matrix";
    break;
  case MATRIX_SINGULARITY:
    msgerr = "Singular matrix";
    break;
  default:
    msgerr = "Undefined error";
    break;
  }
  matrix_error(err,msgerr);
  return(err);
}

// Variável que contém a função tratadora de erros
// Se o usuário quiser mudar a maneira de tratar os erros, deve definir
// uma nova função que receba os mesmos parâmetros ("err" e "msg") e
// em seguida atribuir esta função à variável "matrix_error"

void (*matrix_error)(MATRIX_STATUS err, const char *msg) =
matrix_msg;
