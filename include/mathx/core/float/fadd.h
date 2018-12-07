// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_FLOAT_ADD_H
#define LIBMATHXCORE_FLOAT_ADD_H

#include "mathx/core/float.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API void float_add(mx_float_t *sum, const mx_float_t *addend1, const mx_float_t *addend2);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_FLOAT_ADD_H
