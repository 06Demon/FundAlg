#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "include/utils.h"

#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_PATH_LENGTH 256

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
        case ERROR_MEMORY_ALLOCATION:
            printf("Ошибка: Не удалось выделить память.\n");
            break;
        case ERROR_INVALID_INPUT:
            printf("Ошибка: Входной и выходной файл должны быть разными.\n");
            break;
        case ERROR_EMPTY_PATH:
            printf("Ошибка: Пути к файлам не могут быть пустыми.\n");
            break;
        case ERROR_LARGE_WORD:
            printf("Ошибка: Во входном файле было встречено слово больше 80 символов.\n");
            break;
        default:
            printf("Ошибка: Произошла неизвестная ошибка.\n");
            break;
    }
}

static StatusCode validate_file_paths(const char* input_path, const char* output_path) {
    char real_input_path[MAX_PATH_LENGTH];
    char real_output_path[MAX_PATH_LENGTH];

    if (input_path == NULL || output_path == NULL) 
        return ERROR_NULL_POINTER;
    if (strlen(input_path) == 0 || strlen(output_path) == 0)
        return ERROR_EMPTY_PATH;
    if (strlen(input_path) >= MAX_PATH_LENGTH || strlen(output_path) >= MAX_PATH_LENGTH)
        return BUFFER_OVERFLOW;
    if (_fullpath(real_input_path, input_path, MAX_PATH_LENGTH) && _fullpath(real_output_path, output_path, MAX_PATH_LENGTH))
        {if (strcmp(real_input_path, real_output_path) == 0)
            {return ERROR_INVALID_INPUT;}}
    else
        {if (strcmp(input_path, output_path) == 0)
            {return ERROR_INVALID_INPUT;}}

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
        return 1;
    }
    
    size_t input_len = strlen(input_path);
    if (input_len > 0 && input_path[input_len - 1] == '\n') {
        input_path[input_len - 1] = '\0';
    }
    
    printf("Введите путь к выходному файлу: ");
    if (fgets(output_path, MAX_PATH_LENGTH, stdin) == NULL) {
        printf("Ошибка чтения пути выходного файла.\n");
        return 1;
    }
    
    size_t output_len = strlen(output_path);
    if (output_len > 0 && output_path[output_len - 1] == '\n') {
        output_path[output_len - 1] = '\0';
    }
    
    StatusCode validation_result = validate_file_paths(input_path, output_path);
    if (validation_result != SUCCESS) {
        print_status_message(validation_result);
        return 1;
    }
    
    StatusCode result = process_file(input_path, output_path);
    
    if (result != SUCCESS) {
        print_status_message(result);
        return 1;
    }
    
    printf("Обработка текста успешно завершена.\n");
    return 0;
}