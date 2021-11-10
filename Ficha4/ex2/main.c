#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sched.h>

#include "debug.h"
#include "memory.h"

#define LIMITE 10
#define PING 0
#define PONG 10

void *t_ping(void *arg);
void *t_pong(void *arg);

typedef struct 
{
	int estado;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	
}thread_params_t;

int main(int argc, char *argv[]){

	(void)argc;(void)argv;
	pthread_t t1, t2;
	thread_params_t thread_params;
	thread_params.estado = PING;
	
	//init mutex	
	if ((errno = pthread_mutex_init(&thread_params.mutex, NULL)) != 0)
		ERROR(12, "pthread_mutex_init() failed!");
	
	//init condicao
	if ((errno = pthread_cond_init(&thread_params.cond, NULL)) != 0)
		ERROR(14, "pthread_cond_init() failed!");
	
	if ((errno = pthread_create(&t1, NULL, t_ping, &thread_params)) != 0)
		ERROR(10, "pthread_create() failed!");
	if ((errno = pthread_create(&t2, NULL, t_pong, &thread_params)) != 0)
		ERROR(10, "pthread_create() failed!");
	
	if ((errno = pthread_join(t1, NULL)) != 0)
		ERROR(11, "pthread_join() failed!");	
	if ((errno = pthread_join(t2, NULL)) != 0)
		ERROR(11, "pthread_join() failed!");
	
	//destroy mutex	
	if ((errno = pthread_mutex_destroy(&thread_params.mutex)) != 0)
		ERROR(13, "pthread_mutex_destroy() failed!");
	
	//destroy condicao
	if ((errno = pthread_cond_destroy(&thread_params.cond)) != 0)
		ERROR(15, "pthread_cond_destroy() failed!");
	
	return 0;
}

//t1
void *t_ping(void *arg) 
{
	thread_params_t *params = (thread_params_t *) arg;
	
	for(int i=0;i<LIMITE;i++){
		
		if ((errno = pthread_mutex_lock(&(params->mutex))) != 0) {
			WARNING("pthread_mutex_lock() failed\n");
			return NULL;
		}
		
		while(params->estado!=PING){
			if ((errno = pthread_cond_wait(&(params->cond), &(params->mutex))) != 0) {
				WARNING("pthread_cond_wait() failed");
				return NULL;
			}
		}
		
		printf("PING...   \n");
		
		params->estado = PONG;
		
		if ((errno = pthread_cond_signal(&(params->cond))) != 0){
			WARNING("pthread_cond_signal() failed");
			return NULL;
		}
	
		if ((errno = pthread_mutex_unlock(&(params->mutex))) != 0) {
			WARNING("pthread_mutex_unlock() failed");
			return NULL;
		}
	}
	
	return NULL;
}

//t2
void *t_pong(void *arg) 
{
	thread_params_t *params = (thread_params_t *) arg;
	
	for(int i=0;i<LIMITE;i++){
		
		if ((errno = pthread_mutex_lock(&(params->mutex))) != 0) {
			WARNING("pthread_mutex_lock() failed\n");
			return NULL;
		}
		
		while(params->estado!=PONG){
			if ((errno = pthread_cond_wait(&(params->cond), &(params->mutex))) != 0) {
				WARNING("pthread_cond_wait() failed");
				return NULL;
			}
		}
		
		printf("   ...PONG\n");
		
		params->estado = PING;
			
		if ((errno = pthread_cond_signal(&(params->cond))) != 0){
			WARNING("pthread_cond_signal() failed");
			return NULL;
		}
		
		if ((errno = pthread_mutex_unlock(&(params->mutex))) != 0) {
			WARNING("pthread_mutex_unlock() failed");
			return NULL;
		}
	}
	
	return NULL;
}
