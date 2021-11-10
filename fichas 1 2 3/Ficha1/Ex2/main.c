#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "debug.h"
#include "memory.h"
#include "args.h"

int main(int argc, char *argv[]) {
	struct gengetopt_args_info args;
	if (cmdline_parser(argc, argv, &args)!=0){
		return 1;
	}
	
	pid_t filho1 = fork();
	pid_t filho2 = fork();
	if(filho1 == 0){
		printf("eu sou o filho 1\n");
		printf("PID do processo pai = %d, PID do processo filho = %d\n", getppid(), getpid());
		pid_t pidNeto = fork();
		if(pidNeto == 0){
			printf("eu sou o neto\n");
			printf("PID do processo pai = %d, PID do processo filho = %d\n", getppid(), getpid());
			return 0;
		}
		else if(filho1 > 0){
			wait(NULL);
		}
		else{
			ERROR(1, "Erro na execução do fork()");		
		}
	}
	else if(filho2 == 0){
		printf("eu sou o filho 2\n");
		printf("PID do processo pai = %d, PID do processo filho = %d\n", getppid(), getpid());
		return 0;
	}
	else if(filho1 > 0){
		printf("Eu sou o pai\n");
		printf("PID do processo = %d\n", getpid());
		
		wait(NULL);
		wait(NULL);
		wait(NULL);
	}
	else{
		ERROR(1, "Erro na execução do fork()");		
	}
	
	cmdline_parser_free(&args);
    return 0;
}
