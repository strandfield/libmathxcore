
#include "mathx/core/shift.h"

#include <assert.h>

/*@
 * \fn mx_limb_t unbr_lshift(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest)
 * \brief Shifts an unsigned integer left.
 * \param pointer to the least-signficant limb of the integer
 * \param size of the integer
 * \param shift amount, must be less than sizeof(mx_limb_t)
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

  assert(0 <= shift_amount && shift_amount < sizeof(mx_limb_t));

  shift_complement = sizeof(mx_limb_t) - shift_amount;

  for (i = 0; i < as; ++i) 
  {
    ret = a[i] >> shift_complement;
    dest[i] = a[i] << shift_amount | acc;
    acc = ret;
  }

  return ret;
}

/*@
 * \fn mx_limb_t unbr_rshift(const mx_limb_t *a, mx_size_t as, mx_size_t shift_amount, mx_limb_t *dest)
 * \brief Shifts an unsigned integer right.
 * \param pointer to the least-signficant limb of the integer
 * \param size of the integer
 * \param shift amount, must be less than sizeof(mx_limb_t)
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

  assert(0 <= shift_amount && shift_amount < sizeof(mx_limb_t));

  shift_complement = sizeof(mx_limb_t) - shift_amount;

  for (i = as; i-- > 0;) 
  {
    ret = a[i] << shift_complement;
    dest[i] = a[i] >> shift_amount | acc;
    acc = ret;
  }

  return ret;
}