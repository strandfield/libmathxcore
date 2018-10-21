
#include "mathx/core/div.h"

#include "mathx/core/memory.h"
#include "mathx/core/udiv.h"

#include <stdlib.h>
#include <string.h>

/*@
 * \fn void nbr_div(mx_int_t *quotient, mx_int_t *remainder, const mx_int_t *dividend, const mx_int_t *divisor)
 * \brief Performs euclidean division.
 * \param variable that will receive the quotient
 * \param variable that will receive the remainder
 * \param dividend
 * \param divisor
 *
 * Note that the remainder is always non-negative, i.e. \c{0 <= remainder < abs(divisor)}.
 */
void nbr_div(mx_int_t *quotient, mx_int_t *remainder, const mx_int_t *dividend, const mx_int_t *divisor)
{
  if (divisor->size == 0)
  {
    /// TODO: find a way to raise an error
    return;
  }

  if (abs(divisor->size) == 1)
  {
    nbr_ensure_alloc(quotient, abs(dividend->size));

    mx_limb_t remval = unbr_limb_div(dividend->limbs, abs(dividend->size), divisor->limbs[0], quotient->limbs);

    quotient->size = abs(dividend->size);
    while (quotient->size > 0 && quotient->limbs[quotient->size - 1] == 0) quotient->size -= 1;

    if (remval == 0)
    {
      memset(remainder->limbs, 0, abs(remainder->size) * sizeof(mx_limb_t));
      remainder->size = 0;
    }
    else
    {
      nbr_ensure_alloc(remainder, 1);

      remainder->limbs[0] = remval;
      remainder->size = 1;
    }
  }
  else
  {
    nbr_ensure_alloc(quotient, 1 + abs(dividend->size) - abs(divisor->size));
    nbr_ensure_alloc(remainder, 1 + abs(dividend->size));
    remainder->limbs[remainder->size] = 0; /// TODO: check if needed, or maybe modify unbr_knuth_div

    unbr_knuth_div(dividend->limbs, abs(dividend->size), divisor->limbs, abs(divisor->size), quotient->limbs, &(quotient->size), remainder->limbs, &(remainder->size));
  }


  if (dividend->size < 0 || divisor->size < 0)
  {
    mx_int_t temp;
    nbr_init(&temp);

    mx_int_t absdivisor;
    absdivisor.limbs = divisor->limbs;
    absdivisor.size = abs(divisor->size);

    if (dividend->size < 0)
    {
      nbr_limb_abs_incr(quotient, 1);
      nbr_sub(&temp, &absdivisor, remainder);
      nbr_swap(remainder, &temp);

      quotient->size *= (divisor->size < 0 ? 1 : -1);
    }
    else // divisor->size < 0
    {
      quotient->size *= -1;
    }

    nbr_clear(&temp);
  }
}
