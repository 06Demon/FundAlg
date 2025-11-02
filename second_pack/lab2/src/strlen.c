#include "strlen.h"

StatusCode my_strlen(const char *str, size_t *result) {
    if (str == NULL || result == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    const char *s = str;
    while (*s) ++s;
    *result = (size_t)(s - str);
    
    return STATUS_OK;
}