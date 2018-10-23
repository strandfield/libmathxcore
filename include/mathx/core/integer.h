
#ifndef LIBMATHXCORE_INTEGER_H
#define LIBMATHXCORE_INTEGER_H

#include "mathx/core/defs.h"

struct LIBMATHXCORE_API mx_integer_struct
{
  mx_ssize_t size;
  mx_limb_t *limbs;
  mx_size_t alloc;
};

typedef struct mx_integer_struct mx_int_t;


void LIBMATHXCORE_API int_init(mx_int_t *x);
void LIBMATHXCORE_API int_copy_init(mx_int_t *x, const mx_int_t *src);
void LIBMATHXCORE_API int_limb_init(mx_int_t *x, const mx_limb_t value);
void LIBMATHXCORE_API int_raw_init(mx_int_t *x, mx_ssize_t size, mx_limb_t *limbs, mx_size_t alloc);
LIBMATHXCORE_API void int_string_init(mx_int_t *x, const char *str);

void LIBMATHXCORE_API int_clear(mx_int_t *x);

LIBMATHXCORE_API int int_is_normalized(const mx_int_t *x);
LIBMATHXCORE_API void int_normalize(mx_int_t *x);

LIBMATHXCORE_API void int_ensure_alloc(mx_int_t *x, mx_size_t s);
LIBMATHXCORE_API void int_ensure_alloc_zero(mx_int_t *x, mx_size_t s);

void LIBMATHXCORE_API int_assign(mx_int_t *dest, const mx_int_t *src);
LIBMATHXCORE_API void int_assign_zero(mx_int_t *dest);
LIBMATHXCORE_API void int_limb_assign(mx_int_t *dest, const mx_limb_t limb);

void LIBMATHXCORE_API int_swap(mx_int_t *a, mx_int_t *b);

LIBMATHXCORE_API int int_sign(const mx_int_t *x);
LIBMATHXCORE_API void int_negate(mx_int_t *y, const mx_int_t *x);
LIBMATHXCORE_API void int_abs(mx_int_t *y, const mx_int_t *x);

LIBMATHXCORE_API void int_rightshift(mx_int_t *result, const mx_int_t *x, mx_size_t n);
LIBMATHXCORE_API void int_rightshift_assign(mx_int_t *x, mx_size_t n);

LIBMATHXCORE_API mx_size_t int_print(char *out, mx_size_t s, const mx_int_t *x);

#include "mathx/core/add.h"
#include "mathx/core/comp.h"
#include "mathx/core/div.h"
#include "mathx/core/mul.h"
#include "mathx/core/sub.h"

#endif // LIBMATHXCORE_INTEGER_H
