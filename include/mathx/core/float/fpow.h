
#ifndef LIBMATHXCORE_FLOAT_POW_H
#define LIBMATHXCORE_FLOAT_POW_H

#include "mathx/core/float.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API void float_pow_uint(mx_float_t *result, const mx_float_t *base, unsigned int exp);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_FLOAT_POW_H
