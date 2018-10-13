
#include "mathx/core/integer.h"

#include "mathx/core/memory.h"

#include <stdlib.h> // abs
#include <string.h>

/*@
 * \class mx_integer_struct
 * \brief Represents an arbitrary-precision signed integer.
 *
 * \endclass
 */


/*@
 * \fn void nbr_init(mx_int_t *x)
 * \brief Zero-initialize an integer.
 * \param the integer to initialize
 */
void nbr_init(mx_int_t *x)
{
  x->size = 0;
  x->alloc = 0;
  x->limbs = NULL;
}

/*@
 * \fn void nbr_copy_init(mx_int_t *x, const mx_int_t *src)
 * \brief Performs copy-initialization of an integer.
 * \param the integer to initialize
 * \param integer to copy
 */
void nbr_copy_init(mx_int_t *x, const mx_int_t *src)
{
  x->size = src->size;
  x->alloc = src->alloc;

  if (x->alloc)
  {
    x->limbs = mx_malloc(x->alloc, NULL);
    memcpy(x->limbs, src->limbs, x->alloc * sizeof(mx_limb_t));
  }
  else
  {
    x->limbs = NULL;
  }
}

/*@
 * \fn void nbr_limb_init(mx_int_t *x, const mx_limb_t value)
 * \brief Initializes an integer from a single limb.
 * \param the integer to initialize
 * \param the limb's value
 */
void nbr_limb_init(mx_int_t *x, const mx_limb_t value)
{
  if (value == 0)
  {
    x->alloc = 0;
    x->size = 0;
    x->limbs = NULL;
  }
  else
  {
    x->size = 1;
    x->limbs = mx_malloc(1, &(x->alloc));
    x->limbs[0] = value;
    memset(x->limbs + 1, 0, (x->alloc - 1) * sizeof(mx_limb_t));
  }
}

/*@
 * \fn void nbr_raw_init(mx_int_t *x, mx_ssize_t size, mx_limb_t *limbs, mx_size_t alloc)
 * \brief Initializes an integer given all its internal data members.
 * \param the integer to initialize
 * \param the integer signed size
 * \param pointer to the first limb
 * \param number of allocated limbs
 *
 * This function assumes that allocated limbs that are not part of the integer are 
 * set to zero, i.e. limbs[i] is zero for all i in [abs(size), alloc).
 */
void nbr_raw_init(mx_int_t *x, mx_ssize_t size, mx_limb_t *limbs, mx_size_t alloc)
{
  x->alloc = alloc;
  x->size = size;
  x->limbs = limbs;
}

/*@
 * \fn void nbr_clear(mx_int_t *x)
 * \brief Releases any memory used by an arbitrary-precision integer.
 */
void nbr_clear(mx_int_t *x)
{
  mx_free(x->limbs);
  x->size = 0;
  x->alloc = 0;
}

/*@
 * \fn void nbr_assign(mx_int_t *dest, const mx_int_t *src)
 * \brief Performs integer assignment.
 * \param receiver
 * \param value to assign
 */
void nbr_assign(mx_int_t *dest, const mx_int_t *src)
{
  const mx_size_t srcsize = abs(src->size);

  if (dest->alloc < srcsize)
  {
    mx_free(dest->limbs);
    dest->limbs = mx_malloc(srcsize, &(dest->alloc));
    memset(dest->limbs + srcsize, 0, (dest->alloc - srcsize) * sizeof(mx_limb_t));
  }
  else if(srcsize < (mx_size_t) abs(dest->size))
  {
    memset(dest->limbs + srcsize, 0, (abs(dest->size) - srcsize) * sizeof(mx_limb_t));
  }

  dest->size = src->size;
  memcpy(dest->limbs, src->limbs, srcsize * sizeof(mx_limb_t));
}