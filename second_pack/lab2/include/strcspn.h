#ifndef STRCSPN_H
#define STRCSPN_H

#include "status.h"
#include <stddef.h>

StatusCode my_strcspn(const char *str1, const char *str2, size_t *result);

#endif