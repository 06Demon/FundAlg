#include "../include/utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>

typedef struct {
    char *buf;
    size_t len;
    size_t cap;
} dynbuf_t;

static StatusCode db_init(dynbuf_t *d) {
    if (!d) return NULL_POINTER_ERROR;
    
    d->cap = 256;
    d->len = 0;
    d->buf = malloc(d->cap);
    if (!d->buf) return MEMORY_ALLOCATION_ERROR;
    d->buf[0] = '\0';
    return SUCCESS;
}

static void db_free(dynbuf_t *d) {
    if (d && d->buf) {
        free(d->buf);
        d->buf = NULL;
        d->len = d->cap = 0;
    }
}

static StatusCode db_append(dynbuf_t *d, const char *s) {
    if (!d || !s) return NULL_POINTER_ERROR;
    
    size_t sl = strlen(s);
    if (d->len + sl + 1 > d->cap) {
        size_t nc = d->cap * 2;
        while (nc < d->len + sl + 1) nc *= 2;
        char *nb = realloc(d->buf, nc);
        if (!nb) return MEMORY_ALLOCATION_ERROR;
        d->buf = nb;
        d->cap = nc;
    }
    memcpy(d->buf + d->len, s, sl);
    d->len += sl;
    d->buf[d->len] = '\0';
    return SUCCESS;
}

static char *int_to_roman(int value) {
    if (value == 0) {
        char *r = malloc(2);
        if (!r) return NULL;
        r[0] = '0';
        r[1] = '\0';
        return r;
    }

    int sign = 0;
    long v = value;
    if (v < 0) { sign = 1; v = -v; }

    struct { int val; const char *sym; } tbl[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };

    if (v > 3999) {
        char tmp[64];
        snprintf(tmp, sizeof(tmp), "%s%ld", sign ? "-" : "", v);
        return strdup(tmp);
    }

    size_t cap = 64;
    char *res = malloc(cap);
    if (!res) return NULL;
    size_t len = 0;

    if (sign) res[len++] = '-';

    for (size_t i = 0; i < sizeof(tbl)/sizeof(tbl[0]); ++i) {
        while (v >= tbl[i].val) {
            size_t sl = strlen(tbl[i].sym);
            if (len + sl + 1 >= cap) {
                cap *= 2;
                char *new_res = realloc(res, cap);
                if (!new_res) {
                    free(res);
                    return NULL;
                }
                res = new_res;
            }
            memcpy(res + len, tbl[i].sym, sl);
            len += sl;
            v -= tbl[i].val;
        }
    }
    res[len] = '\0';
    return res;
}

static char *unsigned_to_zeckendorf(unsigned int n) {
    if (n == 0) {
        char *result = malloc(3);
        if (!result) return NULL;
        result[0] = '0';
        result[1] = '1';
        result[2] = '\0';
        return result;
    }

    unsigned int fib[64];
    fib[0] = 1; fib[1] = 2;
    int m = 2;
    while (m < 64) {
        unsigned long next = (unsigned long)fib[m-1] + fib[m-2];
        if (next > UINT_MAX) break;
        fib[m++] = (unsigned int)next;
        if (fib[m-1] > n) break;
    }

    int idx = m - 1;
    while (idx >= 0 && fib[idx] > n) idx--;
    int maxidx = idx;

    char *bits = calloc(maxidx + 2, 1);
    if (!bits) return NULL;

    for (int i = maxidx; i >= 0; --i) {
        if (n >= fib[i]) {
            bits[i] = '1';
            n -= fib[i];
        } else bits[i] = '0';
    }

    char *out = malloc(maxidx + 3);
    if (!out) {
        free(bits);
        return NULL;
    }

    for (int i = 0; i <= maxidx; ++i) out[i] = bits[i];
    out[maxidx + 1] = '1';
    out[maxidx + 2] = '\0';
    free(bits);
    return out;
}

static char *int_to_base(int value, int base, int uppercase) {
    if (base < 2 || base > 36) base = 10;
    long v = value;
    int neg = v < 0;
    if (neg) v = -v;

    char buf[128];
    size_t pos = 0;

    do {
        int rem = (int)(v % base);
        buf[pos++] = (rem < 10) ? ('0' + rem)
                                : (uppercase ? 'A' + rem - 10 : 'a' + rem - 10);
        v /= base;
    } while (v && pos < sizeof(buf) - 1);

    if (neg) buf[pos++] = '-';

    char *res = malloc(pos + 1);
    if (!res) return NULL;

    for (size_t i = 0; i < pos; ++i) res[i] = buf[pos - 1 - i];
    res[pos] = '\0';
    return res;
}

static char *string_from_base_to_decimal(const char *s, int base) {
    if (!s) return strdup("0");
    if (base < 2 || base > 36) base = 10;
    
    char *endptr = NULL;
    long long val = strtoll(s, &endptr, base);
    
    if (endptr == s) {
        return strdup("0");
    }
    
    char buf[64];
    snprintf(buf, sizeof(buf), "%lld", val);
    return strdup(buf);
}

static char *memdump_binary(const void *ptr, size_t nbytes) {
    if (!ptr) {
        char *result = malloc(1);
        if (!result) return NULL;
        result[0] = '\0';
        return result;
    }
    
    const unsigned char *p = (const unsigned char *)ptr;
    size_t needed = nbytes * (8 + 1) + 1;
    char *res = malloc(needed);
    if (!res) return NULL;

    size_t pos = 0;
    for (size_t i = 0; i < nbytes; ++i) {
        for (int b = 7; b >= 0; --b)
            res[pos++] = ((p[i] >> b) & 1) ? '1' : '0';
        if (i + 1 < nbytes) res[pos++] = ' ';
    }
    res[pos] = '\0';
    return res;
}

static StatusCode over_vformat_to_dynbuf(dynbuf_t *out, const char *format, va_list ap_in) {
    if (!out || !format) return NULL_POINTER_ERROR;
    
    const char *p = format;
    va_list ap;
    va_copy(ap, ap_in);

    while (*p) {
        if (*p != '%') {
            const char *q = p;
            while (*q && *q != '%') ++q;
            size_t chunk = (size_t)(q - p);
            char *tmp = malloc(chunk + 1);
            if (!tmp) {
                va_end(ap);
                return MEMORY_ALLOCATION_ERROR;
            }
            memcpy(tmp, p, chunk);
            tmp[chunk] = '\0';
            StatusCode status = db_append(out, tmp);
            free(tmp);
            if (status != SUCCESS) {
                va_end(ap);
                return status;
            }
            p = q;
            continue;
        }

        ++p;
        if (*p == '%') {
            StatusCode status = db_append(out, "%");
            if (status != SUCCESS) {
                va_end(ap);
                return status;
            }
            ++p;
            continue;
        }

        if ((p[0] == 'R' && p[1] == 'o') ||
            (p[0] == 'Z' && p[1] == 'r') ||
            (p[0] == 'C' && (p[1] == 'v' || p[1] == 'V')) ||
            ((p[0] == 't' || p[0] == 'T') && (p[1] == 'o' || p[1] == 'O')) ||
            (p[0] == 'm' && (p[1] == 'i' || p[1] == 'u' || p[1] == 'd' || p[1] == 'f')))
        {
            char c1 = p[0], c2 = p[1];
            p += 2;

            if (c1 == 'R' && c2 == 'o') {
                int v = va_arg(ap, int);
                char *r = int_to_roman(v);
                if (!r) {
                    va_end(ap);
                    return MEMORY_ALLOCATION_ERROR;
                }
                StatusCode status = db_append(out, r);
                free(r);
                if (status != SUCCESS) {
                    va_end(ap);
                    return status;
                }
                continue;
            }

            if (c1 == 'Z' && c2 == 'r') {
                unsigned int v = va_arg(ap, unsigned int);
                char *r = unsigned_to_zeckendorf(v);
                if (!r) {
                    va_end(ap);
                    return MEMORY_ALLOCATION_ERROR;
                }
                StatusCode status = db_append(out, r);
                free(r);
                if (status != SUCCESS) {
                    va_end(ap);
                    return status;
                }
                continue;
            }

            if (c1 == 'C' && (c2 == 'v' || c2 == 'V')) {
                int value = va_arg(ap, int);
                int base = va_arg(ap, int);

                char *r = int_to_base(value, base, (c2 == 'V'));
                if (!r) {
                    va_end(ap);
                    return MEMORY_ALLOCATION_ERROR;
                }
                StatusCode status = db_append(out, r);
                free(r);
                if (status != SUCCESS) {
                    va_end(ap);
                    return status;
                }
                continue;
            }

            if ((c1 == 't' || c1 == 'T') && (c2 == 'o' || c2 == 'O')) {
                const char *s = va_arg(ap, const char *);
                int base = va_arg(ap, int);

                const char *p = s;
    
                while (*p != '\0') {
                    char c = *p;
                    int digit_value;
                    
                    if (c >= '0' && c <= '9') {
                        digit_value = c - '0';
                    } else if (c >= 'a' && c <= 'z') {
                        digit_value = c - 'a' + 10;
                    } else if (c >= 'A' && c <= 'Z') {
                        digit_value = c - 'A' + 10;
                    } else {
                        return INVALID_BASE;
                         
                    }
                    if (digit_value >= base) {
                        return INVALID_BASE;
                    }
                    
                    p++;
                }

                char *r = string_from_base_to_decimal(s, base);
                if (!r) {
                    va_end(ap);
                    return MEMORY_ALLOCATION_ERROR;
                }
                StatusCode status = db_append(out, r);
                free(r);
                if (status != SUCCESS) {
                    va_end(ap);
                    return status;
                }
                continue;
            }

            if (c1 == 'm' && (c2 == 'i' || c2 == 'u' || c2 == 'd' || c2 == 'f')) {
                char *r = NULL;
                if (c2 == 'i') {
                    int v = va_arg(ap, int);
                    r = memdump_binary(&v, sizeof(int));
                } else if (c2 == 'u') {
                    unsigned int v = va_arg(ap, unsigned int);
                    r = memdump_binary(&v, sizeof(unsigned int));
                } else if (c2 == 'd') {
                    double v = va_arg(ap, double);
                    r = memdump_binary(&v, sizeof(double));
                } else if (c2 == 'f') {
                    float v = (float)va_arg(ap, double);
                    r = memdump_binary(&v, sizeof(float));
                }
                
                if (!r) {
                    va_end(ap);
                    return MEMORY_ALLOCATION_ERROR;
                }
                StatusCode status = db_append(out, r);
                free(r);
                if (status != SUCCESS) {
                    va_end(ap);
                    return status;
                }
                continue;
            }
        }

        const char *q = p;
        const char *convchars = "diuoxXfFeEgGaAcspn%";
        while (*q && !strchr(convchars, *q)) ++q;
        if (*q == '\0') {
            StatusCode status = db_append(out, "%");
            if (status != SUCCESS) {
                va_end(ap);
                return status;
            }
            break;
        }

        size_t speclen = (size_t)(q - (p - 1) + 1);
        char *spec = malloc(speclen + 1);
        if (!spec) {
            va_end(ap);
            return MEMORY_ALLOCATION_ERROR;
        }
        memcpy(spec, p - 1, speclen);
        spec[speclen] = '\0';

        char tmpbuf[1024];
        char conv = *q;
        int wrote = 0;

        if (conv == 'd' || conv == 'i') {
            int val = va_arg(ap, int);
            wrote = snprintf(tmpbuf, sizeof(tmpbuf), spec, val);
        } else if (conv == 'u' || conv == 'o' || conv == 'x' || conv == 'X') {
            unsigned int val = va_arg(ap, unsigned int);
            wrote = snprintf(tmpbuf, sizeof(tmpbuf), spec, val);
        } else if (conv == 'c') {
            int val = va_arg(ap, int);
            wrote = snprintf(tmpbuf, sizeof(tmpbuf), spec, val);
        } else if (conv == 's') {
            const char *val = va_arg(ap, const char *);
            wrote = snprintf(tmpbuf, sizeof(tmpbuf), spec, val ? val : "(null)");
        } else if (conv == 'p') {
            void *val = va_arg(ap, void *);
            wrote = snprintf(tmpbuf, sizeof(tmpbuf), spec, val);
        } else if (strchr("fFeEgGaA", conv)) {
            double val = va_arg(ap, double);
            wrote = snprintf(tmpbuf, sizeof(tmpbuf), spec, val);
        }

        if (wrote > 0) {
            StatusCode status = db_append(out, tmpbuf);
            if (status != SUCCESS) {
                free(spec);
                va_end(ap);
                return status;
            }
        }
        free(spec);
        p = q + 1;
    }

    va_end(ap);
    return SUCCESS;
}

StatusCode my_overfprintf(FILE *stream, const char *format, ...) {
    if (!stream || !format) return NULL_POINTER_ERROR;
    
    dynbuf_t out;
    StatusCode status = db_init(&out);
    if (status != SUCCESS) return status;

    va_list ap;
    va_start(ap, format);
    status = over_vformat_to_dynbuf(&out, format, ap);
    va_end(ap);

    if (status != SUCCESS) {
        db_free(&out);
        return status;
    }

    fwrite(out.buf, 1, out.len, stream);
    db_free(&out);
    
    return ferror(stream) ? BUFFER_OVERFLOW : SUCCESS;
}

StatusCode my_oversprintf(char *outbuf, size_t outbuf_size, const char *format, ...) {
    if (!outbuf || !format) return NULL_POINTER_ERROR;
    
    dynbuf_t out;
    StatusCode status = db_init(&out);
    if (status != SUCCESS) return status;

    va_list ap;
    va_start(ap, format);
    status = over_vformat_to_dynbuf(&out, format, ap);
    va_end(ap);

    if (status != SUCCESS) {
        db_free(&out);
        return status;
    }

    if (out.len >= outbuf_size) {
        db_free(&out);
        return BUFFER_OVERFLOW;
    }
    
    strncpy(outbuf, out.buf, outbuf_size);
    outbuf[outbuf_size - 1] = '\0';
    db_free(&out);
    
    return SUCCESS;
}