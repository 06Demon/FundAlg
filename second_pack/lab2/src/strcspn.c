#include "strcspn.h"

StatusCode my_strcspn(const char *str1, const char *str2, size_t *result) {
    if (str1 == NULL || str2 == NULL || result == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    size_t len = 0;
    for (; str1[len]; ++len) {
        const char *p = str2;
        while (*p) {
            if (str1[len] == *p) {
                *result = len;
                return STATUS_OK;
            }
            ++p;
        }
    }
    
    *result = len;
    return STATUS_OK;
}