#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/utils.h"

#ifdef _WIN32
#include <windows.h>
#endif

static void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static void print_menu(void) {
    printf("\n------------------------------\n");
    printf("Выберите вариант:\n");
    printf(" 1. Проверить %%Ro  (римские цифры)\n");
    printf(" 2. Проверить %%Zr  (цекендорфово представление)\n");
    printf(" 3. Проверить %%Cv  (система счисления, нижний регистр)\n");
    printf(" 4. Проверить %%CV  (система счисления, верхний регистр)\n");
    printf(" 5. Проверить чтение из файла (overfscanf)\n");
    printf(" 0. Выход\n");
    printf("Ваш выбор: ");
}

static void handler_of_status(StatusCode status, int count){
    switch (status) {
        case SUCCESS:
            printf("Операция успешно завершена.\nСчитано %d полей\n", count);
            break;
        case NULL_POINTER:
            printf("Ошибка: Обнаружен нулевой указатель.\nСчитано %d полей\n", count);
            break;
        case INVALID_BASE:
            printf("Ошибка: Не верное основание СС.\nСчитано %d полей\n", count);
            break;
        case INVALID_ZECKENDORF:
            printf("Ошибка: Не верно передано цекендорфово число. На конце обязательно должна быть дополнительная единица.\nСчитано %d полей\n", count);
            break;
        case INVALID_ROMAN:
            printf("Ошибка: Не верно передано римское число.\nСчитано %d полей\n", count);
            break;
        case INVALID_OTHER_CC_NUMBER:
            printf("Ошибка: Не верно передано число в другой системе счисления.\nСчитано %d полей\n", count);
            break;
        default:
            printf("Ошибка: Произошла неизвестная ошибка.\nСчитано %d полей\n", count);
            break;
    }
}

static void test_Ro(void) {
    char input[128];
    printf("Введите число в римской записи (пример: MCMXCIV): ");
    if (!fgets(input, sizeof(input), stdin)) return;
    input[strcspn(input, "\n")] = 0;
    int value = 0;
    int count = 0;
    StatusCode status = my_oversscanf(input, &count, "%Ro", &value);
    if (status == SUCCESS)
        printf("Считано успешно! Значение = %d\n", value);
    else 
        handler_of_status(status, count);
}

static void test_Zr(void) {
    char input[128];
    printf("Введите цекендорфово представление (пример: 1011): ");
    if (!fgets(input, sizeof(input), stdin)) return;
    input[strcspn(input, "\n")] = 0;
    unsigned int value = 0;
    int count = 0;
    StatusCode status = my_oversscanf(input, &count, "%Zr", &value);
    if (status == SUCCESS)
        printf("Считано успешно! Значение = %d\n", value);
    else 
        handler_of_status(status, count);
}

static void test_Cv(void) {
    char input[128];
    printf("Введите число в произвольной системе счисления (пример: 1a): ");
    if (!fgets(input, sizeof(input), stdin)) return;
    input[strcspn(input, "\n")] = 0;
    int base;
    printf("Введите основание системы счисления (2..36): ");
    if (scanf("%d", &base) != 1) { clear_stdin(); return; }
    clear_stdin();
    int value = 0;
    int count = 0;
    if (base < 2 || base > 36) base = 10;
    StatusCode status = my_oversscanf(input, &count, "%Cv", &value);
    if (status == SUCCESS)
        printf("Считано успешно! Значение = %d\n", value);
    else 
        handler_of_status(status, count);
}

static void test_CV(void) {
    char input[128];
    printf("Введите число в произвольной системе счисления (пример: 1A): ");
    if (!fgets(input, sizeof(input), stdin)) return;
    input[strcspn(input, "\n")] = 0;
    int base;
    printf("Введите основание системы счисления (2..36): ");
    if (scanf("%d", &base) != 1) { clear_stdin(); return; }
    clear_stdin();
    int value = 0;
    int count = 0;
    if (base < 2 || base > 36) base = 10;
    StatusCode status = my_oversscanf(input, &count, "%CV", &value);
    if (status == SUCCESS)
        printf("Считано успешно! Значение = %d\n", value);
    else 
        handler_of_status(status, count);
}

static void test_file(void) {
    char filename[128];
    printf("Введите имя файла (пример: data.txt): ");
    if (!fgets(filename, sizeof(filename), stdin)) return;
    filename[strcspn(filename, "\n")] = 0;

    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Ошибка открытия файла\n");
        return;
    }

    int roman = 0;
    unsigned int zeck = 0;
    int valCv = 0;
    int valCV = 0;

    printf("Введите основание для %%Cv и %%CV: ");
    int base;
    if (scanf("%d", &base) != 1) { clear_stdin(); fclose(f); return; }
    clear_stdin();

    printf("\nПопытка чтения из файла формата: %%Ro %%Zr %%Cv %%CV\n");
    int count = 0;
    StatusCode status = my_overfscanf(f, &count, "%Ro %Zr %Cv %CV", &roman, &zeck, &valCv, base, &valCV, base);
    fclose(f);

    if (status == SUCCESS) {
        printf("\nРезультаты чтения (%d полей успешно):\n", count);
        printf(" Римское число: %d\n", roman);
        printf(" Цекендорфово представление: %u\n", zeck);
        printf(" Cv (нижний регистр): %d\n", valCv);
        printf(" CV (верхний регистр): %d\n", valCV);
    } else {
        handler_of_status(status, count);
    }
}

int main(void) {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    
    int choice;
    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            clear_stdin();
            continue;
        }
        clear_stdin();

        switch (choice) {
            case 1: test_Ro(); break;
            case 2: test_Zr(); break;
            case 3: test_Cv(); break;
            case 4: test_CV(); break;
            case 5: test_file(); break;
            case 0: printf("Выход из программы.\n"); break;
            default: printf("Неверный выбор.\n"); break;
        }
    } while (choice != 0);

    return 0;
}