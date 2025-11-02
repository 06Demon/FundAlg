#include "memcmp.h"

StatusCode my_memcmp(const void *str1, const void *str2, size_t n, int *result) {
    if (str1 == NULL || str2 == NULL || result == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    const unsigned char *s1 = (const unsigned char *)str1;
    const unsigned char *s2 = (const unsigned char *)str2;
    
    for (size_t i = 0; i < n; ++i) {
        if (s1[i] != s2[i]) {
            *result = (int)s1[i] - (int)s2[i];
            return STATUS_OK;
        }
    }
    
    *result = 0;
    return STATUS_OK;
}