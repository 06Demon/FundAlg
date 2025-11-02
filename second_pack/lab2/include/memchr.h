#ifndef MEMCHR_H
#define MEMCHR_H

#include "status.h"
#include <stddef.h>

StatusCode my_memchr(const void *str, int c, size_t n, void **result);

#endif