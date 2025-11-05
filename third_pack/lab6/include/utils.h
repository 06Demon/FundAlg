#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

typedef enum {
    SUCCESS = 0,          
    BRACKETS_NOT_BALANCED,
    MEMORY_ERROR,
    INVALID_ARGUMENT
} StatusCode;

StatusCode check_brackets(const char *str);

#endif