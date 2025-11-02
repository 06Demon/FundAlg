#ifndef STRERROR_H
#define STRERROR_H

#include "status.h"

StatusCode my_strerror(int errnum, const char **result);

#endif