// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mathx/core/float/fsqrt.h"

#include "mathx/core/isqrt.h"
#include "mathx/core/memory.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/*@
 * \fn void float_sqrt(mx_float_t *result, const mx_float_t *x)
 * \param result variable
 * \param input variable
 * \brief Computes the square root of a floating point number
 *
 */
void float_sqrt(mx_float_t *result, const mx_float_t *x)
{
  if (x->size == 0)
  {
    float_assign_zero(result);
    return;
  }

  assert(x->size > 0);
  const mx_size_t prec = result->prec;

  mx_float_t u = *x;

  const int oddexp = ((u.exp + u.size) & 1);

  mx_size_t tsize = 2 * prec - oddexp;
  const int need_padding = u.size < tsize;

  if (need_padding)
  {
    mx_limb_t *newlimbs = mx_malloc(tsize, &u.alloc);
    memset(newlimbs, 0, (tsize - u.size) * sizeof(mx_limb_t));
    memcpy(newlimbs + (tsize - u.size), u.limbs, u.size * sizeof(mx_limb_t));
    u.limbs = newlimbs;
    u.exp -= (tsize - u.size);
    u.size = tsize;
  }
  else
  {
    assert(u.size >= tsize);
    u.limbs += u.size - tsize;
    u.exp += u.size - tsize;
    u.size = tsize;
  }

  {
    mx_int_t integer;
    integer.alloc = u.alloc;
    integer.size = u.size;
    integer.limbs = u.limbs;

    mx_int_t root;
    int_init(&root);

    int_isqrt(&root, &integer);

    result->limbs = root.limbs;
    result->size = root.size;
    result->alloc = root.alloc;
  }

  assert((u.exp & 1) == 0);
  result->exp = u.exp / 2;

  if (need_padding)
  {
    float_clear(&u);
  }
}
