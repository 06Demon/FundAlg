#include "strrchr.h"
#include <stddef.h>

StatusCode my_strrchr(const char *str, int c, char **result) {
    if (str == NULL || result == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    char ch = (char)c;
    const char *last = NULL;
    
    for (; *str; ++str) {
        if (*str == ch) {
            last = str;
        }
    }
    
    if (ch == '\0') {
        *result = (char *)str;
        return STATUS_OK;
    }
    
    if (last != NULL) {
        *result = (char *)last;
        return STATUS_OK;
    }
    
    *result = NULL;
    return STATUS_ERROR_NOT_FOUND;
}