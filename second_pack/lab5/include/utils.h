#ifndef UTILS_H
#define UTILS_H

typedef enum {
    SUCCESS,
    ERROR_NULL_POINTER,
    ERROR_FILE_OPEN,
    ERROR_FILE_WRITE,
    ERROR_MEMORY_ALLOCATION,
    ERROR_INVALID_INPUT,
    ERROR_EMPTY_PATH,
    BUFFER_OVERFLOW,
    ERROR_LARGE_WORD,
} StatusCode;

StatusCode process_file(const char* input_path, const char* output_path);

#endif