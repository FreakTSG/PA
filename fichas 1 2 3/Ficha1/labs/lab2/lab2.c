#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    printf("O meu PID e':%d \n\n", getpid());
    printf("O PID do meu pai e':%d\n\n", getppid());
    return 0;
}
