#ifndef DECIMAL_CHECKER_H
#define DECIMAL_CHECKER_H

#include <stdarg.h>
#include <stddef.h>

typedef enum {
    SUCCESS = 0,
    INVALID_BASE,
    INVALID_VALUE,
    NULL_POINTER
} StatusCode;

StatusCode check_final_representation(unsigned int base, double epsilon, size_t count, int *results, ...);

#endif