#include "../include/utils.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

static int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

static void find_num_denom(double value, double epsilon, int *num, int *denom) {
    const int max_den = 10000;
    int a = 0, b = 1;
    int c = 1, d = 1;
    int best_num = 0, best_denom = 1;

    while (b < max_den && d < max_den) {
        int new_num = a + c;
        int new_denom = b + d;
        
        double mediant = (double)new_num / (double)new_denom;
        
        if (fabs(value - mediant) <= epsilon) {
            best_num = new_num;
            best_denom = new_denom;
            break;
        }
        
        if (value > mediant) {
            a = new_num;
            b = new_denom;
        } else {
            c = new_num;
            d = new_denom;
        }
    }
    
    *num = best_num;
    *denom = best_denom;
}

static int final_representation(unsigned int base, int num, int denom) {
    int g = gcd(num, denom);
    denom /= g;

    unsigned int b = base;
    for (int p = 2; p <= (int)sqrt(b); ++p) {
        while (b % p == 0) {
            while (denom % p == 0)
                denom /= p;
            b /= p;
        }
    }

    if (b > 1) {
        while (denom % b == 0)
            denom /= b;
    }

    return denom == 1;
}

StatusCode check_final_representation(unsigned int base, double epsilon, size_t count, int *results, ...) {
    if (base < 2 || base > 36) return INVALID_BASE;
    if (!results) return NULL_POINTER;

    va_list args;
    va_start(args, results);

    for (size_t i = 0; i < count; ++i) {
        double val = va_arg(args, double);
        int num, denom;
        if (val > 0.0 && val < 1.0) {
            find_num_denom(val, epsilon, &num, &denom);
            results[i] = final_representation(base, num, denom);
        } else {
            va_end(args);
            return INVALID_VALUE;
        }
    }

    va_end(args);
    return SUCCESS;
}
