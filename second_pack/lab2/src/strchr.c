#include "strchr.h"
#include <stddef.h>


StatusCode my_strchr(const char *str, int c, char **result) {
    if (str == NULL || result == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    char ch = (char)c;
    
    for (; *str; ++str) {
        if (*str == ch) {
            *result = (char *)str;
            return STATUS_OK;
        }
    }
    
    if (ch == '\0') {
        *result = (char *)str;
        return STATUS_OK;
    }
    
    *result = NULL;
    return STATUS_ERROR_NOT_FOUND;
}