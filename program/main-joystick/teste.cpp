#include "../data.h"
#include "export.h"





int main (int argc, char * argv[]){
       Semaphore semaforo (KEY_SEM, 1, 0666);
	Sharedmem memoria (KEY_MEM, sizeof (CONFIG), true, 0666);
	memoria.attach();
	CONFIG * pos = (CONFIG *) memoria.pttomem();
	while (1){		
    		printf("posicao jogador 0  x: %lf y: %lf \n", pos->me[0].x(), pos->me[1].y());
	}


}

