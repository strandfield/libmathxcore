
#include "mathx/core/bezout.h"

#include "mathx/core/euclide.h"

/*@
 * \fn void int_gcd_bezout(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b, mx_int_t *u, mx_int_t *v)
 * \brief Computes the gcd of two integers and the coefficients of Bézout's identity.
 * \param variable to receive the gcd
 * \param first integer
 * \param second integer
 * \param variable to receive the first coefficient
 * \param variable to receive the second coefficient
 *
 * Currently this function uses the extended euclidean algorithm.
 */
void int_gcd_bezout(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b, mx_int_t *u, mx_int_t *v)
{
  int_extended_euclidean_algorithm(gcd, a, b, u, v);
}
