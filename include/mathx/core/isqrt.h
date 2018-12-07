// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_ISQRT_H
#define LIBMATHXCORE_ISQRT_H

#include "mathx/core/integer.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API void int_isqrt(mx_int_t *result, const mx_int_t *n);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_ISQRT_H
