// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mathx/core/integer.h"

#include "mathx/core/memory.h"
#include "mathx/core/print.h"
#include "mathx/core/shift.h"

#include <stdlib.h> // abs
#include <string.h>

/*@
 * \class mx_integer_struct
 * \brief Represents an arbitrary-precision signed integer.
 *
 * \endclass
 */


/*@
 * \fn void int_init(mx_int_t *x)
 * \brief Zero-initialize an integer.
 * \param the integer to initialize
 */
void int_init(mx_int_t *x)
{
  x->size = 0;
  x->alloc = 0;
  x->limbs = NULL;
}

/*@
 * \fn void int_copy_init(mx_int_t *x, const mx_int_t *src)
 * \brief Performs copy-initialization of an integer.
 * \param the integer to initialize
 * \param integer to copy
 */
void int_copy_init(mx_int_t *x, const mx_int_t *src)
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
 * \fn void int_limb_init(mx_int_t *x, const mx_limb_t value)
 * \brief Initializes an integer from a single limb.
 * \param the integer to initialize
 * \param the limb's value
 */
void int_limb_init(mx_int_t *x, const mx_limb_t value)
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
 * \fn void int_raw_init(mx_int_t *x, mx_ssize_t size, mx_limb_t *limbs, mx_size_t alloc)
 * \brief Initializes an integer given all its internal data members.
 * \param the integer to initialize
 * \param the integer signed size
 * \param pointer to the first limb
 * \param number of allocated limbs
 *
 * This function assumes that allocated limbs that are not part of the integer are 
 * set to zero, i.e. limbs[i] is zero for all i in [abs(size), alloc).
 */
void int_raw_init(mx_int_t *x, mx_ssize_t size, mx_limb_t *limbs, mx_size_t alloc)
{
  x->alloc = alloc;
  x->size = size;
  x->limbs = limbs;
}

/*@
 * \fn void int_string_init(mx_int_t *x, const char *str)
 * \brief Initializes an integer from a string.
 * \param the integer to initialize
 * \param null-terminated char string
 *
 */
void int_string_init(mx_int_t *x, const char *str)
{
#if LIBMATHXCORE_LIMB_SIZE == 8
#define DECIMAL_BASE 100
#define DECIMAL_SHIFT 2
#elif LIBMATHXCORE_LIMB_SIZE == 16
#define DECIMAL_BASE 10000
#define DECIMAL_SHIFT 4
#else
#define DECIMAL_BASE 1000000000
#define DECIMAL_SHIFT 9
#endif

  int_init(x);

  mx_int_t base;
  int_limb_init(&base, DECIMAL_BASE);

  mx_int_t buffer;
  int_init(&buffer);

  mx_ssize_t sign = 1;
  if (str[0] == '-')
  {
    sign = -1;
    str++;
  }
  else if (str[0] == '+')
  {
    str++;
  }

  while (*str != '\0')
  {
    mx_limb_t limb = 0;
    base.limbs[0] = 1;
    while (*str != '\0' && base.limbs[0] < DECIMAL_BASE)
    {
      limb = limb * 10 + (*str - '0');
      ++str;
      base.limbs[0] *= 10;
    }

    int_mul(&buffer, x, &base);
    int_swap(&buffer, x);
    int_limb_abs_incr(x, limb);
  }

  x->size *= sign;

  int_clear(&base);
  int_clear(&buffer);

#undef DECIMAL_BASE
#undef DECIMAL_SHIFT
}

/*@
 * \fn void int_clear(mx_int_t *x)
 * \brief Releases any memory used by an arbitrary-precision integer.
 */
void int_clear(mx_int_t *x)
{
  mx_free(x->limbs);
  x->size = 0;
  x->alloc = 0;
}

/*@
 * \fn int int_is_normalized(mx_int_t *x)
 * \brief Returns whether the input is in its normalized form.
 */
int int_is_normalized(const mx_int_t *x)
{
  const mx_size_t s = abs(x->size);
  mx_size_t i = s;

  for (; i < x->alloc; ++i)
  {
    if (x->limbs[i] != 0)
      return 0;
  }

  if (s != 0 && x->limbs[s - 1] == 0)
    return 0;

  return 1;
}

/*@
 * \fn void int_normalize(mx_int_t *x)
 * \brief Normalizes its input.
 */
void int_normalize(mx_int_t *x)
{
  mx_size_t s = abs(x->size);
  memset(x->limbs + s, 0, (x->alloc - s) * sizeof(mx_limb_t));
  while (s > 0 && x->limbs[s - 1] == 0) --s;
  x->size = s * (x->size < 0 ? -1 : 1);
}

/*@
 * \fn void int_ensure_alloc(mx_int_t *x, mx_size_t s)
 * \brief Ensures enough limbs are allocated.
 * \param pointer to integer object
 * \param number of limbs that needs to be allocated
 *
 * Ensures \c x can be manipulated as if there were exactly \c s limbs allocated.
 * In other words, this function will allocate new limbs if not enough are already 
 * allocated, and/or will memset to zero limbs with index \c{>= s}.
 *
 * The size field of \c x is left unchanged, so calling this function might 
 * result in \c x being left in a non-normalized form.
 *
 * This function is mainly for internal purpose.
 */
void int_ensure_alloc(mx_int_t *x, mx_size_t s)
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
 * \fn void int_ensure_alloc_zero(mx_int_t *x, mx_size_t s)
 * \brief Ensures enough limbs are allocated and set to zero.
 * \param pointer to integer object
 * \param number of limbs that needs to be allocated
 *
 * Ensures \c x can be manipulated as if there were exactly \c s zero-initialized limbs allocated.
 *
 * The size field of \c x is left unchanged, so calling this function might
 * result in \c x being left in a non-normalized form.
 *
 * This function is mainly for internal purpose.
 */
void int_ensure_alloc_zero(mx_int_t *x, mx_size_t s)
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
 * \fn void int_assign(mx_int_t *dest, const mx_int_t *src)
 * \brief Performs integer assignment.
 * \param receiver
 * \param value to assign
 */
void int_assign(mx_int_t *dest, const mx_int_t *src)
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

/*@
 * \fn void int_assign_zero(mx_int_t *dest)
 * \brief Assigns the value 0 to an integer
 * \param receiver
 */
void int_assign_zero(mx_int_t *dest)
{
  memset(dest->limbs, 0, abs(dest->size) * sizeof(mx_limb_t));
  dest->size = 0;
}

void int_limb_assign(mx_int_t *dest, const mx_limb_t limb)
{
  if (limb == 0)
  {
    int_assign_zero(dest);
  }
  else
  {
    int_ensure_alloc(dest, 1);
    dest->limbs[0] = limb;
    dest->size = 1;
  }
}

/*@
 * \fn void int_swap(mx_int_t *a, mx_int_t *b)
 * \brief Swaps two integers
 * \param first value
 * \param second value
 */
void int_swap(mx_int_t *a, mx_int_t *b)
{
  const mx_int_t c = *b;

  *b = *a;
  *a = c;
}

/*@
 * \fn int int_sign(const mx_int_t *x)
 * \brief Returns the sign of an integer.
 * \param input value
 * \returns -1, 0 or 1
 */
int int_sign(const mx_int_t *x)
{
  return x->size == 0 ? 0 : (x->size < 0 ? -1 : 1);
}

/*@
 * \fn int int_is_zero(const mx_int_t *x)
 * \brief Returns whether the input is zero
 * \param input value
 * \returns 0 or 1
 */
int int_is_zero(const mx_int_t *x)
{
  return x->size == 0;
}

/*@
 * \fn int int_is_odd(const mx_int_t *x)
 * \brief Returns whether the input is an odd integer
 * \param input value
 * \returns 0 or 1
 */
int int_is_odd(const mx_int_t *x)
{
  if (x->size == 0)
    return 0;
  return (x->limbs[0] & 1);
}

/*@
 * \fn int int_is_even(const mx_int_t *x)
 * \brief Returns whether the input is an even integer
 * \param input value
 * \returns 0 or 1
 */
int int_is_even(const mx_int_t *x)
{
  return (x->size == 0) || ((x->limbs[0] & 1) == 0);
}

/*@
 * \fn void int_negate(mx_int_t *y, const mx_int_t *x)
 * \brief Computes the opposite of an integer
 * \param integer that will receive the result
 * \param input integer
 *
 * Informally, performs \c{y = -x}.
 */
void int_negate(mx_int_t *y, const mx_int_t *x)
{
  int_assign(y, x);
  y->size *= -1;
}

/*@
 * \fn void int_abs(mx_int_t *y, const mx_int_t *x)
 * \brief Computes the absolute value of an integer
 * \param integer that will receive the result
 * \param input integer
 *
 * Informally, performs \c{y = abs(x)}.
 */
void int_abs(mx_int_t *y, const mx_int_t *x)
{
  int_assign(y, x);
  y->size = abs(y->size);
}

/*@
 * \fn void int_rightshift(mx_int_t *result, const mx_int_t *x, mx_size_t n)
 * \brief Shifts an integer right.
 * \param variable to store the result
 * \param input integer
 * \param shift amount
 * 
 */
void int_rightshift(mx_int_t *result, const mx_int_t *x, mx_size_t n)
{
  const mx_size_t limb_shift = n / sizeofbits(mx_limb_t);

  if (limb_shift >= (mx_size_t) abs(x->size))
  {
    int_assign_zero(result);
  }
  else
  {
    n -= limb_shift * sizeofbits(mx_limb_t);
    int_ensure_alloc(result, abs(x->size) - limb_shift);
    result->size = abs(x->size) - limb_shift;
    result->limbs[result->size - 1] = 0;
    uint_rshift(x->limbs + limb_shift, result->size, n, result->limbs);
    if (result->limbs[result->size - 1] == 0)
      result->size -= 1;
    result->size *= int_sign(x);
  }
}

/*@
 * \fn void int_rightshiftmx_int_t *x, mx_size_t n)
 * \brief Shifts an integer right.
 * \param integer to shift right
 * \param shift amount
 * 
 */
void int_rightshift_assign(mx_int_t *x, mx_size_t n)
{
  const mx_size_t limb_shift = n / sizeofbits(mx_limb_t);

  if (limb_shift >= (mx_size_t) abs(x->size))
  {
    int_assign_zero(x);
  }
  else
  {
    n -= limb_shift * sizeofbits(mx_limb_t);
    mx_size_t result_size = abs(x->size) - limb_shift;
    uint_rshift_overlap(x->limbs + limb_shift, result_size, n, x->limbs);
    if (x->limbs[result_size - 1] == 0)
      result_size -= 1;
    memset(x->limbs + result_size, 0, (abs(x->size) - result_size) * sizeof(mx_limb_t));
    x->size = result_size * int_sign(x);
  }
}

/*@
 * \fn mx_size_t int_print(char *out, mx_size_t s, const mx_int_t *x)
 * \brief Writes the decimal representation of an integer
 * \param pointer to output
 * \param size of the output buffer
 * \param integer to print
 * \returns number of char written (excluding null-terminator)
 *
 * This function starts by computing an upper bound of the number of char 
 * that will be written. If the buffer is smaller than this upper bound, 
 * no char are written and this function returns zero. 
 */
mx_size_t int_print(char *out, mx_size_t s, const mx_int_t *x)
{
  if (s < 2)
    return 0;

  if (x->size == 0)
  {
    out[0] = '0';
    out[1] = '\0';
    return 1;
  }

  // upper_bound = sign + #digits + null-terminator
  const mx_size_t upper_bound = (x->size < 0 ? 1 : 0) + uint_print_size(x->limbs, abs(x->size)) + 1;
  if (s < upper_bound)
    return 0;
  
  mx_size_t written = 0;

  if (x->size < 0)
  {
    *(out++) = '-';
    written += 1;
  }

  written += uint_print(x->limbs, abs(x->size), out);

  return written;
}