#include "scanner.h"

void scanner_init(struct scanner *sc, struct scanner_format format)
{
    *sc = (struct scanner){
        .format = format,
    };
}

int scanner_scan(struct scanner *sc, char c) {
    return 1;
}

int scanner_scan_file(struct scanner *sc, FILE *f) {
    return 1;
}

int scanner_scan_buf(struct scanner *sc, void *buf, size_t len) {
    return 1;
}
