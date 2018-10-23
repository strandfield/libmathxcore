
#ifndef LIBMATHXCORE_BEZOUT_H
#define LIBMATHXCORE_BEZOUT_H

#include "mathx/core/integer.h"

LIBMATHXCORE_API void int_gcd_bezout(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b, mx_int_t *u, mx_int_t *v);

#endif // LIBMATHXCORE_BEZOUT_H
