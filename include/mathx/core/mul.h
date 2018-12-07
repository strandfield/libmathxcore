// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_MUL_H
#define LIBMATHXCORE_MUL_H

#include "mathx/core/integer.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API void int_mul(mx_int_t *product, const mx_int_t *factor1, const mx_int_t *factor2);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_MUL_H
