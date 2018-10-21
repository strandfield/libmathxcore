
#ifndef LIBMATHXCORE_USUB_H
#define LIBMATHXCORE_USUB_H

#include "mathx/core/defs.h"

LIBMATHXCORE_API mx_size_t unbr_sub(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *result);

LIBMATHXCORE_API mx_limb_t unbr_limb_decr(mx_limb_t *a, mx_size_t as, mx_limb_t b);

#endif // LIBMATHXCORE_USUB_H
