
#include "mathx/core/mul.h"

#include "mathx/core/memory.h"
#include "mathx/core/umul.h"

#include <stdlib.h>
#include <string.h>

/*@
 * \fn void nbr_mul(mx_int_t *product, const mx_int_t *factor1, const mx_int_t *factor2)
 * \brief Multiplies two integers.
 * \param variable that will receive the product
 * \param first factor
 * \param second factor
 *
 * Informally, performs \c{product = factor1 * factor2}.
 */
void nbr_mul(mx_int_t *product, const mx_int_t *factor1, const mx_int_t *factor2)
{
  if (factor1->size == 0 || factor2->size == 0)
  {
    nbr_assign_zero(product);
  }
  else
  {
    mx_size_t result_size = abs(factor1->size) + abs(factor2->size);
    if (product->alloc < result_size)
    {
      mx_free(product->limbs);
      product->limbs = mx_malloc_zero(result_size, &(product->alloc));
    }
    else
    {
      memset(product->limbs, 0, abs(product->size) * sizeof(mx_limb_t));
    }
    product->size = unbr_mul(factor1->limbs, abs(factor1->size), factor2->limbs, abs(factor2->size), product->limbs);
    product->size *= nbr_sign(factor1) * nbr_sign(factor2);
  }
}
