
#ifndef LIBMATHXCORE_UADD_H
#define LIBMATHXCORE_UADD_H

#include "mathx/core/defs.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API mx_size_t uint_add(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *result);

LIBMATHXCORE_API mx_limb_t uint_limb_incr(mx_limb_t *a, mx_size_t as, mx_limb_t b);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_UADD_H
