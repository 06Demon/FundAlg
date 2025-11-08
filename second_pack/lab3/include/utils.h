#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

typedef enum {
    SUCCESS = 0,
    NULL_POINTER_ERROR,
    MEMORY_ALLOCATION_ERROR,
    INVALID_BASE,
    INVALID_FORMAT,
    BUFFER_OVERFLOW
} StatusCode;

StatusCode my_overfprintf(FILE *stream, const char *format, ...);
StatusCode my_oversprintf(char *outbuf, size_t outbuf_size, const char *format, ...);

#endif