
#ifndef LIBMATHXCORE_POW_H
#define LIBMATHXCORE_POW_H

#include "mathx/core/integer.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API void int_square(mx_int_t *result, const mx_int_t *x);
LIBMATHXCORE_API void int_pow(mx_int_t *result, const mx_int_t *base, const mx_int_t *exp);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_POW_H
