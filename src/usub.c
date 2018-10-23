
#include "mathx/core/usub.h"

/*@
 * \fn mx_size_t uint_sub(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *result)
 * \brief Subtracts two unsigned integers.
 * \param pointer to the least-signficant limb of the first integer
 * \param size of the first integer
 * \param pointer to the least-significant limb of the second integer
 * \param size of the second integer
 * \param pointer to the least-significant limb of the result
 * \returns the size, in limbs, of the result.
 *
 * This function assumes that \c{a} is greater than \c{b}, and that there is enough space to write 
 * the result in \c{result}.
 */
mx_size_t uint_sub(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *result)
{
  mx_limb_t borrow = 0;
  mx_size_t i = 0;

  if (as == bs)
  {
    if (as == 0)
      return 0;

    // The most-significant limbs of `a` and `b` may cancel each other, 
    // and since we know that `a >= b`, we can conclude that no borrow 
    // will propagate to these limbs.
    // Therefore, we can check for them earlier to skip them later
    i = as - 1;

    while (i > 0 && a[i] == b[i])
      --i;

    if (i == 0)
    {
      if (a[0] == b[0])
      {
        return 0;
      }
      else
      {
        result[0] = a[0] - b[0];
        return 1;
      }
    }

    as = bs = i + 1;
  }

  for (i = 0; i < bs; ++i)
  {
    result[i] = a[i] - b[i] - borrow;
    // There is a borrow if:
    //   (a[i] < result[i]) || (borrow && (a[i] <= result[i]))
    // This is equivalent to:
    borrow = (a[i] < result[i]) || (borrow & (a[i] == result[i]));
  }

  for (; i < as; ++i)
  {
    result[i] = a[i] - borrow;
    borrow = (a[i] < result[i]);
  }

  // assert(borrow == 0);
  if (result[as - 1] == 0)
    return as - 1;
  return as;
}

/*@
 * \fn mx_limb_t uint_limb_decr(mx_limb_t *a, mx_size_t as, mx_limb_t b)
 * \brief Decrements an unsigned integer by a limb and returns the final borrow.
 * \param pointer to the least-signficant limb of the integer
 * \param limb to be subtracted
 * \returns the final borrow
 *
 */
mx_limb_t uint_limb_decr(mx_limb_t *a, mx_size_t as, mx_limb_t b)
{
  mx_limb_t borrow;
  mx_size_t i;

  if (as == 0 || b == 0)
  {
    return b;
  }

  borrow = a[0] < (a[0] - b);
  a[0] = a[0] - b;

  for (i = 1; borrow && i < as; ++i)
  {
    borrow = (a[i] == 0);
    a[i] = a[i] - 1;
  }

  return borrow;
}
