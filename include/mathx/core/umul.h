// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_UMUL_H
#define LIBMATHXCORE_UMUL_H

#include "mathx/core/defs.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API mx_size_t uint_long_mul(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *result);
LIBMATHXCORE_API mx_size_t uint_mul(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *result);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_UMUL_H
