#pragma once

#include <stdio.h>

void w_fprintf(FILE *s, char *format, ...);
FILE *w_fopen(const char *filename, const char *modes);
int w_fclose(FILE *s);
int w_fgetc(FILE *s);
