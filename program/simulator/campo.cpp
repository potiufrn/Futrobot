#include "campo.h"
#include "../parameters.h"

Campo::Campo()
{
  // As arestas do campo
  // Início: Lado direito da parede superior
  ponto[0].x() = FIELD_WIDTH/2.0-CORNER_DIMENSION;
  ponto[0].y() = FIELD_HEIGHT/2.0;
  // Parede superior
  ponto[1].x() = -(FIELD_WIDTH/2.0-CORNER_DIMENSION);
  ponto[1].y() = FIELD_HEIGHT/2.0;
  // Canto superior esquerda
  ponto[2].x() = -(FIELD_WIDTH/2.0);
  ponto[2].y() = FIELD_HEIGHT/2.0-CORNER_DIMENSION;
  // Parede esquerda superior: quina superior esquerda
  ponto[3].x() = -(FIELD_WIDTH/2.0);
  ponto[3].y() = GOAL_HEIGHT/2.0;
  // Superior da trave esquerda
  ponto[4].x() = -(FIELD_WIDTH/2.0+GOAL_WIDTH);
  ponto[4].y() = GOAL_HEIGHT/2.0;
  // Fundo da trave esquerda
  ponto[5].x() = -(FIELD_WIDTH/2.0+GOAL_WIDTH);
  ponto[5].y() = -(GOAL_HEIGHT/2.0);
  // Inferior da trave esquerda: quina inferior esquerda
  ponto[6].x() = -(FIELD_WIDTH/2.0);
  ponto[6].y() = -(GOAL_HEIGHT/2.0);
  // Parede esquerda inferior
  ponto[7].x() = -(FIELD_WIDTH/2.0);
  ponto[7].y() = -(FIELD_HEIGHT/2.0-CORNER_DIMENSION);
  // Canto inferior esquerdo
  ponto[8].x() = -(FIELD_WIDTH/2.0-CORNER_DIMENSION);
  ponto[8].y() = -(FIELD_HEIGHT/2.0);
  // Parede inferior
  ponto[9].x() = (FIELD_WIDTH/2.0-CORNER_DIMENSION);
  ponto[9].y() = -(FIELD_HEIGHT/2.0);
  // Canto inferior direito
  ponto[10].x() = (FIELD_WIDTH/2.0);
  ponto[10].y() = -(FIELD_HEIGHT/2.0-CORNER_DIMENSION);
  // Parede direita inferior: quina inferior direita
  ponto[11].x() = (FIELD_WIDTH/2.0);
  ponto[11].y() = -(GOAL_HEIGHT/2.0);
  // Inferior da trave direita
  ponto[12].x() = (FIELD_WIDTH/2.0+GOAL_WIDTH);
  ponto[12].y() = -(GOAL_HEIGHT/2.0);
  // Fundo da trave direita
  ponto[13].x() = (FIELD_WIDTH/2.0+GOAL_WIDTH);
  ponto[13].y() = (GOAL_HEIGHT/2.0);
  // Superior da trave direita: quina superior direita
  ponto[14].x() = (FIELD_WIDTH/2.0);
  ponto[14].y() = (GOAL_HEIGHT/2.0);
  // Parede direita superior
  ponto[15].x() = (FIELD_WIDTH/2.0);
  ponto[15].y() = (FIELD_HEIGHT/2.0-CORNER_DIMENSION);
  // Canto superior direito
  ponto[16].x() = (FIELD_WIDTH/2.0-CORNER_DIMENSION);
  ponto[16].y() = (FIELD_HEIGHT/2.0);
}
