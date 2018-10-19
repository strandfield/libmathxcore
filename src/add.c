
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
      mx_size_t result_size = max(abs(addend1->size), abs(addend2->size)) + 1;
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

/*@
 * \fn void nbr_limb_abs_incr(mx_int_t *x, mx_limb_t y)
 * \brief Increments the absolute value of an integer.
 * \param variable to increment
 * \param value to add
 *
 * This function treats \c x as if it was an unsigned integer and 
 * adds \c y to it.
 */
void nbr_limb_abs_incr(mx_int_t *x, mx_limb_t y)
{
  mx_limb_t carry = unbr_limb_incr(x->limbs, abs(x->size), y);

  if (carry)
  {
    if (x->alloc == (mx_size_t)abs(x->size))
    {
      mx_limb_t *limbs = x->limbs;
      x->limbs = mx_malloc(x->alloc + 1, &(x->alloc));
      memcpy(x->limbs, limbs, abs(x->size) * sizeof(mx_limb_t));
      mx_free(limbs);
      memset(x->limbs + abs(x->size), 0, (x->alloc - abs(x->size)) * sizeof(mx_limb_t));
    }

    x->limbs[abs(x->size)] = carry;
    x->size = (abs(x->size) + 1) * (x->size < 0 ? -1 : 1);
  }
}
