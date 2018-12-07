// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mathx/core/float/fsub.h"

#include "mathx/core/float/fadd.h"

#include "mathx/core/uadd.h"
#include "mathx/core/usub.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static mx_limb_t uint_vector_sub(mx_limb_t *r, const mx_limb_t *u, const mx_limb_t *v, mx_size_t s)
{
  mx_limb_t borrow = 0;
  mx_size_t i = 0;

  if (s == 0)
    return 0;

  for (i = 0; i < s; ++i)
  {
    r[i] = u[i] - v[i] - borrow;
    // There is a borrow if:
    //   (u[i] < r[i]) || (borrow && (u[i] <= r[i]))
    // This is equivalent to:
    borrow = (u[i] < r[i]) || (borrow & (u[i] == r[i]));
  }

  return borrow;
}

// Handles special cases of floating point subtraction.
// Returns 1 if a special case was handled and the result has been computed.
// Returns 0 if the general algorithm can be used on the (possibly modified) inputs.
static int float_sub_handle_special_case(mx_float_t *r, mx_float_t *u, mx_float_t *v, int sign)
{
#define GENERAL_CASE 0
#define SPECIAL_CASE_HANDLED 1

  const mx_limb_t maxdigit = ~((mx_limb_t)0);

  const mx_ssize_t hediff = (u->exp + u->size) - (v->exp + v->size);

  assert(hediff <= 1);

  // The most significant digits of 'u' and 'v' are close enough there could be cancellation. 
  // Some special cases can be treated here, otherwise we transform the input so that 
  // they can be used with the 'general-case' algorithm.

  if (hediff == 0)
  {
    // Skip the most significant limbs that are equal
    while (u->size > 0 && v->size > 0 && v->limbs[v->size - 1] == u->limbs[u->size - 1])
    {
      u->size -= 1;
      v->size -= 1;
    }

    // We know that 'u > v' so 'u' cannot be completely cancelled by 'v'
    assert(u->size > 0);

    if (v->size == 0)
    {
      // the high limbs of 'u' cancels all limbs of 'v', the result is 
      // therefore the low limbs of 'u'
      float_assign(r, u);
      r->size *= sign;
      return SPECIAL_CASE_HANDLED;
    }

    // Check for:
    // x+1 0000 ...
    // x   ffff ...
    if (u->limbs[u->size - 1] != v->limbs[v->size - 1] + 1)
      return GENERAL_CASE;

    assert(u->limbs[u->size - 1] == v->limbs[v->size - 1] + 1);
    u->size--;
    v->size--;
  }
  else
  {
    assert(hediff == 1);

    // Check for:
    // 1 0000 ...
    // 0 ffff ...
    if (u->limbs[u->size - 1] != 1 || v->limbs[v->size - 1] != maxdigit || (u->size >= 2 && u->limbs[u->size - 2] != 0))
      return GENERAL_CASE;

    u->size--;
  }

  // Skip sequences of 0000/ffff
  while (v->size != 0 && u->size != 0 && u->limbs[u->size - 1] == 0 && v->limbs[v->size - 1] == maxdigit)
  {
    u->size--;
    v->size--;
  }

  if (u->size == 0)
  {
    while (v->size != 0 && v->limbs[v->size - 1] == maxdigit)
      v->size--;
    u->exp = v->exp;
  }

  if (v->size == 0)
  {
    mx_size_t size = u->size;
    float_ensure_alloc(r, size + 1);
    memcpy(r->limbs, u->limbs, sizeof(mx_limb_t) * size);
    r->limbs[size] = 1;
    r->size = size + 1;
    r->size *= sign;
    r->exp = u->exp;
    return SPECIAL_CASE_HANDLED;
  }

  const mx_ssize_t prec = r->prec;

  if (u->size > prec - 1)
  {
    u->limbs += u->size - (prec - 1);
    u->size = prec - 1;
    u->exp += u->size - (prec - 1);
  }
  if (v->size > prec - 1)
  {
    v->limbs += v->size - (prec - 1);
    v->size = prec - 1;
    v->exp += v->size - (prec - 1);
  }

  mx_limb_t borrow;

  if (u->size == 0)
  {
    // 1 0000 
    // 0 ffff vv  

    float_ensure_alloc(r, v->size + 1);
    r->limbs[v->size] = 0;
    mx_size_t size = v->size;
    for (mx_size_t i = 0; i < size; ++i)
      r->limbs[i] = ~v->limbs[i];
    borrow = 1 - uint_limb_incr(r->limbs, v->size, 1);
    r->size = v->size;
    r->exp = v->exp;
  }
  else if (u->size >= v->size)
  {
    // 1 0000 uuuu
    // 0 ffff vv  
    float_ensure_alloc(r, u->size + 1);
    r->limbs[u->size] = 0;
    mx_size_t size = u->size - v->size;
    memcpy(r->limbs, u->limbs, sizeof(mx_limb_t) * size);
    borrow = uint_vector_sub(r->limbs + size, u->limbs + size, v->limbs, v->size);
    r->size = u->size;
    r->exp = u->exp;
  }
  else
  {
    // 1 0000 uuuu  
    // 0 ffff vvvvvvv
    float_ensure_alloc(r, v->size + 1);
    r->limbs[v->size] = 0;
    mx_size_t size = v->size - u->size;
    for (mx_size_t i = 0; i < size; ++i)
      r->limbs[i] = ~v->limbs[i];
    borrow = uint_vector_sub(r->limbs + size, u->limbs, v->limbs + size, u->size);
    borrow += uint_limb_decr(r->limbs + size, u->size, 1);
    borrow -= uint_limb_incr(r->limbs, v->size, 1);
    r->size = v->size;
    r->exp = v->exp;
  }

  if (borrow == 0)
  {
    r->limbs[r->size] = 1;
    r->size++;
  }
  else
  {
    while (r->size > 0 && r->limbs[r->size - 1] == 0)
      r->size--;
  }

  r->size *= sign;

  return SPECIAL_CASE_HANDLED;

#undef GENERAL_CASE
#undef SPECIAL_CASE_HANDLED
}

/*@
 * \fn void float_sub(mx_float_t *difference, const mx_float_t *minuend, const mx_float_t *subtrahend)
 * \brief Subtracts two floating point numbers.
 * \param variable that will receive the difference
 * \param minuend
 * \param subtrahend
 *
 */
void float_sub(mx_float_t *difference, const mx_float_t *minuend, const mx_float_t *subtrahend)
{
  if (subtrahend->size == 0)
  {
    float_assign(difference, minuend);
    return;
  }
  else if (minuend->size == 0)
  {
    float_neg(difference, minuend);
    return;
  }

  if (float_sign(minuend) != float_sign(subtrahend))
  {
    mx_float_t neg = *subtrahend;
    neg.size *= -1;
    float_add(difference, minuend, &neg);
    return;
  }

  assert(float_sign(minuend) == float_sign(subtrahend) && float_sign(minuend) != 0);

  int sign = float_sign(minuend);
  mx_float_t u = *minuend;
  mx_float_t v = *subtrahend;
  mx_float_t *r = difference;
  u.size = abs(u.size);
  v.size = abs(v.size);
  // Make u the largest input
  const int comp = float_comp(&u, &v);
  if (comp == 0)
  {
    float_assign_zero(r);
    return;
  }
  else if (comp < 0)
  {
    v = *minuend;
    u = *subtrahend;
    u.size = abs(u.size);
    v.size = abs(v.size);
    sign *= -1;
  }

  const mx_ssize_t prec = r->prec;

  // From now on, 'u' is strictly larger than 'v',
  // we are going to compute 'r=u-v' and change the sign if needed.

  const mx_ssize_t hediff = (u.exp + u.size) - (v.exp + v.size);
  assert(hediff >= 0);

  //  uuuuuuuuu
  //         vvvvvvv
  // ^______^         
  //  hediff

  if (hediff <= 1)
  {
    // The most significant digits of 'u' and 'v' are really close, 
    // there may be cancellation. 
    // We need to check for this, and treat such case differently.

    if (float_sub_handle_special_case(r, &u, &v, sign))
      return;
  }

  // General case

  // We are going to write at most 'prec' digits to r,
  // we may ignore some digits of 'u' and 'v' if they have too many.
  if (u.size > prec)
  {
    u.limbs += u.size - prec;
    u.exp += u.size - prec;
    u.size = prec;
  }
  if (v.size + hediff > prec)
  {
    //  uuuuuuuuu
    //         vvvvvvv
    //  rrrrrrrrrrr
    // ^______^    ^__^    
    //  hediff     v.size + hediff - prec

    v.limbs += v.size + hediff - prec;
    v.exp += v.size + hediff - prec;
    v.size = prec - hediff;

    /// TODO: Should we also remove least zeros of v ?
  }

  if (hediff >= prec)
  {
    // 'v' is completely ignored
    assert(v.size <= 0);
    float_assign(r, &u);
    r->size *= sign;
    return;
  }
  else
  {
    if (u.size > hediff)
    {
      // 'u' and 'v' partially overlap

      if (hediff == 0)
      {
        // the leading digits of 'u' and 'v' overlap

        if (u.size >= v.size)
        {
          // uuuu
          // vv  
          float_ensure_alloc(r, u.size);
          mx_size_t size = u.size - v.size;
          memcpy(r->limbs, u.limbs, size * sizeof(mx_limb_t));
          r->size = uint_sub(u.limbs + size, u.size - size, v.limbs, v.size, r->limbs + size);
          r->size += size;
          r->exp = u.exp;
        }
        else 
        {
          // uuuu
          // vvvvvvv
          float_ensure_alloc(r, v.size);
          mx_size_t size = v.size - u.size;
          r->limbs[0] = (((mx_limb_t)0) - v.limbs[0]);
          for (mx_size_t i = 1; i < size; ++i)
            r->limbs[i] = ~v.limbs[i];
          r->size = uint_sub(u.limbs, u.size, v.limbs + size, v.size - size, r->limbs + size);
          r->size += size;
          mx_limb_t borrow = uint_limb_decr(r->limbs + size, u.size, 1);
          assert(borrow == 0);
          r->exp = v.exp;
        }
      }
      else
      {
        if (v.size + hediff <= u.size)
        {
          // uuuu
          //   v
          float_ensure_alloc(r, u.size);
          mx_size_t size = u.size - hediff - v.size;
          memcpy(r->limbs, u.limbs, sizeof(mx_limb_t) * size);
          r->size = uint_sub(u.limbs + size, u.size - size, v.limbs, v.size, r->limbs + size);
          r->size += size;
          r->exp = u.exp;
        }
        else
        {
          // uuuu
          //   vvvvv
          float_ensure_alloc(r, v.size + hediff);
          mx_size_t size = v.size + hediff - u.size;
          r->limbs[0] = (((mx_limb_t)0) - v.limbs[0]);
          for (mx_size_t i = 1; i < size; ++i)
            r->limbs[i] = ~v.limbs[i];
          r->size = uint_sub(u.limbs, u.size, v.limbs + size, v.size - size, r->limbs + size);
          r->size += size;
          mx_limb_t borrow = uint_limb_decr(r->limbs + size, u.size, 1);
          assert(borrow == 0);
          r->exp = v.exp;
        }
      }
    }
    else
    {
      // uuuu
      //      vv
      float_ensure_alloc(r, v.size + hediff);
      mx_size_t size = v.size + hediff - u.size;
      r->limbs[0] = (((mx_limb_t)0) - v.limbs[0]);
      for (mx_ssize_t i = 1; i < v.size; ++i)
        r->limbs[i] = ~v.limbs[i];
      for (mx_size_t i = v.size; i < size; ++i)
        r->limbs[i] = ~((mx_limb_t)0);
      memcpy(r->limbs + size, u.limbs, u.size);
      mx_limb_t borrow = uint_limb_decr(r->limbs + size, u.size, 1);
      assert(borrow == 0);
      r->size = v.size + hediff;
      r->exp = v.exp;
    }

    // Normalizing the result
    while (r->size > 0 && r->limbs[r->size - 1] == 0)
      r->size -= 1;

    // Applying sign modifier
    r->size *= sign;
  }
}