
#ifndef LIBMATHXCORE_ADD_H
#define LIBMATHXCORE_ADD_H

#include "mathx/core/integer.h"

LIBMATHXCORE_API void nbr_add(mx_int_t *sum, const mx_int_t *addend1, const mx_int_t *addend2);

LIBMATHXCORE_API void nbr_limb_abs_incr(mx_int_t *x, mx_limb_t y);

#endif // LIBMATHXCORE_ADD_H
