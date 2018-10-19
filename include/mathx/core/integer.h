
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


void LIBMATHXCORE_API nbr_init(mx_int_t *x);
void LIBMATHXCORE_API nbr_copy_init(mx_int_t *x, const mx_int_t *src);
void LIBMATHXCORE_API nbr_limb_init(mx_int_t *x, const mx_limb_t value);
void LIBMATHXCORE_API nbr_raw_init(mx_int_t *x, mx_ssize_t size, mx_limb_t *limbs, mx_size_t alloc);

void LIBMATHXCORE_API nbr_clear(mx_int_t *x);

LIBMATHXCORE_API int nbr_is_normalized(const mx_int_t *x);
LIBMATHXCORE_API void nbr_normalize(mx_int_t *x);

LIBMATHXCORE_API void nbr_ensure_alloc(mx_int_t *x, mx_size_t s);
LIBMATHXCORE_API void nbr_ensure_alloc_zero(mx_int_t *x, mx_size_t s);

void LIBMATHXCORE_API nbr_assign(mx_int_t *dest, const mx_int_t *src);
LIBMATHXCORE_API void nbr_assign_zero(mx_int_t *dest);

void LIBMATHXCORE_API nbr_swap(mx_int_t *a, mx_int_t *b);

LIBMATHXCORE_API int nbr_sign(const mx_int_t *x);

#include "mathx/core/add.h"
#include "mathx/core/comp.h"
#include "mathx/core/mul.h"
#include "mathx/core/sub.h"

#endif // LIBMATHXCORE_INTEGER_H
