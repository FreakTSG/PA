#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <arpa/inet.h>

#include "debug.h"
#include "memory.h"

int main(void) {

    /* Verificar tamanho do tipo short */
    assert(sizeof(short)==2);
	
    /* Determinar endianess da máquina local */
	unsigned short test = 0x1122;
	if(htons(test)==test){
		printf("big endain\n");
	}else{
		printf("little endain\n");
	}
		
    return 0;
}
