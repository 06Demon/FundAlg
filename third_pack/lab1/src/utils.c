#include "../include/utils.h"
#include <stdlib.h>

static const char DIGITS[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";

StatusCode decimal_to_base2r(int decimal, int r, char **result) {
    if (result == NULL) return ERROR_NULL_POINTER;
    if (r < 1 || r > 5) return ERROR_INVALID_BASE;
    if (decimal < 0) return ERROR_NEGATIVE_NUMBER;

    if (decimal == 0) {
        *result = (char*)malloc(2 * sizeof(char));
        if (*result == NULL) return ERROR_MEMORY_ALLOCATION;
        (*result)[0] = '0';
        (*result)[1] = '\0';
        return SUCCESS;
    }

    unsigned int num = (unsigned int)decimal;
    const unsigned int mask = (1u << r) - 1;
    char buffer[32];
    int index = 0;

    while (num != 0) {
        unsigned int digit = num & mask;
        buffer[index++] = DIGITS[digit];
        num = num >> r;
    }

    *result = (char*)malloc((index + 1) * sizeof(char));
    if (*result == NULL) return ERROR_MEMORY_ALLOCATION;

    for (int i = 0; i < index; i++) {
        (*result)[i] = buffer[index - 1 - i];
    }
    (*result)[index] = '\0';

    return SUCCESS;
}