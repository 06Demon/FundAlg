#include <stdio.h>
#include <stdlib.h>
#include "include/utils.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    char buffer[1024];
    
    printf("Введите строку для проверки скобок: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        fprintf(stderr, "Ошибка чтения ввода\n");
        return 1;
    }
    
    StatusCode result = check_brackets(buffer);
    
    switch (result) {
        case SUCCESS:
            printf("Результат: Скобки сбалансированы\n");
            break;
        case BRACKETS_NOT_BALANCED:
            printf("Результат: Скобки не сбалансированы\n");
            break;
        case MEMORY_ERROR:
            fprintf(stderr, "Ошибка выделения памяти\n");
            return 1;
        case INVALID_ARGUMENT:
            fprintf(stderr, "Некорректные аргументы\n");
            return 1;
        default:
            fprintf(stderr, "Неизвестная ошибка\n");
            return 1;
    }
    
    return 0;
}