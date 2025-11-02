#include "memchr.h"

StatusCode my_memchr(const void *str, int c, size_t n, void **result) {
    if (str == NULL || result == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    const unsigned char *s = (const unsigned char *)str;
    unsigned char uc = (unsigned char)c;
    
    for (size_t i = 0; i < n; ++i) {
        if (s[i] == uc) {
            *result = (void *)(s + i);
            return STATUS_OK;
        }
    }
    
    *result = NULL;
    return STATUS_ERROR_NOT_FOUND;
}