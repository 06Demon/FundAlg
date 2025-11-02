#ifndef MEMCMP_H
#define MEMCMP_H

#include "status.h"
#include <stddef.h>

StatusCode my_memcmp(const void *str1, const void *str2, size_t n, int *result);

#endif