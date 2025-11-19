#include <stdio.h>
#include "include/utils.h"

#ifdef _WIN32
#include <windows.h>
#endif


int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    int number, r;
    char *result = NULL;
    
    printf("Введите десятичное число: ");
    if (scanf("%d", &number) != 1) {
        printf("Ошибка ввода числа\n");
        return 1;
    }
    
    printf("Введите r (1-5): ");
    if (scanf("%d", &r) != 1) {
        printf("Ошибка ввода r\n");
        return 1;
    }

    StatusCode status = decimal_to_base2r(number, r, &result);
    
    switch (status) {
        case SUCCESS:
            printf("Результат: %s\n", result);
            break;
        case ERROR_INVALID_BASE:
            printf("Ошибка: неверное значение r\n");
            break;
        case ERROR_NEGATIVE_NUMBER:
            printf("Ошибка: отрицательное число\n");
            break;
        case ERROR_MEMORY_ALLOCATION:
            printf("Ошибка выделения памяти\n");
            break;
        default:
            printf("Неизвестная ошибка\n");
    }
    
    if (result) free(result);
    return 0;
}