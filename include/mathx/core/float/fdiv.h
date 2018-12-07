// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_FLOAT_DIV_H
#define LIBMATHXCORE_FLOAT_DIV_H

#include "mathx/core/float.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API void float_div(mx_float_t *result, const mx_float_t *a, const mx_float_t *b);
LIBMATHXCORE_API void float_inv(mx_float_t *result, const mx_float_t *x);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_FLOAT_DIV_H
