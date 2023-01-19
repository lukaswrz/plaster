#pragma once

#include <stdio.h>

struct scanner_format {
    char start;
    char sep;
    char open;
    char close;
};

struct scanner {
    struct scanner_format format;

    size_t pos;
};

void scanner_init(struct scanner *sc, struct scanner_format format);
int scanner_scan(struct scanner *sc, char c);
int scanner_scan_file(struct scanner *sc, FILE *f);
int scanner_scan_buf(struct scanner *sc, void *buf, size_t len);
