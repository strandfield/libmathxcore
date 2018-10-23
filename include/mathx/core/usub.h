
#ifndef LIBMATHXCORE_USUB_H
#define LIBMATHXCORE_USUB_H

#include "mathx/core/defs.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API mx_size_t uint_sub(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *result);

LIBMATHXCORE_API mx_limb_t uint_limb_decr(mx_limb_t *a, mx_size_t as, mx_limb_t b);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_USUB_H
