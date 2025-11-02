#include "strpbrk.h"
#include <stddef.h>

StatusCode my_strpbrk(const char *str1, const char *str2, char **result) {
    if (str1 == NULL || str2 == NULL || result == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    for (; *str1; ++str1) {
        for (const char *p = str2; *p; ++p) {
            if (*str1 == *p) {
                *result = (char *)str1;
                return STATUS_OK;
            }
        }
    }
    
    *result = NULL;
    return STATUS_ERROR_NOT_FOUND;
}