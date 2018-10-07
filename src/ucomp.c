
#include "mathx/core/ucomp.h"

/*@
 * \fn mx_ssize_t unbr_comp(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs)
 * \brief Compares two unsigned integers.
 * \param pointer to the least-signficant limb of the first integer
 * \param size of the first integer
 * \param pointer to the least-significant limb of the second integer
 * \param size of the second integer
 * \returns zero, minus one or plus one
 *
 * Returns the sign of the difference of the two unsigned integers (informaly \c{a-b}),
 * with the convention that the sign of zero is zero.
 */
mx_ssize_t unbr_comp(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs)
{
  mx_ssize_t comp = (as < bs) ? -1 : (as > bs);
  if (comp != 0 || (as == 0 && bs == 0))
    return comp;

  mx_size_t i = as - 1;
  for (; i >= 0; --i)
  {
    comp = (a[i] < b[i]) ? -1 : (a[i] > b[i]);
    if (comp != 0)
      return comp;
  }

  return 0;
}
