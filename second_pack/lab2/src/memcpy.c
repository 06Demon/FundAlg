#include "memcpy.h"
#include <string.h>

StatusCode my_memcpy(void *dest, const void *src, size_t n) {
    if (dest == NULL || src == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    if (dest == src) {
        return STATUS_OK;
    }

    size_t src_len = strlen((const char *)src);

    if (n > src_len) {
        return STATUS_ERROR_OUT_OF_RANGE;
    }
    
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    

    for (size_t i = 0; i < n; ++i) {
        d[i] = s[i];
    }
    
    return STATUS_OK;
}