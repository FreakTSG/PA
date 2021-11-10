#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/wait.h>

#include "debug.h"
#include "memory.h"
#include "args.h"

FILE *file = NULL;

int verificaFicheiro(char *nomeFicheiro);
void  enviarSinal(void);
void trata_sinal(int sinal);

struct gengetopt_args_info args_info;

int main (int argc, char *argv[])
{
  struct sigaction act;

	/* Silencia os warnings */
	(void)argc;
	(void)argv;
	
  if (cmdline_parser(argc, argv, &args_info) != 0) {
      exit(1);
  }
  if (verificaFicheiro(args_info.filename_arg)==0)
  {
    /* code */
    ERROR(1,"Ficheiro nao reconhecido,%s",args_info.filename_arg);
  }
  
  pid_t pid;
  pid=fork();
  switch(pid){
    case -1:
      ERROR(1,"Erro no fork");
      break;
    case 0://filho
      enviarSinal();
      break;
    default://pai
		
      act.sa_handler = trata_sinal;

      /* Mascara sem sinais para nao os bloquear */
      sigemptyset(&act.sa_mask);

      /* Comportamento por omissao -- fidedigno */
      //act.sa_flags = 0;

      //act.sa_flags |= SA_RESTART;
		act.sa_flags = SA_RESTART;
      /* Captura do sinal SIGUSR1 */
      if (sigaction(SIGUSR1, &act, NULL) != 0){
        ERROR(1, "sigaction - SIGUSR1");
      }
      pause();
      //wait(NULL);
      break; 
  }
 	
	return 0;
}

void  enviarSinal(void){
  while(1){
    kill(getppid(),SIGUSR1);
    sleep(5);
  }
}

int verificaFicheiro(char *nomeFicheiro){
  //FILE *file = NULL;
  char buffer[20];

  file = fopen(nomeFicheiro,"r");

  if (file==NULL)
  {
      ERROR(1,"Ficheiro enixistente");
      return 0;
  }else{
    fgets(buffer, 20, file);//outra solucao - getline!!
    fclose(file);
    return 1;
  }
}

void trata_sinal(int sinal){
  
  FILE *stream;
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  
  //char buffer[20];
  file = fopen(args_info.filename_arg,"r");

	if (stream == NULL){
		WARNING("Nao abriu o ficheiro %s", file);
	}else{
		if ((nread=getline(&line, &len, stream))>0){
		printf("Primeira linha: %s", nread);
		fclose(file);
		}
	}
}
