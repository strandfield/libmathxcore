
#include "mathx/core/euclide.h"

#include <stdlib.h>

/*@
 * \fn void nbr_euclidean_algorithm(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b)
 * \brief Computes the gcd of two integers using the euclidean algorithm
 * \param variable to receive the gcd
 * \param first integer
 * \param second integer
 *
 * This function computes the positive \c{gcd(a, b)}.
 */
void nbr_euclidean_algorithm(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b)
{
  mx_int_t dividend;
  mx_int_t divisor;
  mx_int_t remainder;

  nbr_copy_init(&dividend, a);
  dividend.size = abs(dividend.size);
  nbr_copy_init(&divisor, b);
  divisor.size = abs(divisor.size);
  nbr_init(&remainder);

  nbr_div(gcd, &remainder, &dividend, &divisor);

  while (remainder.size != 0)
  {
    nbr_swap(&dividend, &divisor);
    nbr_swap(&divisor, &remainder);

    nbr_div(gcd, &remainder, &dividend, &divisor);
  }

  nbr_assign(gcd, &divisor);

  nbr_clear(&divisor);
  nbr_clear(&dividend);
  nbr_clear(&remainder);
}
