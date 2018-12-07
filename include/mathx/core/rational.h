// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_RATIONAL_H
#define LIBMATHXCORE_RATIONAL_H

#include "mathx/core/integer.h"

#ifdef __cplusplus
extern "C" {
#endif

struct LIBMATHXCORE_API mx_rational_struct
{
  mx_int_t num;
  mx_int_t den;
};

typedef struct mx_rational_struct mx_rat_t;


LIBMATHXCORE_API void rat_init(mx_rat_t *x);
LIBMATHXCORE_API void rat_copy_init(mx_rat_t *x, const mx_rat_t *src);
LIBMATHXCORE_API void rat_limb_init(mx_rat_t *x, const mx_limb_t value);
LIBMATHXCORE_API void rat_raw_init(mx_rat_t *x, const mx_int_t *num, const mx_int_t *den);
/// TODO:feature: LIBMATHXCORE_API void int_string_init(mx_int_t *x, const char *str);

LIBMATHXCORE_API void rat_clear(mx_rat_t *x);

LIBMATHXCORE_API int rat_is_normalized(const mx_rat_t *x);
LIBMATHXCORE_API void rat_normalize(mx_rat_t *x);

LIBMATHXCORE_API void rat_assign(mx_rat_t *dest, const mx_rat_t *src);
LIBMATHXCORE_API void rat_assign_zero(mx_rat_t *dest);
LIBMATHXCORE_API void rat_limb_assign(mx_rat_t *dest, const mx_limb_t limb);

LIBMATHXCORE_API void rat_swap(mx_rat_t *a, mx_rat_t *b);

LIBMATHXCORE_API int rat_sign(const mx_rat_t *x);
LIBMATHXCORE_API int rat_is_zero(const mx_rat_t *x);
LIBMATHXCORE_API void rat_negate(mx_rat_t *y, const mx_rat_t *x);
LIBMATHXCORE_API void rat_abs(mx_rat_t *y, const mx_rat_t *x);

LIBMATHXCORE_API int rat_is_integer(const mx_rat_t *x);

LIBMATHXCORE_API mx_size_t rat_print(char *out, mx_size_t s, const mx_rat_t *x);

LIBMATHXCORE_API int rat_comp(const mx_rat_t *a, const mx_rat_t *b);

LIBMATHXCORE_API void rat_add(mx_rat_t *result, const mx_rat_t *a, const mx_rat_t *b);
LIBMATHXCORE_API void rat_sub(mx_rat_t *result, const mx_rat_t *a, const mx_rat_t *b);
LIBMATHXCORE_API void rat_mul(mx_rat_t *result, const mx_rat_t *a, const mx_rat_t *b);
LIBMATHXCORE_API void rat_div(mx_rat_t *result, const mx_rat_t *a, const mx_rat_t *b);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_RATIONAL_H
