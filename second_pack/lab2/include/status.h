#ifndef STATUS_H
#define STATUS_H

typedef enum {
    STATUS_OK = 0,
    STATUS_ERROR_NULL_POINTER,
    STATUS_ERROR_INVALID_PARAMETER,
    STATUS_ERROR_NOT_FOUND,
    STATUS_ERROR_BUFFER_OVERFLOW,
    STATUS_ERROR_EMPTY_STRING,
    STATUS_ERROR_OUT_OF_RANGE
} StatusCode;

const char* status_message(StatusCode status);

#endif