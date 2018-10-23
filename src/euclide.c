
#include "mathx/core/euclide.h"

#include <stdlib.h>

/*@
 * \fn void int_euclidean_algorithm(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b)
 * \brief Computes the gcd of two integers using the euclidean algorithm
 * \param variable to receive the gcd
 * \param first integer
 * \param second integer
 *
 * This function computes the positive \c{gcd(a, b)}.
 */
void int_euclidean_algorithm(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b)
{
  mx_int_t dividend;
  mx_int_t divisor;
  mx_int_t remainder;

  int_copy_init(&dividend, a);
  dividend.size = abs(dividend.size);
  int_copy_init(&divisor, b);
  divisor.size = abs(divisor.size);
  int_init(&remainder);

  int_div(gcd, &remainder, &dividend, &divisor);

  while (remainder.size != 0)
  {
    int_swap(&dividend, &divisor);
    int_swap(&divisor, &remainder);

    int_div(gcd, &remainder, &dividend, &divisor);
  }

  int_assign(gcd, &divisor);

  int_clear(&divisor);
  int_clear(&dividend);
  int_clear(&remainder);
}

/*@
 * \fn void int_extended_euclidean_algorithm(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b, mx_int_t *u, mx_int_t *v)
 * \brief Computes the gcd of two integers and the coefficients of Bézout's identity using the euclidean extended algorithm.
 * \param variable to receive the gcd
 * \param first integer
 * \param second integer
 * \param variable to receive the first coefficient
 * \param variable to receive the second coefficient
 *
 */
void int_extended_euclidean_algorithm(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b, mx_int_t *u, mx_int_t *v)
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

  int_copy_init(&dividend, a);
  divisor = gcd; int_assign(divisor, b);
  int_init(&remainder);
  // Assign u0 = 1, u1 = 0, v0 = 0, v1 = 1
  u_0 = u; int_limb_assign(u_0, 1);
  int_limb_init(&u_1, 0);
  v_0 = v; int_limb_assign(v_0, 0);
  int_limb_init(&v_1, 1);
  int_init(&u_temp);
  int_init(&v_temp);
  int_init(&product);
  int_init(&quotient);

  // Compute a = b*q + r with 0 <= r < abs(b)
  int_div(&quotient, &remainder, &dividend, divisor);

  // Compute (u0, u1) = (u1, u0 - u1*q)
  int_assign(&u_temp, &u_1);
  int_mul(&product, &quotient, &u_1);
  int_sub(&u_1, u_0, &product);
  int_swap(u_0, &u_temp);

  // Compute (v0, v1) = (v1, v0 - v1*q)
  int_assign(&v_temp, &v_1);
  int_mul(&product, &quotient, &v_1);
  int_sub(&v_1, v_0, &product);
  int_swap(v_0, &v_temp);


  while (remainder.size != 0)
  {
    // Compute (a, b) = (b, r)
    int_swap(&dividend, divisor);
    int_swap(divisor, &remainder);

    // Compute a = b*q + r with 0 <= r < abs(b)
    int_div(&quotient, &remainder, &dividend, divisor);

    // Compute (u0, u1) = (u1, u0 - u1*q)
    int_assign(&u_temp, &u_1);
    int_mul(&product, &quotient, &u_1);
    int_sub(&u_1, u_0, &product);
    int_swap(u_0, &u_temp);

    // Compute (v0, v1) = (v1, v0 - v1*q)
    int_assign(&v_temp, &v_1);
    int_mul(&product, &quotient, &v_1);
    int_sub(&v_1, v_0, &product);
    int_swap(v_0, &v_temp);
  }

  if (gcd->size < 0)
  {
    u->size *= -1;
    v->size *= -1;
    gcd->size *= -1;
  }

  int_clear(&dividend);
  int_clear(&remainder);
  int_clear(&u_1);
  int_clear(&v_1);
  int_clear(&u_temp);
  int_clear(&v_temp);
  int_clear(&product);
  int_clear(&quotient);
}
