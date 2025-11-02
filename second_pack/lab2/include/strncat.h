#ifndef STRNCAT_H
#define STRNCAT_H

#include "status.h"
#include <stddef.h>

StatusCode my_strncat(char *dest, size_t destize, const char *src, size_t n);

#endif