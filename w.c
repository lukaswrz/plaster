#include "w.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

void w_fprintf(FILE *s, char *format, ...) {
    va_list args;
    va_start(args, format);

    if (vfprintf(s, format, args) < 0) {
        exit(1);
    }

    va_end(args);
}

FILE *w_fopen(const char *filename, const char *modes) {
    FILE *r = fopen(filename, modes);
    if (r == NULL) {
        w_fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }
    return r;
}

int w_fclose(FILE *s) {
    int r = fclose(s);
    if (r == EOF) {
        w_fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }
    return r;
}

int w_fgetc(FILE *s) {
    int r = fgetc(s);
    if (r < 0 && r != EOF) {
        w_fprintf(stderr, "Unable to read from stream\n");
        exit(1);
    }
    return r;
}
