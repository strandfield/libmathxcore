// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_FLOAT_MUL_H
#define LIBMATHXCORE_FLOAT_MUL_H

#include "mathx/core/float.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API void float_mul(mx_float_t *product, const mx_float_t *factor1, const mx_float_t *factor2);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_FLOAT_MUL_H
