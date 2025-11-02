#include "strtok.h"
#include <stddef.h>

char *my_strtok(char *str, const char *delim) {
    static char *saveptr = NULL;
    char *s = (str != NULL) ? str : saveptr;
    if (s == NULL) return NULL;

    while (*s) {
        const char *d = delim;
        int is_delim = 0;
        while (*d) {
            if (*s == *d) { is_delim = 1; break; }
            ++d;
        }
        if (!is_delim) break;
        ++s;
    }

    if (*s == '\0') {
        saveptr = NULL;
        return NULL;
    }

    char *token = s;
    while (*s) {
        const char *d = delim;
        while (*d) {
            if (*s == *d) {
                *s = '\0';
                saveptr = s + 1;
                return token;
            }
            ++d;
        }
        ++s;
    }

    saveptr = NULL;
    return token;
}
