#include "strerror.h"
#include <stddef.h>

static const char *messages_linux[] = {
    "No error", "Operation not permitted", "No such file or directory",
    "No such process", "Interrupted system call", "Input/output error",
    "No such device or address", "Argument list too long", "Exec format error",
    "Bad file descriptor", "No child processes", "Resource temporarily unavailable",
    "Cannot allocate memory", "Permission denied", "Bad address",
    "Block device required", "Device or resource busy", "File exists",
    "Invalid cross-device link", "No such device", "Not a directory",
    "Is a directory", "Invalid argument", "Too many open files in system",
    "Too many open files", "Inappropriate ioctl for device", "Text file busy",
    "File too large", "No space left on device", "Illegal seek",
    "Read-only file system", "Too many links", "Broken pipe",
    "Numerical argument out of domain", "Numerical result out of range"
};

#if defined(__APPLE__)
static const char *messages_mac[] = {
    "Undefined error: 0", "Operation not permitted", "No such file or directory",
    "No such process", "Interrupted system call", "Input/output error",
    "Device not configured", "Argument list too long", "Exec format error",
    "Bad file descriptor", "No child processes", "Resource deadlock avoided",
    "Cannot allocate memory", "Permission denied", "Bad address",
    "Block device required", "Resource busy", "File exists", "Cross-device link",
    "Operation not supported by device", "Not a directory", "Is a directory",
    "Invalid argument", "Too many open files in system", "Too many open files",
    "Inappropriate ioctl for device", "Text file busy", "File too large",
    "No space left on device", "Illegal seek", "Read-only file system",
    "Too many links", "Broken pipe", "Numerical argument out of domain",
    "Result too large"
};
#endif

StatusCode my_strerror(int errnum, const char **result) {
    if (result == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }
    
    if (errnum < 0) {
        *result = "Unknown error";
        return STATUS_ERROR_OUT_OF_RANGE;
    }
    
#if defined(__APPLE__)
    size_t cnt = sizeof(messages_mac) / sizeof(messages_mac[0]);
    if ((size_t)errnum >= cnt) {
        *result = "Unknown error";
        return STATUS_ERROR_OUT_OF_RANGE;
    }
    *result = messages_mac[errnum];
#elif defined(__linux__)
    size_t cnt = sizeof(messages_linux) / sizeof(messages_linux[0]);
    if ((size_t)errnum >= cnt) {
        *result = "Unknown error";
        return STATUS_ERROR_OUT_OF_RANGE;
    }
    *result = messages_linux[errnum];
#else
    size_t cnt = sizeof(messages_linux) / sizeof(messages_linux[0]);
    if ((size_t)errnum >= cnt) {
        *result = "Unknown error";
        return STATUS_ERROR_OUT_OF_RANGE;
    }
    *result = messages_linux[errnum];
#endif
    
    return STATUS_OK;
}