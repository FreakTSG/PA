#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "debug.h"

#define C_ERRO_PTHREAD_CREATE 1
#define C_ERRO_PTHREAD_JOIN 2

void *hello(void *arg);

int main(int argc, char *argv[]) 
{
	(void)argc;(void)argv;

	pid_t pid;
	pthread_t tid,tid1;

	for (int i = 0; i < 3; ++i)
	{
		pid=fork();

		if (pid == 0)
		{
			if ((errno = pthread_create(&tid, NULL, hello, NULL)) != 0) {
				ERROR(C_ERRO_PTHREAD_CREATE, "pthread_create() failed!");
			}
			if ((errno = pthread_create(&tid1, NULL, hello, NULL)) != 0) {
				ERROR(C_ERRO_PTHREAD_CREATE, "pthread_create() failed!");
			}

			if ((errno = pthread_join(tid, NULL)) != 0) {
				ERROR(C_ERRO_PTHREAD_JOIN, "pthread_join() failed!");
			}
			if ((errno = pthread_join(tid1, NULL)) != 0) {
				ERROR(C_ERRO_PTHREAD_JOIN, "pthread_join() failed!");
			}
			printf("\n\n");
			return 0;
			
		}else if (pid > 0){
			
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		wait(NULL);
	}
	
	
	exit(0);
}


void *hello(void *arg) 
{
	(void)arg;

	printf("My name is Thread Posix Thread= [%lu]\n My father is : %d\n", (unsigned long) pthread_self(),getppid() );
	return NULL;//Implicitamente, chegou ao final da funcao , fez o return
}

