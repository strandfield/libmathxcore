
#ifndef LIBMATHXCORE_PRINT_H
#define LIBMATHXCORE_PRINT_H

#include "mathx/core/defs.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API mx_size_t uint_print(const mx_limb_t *a, const mx_size_t as, char *output);
LIBMATHXCORE_API mx_size_t uint_print_size(const mx_limb_t *a, const mx_size_t as);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_PRINT_H
