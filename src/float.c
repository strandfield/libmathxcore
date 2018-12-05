
#include "mathx/core/float.h"

#include "mathx/core/memory.h"
#include "mathx/core/ucomp.h"
#include "mathx/core/print.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*@
 * \class mx_real_struct
 * \brief Represents a floating-point number.
 *
 * This struct is usually used through its typedef: \t mx_float_t.
 *
 * \endclass
 */

static mx_size_t g_default_prec = 16 / sizeof(mx_limb_t);

/*@
 * \fn mx_size_t float_default_prec()
 * \brief Returns the default precision for floating-point numbers.
 *
 */
mx_size_t float_default_prec()
{
  return g_default_prec;
}

/*@
 * \fn mx_size_t float_set_default_prec(mx_size_t p)
 * \param new value
 * \brief Sets the default precision for floating-point numbers.
 * \returns the previous value
 */
mx_size_t float_set_default_prec(mx_size_t p)
{
  mx_size_t ret = g_default_prec;
  g_default_prec = p;
  return ret;
}

/*@
 * \fn mx_size_t float_prec_bits(mx_size_t bitcount)
 * \param bit count
 * \brief Returns a precision based on a bit count
 * 
 */
mx_size_t float_prec_bits(mx_size_t bitcount)
{
  mx_size_t ret = bitcount / sizeofbits(mx_limb_t);
  if (ret * sizeofbits(mx_limb_t) < ret)
    ++ret;
  return ret;
}

/*@
 * \fn void float_init(mx_float_t *x)
 * \param real number to initialize
 * \brief Initialize a floating-point number with the value 0
 * 
 * This function uses the global default precision to initialize 
 * the \c prec field of \c x.
 * \sa float_default_prec
 */
void float_init(mx_float_t *x)
{
  x->alloc = 0;
  x->exp = 0;
  x->limbs = NULL;
  x->prec = g_default_prec;
  x->size = 0;
}

/*@
 * \fn void float_init_prec(mx_float_t *x, mx_size_t p)
 * \param floating-point number to initialize
 * \param precision
 * \brief Initialize a floating-point number with the value 0 and the desired precision.
 * 
 */
void float_init_prec(mx_float_t *x, mx_size_t p)
{
  x->alloc = 0;
  x->exp = 0;
  x->limbs = NULL;
  x->prec = p;
  x->size = 0;
}

/*@
 * \fn void float_raw_init(mx_float_t *x, mx_ssize_t size, mx_limb_t *limbs, mx_size_t alloc, mx_exp_t exp, mx_size_t prec)
 * \param floating-point number to initialize
 * \param signed size
 * \param limbs
 * \param number of limbs allocated
 * \param exponent
 * \param precision, in number of limbs
 * \brief Initialize a floating-point number from its internal data members
 * 
 */
void float_raw_init(mx_float_t *x, mx_ssize_t size, mx_limb_t *limbs, mx_size_t alloc, mx_exp_t exp, mx_size_t prec)
{
  x->size = size;
  x->limbs = limbs;
  x->alloc = alloc;
  x->exp = exp;
  x->prec = prec;
}


/*@
 * \fn void float_ensure_alloc(mx_float_t *x, mx_size_t s)
 * \brief Ensures enough limbs are allocated.
 * \param pointer to floating point number
 * \param number of limbs that needs to be allocated
 *
 * Floating point version of \m int_ensure_alloc.
 */
void float_ensure_alloc(mx_float_t *x, mx_size_t s)
{
  if (x->alloc < s)
  {
    mx_free(x->limbs);
    x->limbs = mx_malloc(s, &(x->alloc));
    memset(x->limbs + s, 0, (x->alloc - s) * sizeof(mx_limb_t));
  }
  else
  {
    if (s < (mx_size_t) abs(x->size))
    {
      memset(x->limbs + s, 0, (abs(x->size) - s) * sizeof(mx_limb_t));
    }
  }
}

/*@
 * \fn void float_ensure_alloc_zero(mx_float_t *x, mx_size_t s)
 * \brief Ensures enough limbs are allocated and set to zero.
 * \param pointer to floating point number
 * \param number of limbs that needs to be allocated
 *
 * Floating point version of \m int_ensure_alloc_zero.
 */
void float_ensure_alloc_zero(mx_float_t *x, mx_size_t s)
{
  if (x->alloc < s)
  {
    mx_free(x->limbs);
    x->limbs = mx_malloc_zero(s, &(x->alloc));
  }
  else
  {
    memset(x->limbs, 0, abs(x->size) * sizeof(mx_limb_t));
  }
}

/*@
 * \fn void float_set_prec(mx_float_t *x, mx_size_t p)
 * \param input floating-point number
 * \param precision
 * \brief Sets the precision of a floating-point number
 * 
 * If the current precision of \c x is greater than \c p, 
 * limbs are removed from the number.
 */
void float_set_prec(mx_float_t *x, mx_size_t p)
{
  x->prec = p;

  if ((mx_size_t) abs(x->size) > x->prec)
  {
    const mx_size_t usize = abs(x->size);
    const mx_size_t diff = usize - x->prec;
    memmove(x->limbs, x->limbs + diff, x->prec * sizeof(mx_limb_t));
    memset(x->limbs + x->prec, 0, diff * sizeof(mx_limb_t));
    x->exp += diff;
    x->size = ((x->size < 0) ? -1 : 1) * ((mx_ssize_t) x->prec);
  }
}

/*@
 * \fn void float_copy(mx_float_t *dest, const mx_float_t *src)
 * \param destination variable
 * \param source variable
 * \brief Copies a floating point number.
 * 
 */
void float_copy(mx_float_t *dest, const mx_float_t *src)
{
  /// TODO: optimize, avoid unnecessary memory allocation

  dest->exp = src->exp;
  dest->prec = src->prec;
  dest->size = src->size;
  dest->alloc = abs(src->size);

  if (dest->alloc > 0)
  {
    dest->limbs = mx_malloc(dest->alloc, &(dest->alloc));
    memcpy(dest->limbs, src->limbs, sizeof(mx_limb_t) * abs(src->size));
    memset(dest->limbs + abs(src->size), 0, (dest->alloc - abs(src->size)) * sizeof(mx_limb_t));
  }
}

/*@
 * \fn void float_assign(mx_float_t *dest, const mx_float_t *src)
 * \param destination variable
 * \param source variable
 * \brief Assigns a floating point number to another.
 * 
 * Unlike \m float_copy, this function takes into account the precision 
 * of the destination variable.
 */
void float_assign(mx_float_t *dest, const mx_float_t *src)
{
  mx_size_t p = dest->prec;
  float_copy(dest, src);
  float_set_prec(dest, p);
}

/*@
 * \fn void float_assign_zero(mx_float_t *dest)
 * \param destination variable
 * \brief Assigns zero to a floating point number.
 * 
 */
void float_assign_zero(mx_float_t *dest)
{
  memset(dest->limbs, 0, sizeof(mx_limb_t) * abs(dest->size));
  dest->size = 0;
  dest->exp = 0;
}

/*@
 * \fn void float_assign_limb(mx_float_t *dest, mx_limb_t val)
 * \param result variable
 * \param limb value
 * \brief Assigns a single limb to a floating point number.
 * 
 */
void float_assign_limb(mx_float_t *dest, mx_limb_t val)
{
  if (val == 0)
    return float_assign_zero(dest);

  float_ensure_alloc(dest, 1);
  dest->limbs[0] = val;
  dest->size = 1;
  dest->exp = 0;
}

/*@
 * \fn void float_neg(mx_float_t *dest, const mx_float_t *src)
 * \param destination variable
 * \param source variable
 * \brief Assigns the opposite of a floating point number to another.
 * 
 */
void float_neg(mx_float_t *dest, const mx_float_t *src)
{
  float_assign(dest, src);
  dest->size *= -1;
}


/*@
 * \fn void float_swap(mx_float_t *a, mx_float_t *b)
 * \param first variable
 * \param second variable
 * \brief Swaps two floating point number
 * 
 */
void float_swap(mx_float_t *a, mx_float_t *b)
{
  mx_float_t temp = *b;
  *b = *a;
  *a = temp;
}

/*@
 * \fn void float_clear(mx_float_t *x)
 * \param input floating-point number
 * \brief Clears a floating-point number
 * 
 */
void float_clear(mx_float_t *x)
{
  x->alloc = 0;
  x->exp = 0;
  mx_free(x->limbs);
  x->limbs = NULL;
  x->prec = 0;
  x->size = 0;
}

/*@
 * \fn int float_sign(const mx_float_t *x)
 * \param input number
 * \brief Returns the sign of a floating point number.
 * 
 */
int float_sign(const mx_float_t *x)
{
  return x->size < 0 ? -1 : (x->size > 0 ? 1 : 0);
}

/*@
 * \fn int float_comp(const mx_float_t *a, const mx_float_t *b)
 * \param first floating point number
 * \param second floating point number
 * \brief Compares two floating point numbers
 * 
 */
int float_comp(const mx_float_t *a, const mx_float_t *b)
{
  if (float_sign(a) != float_sign(b))
    return float_sign(a) < float_sign(b) ? -1 : 1;
  else if (a->size == 0 && b->size == 0)
    return 0;

  assert(float_sign(a) == float_sign(b) && float_sign(a) != 0);

  const mx_size_t abs_asize = abs(a->size);
  const mx_size_t abs_bsize = abs(b->size);

  if (abs_asize + a->exp > abs_bsize + b->exp)
    return float_sign(a) * 1;
  else if (abs_bsize + b->exp > abs_asize + a->exp)
    return float_sign(a) * -1;

  // The most significant digits overlap
  if (abs_asize > abs_bsize)
  {
    const int c = uint_comp(a->limbs + (abs_asize - abs_bsize), abs_bsize, b->limbs, abs_bsize);
    if (c != 0)
      return c * float_sign(a);
    for (mx_size_t i = 0; i < abs_asize - abs_bsize; ++i)
    {
      if (a->limbs[i] != 0)
        return 1 * float_sign(a);
    }
    return 0;
  }
  else if (abs_asize < abs_bsize)
  {
    const int c = uint_comp(a->limbs, abs_asize, b->limbs + (abs_bsize - abs_asize), abs_asize);
    if (c != 0)
      return c * float_sign(a);
    for (mx_size_t i = 0; i < (abs_bsize - abs_asize); ++i)
    {
      if (b->limbs[i] != 0)
        return -1 * float_sign(a);
    }
    return 0;
  }
  else
  {
    const int c = uint_comp(a->limbs, abs_asize, b->limbs, abs_bsize);
    return c * float_sign(a);
  }
}

/*@
 * \fn mx_size_t float_raw_print(char *out, mx_size_t s, const mx_float_t *x)
 * \param output buffer
 * \param size of output buffer
 * \param input floating point number
 * \returns number of character written
 * \brief Prints a floating point number (mantissa & exponent).
 * 
 */
mx_size_t float_raw_print(char *out, mx_size_t s, const mx_float_t *x)
{
  mx_size_t written = 0;

  *(out++) = '(';
  ++written;

  mx_size_t w = uint_print(x->limbs, abs(x->size), out);
  out += w;
  written += w;

  *(out++) = ',';
  ++written;

  {
    mx_exp_t e = x->exp;
    if (e < 0)
    {
      *(out++) = '-';
      ++written;
      e *= -1;
    }

    if (e != 0)
    {
      w = sprintf(out, "%d", e);
      out += w;
      written += w;
    }
  }

  *(out++) = ')';
  ++written;

  *out = '\0';

  return written;
}
