#ifndef STRNCMP_H
#define STRNCMP_H

#include "status.h"
#include <stddef.h>

StatusCode my_strncmp(const char *str1, const char *str2, size_t n, int *result);

#endif