// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mathx/core/umul.h"

#include <assert.h>


/*@
 * \fn mx_size_t uint_long_mul(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *result)
 * \brief Multiplies two unsigned integers using long-multiplication.
 *
 * This function is used as an implementation for \m uint_mul.
 */

#if defined(LIBMATHXCORE_HAS_LONGLIMB_TYPE)

mx_size_t uint_long_mul(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *result)
{
  mx_size_t i;
  mx_longlimb_t limb_mul;
  mx_longlimb_t multiplier;
  mx_limb_t* r_it;
  const mx_limb_t* b_it;
  const mx_limb_t* b_end = b + bs;

  if (as == 0 || bs == 0)
    return 0;

  for (i = 0; i < as; ++i)
  {
    limb_mul = 0;
    multiplier = a[i];
    r_it = result + i;
    b_it = b;

    while (b_it < b_end)
    {
      limb_mul += *b_it * multiplier + *r_it;
      *r_it = (mx_limb_t)limb_mul;
      limb_mul >>= sizeofbits(mx_limb_t);

      assert(limb_mul <= ((mx_longlimb_t) -1) >> sizeofbits(mx_limb_t));

      b_it++; r_it++;
    }

    if (limb_mul)
      *r_it += (mx_limb_t)limb_mul; // TODO: would '=' be enough here ?

    assert((limb_mul >> sizeofbits(mx_limb_t)) == 0);
  }

  i = as + bs;
  // TODO: how many limbs can be null ?
  while (i > 0 && result[i - 1] == 0) --i;
  return i;
}

#else
#error "long-multiplication algorithm not implemented when mx_longlimb_t is not available"
#endif // defined(LIBMATHXCORE_HAS_LONGLIMB_TYPE)


/*@
 * \fn mx_size_t uint_mul(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *result)
 * \brief Multiplies two unsigned integers.
 * \param pointer to the least-signficant limb of the first integer
 * \param size of the first integer
 * \param pointer to the least-significant limb of the second integer
 * \param size of the second integer
 * \param pointer to the least-significant limb of the result
 * \returns the size, in limbs, of the result.
 *
 * This function assumes that there is enough space to write the result in \c{result} and that
 * it is zero-initialized.
 */
mx_size_t uint_mul(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *result)
{
  return uint_long_mul(a, as, b, bs, result);
}
