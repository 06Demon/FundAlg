#ifndef MEMCPY_H
#define MEMCPY_H

#include "status.h"
#include <stddef.h>

StatusCode my_memcpy(void *dest, const void *src, size_t n);

#endif