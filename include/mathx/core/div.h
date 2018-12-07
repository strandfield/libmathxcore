// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_DIV_H
#define LIBMATHXCORE_DIV_H

#include "mathx/core/integer.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API void int_div(mx_int_t *quotient, mx_int_t *remainder, const mx_int_t *dividend, const mx_int_t *divisor);
LIBMATHXCORE_API void int_mod(mx_int_t *mod, const mx_int_t *dividend, const mx_int_t *divisor);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_DIV_H
