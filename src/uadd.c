
#include "mathx/core/uadd.h"

/*@
 * \fn mx_size_t unbr_add(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *result)
 * \brief Adds two unsigned integers.
 * \param pointer to the least-signficant limb of the first integer
 * \param size of the first integer
 * \param pointer to the least-significant limb of the second integer
 * \param size of the second integer
 * \param pointer to the least-significant limb of the result
 * \returns the size, in limbs, of the result.
 *
 * Writes the sum of \c a and \c b into \c result and returns the number of limbs written.
 * Note that it is the caller's responsability to ensure that enough space is available 
 * to write the result.
 */
mx_size_t unbr_add(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *result)
{
  mx_limb_t carry = 0;
  mx_size_t i = 0;

  if (as < bs)
    return unbr_add(b, bs, a, as, result);

  for (; i < bs; ++i)
  {
    result[i] = a[i] + b[i] + carry;
    // There is a carry if:
    //   (result[i] < a[i]) || (carry && (result[i] <= a[i]))
    // This is equivalent to:
    carry = (result[i] < a[i]) || (carry & (result[i] == a[i]));
  }

  for (; i < as; ++i)
  {
    result[i] = a[i] + carry;
    carry = (result[i] < a[i]);
  }

  if (carry)
  {
    result[i] = carry;
    ++i;
  }

  return i;
}
