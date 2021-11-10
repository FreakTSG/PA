#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "debug.h"
#include "memory.h"
#include "args.h"

int main(int argc, char *argv[]) {
    /* Receber porto a registar */
    struct gengetopt_args_info args;
    cmdline_parser(argc, argv, &args);

    /* Criar e registar socket UDP IPv4 */
	int sock_udp_ipv4 = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_udp_ipv4 == -1){
		ERROR(1,"Socket() failed");
	}
    /* Registo local - bind */
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(args.port_arg);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
	if(bind(sock_udp_ipv4, (struct sockaddr*) &addr, sizeof(struct sockaddr_in))==-1){
		WARNING("bind failed");
		/* Liberta memória */
	    	close(sock_udp_ipv4);
	    	return 2;
	}
	
    printf("Listening on port %hd\n", args.port_arg);
    while(1){
    	
    }
    
    //close(sock_udp_ipv4);
    
    cmdline_parser_free(&args);

    return 0;
}

/*
 * Perguntas:
 *  a)  O que sucede quando é indicado um porto entre 1 e 1023 (inclusive)?
 R: Não executa porque de 0 a 1023 sao portos reservados.
 s
 *  b)  O que sucede quando se tenta registar um porto que já está registado?
 *      Nota: é possível obter uma lista dos portos UDP registados no sistema
 * através do utilitário netstat, executado da seguinte forma: netstat -u -l
 R: O bind falha pois não podemos ter 2 portos a usar o mesmo socket.
 */
