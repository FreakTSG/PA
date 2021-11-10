#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <time.h>

#include "debug.h"
#include "common.h"
#include "client_args.h"

int main(int argc, char *argv[]) {
	/* Estrutura gerada pelo utilitario gengetopt */
	struct gengetopt_args_info args_info;

	/* Processa os parametros da linha de comando */
	if (cmdline_parser(argc, argv, &args_info) != 0)
		return 1;

	int udp_client_socket;	
	if ((udp_client_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		ERROR(21, "Can't create udp_client_socket (IPv4)");
	
	socklen_t udp_server_endpoint_length = sizeof(struct sockaddr_in);	
	struct sockaddr_in udp_server_endpoint;
	memset(&udp_server_endpoint, 0, sizeof(struct sockaddr_in));
	udp_server_endpoint.sin_family = AF_INET;
	switch (inet_pton(AF_INET, args_info.ip_arg , &udp_server_endpoint.sin_addr.s_addr)) {
		case 0:
			fprintf(stderr, "[%s@%d] ERROR - Cannot convert IP address (IPv4): Invalid Network Address\n",
				__FILE__, __LINE__);
			exit(22);
		case -1:
			ERROR(22, "Cannot convert IP address (IPv4)");
	}
	udp_server_endpoint.sin_port = htons(args_info.porto_arg);
	
	char buffer[MAX_SIZE];
	do{
		printf("\nIndroduza a mensagem a enviar: \n");
		if(fgets(buffer, MAX_SIZE-1, stdin)==NULL){ //indicamos o maximo de valores pertendidos
			if(feof(stdin))
				break;
			ERROR(2, "fgets() failed");
		}
		
		if(strlen(buffer)>0){
			buffer[strlen(buffer)-1] = '\0';
		}
		
		if(strlen(buffer)==0){
			continue;
		}
		
		ssize_t udp_read_bytes, udp_sent_bytes;
		printf("a enviar dados para o servidor... "); fflush(stdout);
		if ((udp_sent_bytes = sendto(udp_client_socket, buffer, MAX_SIZE, 0, (struct sockaddr *) &udp_server_endpoint, udp_server_endpoint_length)) == -1)
			ERROR(24, "Can't sendto server");
		printf("ok.  (%d bytes sent)\n", (int)udp_sent_bytes);
		
		char echo[MAX_SIZE];
		
		printf("à espera de dados do servidor... "); fflush(stdout);
		if ((udp_read_bytes = recvfrom(udp_client_socket, echo, MAX_SIZE, 0, (struct sockaddr *) &udp_server_endpoint, &udp_server_endpoint_length)) == -1)
			ERROR(25, "Can't recvfrom server");
		printf("ok.  (%d bytes received)\n", (int)udp_read_bytes);	
		
		printf ("Echo: %s\n", echo);
		
		
	}while(strcmp(buffer, "fim") !=0);
    
	if (close(udp_client_socket) == -1)
		ERROR(23, "Can't close udp_client_socket (IPv4)");
    return 0;
}
