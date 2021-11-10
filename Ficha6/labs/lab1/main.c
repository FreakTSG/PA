#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include<arpa/inet.h>

#include "debug.h"
#include "memory.h"
#include "args.h"

void show_bytess(void *address, size_t size);

int main(int argc, char *argv[]) {
    /* Receber endereço IPv4 em formato texto */
    struct gengetopt_args_info args;
    cmdline_parser(argc, argv, &args);
	
    /* Converter e mostrar endereço em formato: byte.byte.byte.byte */
    struct in_addr IPv4_addr;
    
	switch(inet_pton(AF_INET, args.address_arg, &IPv4_addr)){
		case -1:
			ERROR(1, "inet_pton() failed");
			break;
		case 0:
			ERROR(2, "address ' %s 'is not valid ", args.address_arg);
			break;
		default:
			printf("IPv4 addr: ");
			show_bytess(&IPv4_addr, sizeof(IPv4_addr));
			break;
	}
    /* Libertar memória alocada pelo gengetopt */
    cmdline_parser_free(&args);
    return 0;
}

void show_bytess(void *address, size_t size){
	
	unsigned char *ptr = address;
	printf("0x");
	for(size_t i=0; i<size; i++){
		printf("%02x", ptr[i]);
	}
	printf("\n");
}
