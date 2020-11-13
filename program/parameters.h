#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

// Dimensões do campo (m)
#define FIELD_WIDTH 1.50
#define FIELD_HEIGHT 1.30

// Dimensões da trave
#define GOAL_WIDTH 0.10
#define GOAL_HEIGHT 0.40

// Largura/altura da quina
#define CORNER_DIMENSION 0.07

// Raio do círculo central
#define CIRCLE_RADIUS 0.20

// Dimensões da grande área
#define GOAL_FIELD_HEIGHT 0.70
#define GOAL_FIELD_WIDTH 0.15
#define ARC_HEIGHT 0.20
#define ARC_WIDTH 0.05
#define ARC_APOTHEM (ARC_HEIGHT*ARC_HEIGHT-4*(ARC_WIDTH*ARC_WIDTH))/(8*ARC_WIDTH)
#define ARC_RADIUS (ARC_WIDTH + ARC_APOTHEM)

//Bolas paradas
#define PK_X 0.375
#define PK_Y 0.0
#define FB_X 0.375
#define FB_Y 0.4
#define DIST_FB 0.2

//Câmera
//#define CAMERA_HEIGHT 2.5
#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480

#define CAMERA_HEIGHT 2.10

// Dimensões dos objetos (m)
// Robô
#define ROBOT_EDGE 0.075
#define ROBOT_RADIUS (ROBOT_EDGE/sqrt(2.0))
#define ROBOT_HEIGHT 0.075

// Bola
#define BALL_RADIUS 0.02135
#define BALL_HEIGHT 0.03

// Período de amostragem
#define VEL_BOLA_PARADA 0.025 //CHECAR
#define CONSTANTE_BOLA_FUTURA 0.8 // [s]/[m]

#endif
