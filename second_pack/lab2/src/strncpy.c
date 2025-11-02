#include "strncpy.h"

StatusCode my_strncpy(char *dest, size_t dest_size, const char *src, size_t n) {
    if (dest == NULL || src == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    if (dest_size == 0) {
        return STATUS_ERROR_BUFFER_OVERFLOW;
    }
    
    size_t i = 0;
    size_t copy_limit = (n < dest_size - 1) ? n : dest_size - 1;
    
    for (; i < copy_limit && src[i] != '\0'; ++i) {
        dest[i] = src[i];
    }
    
    for (; i < n && i < dest_size - 1; ++i) {
        dest[i] = '\0';
    }
    
    if (dest_size > 0) {
        size_t null_pos = (n < dest_size - 1) ? n : dest_size - 1;
        dest[null_pos] = '\0';
    }
    
    return STATUS_OK;
}