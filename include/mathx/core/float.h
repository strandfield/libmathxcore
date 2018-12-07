// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_FLOAT_H
#define LIBMATHXCORE_FLOAT_H

#include "mathx/core/defs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef mx_ssize_t mx_exp_t;

struct LIBMATHXCORE_API mx_float_struct
{
  mx_ssize_t size;
  mx_limb_t *limbs;
  mx_size_t alloc;
  mx_size_t prec; // in number of limbs
  mx_exp_t exp; // in base of mx_limb_t
};

typedef struct mx_float_struct mx_float_t;

LIBMATHXCORE_API mx_size_t float_default_prec();
LIBMATHXCORE_API mx_size_t float_set_default_prec(mx_size_t p);
LIBMATHXCORE_API mx_size_t float_prec_bits(mx_size_t bitcount);
LIBMATHXCORE_API mx_size_t float_prec_bytes(mx_size_t count);

LIBMATHXCORE_API void float_init(mx_float_t *x);
LIBMATHXCORE_API void float_init_prec(mx_float_t *x, mx_size_t p);
LIBMATHXCORE_API void float_raw_init(mx_float_t *x, mx_ssize_t size, mx_limb_t *limbs, mx_size_t alloc, mx_exp_t exp, mx_size_t prec);

LIBMATHXCORE_API void float_ensure_alloc(mx_float_t *x, mx_size_t s);
LIBMATHXCORE_API void float_ensure_alloc_zero(mx_float_t *x, mx_size_t s);

LIBMATHXCORE_API void float_set_prec(mx_float_t *x, mx_size_t p);

LIBMATHXCORE_API void float_copy(mx_float_t *dest, const mx_float_t *src);
LIBMATHXCORE_API void float_assign(mx_float_t *dest, const mx_float_t *src);
LIBMATHXCORE_API void float_assign_zero(mx_float_t *dest);
LIBMATHXCORE_API void float_assign_limb(mx_float_t *dest, mx_limb_t val);
LIBMATHXCORE_API void float_neg(mx_float_t *dest, const mx_float_t *src);

LIBMATHXCORE_API void float_avg(mx_float_t *result, const mx_float_t *a, const mx_float_t *b);

LIBMATHXCORE_API void float_lshift_assign(mx_float_t *x, mx_size_t n);

LIBMATHXCORE_API void float_swap(mx_float_t *a, mx_float_t *b);

LIBMATHXCORE_API void float_clear(mx_float_t *x);

LIBMATHXCORE_API int float_sign(const mx_float_t *x);

LIBMATHXCORE_API int float_comp(const mx_float_t *a, const mx_float_t *b);

LIBMATHXCORE_API mx_size_t float_raw_print(char *out, mx_size_t s, const mx_float_t *x);

#ifdef __cplusplus
} // extern "C"
#endif

#include "mathx/core/float/fadd.h"
#include "mathx/core/float/fsub.h"
#include "mathx/core/float/fmul.h"
#include "mathx/core/float/fdiv.h"
#include "mathx/core/float/fprint.h"

#endif // LIBMATHXCORE_FLOAT_H
