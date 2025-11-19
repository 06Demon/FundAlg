#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

typedef enum {
    SUCCESS,
    ERROR_INVALID_BASE,
    ERROR_NEGATIVE_NUMBER,
    ERROR_MEMORY_ALLOCATION,
    ERROR_NULL_POINTER
} StatusCode;

StatusCode decimal_to_base2r(int decimal, int r, char **result);

#endif