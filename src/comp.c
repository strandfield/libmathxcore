
#include "mathx/core/comp.h"

#include "mathx/core/ucomp.h"

#include <stdlib.h> // abs

/*@
 * \fn mx_ssize_t nbr_comp(const mx_int_t *a, const mx_int_t *b)
 * \brief Compares two signed integers.
 * \param first integer
 * \param second integer
 * \returns zero, minus one or plus one
 *
 * Returns:
 * \begin{list}
 *   \li 0 if \c{a == b}
 *   \li 1 if \c{a > b}
 *   \li -1 if \c{a < b}
 * \end{list}
 */
mx_ssize_t nbr_comp(const mx_int_t *a, const mx_int_t *b)
{
  if (a->size < b->size)
    return -1;
  else if (a->size > b->size)
    return 1;

  return nbr_sign(a) * unbr_comp(a->limbs, abs(a->size), b->limbs, abs(b->size));
}
