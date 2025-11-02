#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/utils.h"

#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_PATH_LENGTH 256
#define EPSILON 1e-9

static void print_status_message(StatusCode status) {
    switch (status) {
        case SUCCESS:
            printf("Операция успешно завершена.\n");
            break;
        case ERROR_NULL_POINTER:
            printf("Ошибка: Обнаружен нулевой указатель.\n");
            break;
        case ERROR_FILE_OPEN:
            printf("Ошибка: Не удалось открыть файл.\n");
            break;
        case ERROR_FILE_READ:
            printf("Ошибка: Не удалось прочитать файл.\n");
            break;
        case ERROR_FILE_WRITE:
            printf("Ошибка: Не удалось записать в файл.\n");
            break;
        case ERROR_MEMORY_ALLOCATION:
            printf("Ошибка: Не удалось выделить память.\n");
            break;
        case ERROR_INVALID_INPUT:
            printf("Ошибка: Некорректные входные данные.\n");
            break;
        default:
            printf("Ошибка: Произошла неизвестная ошибка.\n");
            break;
    }
}

static StatusCode validate_different_files(const char* input_path, const char* output_path) {
    if (strcmp(input_path, output_path) == 0) {
        return ERROR_INVALID_INPUT;
    }
    return SUCCESS;
}

int main() {
    #ifdef _WIN32
	    SetConsoleOutputCP(CP_UTF8);
    #endif
    char input_path[MAX_PATH_LENGTH];
    char output_path[MAX_PATH_LENGTH];
    
    printf("Введите путь к входному файлу: ");
    if (fgets(input_path, MAX_PATH_LENGTH, stdin) == NULL) {
        printf("Ошибка чтения пути входного файла.\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_path);
    if (input_len > 0 && input_path[input_len - 1] == '\n') {
        input_path[input_len - 1] = '\0';
    }
    
    printf("Введите путь к выходному файлу: ");
    if (fgets(output_path, MAX_PATH_LENGTH, stdin) == NULL) {
        printf("Ошибка чтения пути выходного файла.\n");
        return EXIT_FAILURE;
    }
    
    size_t output_len = strlen(output_path);
    if (output_len > 0 && output_path[output_len - 1] == '\n') {
        output_path[output_len - 1] = '\0';
    }
    
    if (strlen(input_path) == 0 || strlen(output_path) == 0) {
        printf("Ошибка: Пути к файлам не могут быть пустыми.\n");
        return EXIT_FAILURE;
    }
    
    StatusCode validation_result = validate_different_files(input_path, output_path);
    if (validation_result != SUCCESS) {
        printf("Ошибка: Входной и выходной файл должны быть разными.\n");
        return EXIT_FAILURE;
    }
    
    StatusCode result = process_file(input_path, output_path);
    
    if (result != SUCCESS) {
        print_status_message(result);
        return EXIT_FAILURE;
    }
    
    printf("Обработка текста успешно завершена.\n");
    return EXIT_SUCCESS;
}