#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <signal.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/wait.h>

#include "debug.h"
#include "memory.h"
#include "args.h"

struct gengetopt_args_info args_info;
void trata_sinal(int signal); 
void execute_command(int signal);
 
void trata_sinal(int signal) {    
    int aux;    /* Cópia da variável global errno */    
    aux = errno;       
    if(signal == SIGUSR1){       
        printf("[INFO] kill -s SIGUSR1 %d   (to execute command)\n",getpid());    
        
    } /* Restaura valor da variável global errno */   
    if (signal == SIGINT)
     {
        printf("[INFO] kill -s SIGINT %d   (to terminate) \n",getpid());
         /* code */
     } 
    errno = aux;     
}

void execute_command(int signal) {    
    int aux;    /* Cópia da variável global errno */    
    aux = errno;       
    if(signal == SIGUSR1){       
        printf("[INFO] SIGUSR1 received. Executing command ‘%s'\n",args_info.comando_arg);    
        
    } /* Restaura valor da variável global errno */   
    errno = aux;     
}
 

int main(int argc, char* argv[]){

    char buf[100];     
    struct sigaction act;    
    ssize_t n; 

    (void) argc;
    (void) argv;

     
    
     if (cmdline_parser(argc, argv, &args_info) != 0) {
        exit(1);
    }
    
    printf("[PID:%d]: comand to execute is '%s'\n",getpid(),args_info.comando_arg);
     
     /* Definir a rotina de resposta a sinais */     
     act.sa_handler = trata_sinal;         

     /* mascara sem sinais -- nao bloqueia os sinais */    
     sigemptyset(&act.sa_mask);       

     act.sa_flags = 0;           /*fidedigno*/    
     act.sa_flags |= SA_RESTART; /*recupera chamadas bloqueantes*/       

     /* Captura do sinal SIGUSR1 */    
     if(sigaction(SIGUSR1, &act, NULL) < 0){       
        ERROR(1, "sigaction - SIGUSR1");    
    } 

     /* Definir a rotina de resposta a sinais */     
     act.sa_handler = trata_sinal;         

     /* mascara sem sinais -- nao bloqueia os sinais */    
     sigemptyset(&act.sa_mask);       

     act.sa_flags = 0;           /*fidedigno*/    
     act.sa_flags |= SA_RESTART; /*recupera chamadas bloqueantes*/       

     /* Captura do sinal SIGUSR1 */    
     if(sigaction(SIGINT, &act, NULL) < 0){       
        ERROR(1, "sigaction - SIGUSR1");    
    } 

    n = read(0, buf, 100); 
    
    exit(0);
}


