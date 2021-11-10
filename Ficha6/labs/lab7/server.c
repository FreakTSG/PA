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
#include <limits.h>
#include <math.h> //descomentar no makefile LIBS=-lm #-pthread

#include "debug.h"
#include "common.h"
#include "server_opt.h"

int main(int argc, char *argv[]) {
    /* Estrutura gerada pelo utilitario gengetopt */
    struct gengetopt_args_info args_info;

    /* Processa os parametros da linha de comando */
    if (cmdline_parser(argc, argv, &args_info) != 0) {
        exit(ERR_ARGS);
    }

	//verifica se o porto está no intervalo
    if(args_info.port_arg<1 || args_info.port_arg>USHRT_MAX){
    	ERROR(ERR_ARGS, "Valor do porto fora do intervalo [1;65535]");
    }
	
	// UDP IPv4: cria socket
	int udp_server_socket;
	if ((udp_server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		ERROR(31, "Can't create udp_server_socket (IPv4)");
	
	// UDP IPv4: bind a IPv4/porto 
	struct sockaddr_in udp_server_endpoint;
	memset(&udp_server_endpoint, 0, sizeof(struct sockaddr_in));
	
	udp_server_endpoint.sin_family = AF_INET;
	udp_server_endpoint.sin_addr.s_addr = htonl(INADDR_ANY);  	// Todas as interfaces de rede
	udp_server_endpoint.sin_port = htons(args_info.port_arg);	// Server port
	
	if (bind(udp_server_socket, (struct sockaddr *) &udp_server_endpoint, sizeof(struct sockaddr_in)) == -1)
		ERROR(32, "Can't bind @udp_server_endpoint info");
		
	//loop recevefrom, sendto
	while(1){
		uint16_t number; //= unsigned short
		
		// UDP IPv4: variáveis auxiliares para sendto() / recvfrom()
		socklen_t udp_client_endpoint_length = sizeof(struct sockaddr_in);
		struct sockaddr_in udp_client_endpoint;
		ssize_t udp_read_bytes, udp_sent_bytes;
		
		
		printf("à espera de dados do cliente... "); fflush(stdout);
		if ((udp_read_bytes = recvfrom(udp_server_socket, &number, sizeof(number), 0, (struct sockaddr *) &udp_client_endpoint, &udp_client_endpoint_length)) == -1)
		ERROR(34, "Can't recvfrom client");
	printf("ok.  (%d bytes received)\n", (int)udp_read_bytes);
		
		//converter os dados que vêm em formato rede para host
		
		number= ntohs(number);
		double result = sqrt(number);
		char response[MAX_MSG_SIZE];
		
		//converter double para um array de chars (string)
		snprintf(response, MAX_MSG_SIZE-1, "%.2lf", result);
				
		
		printf("a enviar dados para o cliente... "); fflush(stdout);
		if ((udp_sent_bytes = sendto(udp_server_socket, response, strlen(response), 0, (struct sockaddr *) &udp_client_endpoint, udp_client_endpoint_length)) == -1)
		ERROR(35, "Can't sendto client");
	printf("ok.  (%d bytes sent)\n", (int)udp_sent_bytes);
		
	}
	
	
    cmdline_parser_free(&args_info);

    return 0;
}
