
#include "mathx/core/add.h"

#include "mathx/core/memory.h"
#include "mathx/core/uadd.h"
#include "mathx/core/ucomp.h"
#include "mathx/core/usub.h"

#include <stdlib.h>
#include <string.h>

/*@
 * \fn void nbr_add(mx_int_t *sum, const mx_int_t *addend1, const mx_int_t *addend2)
 * \brief Adds two integers.
 * \param variable that will receive the sum
 * \param first addend
 * \param second addend
 *
 */
void nbr_add(mx_int_t *sum, const mx_int_t *addend1, const mx_int_t *addend2)
{
  if (addend1->size == 0)
  {
    nbr_assign(sum, addend2);
  }
  else if (addend2->size == 0)
  {
    nbr_assign(sum, addend1);
  }
  else
  {
    if (nbr_sign(addend1) == nbr_sign(addend2))
    {
      mx_size_t result_size = abs(addend1->size) + abs(addend2->size) + 1;
      if (sum->alloc < result_size)
      {
        mx_free(sum->limbs);
        sum->limbs = mx_malloc(result_size, &(sum->alloc));
      }
      sum->size = unbr_add(addend1->limbs, abs(addend1->size), addend2->limbs, abs(addend2->size), sum->limbs);
      memset(sum->limbs + sum->size, 0, (sum->alloc - sum->size) * sizeof(mx_limb_t));
      sum->size *= nbr_sign(addend1);
    }
    else
    {
      const int abscomp = unbr_comp(addend1->limbs, abs(addend1->size), addend2->limbs, abs(addend2->size));
      if (abscomp == 0)
      {
        nbr_assign_zero(sum);
      }
      else
      {
        const mx_int_t *large, *small;
        if (abscomp == -1)
        {
          large = addend2;
          small = addend1;
        }
        else if (abscomp == 1)
        {
          large = addend1;
          small = addend2;
        }

        if (sum->alloc < (mx_size_t) abs(large->size))
        {
          mx_free(sum->limbs);
          sum->limbs = mx_malloc(abs(large->size), &(sum->alloc));
        }

        sum->size = unbr_sub(large->limbs, abs(large->size), small->limbs, abs(small->size), sum->limbs);
        memset(sum->limbs + sum->size, 0, (sum->alloc - sum->size) * sizeof(mx_limb_t));
        sum->size *= nbr_sign(large);
      }
    }
  }
}
