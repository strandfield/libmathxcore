
#include "mathx/core/shift.h"

#include <assert.h>
#include <stdlib.h> // for abs

/*@
 * \fn mx_limb_t unbr_lshift(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest)
 * \brief Shifts an unsigned integer left.
 * \param pointer to the least-signficant limb of the integer
 * \param size of the integer
 * \param shift amount, must be less than sizeof(mx_limb_t)*8
 * \param pointer to the least-significant limb of the result
 * \returns the shifted out most significant bits
 *
 * Note that the pointers \c a and \c dest can be equal, but otherwise the memory blocks 
 * should not overlap.
 */
mx_limb_t unbr_lshift(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest)
{
  mx_size_t i;
  mx_size_t shift_complement;
  mx_limb_t acc = 0;
  mx_limb_t ret = 0;

  assert(0 <= shift_amount && shift_amount < sizeofbits(mx_limb_t));

  shift_complement = sizeofbits(mx_limb_t) - shift_amount;

  for (i = 0; i < as; ++i) 
  {
    ret = a[i] >> shift_complement;
    dest[i] = a[i] << shift_amount | acc;
    acc = ret;
  }

  return ret;
}

/*@
 * \fn mx_limb_t unbr_lshift_overlap(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest)
 * \brief Shifts an unsigned integer left to a possibly overlapping destination.
 *
 * This function provides the same functionnality as \m unbr_lshift, except that the input and
 * and output limbs are allowed to overlap here.
 * Note that this function is slightly slower than its counterpart due to additional checks
 * that are being made.
 */
mx_limb_t unbr_lshift_overlap(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest)
{
  mx_size_t i;
  mx_size_t shift_complement;
  mx_limb_t ret = 0;
  const mx_ssize_t dist = dest - a;

  if (((mx_size_t) abs(dist)) >= as || dist <= 0 || as == 0)
  {
    return unbr_lshift(a, as, shift_amount, dest);
  }

  assert(0 <= shift_amount && shift_amount < sizeofbits(mx_limb_t));

  shift_complement = sizeofbits(mx_limb_t) - shift_amount;

  ret = a[as - 1] >> shift_complement;

  for (i = as; --i > 0;)
  {
    dest[i] = (a[i] << shift_amount) | (a[i-1] >> shift_complement);
  }

  dest[0] = a[0] << shift_amount;

  return ret;
}

/*@
 * \fn mx_limb_t unbr_rshift(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest)
 * \brief Shifts an unsigned integer right.
 * \param pointer to the least-signficant limb of the integer
 * \param size of the integer
 * \param shift amount, must be less than sizeof(mx_limb_t)*8
 * \param pointer to the least-significant limb of the result
 * \returns the shifted out least significant bits
 *
 * The return value contains the bits of \c a that have been shifted out, 
 * as if there was an additional limb on the right (i.e. the most significant bits are set).
 *
 * Note that the pointers \c a and \c dest can be equal, but otherwise the memory blocks
 * should not overlap.
 */
mx_limb_t unbr_rshift(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest)
{
  mx_size_t i;
  mx_size_t shift_complement;
  mx_limb_t acc = 0;
  mx_limb_t ret = 0;

  assert(0 <= shift_amount && shift_amount < sizeofbits(mx_limb_t));

  shift_complement = sizeofbits(mx_limb_t) - shift_amount;

  for (i = as; i-- > 0;) 
  {
    ret = a[i] << shift_complement;
    dest[i] = a[i] >> shift_amount | acc;
    acc = ret;
  }

  return ret;
}

/*@
 * \fn mx_limb_t unbr_rshift_overlap(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest)
 * \brief Shifts an unsigned integer right to a possibly overlapping destination.
 *
 * This function provides the same functionnality as \m unbr_rshift, except that the input and
 * and output limbs are allowed to overlap here.
 * Note that this function is slightly slower than its counterpart due to additional checks
 * that are being made.
 */
mx_limb_t unbr_rshift_overlap(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest)
{
  mx_size_t i;
  mx_size_t shift_complement;
  mx_limb_t ret;
  const mx_ssize_t dist = dest - a;

  if (((mx_size_t)abs(dist)) >= as || dist >= 0 || as == 0)
  {
    return unbr_rshift(a, as, shift_amount, dest);
  }

  assert(0 <= shift_amount && shift_amount < sizeofbits(mx_limb_t));

  shift_complement = sizeofbits(mx_limb_t) - shift_amount;

  ret = a[0] << shift_complement;

  for (i = 0; i < as - 1; ++i)
  {
    dest[i] = (a[i] >> shift_amount) | (a[i + 1] << shift_complement);
  }

  dest[as-1] = a[as-1] >> shift_amount;

  return ret;
}