#include "strncat.h"

StatusCode my_strncat(char *dest, size_t dest_size, const char *src, size_t n) {
    if (dest == NULL || src == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    if (dest_size == 0) {
        return STATUS_ERROR_BUFFER_OVERFLOW;
    }
    
    size_t dest_len = 0;
    while (dest_len < dest_size - 1 && dest[dest_len] != '\0') {
        dest_len++;
    }
    
    if (dest_len >= dest_size - 1) {
        return STATUS_ERROR_BUFFER_OVERFLOW;
    }
    
    size_t available = dest_size - dest_len - 1;
    size_t to_copy = (n < available) ? n : available;
    
    for (size_t i = 0; i < to_copy && src[i] != '\0'; ++i) {
        dest[dest_len + i] = src[i];
    }
    
    dest[dest_len + to_copy] = '\0';
    
    return STATUS_OK;
}