#include "status.h"

const char* status_message(StatusCode status) {
    switch (status) {
        case STATUS_OK: return "Success";
        case STATUS_ERROR_NULL_POINTER: return "Null pointer error";
        case STATUS_ERROR_INVALID_PARAMETER: return "Invalid parameter";
        case STATUS_ERROR_NOT_FOUND: return "Not found";
        case STATUS_ERROR_BUFFER_OVERFLOW: return "Buffer overflow";
        case STATUS_ERROR_EMPTY_STRING: return "Empty string";
        case STATUS_ERROR_OUT_OF_RANGE: return "Out of range";
        default: return "Unknown error";
    }
}