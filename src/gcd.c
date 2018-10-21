
#include "mathx/core/gcd.h"

#include "mathx/core/euclide.h"

/*@
 * \fn void nbr_gcd(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b)
 * \brief Computes the positive gcd of two integers.
 * \param variable to receive the gcd
 * \param first integer
 * \param second integer
 *
 * This function currently uses the euclidean algorithm to compute the gcd.
 */
void nbr_gcd(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b)
{
  nbr_euclidean_algorithm(gcd, a, b);
}
