// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_ADD_H
#define LIBMATHXCORE_ADD_H

#include "mathx/core/integer.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API void int_add(mx_int_t *sum, const mx_int_t *addend1, const mx_int_t *addend2);

LIBMATHXCORE_API void int_limb_abs_incr(mx_int_t *x, mx_limb_t y);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_ADD_H
