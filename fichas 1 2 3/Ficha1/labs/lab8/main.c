#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "debug.h"

int main(void) {
    int result = system("ps -l");
    if (result < 0) {
        ERROR(1, "Chamada 'a funcao system() falhou.");
    } else {
        printf("Chamada 'a funcao system() retornou: %d.\n", result);
    }

    return 0;
}
