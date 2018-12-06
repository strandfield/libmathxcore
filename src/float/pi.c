
#include "mathx/core/float/pi.h"

#include "mathx/core/float/fsqrt.h"

#include "mathx/core/integer.h"
#include "mathx/core/shift.h"

#include <assert.h>
#include <stdlib.h>

/*@
 * \fn void float_assign_pi(mx_float_t *x)
 * \param output variable
 * \brief Assigns pi to a floating point number
 *
 * Note that the precision of \c x is used to decide how 
 * many digits of pi should be computed.
 */
void float_assign_pi(mx_float_t *x)
{
  float_pi_gauss_legendre(x);
}


static void pi_gauss_legendre_b0(mx_float_t *r)
{
  mx_float_t two;
  float_init(&two);
  float_assign_limb(&two, 2);

  mx_float_t sqrt_two;
  float_init_prec(&sqrt_two, r->prec);

  float_sqrt(&sqrt_two, &two);

  float_clear(&two);

  float_inv(r, &sqrt_two);

  float_clear(&sqrt_two);
}

mx_limb_t limb_diff(mx_limb_t a, mx_limb_t b)
{
  mx_limb_t ab = a - b;
  mx_limb_t ba = b - a;
  return ab > ba ? ba : ab;
}

/*@
 * \fn void float_pi_gauss_legendre(mx_float_t *r)
 * \param output variable
 * \brief Computes pi using the Gauss-Legendre algorithm
 *
 * See en.wikipedia.org/wiki/Gauss-Legendre_algorithm
 */
void float_pi_gauss_legendre(mx_float_t *r)
{
  mx_float_t a;
  mx_float_t b;
  mx_float_t t;
  mx_float_t p;

  const mx_size_t prec = r->prec;

  // a_0 = 1
  float_init_prec(&a, prec);
  float_assign_limb(&a, 1);

  // b_0 = 1 / sqrt(2)
  float_init_prec(&b, prec);
  pi_gauss_legendre_b0(&b);

  // t_0 = 1/4
  float_init_prec(&t, prec);
  float_assign_limb(&t, 1 << (sizeofbits(mx_limb_t) - 2));
  t.exp = -1;

  // p_0 = 1
  float_init_prec(&p, prec);
  float_assign_limb(&p, 1);


  int iter = 0;
  int done = 0;

  while (iter < 15 && !done)
  {
    mx_float_t an;
    float_init_prec(&an, prec);

    // a_{n+1} <- (a+b)/2
    float_avg(&an, &a, &b);

    // temp <- ab
    mx_float_t temp;
    float_init_prec(&temp, prec);
    float_mul(&temp, &a, &b);
    
    // b_{n+1} <- sqrt(ab)
    mx_float_t bn;
    float_init_prec(&bn, prec);
    float_sqrt(&bn, &temp);

    // temp <- a_{n+1} - a_n
    float_sub(&temp, &an, &a);
    // temp2 <- temp^2
    mx_float_t temp2;
    float_init_prec(&temp2, prec);
    float_mul(&temp2, &temp, &temp);

    // ptemp2 <- p * temp2
    mx_float_t ptemp2;
    float_init_prec(&ptemp2, prec);
    float_mul(&ptemp2, &p, &temp2);

    // t_{n+1} <- t_n - p_n * (a_{n+1} - a_n)^2
    mx_float_t tn;
    float_init_prec(&tn, prec);
    float_sub(&tn, &t, &ptemp2);

    // p_{n+1} <- 2 * p_{n}
    float_lshift_assign(&p, 1);

    {
      an.limbs += 1;
      an.size -= 1;
      bn.limbs += 1;
      bn.size -= 1;

      const int comp = float_comp(&an, &bn);

      if (comp == 0)
      {
        if (limb_diff(a.limbs[0], b.limbs[0]) <= (1 << (sizeofbits(mx_limb_t) / 4)))
          done = 1;
      }

      an.limbs -= 1;
      an.size += 1;
      bn.limbs -= 1;
      bn.size += 1;
    }

    float_swap(&an, &a);
    float_swap(&bn, &b);
    float_swap(&tn, &t);
    
    float_clear(&an);
    float_clear(&bn);
    float_clear(&tn);
    float_clear(&temp);
    float_clear(&temp2);
    float_clear(&ptemp2);

    iter++;
  }


  // temp <- a+b
  mx_float_t temp;
  float_init_prec(&temp, prec);
  float_add(&temp, &a, &b);
  // temp2 <- temp * temp
  mx_float_t temp2;
  float_init_prec(&temp2, prec);
  float_mul(&temp2, &temp, &temp);
  // t <- 4*t
  float_lshift_assign(&t, 2);
  // pi <- temp2 / t
  float_div(r, &temp2, &t);

  float_clear(&a);
  float_clear(&b);
  float_clear(&t);
  float_clear(&p);
  float_clear(&temp);
  float_clear(&temp2);
}
