#include "memset.h"

StatusCode my_memset(void *str, int c, size_t n) {
    if (str == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    unsigned char *s = (unsigned char *)str;
    unsigned char uc = (unsigned char)c;
    
    for (size_t i = 0; i < n; ++i) {
        s[i] = uc;
    }
    
    return STATUS_OK;
}