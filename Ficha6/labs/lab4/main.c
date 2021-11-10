#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <sys/socket.h>

#include "debug.h"
#include "memory.h"

int main(void) {

    /* a) Criar um socket UDP / IPv4 */
	int sock_udp_ipv4 = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_udp_ipv4 == -1){
		ERROR(1,"Socket() failed");
	}

    /* b) Criar um socket UDP / IPv6 */
	int sock_udp_ipv6 = socket(AF_INET6, SOCK_DGRAM, 0);
	if(sock_udp_ipv6 == -1){
		ERROR(1,"Socket() failed");
	}

    /* c) Mostrar descritores dos sockets */
	printf("sock:udp_ipv4 - %d\n", sock_udp_ipv4);	
	printf("sock:udp_ipv6 - %d\n", sock_udp_ipv6);	

    /* d) Fechar os descritores dos sockets */
	close(sock_udp_ipv4);
	close(sock_udp_ipv6);

    return 0;
}
