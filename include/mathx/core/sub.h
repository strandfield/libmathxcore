
#ifndef LIBMATHXCORE_SUB_H
#define LIBMATHXCORE_SUB_H

#include "mathx/core/integer.h"

LIBMATHXCORE_API void int_sub(mx_int_t *difference, const mx_int_t *minuend, const mx_int_t *subtrahend);

LIBMATHXCORE_API void int_limb_abs_decr(mx_int_t *x, mx_limb_t y);

#endif // LIBMATHXCORE_SUB_H
