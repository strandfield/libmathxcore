
#ifndef LIBMATHXCORE_SHIFT_H
#define LIBMATHXCORE_SHIFT_H

#include "mathx/core/defs.h"

LIBMATHXCORE_API mx_limb_t uint_lshift(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest);
LIBMATHXCORE_API mx_limb_t uint_lshift_overlap(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest);

LIBMATHXCORE_API mx_limb_t uint_rshift(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest);
LIBMATHXCORE_API mx_limb_t uint_rshift_overlap(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest);

#endif // LIBMATHXCORE_SHIFT_H
