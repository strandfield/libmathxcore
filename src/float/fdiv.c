
#include "mathx/core/float/fdiv.h"

#include "mathx/core/integer.h"
#include "mathx/core/memory.h"
#include "mathx/core/udiv.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/*@
 * \fn void float_div(mx_float_t *result, const mx_float_t *a, const mx_float_t *b)
 * \brief Divides two floating-point numbers.
 * \param variable that will receive the result
 * \param dividend
 * \param divisor
 *
 */
void float_div(mx_float_t *result, const mx_float_t *a, const mx_float_t *b)
{
  assert(b->size != 0);

  if (a->size == 0)
    return float_assign_zero(result);

  mx_float_t u = *a;
  mx_float_t v = *b;
  u.size = abs(u.size);
  v.size = abs(v.size);
  mx_float_t *r = result;
  const int sign = float_sign(a) * float_sign(b);
  const mx_size_t prec = r->prec;

  const mx_ssize_t rsize = u.size - v.size + 1; 

  // We are going to pad 'u' with zeros to get the desired quotient size
  const mx_ssize_t zeros = prec + 1 - rsize;
  const int need_padding = (zeros > 0);
  
  if (need_padding)
  {
    // We are going to reallocate space for 'u' and pad it with zeros
    mx_limb_t *newlimbs = mx_malloc(u.size + zeros, &u.alloc);
    memset(newlimbs, 0, zeros * sizeof(mx_limb_t));
    memcpy(newlimbs + zeros, u.limbs, u.size * sizeof(mx_limb_t));
    u.limbs = newlimbs;
    u.size += zeros;
    u.exp -= zeros;
  }
  else
  {
    // Otherwise we may even omit some digits of 'u'
    const mx_ssize_t chop = max(-zeros, 0);
    u.limbs += chop;
    u.size -= chop;
    u.exp += chop;
  }

  assert(u.size - v.size + 1 == prec + 1);
  
  if (v.size == 1)
  {
    float_ensure_alloc(r, u.size);
    mx_limb_t rem = uint_limb_div(u.limbs, u.size, v.limbs[0], r->limbs);
    r->size = u.size;
    while (r->size > 0 && r->limbs[r->size - 1] == 0)
      r->size--;
  }
  else
  {
    mx_int_t rem;
    int_init(&rem);
    int_ensure_alloc(&rem, u.size + 1);

    float_ensure_alloc(r, u.size + 1 - v.size);

    uint_knuth_div(u.limbs, u.size, v.limbs, v.size, r->limbs, &r->size, rem.limbs, &rem.size);

    int_clear(&rem);
  }

  r->size *= sign;
  r->exp = u.exp - v.exp;
  r->prec = prec;

  if (need_padding)
  {
    float_clear(&u);
  }
}

/*@
 * \fn void float_inv(mx_float_t *result, const mx_float_t *x)
 * \brief Computes the inverse of a floating point number
 * \param variable that will receive the result
 * \param input variable
 *
 */
void float_inv(mx_float_t *result, const mx_float_t *x)
{
  const mx_size_t prec = result->prec;

  mx_float_t dividend;
  float_init(&dividend);

  const mx_size_t zeros = prec + 1 + abs(x->size);
  float_ensure_alloc_zero(&dividend, zeros + 1);
  dividend.limbs[zeros] = 1;
  dividend.exp = -zeros;
  dividend.size = zeros + 1;
  
  float_div(result, &dividend, x);

  float_clear(&dividend);
}