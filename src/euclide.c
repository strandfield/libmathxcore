
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

/*@
 * \fn void nbr_extended_euclidean_algorithm(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b, mx_int_t *u, mx_int_t *v)
 * \brief Computes the gcd of two integers and the coefficients of Bézout's identity using the euclidean extended algorithm.
 * \param variable to receive the gcd
 * \param first integer
 * \param second integer
 * \param variable to receive the first coefficient
 * \param variable to receive the second coefficient
 *
 */
void nbr_extended_euclidean_algorithm(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b, mx_int_t *u, mx_int_t *v)
{
  mx_int_t dividend;
  mx_int_t *divisor;
  mx_int_t remainder;
  mx_int_t *u_0;
  mx_int_t u_1;
  mx_int_t u_temp;
  mx_int_t *v_0;
  mx_int_t v_1;
  mx_int_t v_temp;
  mx_int_t product;
  mx_int_t quotient;

  nbr_copy_init(&dividend, a);
  divisor = gcd; nbr_assign(divisor, b);
  nbr_init(&remainder);
  // Assign u0 = 1, u1 = 0, v0 = 0, v1 = 1
  u_0 = u; nbr_limb_assign(u_0, 1);
  nbr_limb_init(&u_1, 0);
  v_0 = v; nbr_limb_assign(v_0, 0);
  nbr_limb_init(&v_1, 1);
  nbr_init(&u_temp);
  nbr_init(&v_temp);
  nbr_init(&product);
  nbr_init(&quotient);

  // Compute a = b*q + r with 0 <= r < abs(b)
  nbr_div(&quotient, &remainder, &dividend, divisor);

  // Compute (u0, u1) = (u1, u0 - u1*q)
  nbr_assign(&u_temp, &u_1);
  nbr_mul(&product, &quotient, &u_1);
  nbr_sub(&u_1, u_0, &product);
  nbr_swap(u_0, &u_temp);

  // Compute (v0, v1) = (v1, v0 - v1*q)
  nbr_assign(&v_temp, &v_1);
  nbr_mul(&product, &quotient, &v_1);
  nbr_sub(&v_1, v_0, &product);
  nbr_swap(v_0, &v_temp);


  while (remainder.size != 0)
  {
    // Compute (a, b) = (b, r)
    nbr_swap(&dividend, divisor);
    nbr_swap(divisor, &remainder);

    // Compute a = b*q + r with 0 <= r < abs(b)
    nbr_div(&quotient, &remainder, &dividend, divisor);

    // Compute (u0, u1) = (u1, u0 - u1*q)
    nbr_assign(&u_temp, &u_1);
    nbr_mul(&product, &quotient, &u_1);
    nbr_sub(&u_1, u_0, &product);
    nbr_swap(u_0, &u_temp);

    // Compute (v0, v1) = (v1, v0 - v1*q)
    nbr_assign(&v_temp, &v_1);
    nbr_mul(&product, &quotient, &v_1);
    nbr_sub(&v_1, v_0, &product);
    nbr_swap(v_0, &v_temp);
  }

  if (gcd->size < 0)
  {
    u->size *= -1;
    v->size *= -1;
    gcd->size *= -1;
  }

  nbr_clear(&dividend);
  nbr_clear(&remainder);
  nbr_clear(&u_1);
  nbr_clear(&v_1);
  nbr_clear(&u_temp);
  nbr_clear(&v_temp);
  nbr_clear(&product);
  nbr_clear(&quotient);
}
