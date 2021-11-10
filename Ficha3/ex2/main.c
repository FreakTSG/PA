#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#include "args.h"
#include "debug.h"
#include "memory.h"

#define ERROR_PTHREADS 1;

typedef struct 
{
	int incremento;
}thread_params_t;

int G_shared_counter;

struct gengetopt_args_info args_info;

void *task(void *arg);

int main(int argc, char *argv[]){
	(void)argc; (void)argv;
	
	if (cmdline_parser(argc, argv, &args_info) != 0) {
	  exit(1);
	}

	printf("N de threads a criar : %d\n", args_info.num_threads_arg);
	printf("Variavel a incrementar: %d\n", args_info.variavelGlobal_arg );
	
	thread_params_t thread_params;
	
	thread_params.incremento = args_info.variavelGlobal_arg;
	int num_threads = args_info.num_threads_arg;
	pthread_t *tids = MALLOC(sizeof(pthread_t)*num_threads);

	// Criação das threads + passagem de parâmetro
	for (int i = 0; i < num_threads; i++){
		if ((errno = pthread_create(&tids[i], NULL, task, &thread_params)) != 0)
			ERROR(10, "Erro no pthread_create()!");	
	}

	for (int i = 0; i < num_threads; i++){
		if ((errno = pthread_join(tids[i], NULL)) != 0)
			ERROR(11, "Erro no pthread_join()!\n");
	}
	FREE(tids);
	printf("G_shared_counter = %d (expecting %d)\n",G_shared_counter,num_threads*args_info.variavelGlobal_arg );
	return 0;
}

void *task(void *arg) 
{
	// cast para o tipo de dados enviado pela 'main thread'
	thread_params_t *params = (thread_params_t *) arg;
	int incremento = params->incremento;
	int i,local;

	for (i = 0; i < incremento; ++i)
	{
		local = G_shared_counter;
		sched_yield();
		local++;
		G_shared_counter=local;
	}
	return NULL;
}
