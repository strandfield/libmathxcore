// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_FLOAT_SUB_H
#define LIBMATHXCORE_FLOAT_SUB_H

#include "mathx/core/float.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API void float_sub(mx_float_t *difference, const mx_float_t *minuend, const mx_float_t *subtrahend);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_FLOAT_SUB_H
