
#ifndef LIBMATHXCORE_SHIFT_H
#define LIBMATHXCORE_SHIFT_H

#include "mathx/core/defs.h"

LIBMATHXCORE_API mx_limb_t unbr_lshift(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest);

LIBMATHXCORE_API mx_limb_t unbr_rshift(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest);

#endif // LIBMATHXCORE_SHIFT_H
