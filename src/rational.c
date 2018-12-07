// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mathx/core/rational.h"

#include "mathx/core/gcd.h"

#include <assert.h>

/*@
 * \class mx_rational_struct
 * \brief Represents a rational number.
 *
 * This struct is usually used through its typedef: \t mx_rat_t.
 *
 * \endclass
 */

/*@
 * \fn void rat_init(mx_rat_t *x)
 * \brief Zero-initialize a rational number.
 * \param the rational to initialize
 */
void rat_init(mx_rat_t *x)
{
  int_init(&(x->num));
  int_limb_init(&(x->den), 1);
}

/*@
 * \fn void rat_copy_init(mx_rat_t *x, const mx_rat_t *src)
 * \brief Copy-initialize a rational number.
 * \param the rational to initialize
 * \param the rational to copy
 */
void rat_copy_init(mx_rat_t *x, const mx_rat_t *src)
{
  int_copy_init(&(x->num), &(src->num));
  int_copy_init(&(x->den), &(src->den));
}

/*@
 * \fn void rat_limb_init(mx_rat_t *x, const mx_limb_t value)
 * \brief Initializes a rational number from its numerator
 * \param the rational to initialize
 * \param the value of the numerator
 */
void rat_limb_init(mx_rat_t *x, const mx_limb_t value)
{
  int_limb_init(&(x->num), value);
  int_limb_init(&(x->den), 1);
}

/*@
 * \fn void rat_raw_init(mx_rat_t *x, const mx_int_t *num, const mx_int_t *den)
 * \brief Initializes a rational object given its internal data members
 * \param the rational to initialize
 * \param the numerator
 * \param the denominator
 *
 * Note that by calling this function, you implicitly transfer ownership of the integers 
 * to the \t mx_rat_t object.
 *
 * This function assumes that the resulting rational will be in normalized 
 * (or canonical form); i.e., the numerator and denominator do not share any common factors 
 * and the denominator is non-negative.
 */
void rat_raw_init(mx_rat_t *x, const mx_int_t *num, const mx_int_t *den)
{
  x->num = *num;
  x->den = *den;
}

/*@
 * \fn void rat_clear(mx_rat_t *x)
 * \brief Free a rational object.
 * \param the object to free
 *
 */
void rat_clear(mx_rat_t *x)
{
  int_clear(&(x->num));
  int_clear(&(x->den));
}

/*@
 * \fn int rat_is_normalized(const mx_rat_t *x)
 * \brief Returns whether a rational object is in normalized form.
 * \param the input rational
 * \returns 0 or 1
 *
 * Most functions expect their inputs to be in canonical (or normalized) form. 
 * A rational is in canonical form if there are no common factors between the 
 * numerator and the denominator and if the denominator is positive; i.e., the 
 * sign of a rational number is carried by its numerator.
 *
 * This function is provided mostly for testing purpose (e.g. asserts, etc..). 
 * If you are unsure whether your object is in canonical form, you should 
 * rather call \m rat_normalize directly.
 */
int rat_is_normalized(const mx_rat_t *x)
{
  if (x->den.size < 0)
    return 0;

  mx_int_t g;
  int_init(&g);

  int_gcd(&g, &(x->den), &(x->num));

  const int result = (g.size == 1 && g.limbs[0] == 1);
  int_clear(&g);

  return result;
}

/*@
 * \fn void rat_normalize(mx_rat_t *x)
 * \brief Puts the input in canonical form.
 * \param the input rational to canonicalize
 *
 * \sa rat_is_normalized
 */
void rat_normalize(mx_rat_t *x)
{
  const mx_ssize_t sign_adjustement = x->den.size < 0 ? -1 : 1;
  x->den.size *= sign_adjustement;
  x->num.size *= sign_adjustement;

  mx_int_t g;
  int_init(&g);

  int_gcd(&g, &(x->den), &(x->num));

  const int is_canonicalized = (g.size == 1 && g.limbs[0] == 1);
  if (!is_canonicalized)
  {
    mx_int_t canon;
    int_init(&canon);
    mx_int_t rem;
    int_init(&rem);

    // We do all computations on positive integers to avoid surprises
    mx_ssize_t sign = int_sign(&(x->num));
    x->num.size *= sign;
    int_div(&canon, &rem, &(x->num), &g);
    assert(int_is_zero(&rem));
    int_swap(&canon, &(x->num));
    x->num.size *= sign;

    sign = int_sign(&(x->den));
    x->den.size *= sign;
    int_div(&canon, &rem, &(x->den), &g);
    assert(int_is_zero(&rem));
    int_swap(&canon, &(x->den));
    x->den.size *= sign;

    int_clear(&canon);
    int_clear(&rem);
  }

  int_clear(&g);
}

/*@
 * \fn void rat_assign(mx_rat_t *dest, const mx_rat_t *src)
 * \brief Rational assignment
 * \param variable to be assigned
 * \param new value
 *
 */
void rat_assign(mx_rat_t *dest, const mx_rat_t *src)
{
  int_assign(&(dest->num), &(src->num));
  int_assign(&(dest->den), &(src->den));
}

/*@
 * \fn void rat_assign_zero(mx_rat_t *x)
 * \brief Sets the value of a rational to zero
 * \param variable to be assigned
 *
 */
void rat_assign_zero(mx_rat_t *x)
{
  int_limb_assign(&(x->den), 1);
  int_limb_assign(&(x->num), 0);
}

/*@
 * \fn void rat_limb_assign(mx_rat_t *x, const mx_limb_t limb)
 * \brief Sets the value of a rational to a positive integer
 * \param variable to be assigned
 * \param value of numerator
 *
 */
void rat_limb_assign(mx_rat_t *x, const mx_limb_t limb)
{
  int_limb_assign(&(x->den), 1);
  int_limb_assign(&(x->num), limb);
}

/*@
 * \fn void rat_swap(mx_rat_t *a, mx_rat_t *b)
 * \brief Swaps two rational numbers
 * \param first variable
 * \param second variable
 *
 */
void rat_swap(mx_rat_t *a, mx_rat_t *b)
{
  const mx_rat_t temp = *b;

  *b = *a;
  *a = temp;
}

/*@
 * \fn int rat_sign(const mx_rat_t *x)
 * \brief Returns the sign of the rational number
 * \param input rational number
 * \returns 0, 1 or -1
 *
 * This simply returns the sign of the numerator.
 */
int rat_sign(const mx_rat_t *x)
{
  return int_sign(&(x->num));
}

/*@
 * \fn int rat_is_zero(const mx_rat_t *x)
 * \brief Returns whether the rational is zero
 * \param input rational number
 * \returns 0 or 1
 *
 * This simply returns whether the numerator is zero.
 */
int rat_is_zero(const mx_rat_t *x)
{
  return int_is_zero(&(x->num));
}

/*@
 * \fn void rat_negate(mx_rat_t *y, const mx_rat_t *x)
 * \brief Computes the opposite of a rational
 * \param variable to receive the result
 * \param input variable
 *
 */
void rat_negate(mx_rat_t *y, const mx_rat_t *x)
{
  int_negate(&(y->num), &(x->num));
  int_assign(&(y->den), &(x->den));
}

/*@
 * \fn void rat_abs(mx_rat_t *y, const mx_rat_t *x)
 * \brief Computes the absolute value of a rational
 * \param variable to receive the result
 * \param input variable
 *
 */
void rat_abs(mx_rat_t *y, const mx_rat_t *x)
{
  int_abs(&(y->num), &(x->num));
  int_assign(&(y->den), &(x->den));
}

/*@
 * \fn int rat_is_integer(const mx_rat_t *x)
 * \brief Returns whether the rational number is in fact an integer
 * \param input variable
 *
 * This simply returns whether the denominator is one.
 */
int rat_is_integer(const mx_rat_t *x)
{
  return (x->den.size == 1 && x->den.limbs[0] == 1);
}

/*@
 * \fn mx_size_t rat_print(char *out, mx_size_t s, const mx_rat_t *x)
 * \brief Prints a rational number
 * \param pointer to output buffer
 * \param size of output buffer
 * \param input rational
 * \returns number of char written
 *
 * The return value does not include the null-terminator. 
 * This function uses \m int_print internally and therefore exhibits similar behavior:
 * if the function guesses that the output buffer may not be large enough, it won't
 * even try to print the rational at all even though there may actually be enough space.
 */
mx_size_t rat_print(char *out, mx_size_t s, const mx_rat_t *x)
{
  mx_size_t printed = int_print(out, s, &(x->num));
  if (printed == 0)
    return 0;

  if (printed == s && !rat_is_integer(x))
    return 0;
  else if (rat_is_integer(x))
    return printed;

  out[printed] = '/';
  printed++;

  mx_size_t written = int_print(out + printed, s - printed, &(x->den));
  if (written == 0)
    return 0;

  return printed + written;
}

/*@
 * \fn int rat_comp(const mx_rat_t *a, const mx_rat_t *b)
 * \brief Compares two rational numbers
 * \param first rational
 * \param second rational
 * \returns -1, 0 or 1
 *
 */
int rat_comp(const mx_rat_t *a_over_b, const mx_rat_t *c_over_d)
{
  if (rat_sign(a_over_b) != rat_sign(c_over_d))
  {
    const int s = rat_sign(a_over_b);

    if (s == 0)
      return rat_sign(c_over_d) == 1 ? -1 : 1;
    else
      return s;
  }

  const mx_int_t *a = &(a_over_b->num);
  const mx_int_t *b = &(a_over_b->den);
  const mx_int_t *c = &(c_over_d->num);
  const mx_int_t *d = &(c_over_d->den);

  mx_int_t ad;
  mx_int_t cb;

  int_init(&ad);
  int_init(&cb);

  int_mul(&ad, a, d);
  int_mul(&cb, c, b);

  const int ret = int_comp(&ad, &cb);

  int_clear(&ad);
  int_clear(&cb);

  return ret;
}

/*@
 * \fn void rat_add(mx_rat_t *sum, const mx_rat_t *a, const mx_rat_t *b)
 * \brief Adds two rational numbers
 * \param variable receiving the result
 * \param first term
 * \param second term
 *
 */
void rat_add(mx_rat_t *sum, const mx_rat_t *a_over_b, const mx_rat_t *c_over_d)
{
  const mx_int_t *a = &(a_over_b->num);
  const mx_int_t *b = &(a_over_b->den);
  const mx_int_t *c = &(c_over_d->num);
  const mx_int_t *d = &(c_over_d->den);

  mx_int_t ad;
  mx_int_t cb;

  int_init(&ad);
  int_init(&cb);

  int_mul(&ad, a, d);
  int_mul(&cb, c, b);

  int_add(&(sum->num), &ad, &cb);

  int_mul(&(sum->den), b, d);

  int_clear(&ad);
  int_clear(&cb);

  rat_normalize(sum);
}

/*@
 * \fn void rat_sub(mx_rat_t *diff, const mx_rat_t *a, const mx_rat_t *b)
 * \brief Subtracts two rational numbers
 * \param variable receiving the result
 * \param minuend
 * \param subtrahend
 *
 */
void rat_sub(mx_rat_t *diff, const mx_rat_t *a_over_b, const mx_rat_t *c_over_d)
{
  const mx_int_t *a = &(a_over_b->num);
  const mx_int_t *b = &(a_over_b->den);
  const mx_int_t *c = &(c_over_d->num);
  const mx_int_t *d = &(c_over_d->den);

  mx_int_t ad;
  mx_int_t cb;

  int_init(&ad);
  int_init(&cb);

  int_mul(&ad, a, d);
  int_mul(&cb, c, b);

  int_sub(&(diff->num), &ad, &cb);

  int_mul(&(diff->den), b, d);

  int_clear(&ad);
  int_clear(&cb);

  rat_normalize(diff);
}

/*@
 * \fn void rat_mul(mx_rat_t *product, const mx_rat_t *a, const mx_rat_t *b)
 * \brief Multiplies two rational numbers
 * \param variable receiving the result
 * \param first factor
 * \param second factor
 *
 */
void rat_mul(mx_rat_t *product, const mx_rat_t *a_over_b, const mx_rat_t *c_over_d)
{
  const mx_int_t *a = &(a_over_b->num);
  const mx_int_t *b = &(a_over_b->den);
  const mx_int_t *c = &(c_over_d->num);
  const mx_int_t *d = &(c_over_d->den);

  /// TODO:performance: try removing common factors beforehand, 
  // this would remove the need to canonicalize afterwards.

  int_mul(&(product->num), a, c);
  int_mul(&(product->den), b, d);

  rat_normalize(product);
}

/*@
 * \fn void rat_div(mx_rat_t *result, const mx_rat_t *a, const mx_rat_t *b)
 * \brief Divides two rational numbers
 * \param variable receiving the result
 * \param first input
 * \param second input
 *
 */
void rat_div(mx_rat_t *result, const mx_rat_t *a_over_b, const mx_rat_t *c_over_d)
{
  const mx_int_t *a = &(a_over_b->num);
  const mx_int_t *b = &(a_over_b->den);
  const mx_int_t *c = &(c_over_d->num);
  const mx_int_t *d = &(c_over_d->den);

  /// TODO:performance: try removing common factors beforehand, 
  // this would remove the need to canonicalize afterwards.

  int_mul(&(result->num), a, d);
  int_mul(&(result->den), b, c);

  rat_normalize(result);
}
