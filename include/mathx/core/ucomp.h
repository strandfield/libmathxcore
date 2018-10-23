
#ifndef LIBMATHXCORE_UCOMP_H
#define LIBMATHXCORE_UCOMP_H

#include "mathx/core/defs.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API mx_ssize_t uint_comp(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_UCOMP_H
