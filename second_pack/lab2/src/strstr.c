#include "strstr.h"
#include <stddef.h>
StatusCode my_strstr(const char *haystack, const char *needle, char **result) {
    if (haystack == NULL || needle == NULL || result == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    if (*needle == '\0') {
        *result = (char *)haystack;
        return STATUS_OK;
    }
    
    for (; *haystack; ++haystack) {
        const char *h = haystack;
        const char *n = needle;
        
        while (*h && *n && *h == *n) {
            ++h;
            ++n;
        }
        
        if (*n == '\0') {
            *result = (char *)haystack;
            return STATUS_OK;
        }
    }
    
    *result = NULL;
    return STATUS_ERROR_NOT_FOUND;
}