#include "strncmp.h"

StatusCode my_strncmp(const char *str1, const char *str2, size_t n, int *result) {
    if (str1 == NULL || str2 == NULL || result == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    for (size_t i = 0; i < n; ++i) {
        unsigned char c1 = (unsigned char)str1[i];
        unsigned char c2 = (unsigned char)str2[i];
        
        if (c1 != c2) {
            *result = (int)c1 - (int)c2;
            return STATUS_OK;
        }
        if (c1 == '\0') {
            *result = 0;
            return STATUS_OK;
        }
    }
    
    *result = 0;
    return STATUS_OK;
}