#ifndef OVERH
#define OVERH

#include <stdio.h>

typedef enum {
    SUCCESS = 0,
    NULL_POINTER,
    INVALID_ROMAN,
    INVALID_ZECKENDORF,
    INVALID_BASE,
    INVALID_OTHER_CC_NUMBER,
} StatusCode;

StatusCode my_overfscanf(FILE *stream, const char *format, ...);
StatusCode my_oversscanf(const char *s, const char *format, ...);

#endif