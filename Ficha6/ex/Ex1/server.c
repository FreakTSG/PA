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
#include "server_args.h"

int main(int argc, char *argv[]) {
    /* Estrutura gerada pelo utilitario gengetopt para
     guardar os parametros de entrada */
    struct gengetopt_args_info args_info;

    /* Processa os parametros da linha de comando */
    if (cmdline_parser(argc, argv, &args_info) != 0)
        return 1;

    int udp_server_socket;
	if ((udp_server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		ERROR(31, "Can't create udp_server_socket (IPv4)");

    struct sockaddr_in ser_addr;
    /* Preenche a estrutura */
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(args_info.porto_arg);

    /* Efetua o registo */
    if (bind(udp_server_socket, (struct sockaddr *)&ser_addr, sizeof(ser_addr)) == -1)
        ERROR(3, "bind server");

	while (1){
		char buffer[MAX_SIZE];
		socklen_t udp_client_endpoint_length = sizeof(struct sockaddr_in);
		struct sockaddr_in udp_client_endpoint;
		ssize_t udp_read_bytes, udp_sent_bytes;
		
		printf("à espera de dados do cliente... "); fflush(stdout);
		if ((udp_read_bytes = recvfrom(udp_server_socket, buffer, MAX_SIZE, 0, (struct sockaddr *) &udp_client_endpoint, &udp_client_endpoint_length)) == -1)
			ERROR(34, "Can't recvfrom client");
		printf("ok.  (%d bytes received)\n", (int)udp_read_bytes);
		
		printf("Mensagem recebida: %s\n", buffer);
		
		printf("a enviar dados para o cliente... "); fflush(stdout);
		if ((udp_sent_bytes = sendto(udp_server_socket, buffer, MAX_SIZE, 0, (struct sockaddr *) &udp_client_endpoint, udp_client_endpoint_length)) == -1)
			ERROR(35, "Can't sendto client");
		printf("ok.  (%d bytes sent)\n", (int)udp_sent_bytes);
	}
    
    return 0;
}

