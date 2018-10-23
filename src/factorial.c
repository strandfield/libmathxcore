
#include "mathx/core/factorial.h"

/*@
 * \fn void int_factorial(mx_int_t *fn, const mx_int_t *n)
 * \brief Computes the factorial of a number.
 * \param variable to receive the result
 * \param input
 *
 * If \c n is negative, this function assigns zero to \c fn.
 */
void int_factorial(mx_int_t *fn, const mx_int_t *n)
{
  mx_int_t i;
  mx_int_t temp;

  if (n->size < 0)
  {
    int_assign_zero(fn);
    return;
  }

  int_copy_init(&i, n);
  int_init(&temp);

  int_limb_assign(fn, 1);

  while (i.size != 0)
  {
    int_mul(&temp, fn, &i);

    int_swap(fn, &temp);

    int_limb_abs_decr(&i, 1);
  }

  int_clear(&temp);
  int_clear(&i);
}
