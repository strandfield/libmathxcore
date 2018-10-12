
#ifndef LIBMATHXCORE_UDIV_H
#define LIBMATHXCORE_UDIV_H

#include "mathx/core/defs.h"

LIBMATHXCORE_API void unbr_knuth_div(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *quo, mx_size_t *quos, mx_limb_t *rem, mx_size_t *rems);

#endif // LIBMATHXCORE_UDIV_H
