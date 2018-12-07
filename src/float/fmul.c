// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mathx/core/float/fmul.h"

#include "mathx/core/umul.h"

#include <assert.h>
#include <stdlib.h>

/*@
 * \fn void float_mul(mx_float_t *product, const mx_float_t *factor1, const mx_float_t *factor2)
 * \brief Multiplies two floating-point numbers.
 * \param variable that will receive the result
 * \param first factor
 * \param second factor
 *
 */
void float_mul(mx_float_t *product, const mx_float_t *factor1, const mx_float_t *factor2)
{
  if (factor1->size == 0 || factor2->size == 0)
    return float_assign_zero(product);

  mx_float_t u = *factor1;
  mx_float_t v = *factor2;
  u.size = abs(u.size);
  v.size = abs(v.size);
  mx_float_t *r = product;
  const mx_ssize_t prec = r->prec;
  const int sign = float_sign(&u) * float_sign(&v);

  if (u.size > prec)
  {
    u.limbs += u.size - prec;
    u.exp += u.size - prec;
    u.size = prec;
  }
  if (v.size > prec)
  {
    v.limbs += v.size - prec;
    v.exp += v.size - prec;
    v.size = prec;
  }

  float_ensure_alloc_zero(r, u.size + v.size);

  r->size = uint_mul(u.limbs, u.size, v.limbs, v.size, r->limbs);
  r->exp = v.exp + u.exp;

  if (r->size > prec)
    float_set_prec(r, (mx_size_t) prec);

  r->size *= sign;
}
