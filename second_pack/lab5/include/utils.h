#ifndef UTILS_H
#define UTILS_H

typedef enum {
    SUCCESS,
    ERROR_NULL_POINTER,
    ERROR_FILE_OPEN,
    ERROR_FILE_READ,
    ERROR_FILE_WRITE,
    ERROR_MEMORY_ALLOCATION,
    ERROR_INVALID_INPUT
} StatusCode;

StatusCode process_file(const char* input_path, const char* output_path);

#endif