
#include "mathx/core/isqrt.h"

#include <stdlib.h>


mx_size_t int_isqrt_highestbitindex(const mx_int_t *n)
{
  mx_size_t p = sizeofbits(mx_limb_t) - 1;
  mx_size_t i = (1 << p);
  const mx_limb_t high_limb = n->limbs[abs(n->size) - 1];

  while ((high_limb & i) == 0)
  {
    i >>= 1;
    --p;
  }

  return ((abs(n->size) - 1) *  sizeofbits(mx_limb_t)) + p;
}

/*@
 * \fn void int_isqrt(mx_int_t *result, const mx_int_t *n)
 * \brief Computes the integer square root of a positive integer
 * \param variable to receive the result
 * \param input integer
 *
 *
 */
void int_isqrt(mx_int_t *result, const mx_int_t *n)
{
  mx_int_t temp_sqrt;
  mx_int_t temp;
  mx_int_t remainder;
  int h;
  int half;
  int iter;

  if (n->size <= 0)
  {
    int_limb_assign(result, 0);
    return;
  }

  int_init(&temp_sqrt);
  int_init(&temp);
  int_init(&remainder);
  h = int_isqrt_highestbitindex(n);
  half = (h % 2 == 0 ? (h / 2) : ((h + 1) / 2));

  int_rightshift(&temp_sqrt, n, h - half);

  // Compute x1 = x0 - (n - x0^2) / (2*x0) = ((n/x0) + x0) / 2
  int_div(&temp, &remainder, n, &temp_sqrt);
  int_add(result, &temp, &temp_sqrt);
  int_rightshift_assign(result, 1);

  iter = 0;
  while (int_comp(result, &temp_sqrt) != 0 && iter < 15)
  {
    int_swap(&temp_sqrt, result);

    int_div(&temp, &remainder, n, &temp_sqrt);
    int_add(result, &temp, &temp_sqrt);
    int_rightshift_assign(result, 1);

    iter++;
  }

  int_clear(&remainder);
  int_clear(&temp_sqrt);
  int_clear(&temp);
}
