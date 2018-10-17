
#include "mathx/core/sub.h"

#include "mathx/core/memory.h"
#include "mathx/core/uadd.h"
#include "mathx/core/ucomp.h"
#include "mathx/core/usub.h"

#include <stdlib.h>
#include <string.h>

/*@
 * \fn void nbr_sub(mx_int_t *difference, const mx_int_t *minuend, const mx_int_t *subtrahend)
 * \brief Subtracts two integers.
 * \param variable that will receive the difference
 * \param the minuend
 * \param the subtrahend
 *
 * Performs \c{difference = minuend - subtrahend}.
 */
void nbr_sub(mx_int_t *difference, const mx_int_t *minuend, const mx_int_t *subtrahend)
{
  if (subtrahend->size == 0)
  {
    nbr_assign(difference, minuend);
  }
  else if (minuend->size == 0)
  {
    nbr_assign(difference, subtrahend);
    difference->size *= -1;
  }
  else
  {
    if (nbr_sign(minuend) != nbr_sign(subtrahend))
    {
      mx_size_t result_size = abs(minuend->size) + abs(subtrahend->size) + 1;
      if (difference->alloc < result_size)
      {
        mx_free(difference->limbs);
        difference->limbs = mx_malloc(result_size, &(difference->alloc));
      }
      difference->size = unbr_add(minuend->limbs, abs(minuend->size), subtrahend->limbs, abs(subtrahend->size), difference->limbs);
      memset(difference->limbs + difference->size, 0, (difference->alloc - difference->size) * sizeof(mx_limb_t));
      difference->size *= nbr_sign(minuend);
    }
    else
    {
      const int abscomp = unbr_comp(minuend->limbs, abs(minuend->size), subtrahend->limbs, abs(subtrahend->size));
      if (abscomp == 0)
      {
        nbr_assign_zero(difference);
      }
      else
      {
        const mx_int_t *large, *small;
        if (abscomp == -1)
        {
          large = subtrahend;
          small = minuend;
        }
        else if (abscomp == 1)
        {
          large = minuend;
          small = subtrahend;
        }

        if (difference->alloc < (mx_size_t) abs(large->size))
        {
          mx_free(difference->limbs);
          difference->limbs = mx_malloc(abs(large->size), &(difference->alloc));
        }

        difference->size = unbr_sub(large->limbs, abs(large->size), small->limbs, abs(small->size), difference->limbs);
        memset(difference->limbs + difference->size, 0, (difference->alloc - difference->size) * sizeof(mx_limb_t));
        difference->size *= abscomp * nbr_sign(minuend);
      }
    }
  }
}
