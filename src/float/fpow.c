
#include "mathx/core/float/fpow.h"

#include "mathx/core/float/fmul.h"

#include <assert.h>
#include <stdlib.h>

/*@
 * \fn void float_pow_uint(mx_float_t *result, const mx_float_t *base, unsigned int exp)
 * \param result variable
 * \param base
 * \param exponent
 * \brief Floating point exponentiation by an integer
 *
 */
void float_pow_uint(mx_float_t *result, const mx_float_t *base, unsigned int exp)
{
  unsigned int e = exp;
  mx_float_t b;
  float_init_prec(&b, result->prec);
  float_assign(&b, base);
  mx_float_t b2;
  float_init_prec(&b2, result->prec);
  mx_float_t temp;
  float_init_prec(&temp, result->prec);
  float_assign_limb(result, 1);

  if ((e & 1) != 0)
    float_assign(result, &b);

  for (e >>= 1; e != 0; e >>= 1)
  {
    float_mul(&b2, &b, &b);
    float_swap(&b2, &b);

    if ((e & 1) != 0)
    {
      float_mul(&temp, result, &b);
      float_swap(&temp, result);
    }
  }

  float_clear(&b);
  float_clear(&b2);
  float_clear(&temp);
}
