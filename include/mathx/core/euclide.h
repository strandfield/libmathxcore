// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_EUCLIDE_H
#define LIBMATHXCORE_EUCLIDE_H

#include "mathx/core/integer.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API void int_euclidean_algorithm(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b);
LIBMATHXCORE_API void int_extended_euclidean_algorithm(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b, mx_int_t *u, mx_int_t *v);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_EUCLIDE_H
