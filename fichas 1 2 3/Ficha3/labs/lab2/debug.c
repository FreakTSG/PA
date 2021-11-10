#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>

#include "debug.h"

void debug(const char *file, const int line, char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "[%s@%d] DEBUG - ", file, line);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    fflush(stderr);
}

void warning(const char *file, const int line, char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "[%s@%d] WARNING - ", file, line);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, ": %s\n", strerror(errno));
    fflush(stderr);
}

void error(const char *file, const int line, int exitCode, char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "[%s@%d] ERROR - ", file, line);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, ": %s\n", strerror(errno));
    fflush(stderr);
    exit(exitCode);
}

void show_bytes(char *ptr, size_t num_bytes) {
    unsigned char *work_ptr = (unsigned char *)ptr;
    for (size_t i = 0; i < num_bytes; i++) {
        printf("[%02zu] %hhx\n", i, *work_ptr);
        work_ptr++;
    }
}
