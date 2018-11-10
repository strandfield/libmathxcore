
#include "mathx/core/pow.h"

#include <assert.h>

/*@
 * \fn void int_square(mx_int_t *result, const mx_int_t *x)
 * \brief Computes the square of an integer
 * \param variable that will receive the result
 * \param input value
 *
 */
void int_square(mx_int_t *result, const mx_int_t *x)
{
  int_mul(result, x, x);
}

/*@
 * \fn void int_pow(mx_int_t *result, const mx_int_t *base, const mx_int_t *exp)
 * \brief Computes integer exponentiation
 * \param variable that will receive the result
 * \param base
 * \param exponent
 *
 * This function assumes that the exponent is non-negative.
 */
void int_pow(mx_int_t *result, const mx_int_t *x, const mx_int_t *y)
{
  assert(y->size >= 0);

  mx_int_t exp;
  mx_int_t factor;
  mx_int_t temp;

  int_copy_init(&exp, y);
  int_copy_init(&factor, x);
  int_init(&temp);

  int_limb_assign(result, 1);

  while (!int_is_zero(&exp))
  {
    if (int_is_odd(&exp))
    {
      int_mul(&temp, &factor, result);
      int_swap(result, &temp);
    }

    int_square(&temp, &factor);
    int_swap(&factor, &temp);

    int_rightshift_assign(&exp, 1);
  }

  int_clear(&exp);
  int_clear(&factor);
  int_clear(&temp);
}
