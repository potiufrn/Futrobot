/****************************************************************/
/*   Defini��es dos par�metros de comunica��o entre processos   */
/****************************************************************/

#ifndef _COMUNICACAOH_
#define _COMUNICACAOH_

// Par�metros da comunica��o via socket entre
// o programa principal e o simulador

#define DEFAULT_MULTICAST_ADDR "224.0.0.5"
#define DEFAULT_SERVER_ADDR "127.0.0.1"
#define PORT_MCAST  10020
#define PORT_CMD    20011

// Par�metros da comunica��o via mem�ria compartilhada entre
// o simulador e o visualizador

#define KEY_SEM_COMPART 0x2001
#define KEY_MEM_COMPART 0x2002

// Par�metros da comunica��o via mem�ria compartilhada entre
// o futrobot e a interface

#define KEY_SEM_BLUE 715896
#define KEY_MEM_BLUE 123473

#define KEY_SEM_YELLOW 752478
#define KEY_MEM_YELLOW 138912

#endif
