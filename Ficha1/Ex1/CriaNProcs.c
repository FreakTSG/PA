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
	int num_procs = args.num_procs_arg;

	for(int i=0; i<num_procs; i++){
		pid_t pid = fork();
		if(pid == 0){
			printf("Processo %d (PID=%d)\n", i, getpid());
			return 0;
		}
		else if(pid > 0){
			//printf("PID do processo pai = %d, PID do processo filho = %d\n", getppid(), getpid());
			wait(NULL);
		}
		else{
			ERROR(1, "Erro na execução do fork()");		
		}
		
	}
	
	cmdline_parser_free(&args);
    return 0;
}
