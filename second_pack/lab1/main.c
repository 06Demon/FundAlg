#include <stdio.h>
#include "include/utils.h"

#ifdef _WIN32
#include <windows.h>
#endif


int main(void) {
    #ifdef _WIN32
	    SetConsoleOutputCP(CP_UTF8);
    #endif

    unsigned int count_fractions = 4;

    int results[count_fractions];
    StatusCode status = check_final_representation(10, 1.0e-9, count_fractions, results, 0.5, 0.25, 0.3333, 1.0/3.0);

    if (status != SUCCESS) {
        printf("Ошибка при проверке дробей\n");
        
        if(status == INVALID_BASE){
            printf("Одно из оснований СС неверно (основание должно быть в пределах от 2 до 36 включительно)");
        }
        
        else if (status == INVALID_VALUE) {
            printf("Одно из чисел передано не в виде десятичной дроби");
        }

        else if (status == NULL_POINTER) {
            printf("Не передан указатель на массив для результата");
        }
    }
    else {
        for (unsigned int i = 0; i < count_fractions; ++i) {
            printf("Дробь %d: %s\n", i + 1, results[i] ? "конечна" : "бесконечна");
        }
    }

    return 0;
}
