
#ifndef LIBMATHXCORE_FLOAT_PRINT_H
#define LIBMATHXCORE_FLOAT_PRINT_H

#include "mathx/core/float.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API mx_size_t float_digits_for_prec(mx_size_t prec);
LIBMATHXCORE_API mx_size_t float_print(char *out, mx_size_t s, const mx_float_t *x);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_FLOAT_PRINT_H
