#define _POSIX_C_SOURCE 200809L
#include "../include/utils.h"
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

static int roman_value(char c) {
    switch (toupper((unsigned char)c)) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0;
    }
}

static StatusCode parse_roman_file(FILE *f, int *out) {
    int c;
    char buf[64];
    size_t idx = 0;
    
    while ((c = fgetc(f)) != EOF && isspace((unsigned char)c)) {}
    if (c == EOF) return NULL_POINTER;
    
    if (!roman_value((char)c)) {
        ungetc(c, f);
        return INVALID_ROMAN;
    }
    
    do {
        if (idx + 1 < sizeof(buf)) buf[idx++] = (char)c;
        c = fgetc(f);
    } while (c != EOF && roman_value((char)c));
    
    if (c != EOF) ungetc(c, f);
    if (idx == 0) return INVALID_ROMAN;
    
    buf[idx] = '\0';
    int total = 0;
    for (size_t i = 0; i < idx; ++i) {
        int val = roman_value(buf[i]);
        int next = (i+1<idx)? roman_value(buf[i+1]) : 0;
        if (val < next) total -= val; 
        else total += val;
    }
    *out = total;
    return SUCCESS;
}

static StatusCode parse_roman_str(const char **ps, int *out) {
    const char *s = *ps;
    while (*s && isspace((unsigned char)*s)) ++s;
    if (!*s) return NULL_POINTER;
    if (!roman_value(*s)) return INVALID_ROMAN;
    
    char buf[64];
    size_t idx = 0;
    while (*s && roman_value(*s)) {
        if (idx + 1 < sizeof(buf)) buf[idx++] = *s;
        s++;
    }
    if (idx == 0) return INVALID_ROMAN;
    
    buf[idx] = '\0';
    int total = 0;
    for (size_t i = 0; i < idx; ++i) {
        int val = roman_value(buf[i]);
        int next = (i+1<idx)? roman_value(buf[i+1]) : 0;
        if (val < next) total -= val; 
        else total += val;
    }
    *out = total;
    *ps = s;
    return SUCCESS;
}

static unsigned long long fib_at(int k) {
    if (k <= 0) return 0ULL;
    if (k == 1 || k == 2) return 1ULL;
    unsigned long long a = 1, b = 1, c;
    for (int i = 3; i <= k; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

static StatusCode parse_zeckendorf_file(FILE *f, unsigned int *out) {
    int c;
    while ((c = fgetc(f)) != EOF && isspace((unsigned char)c)) {}
    if (c == EOF) return NULL_POINTER;
    if (c != '0' && c != '1') { ungetc(c,f); return INVALID_ZECKENDORF; }
    
    char buf[512];
    size_t idx = 0;
    do {
        if (idx + 1 < sizeof(buf)) buf[idx++] = (char)c;
        else return INVALID_ZECKENDORF;
        c = fgetc(f);
    } while (c != EOF && (c == '0' || c == '1'));
    
    if (c != EOF) ungetc(c, f);
    if (idx == 0) return INVALID_ZECKENDORF;
    if (buf[idx-1] != '1') return INVALID_ZECKENDORF;
    if (idx < 2) return INVALID_ZECKENDORF;

    
    for (size_t i = 0; i < idx - 2; i++) {
        if (buf[i] == '1' && buf[i+1] == '1') {
            return INVALID_ZECKENDORF;
        }
    }
    
    unsigned long long sum = 0;
    for (size_t i = 0; i + 1 < idx; ++i) {
        if (buf[i] == '1') {
            int k = (int)(i + 2);
            unsigned long long fval = fib_at(k);
            sum += fval;
            if (sum > (unsigned long long)UINT_MAX) sum = (unsigned long long)UINT_MAX;
        }
    }
    *out = (unsigned int)sum;
    return SUCCESS;
}

static StatusCode parse_zeckendorf_str(const char **ps, unsigned int *out) {
    const char *s = *ps;
    while (*s && isspace((unsigned char)*s)) ++s;
    if (!*s) return NULL_POINTER;
    if (*s != '0' && *s != '1') return INVALID_ZECKENDORF;
    
    char buf[512];
    size_t idx = 0;
    while (*s && (*s == '0' || *s == '1')) {
        if (idx + 1 < sizeof(buf)) buf[idx++] = *s;
        else return 0;
        ++s;
    }
    if (idx == 0) return INVALID_ZECKENDORF;
    if (buf[idx-1] != '1') return INVALID_ZECKENDORF;
    if (idx < 2) return INVALID_ZECKENDORF;

    for (size_t i = 0; i < idx - 2; i++) {
        if (buf[i] == '1' && buf[i+1] == '1') {
            return INVALID_ZECKENDORF;
        }
    }
    
    unsigned long long sum = 0;
    for (size_t i = 0; i + 1 < idx; ++i) {
        if (buf[i] == '1') {
            int k = (int)(i + 2);
            unsigned long long fval = fib_at(k);
            sum += fval;
            if (sum > (unsigned long long)UINT_MAX) sum = (unsigned long long)UINT_MAX;
        }
    }
    *out = (unsigned int)sum;
    *ps = s;
    return SUCCESS;
}

static StatusCode parse_base_int_file(FILE *f, int *dest, int base, int case_flag) {
    int c;
    while ((c = fgetc(f)) != EOF && isspace((unsigned char)c)) {}
    if (c == EOF) return NULL_POINTER;
    
    char buf[128];
    size_t idx = 0;
    if (c == '+' || c == '-') {
        if (idx + 1 < sizeof(buf)) buf[idx++] = (char)c;
        c = fgetc(f);
    }
    
    while (c != EOF) {
        char ch = (char)c;
        char ch2 = ch;
        if (isalpha((unsigned char)ch)) {
            if (case_flag < 0) ch2 = (char)tolower((unsigned char)ch);
            else if (case_flag > 0) ch2 = (char)toupper((unsigned char)ch);
        }
        int val;
        if (ch2 >= '0' && ch2 <= '9') val = ch2 - '0';
        else if (ch2 >= 'a' && ch2 <= 'z') val = ch2 - 'a' + 10;
        else if (ch2 >= 'A' && ch2 <= 'Z') val = ch2 - 'A' + 10;
        else break;
        if (val >= base) return INVALID_BASE;
        if (idx + 1 < sizeof(buf)) buf[idx++] = ch2;
        else break;
        c = fgetc(f);
    }
    if (c != EOF) ungetc(c, f);
    if (idx == 0) return INVALID_OTHER_CC_NUMBER;
    
    buf[idx] = '\0';
    char *endptr = NULL;
    long val = strtol(buf, &endptr, base);
    if (endptr == buf) return INVALID_OTHER_CC_NUMBER;
    *dest = (int)val;
    return SUCCESS;
}

static StatusCode parse_base_int_str(const char **ps, int *dest, int base, int case_flag) {
    const char *s = *ps;
    while (*s && isspace((unsigned char)*s)) ++s;
    if (!*s) return NULL_POINTER;
    
    char buf[128];
    size_t idx = 0;
    if (*s == '+' || *s == '-') {
        if (idx + 1 < sizeof(buf)) buf[idx++] = *s;
        ++s;
    }
    
    while (*s) {
        char ch = *s;
        char ch2 = ch;
        if (isalpha((unsigned char)ch)) {
            if (case_flag < 0) ch2 = (char)tolower((unsigned char)ch);
            else if (case_flag > 0) ch2 = (char)toupper((unsigned char)ch);
        }
        int val;
        if (ch2 >= '0' && ch2 <= '9') val = ch2 - '0';
        else if (ch2 >= 'a' && ch2 <= 'z') val = ch2 - 'a' + 10;
        else if (ch2 >= 'A' && ch2 <= 'Z') val = ch2 - 'A' + 10;
        else break;
        if (val >= base) return INVALID_BASE;
        if (idx + 1 < sizeof(buf)) buf[idx++] = ch2;
        else break;
        ++s;
    }
    if (idx == 0) return INVALID_OTHER_CC_NUMBER;
    
    buf[idx] = '\0';
    char *endptr = NULL;
    long val = strtol(buf, &endptr, base);
    if (endptr == buf) return INVALID_OTHER_CC_NUMBER;
    *dest = (int)val;
    *ps = s;
    return SUCCESS;
}

static int match_char_file(FILE *f, char want) {
    int c = fgetc(f);
    if (c == EOF) return 0;
    if ((char)c == want) return 1;
    ungetc(c, f);
    return 0;
}

static int match_char_str(const char **ps, char want) {
    const char *s = *ps;
    if (!*s) return 0;
    if (*s == want) {
        ++(*ps);
        return 1;
    }
    return 0;
}

StatusCode my_overfscanf(FILE *stream, int *ret, const char *format, ...) {
    if (!stream || !format || !ret) return NULL_POINTER;
    
    va_list ap;
    va_start(ap, format);
    int assigned = 0;
    const char *p = format;
    StatusCode final_status = SUCCESS;
    
    while (*p) {
        if (*p == '%') {
            ++p;
            if (p[0] == 'R' && p[1] == 'o') {
                p += 2;
                int *dest = va_arg(ap, int *);
                StatusCode status = parse_roman_file(stream, dest);
                if (status != SUCCESS) {
                    final_status = status;
                    break;
                }
                assigned++;
                continue;
            } else if (p[0] == 'Z' && p[1] == 'r') {
                p += 2;
                unsigned int *dest = va_arg(ap, unsigned int *);
                StatusCode status = parse_zeckendorf_file(stream, dest);
                if (status != SUCCESS) {
                    final_status = status;
                    break;
                }
                assigned++;
                continue;
            } else if (p[0] == 'C' && p[1] == 'v') {
                p += 2;
                int *dest = va_arg(ap, int *);
                int base = va_arg(ap, int);
                StatusCode status = parse_base_int_file(stream, dest, base, -1);
                if (status != SUCCESS) {
                    final_status = status;
                    break;
                }
                assigned++;
                continue;
            } else if (p[0] == 'C' && p[1] == 'V') {
                p += 2;
                int *dest = va_arg(ap, int *);
                int base = va_arg(ap, int);
                StatusCode status = parse_base_int_file(stream, dest, base, +1);
                if (status != SUCCESS) {
                    final_status = status;
                    break;
                }
                assigned++;
                continue;
            } else {
                char conv = *p;
                if (conv == 'd') {
                    int *dest = va_arg(ap, int *);
                    int result = fscanf(stream, "%d", dest);
                    if (result != 1) {
                        final_status = (assigned == 0) ? SCAN_ERROR : SUCCESS;
                        break;
                    }
                    assigned++;
                    ++p;
                    continue;
                } else if (conv == 'u') {
                    unsigned int *dest = va_arg(ap, unsigned int *);
                    int result = fscanf(stream, "%u", dest);
                    if (result != 1) {
                        final_status = (assigned == 0) ? SCAN_ERROR : SUCCESS;
                        break;
                    }
                    assigned++;
                    ++p;
                    continue;
                } else if (conv == 'c') {
                    char *dest = va_arg(ap, char *);
                    int c = fgetc(stream);
                    if (c == EOF) {
                        final_status = (assigned == 0) ? SCAN_ERROR : SUCCESS;
                        break;
                    }
                    *dest = (char)c;
                    assigned++;
                    ++p;
                    continue;
                } else if (conv == 's') {
                    char *dest = va_arg(ap, char *);
                    int result = fscanf(stream, "%s", dest);
                    if (result != 1) {
                        final_status = (assigned == 0) ? SCAN_ERROR : SUCCESS;
                        break;
                    }
                    assigned++;
                    ++p;
                    continue;
                } else if (conv == 'f') {
                    float *dest = va_arg(ap, float *);
                    int result = fscanf(stream, "%f", dest);
                    if (result != 1) {
                        final_status = (assigned == 0) ? SCAN_ERROR : SUCCESS;
                        break;
                    }
                    assigned++;
                    ++p;
                    continue;
                } else if (isspace((unsigned char)conv)) {
                    int c;
                    while ((c = fgetc(stream)) != EOF && isspace((unsigned char)c)) {}
                    if (c != EOF) ungetc(c, stream);
                    ++p;
                    continue;
                } else {
                    if (!match_char_file(stream, conv)) {
                        final_status = (assigned == 0) ? SCAN_ERROR : SUCCESS;
                        break;
                    }
                    ++p;
                    continue;
                }
            }
        } else if (isspace((unsigned char)*p)) {
            int c;
            while ((c = fgetc(stream)) != EOF && isspace((unsigned char)c)) {}
            if (c != EOF) ungetc(c, stream);
            ++p;
        } else {
            if (!match_char_file(stream, *p)) {
                final_status = (assigned == 0) ? SCAN_ERROR : SUCCESS;
                break;
            }
            ++p;
        }
    }
    
    va_end(ap);
    *ret = assigned;
    
    if (final_status == SCAN_ERROR && assigned > 0) {
        return SUCCESS;
    }
    
    return final_status;
}

StatusCode my_oversscanf(const char *s, int *ret, const char *format, ...) {
    if (!s || !format || !ret) return NULL_POINTER;
    
    va_list ap;
    va_start(ap, format);
    const char *p = format;
    const char *sp = s;
    int assigned = 0;
    StatusCode final_status = SUCCESS;
    
    while (*p) {
        if (*p == '%') {
            ++p;
            if (p[0] == 'R' && p[1] == 'o') {
                p += 2;
                int *dest = va_arg(ap, int *);
                StatusCode status = parse_roman_str(&sp, dest);
                if (status != SUCCESS) {
                    final_status = status;
                    break;
                }
                assigned++;
                continue;
            } else if (p[0] == 'Z' && p[1] == 'r') {
                p += 2;
                unsigned int *dest = va_arg(ap, unsigned int *);
                StatusCode status = parse_zeckendorf_str(&sp, dest);
                if (status != SUCCESS) {
                    final_status = status;
                    break;
                }
                assigned++;
                continue;
            } else if (p[0] == 'C' && p[1] == 'v') {
                p += 2;
                int *dest = va_arg(ap, int *);
                int base = va_arg(ap, int);
                if (base < 2 || base > 36) base = 10;
                StatusCode status = parse_base_int_str(&sp, dest, base, -1);
                if (status != SUCCESS) {
                    final_status = status;
                    break;
                }
                assigned++;
                continue;
            } else if (p[0] == 'C' && p[1] == 'V') {
                p += 2;
                int *dest = va_arg(ap, int *);
                int base = va_arg(ap, int);
                if (base < 2 || base > 36) base = 10;
                StatusCode status = parse_base_int_str(&sp, dest, base, +1);
                if (status != SUCCESS) {
                    final_status = status;
                    break;
                }
                assigned++;
                continue;
            } else {
                char conv = *p;
                if (conv == 'd') {
                    int *dest = va_arg(ap, int *);
                    int consumed;
                    int result = sscanf(sp, "%d%n", dest, &consumed);
                    if (result != 1) {
                        final_status = SCAN_ERROR;
                        break;
                    }
                    sp += consumed;
                    assigned++;
                    ++p;
                    continue;
                } else if (conv == 'u') {
                    unsigned int *dest = va_arg(ap, unsigned int *);
                    int consumed;
                    int result = sscanf(sp, "%u%n", dest, &consumed);
                    if (result != 1) {
                        final_status = SCAN_ERROR;
                        break;
                    }
                    sp += consumed;
                    assigned++;
                    ++p;
                    continue;
                } else if (conv == 'c') {
                    char *dest = va_arg(ap, char *);
                    if (*sp == '\0') {
                        final_status = SCAN_ERROR;
                        break;
                    }
                    *dest = *sp++;
                    assigned++;
                    ++p;
                    continue;
                } else if (conv == 's') {
                    char *dest = va_arg(ap, char *);
                    int consumed;
                    int result = sscanf(sp, "%s%n", dest, &consumed);
                    if (result != 1) {
                        final_status = SCAN_ERROR;
                        break;
                    }
                    sp += consumed;
                    assigned++;
                    ++p;
                    continue;
                } else if (conv == 'f') {
                    float *dest = va_arg(ap, float *);
                    int consumed;
                    int result = sscanf(sp, "%f%n", dest, &consumed);
                    if (result != 1) {
                        final_status = SCAN_ERROR;
                        break;
                    }
                    sp += consumed;
                    assigned++;
                    ++p;
                    continue;
                } else if (isspace((unsigned char)conv)) {
                    while (*sp && isspace((unsigned char)*sp)) ++sp;
                    ++p;
                    continue;
                } else {
                    if (!match_char_str(&sp, conv)) {
                        final_status = SCAN_ERROR;
                        break;
                    }
                    ++p;
                    continue;
                }
            }
        } else if (isspace((unsigned char)*p)) {
            while (*sp && isspace((unsigned char)*sp)) ++sp;
            ++p;
        } else {
            if (!match_char_str(&sp, *p)) {
                final_status = SCAN_ERROR;
                break;
            }
            ++p;
        }
    }
    
    va_end(ap);
    *ret = assigned;
    
    if (final_status == SCAN_ERROR && assigned > 0) {
        return SUCCESS;
    }
    
    return final_status;
}