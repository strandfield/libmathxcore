// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mathx/core/float/fprint.h"

#include "mathx/core/memory.h"
#include "mathx/core/print.h"

#include "mathx/core/float/fpow.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*@
 * \fn mx_size_t float_digits_for_prec(mx_size_t prec)
 */
mx_size_t float_digits_for_prec(mx_size_t prec)
{
  return (mx_size_t) floor(log10(prec * sizeofbits(mx_limb_t)));
}

mx_float_t get_pow_ten(unsigned int n, mx_size_t prec)
{
  mx_float_t ten;
  float_init(&ten);
  float_assign_limb(&ten, 10);

  mx_float_t result;
  float_init_prec(&result, 2 * prec);
  float_pow_uint(&result, &ten, n);

  float_clear(&ten);

  return result;
}

/*@
 * \fn mx_size_t float_sprint(char *out, mx_size_t s, const mx_float_t *x)
 * \param output buffer
 * \param size of buffer
 * \param input number
 * \returns number of char written
 * \brief Prints a floating point number.
 *
 * A quick upper bound of the size required to write the result is computed.
 * If \c s is less than that estimate, this function returns 0 and does not 
 * attempt to print the floating point number.
 */
mx_size_t float_sprint(char *out, mx_size_t s, const mx_float_t *x)
{
  mx_int_t digits;
  int_init(&digits);

  const int n = float_dec_digits(&digits, x);

  const mx_size_t size_estimate = uint_print_size(digits.limbs, abs(digits.size)) + 1 + 1 + 9;
  if (s < size_estimate)
  {
    int_clear(&digits);
    return 0;
  }

  mx_size_t written = 0;

  if (digits.size < 0)
  {
    out[written] = '-';
    ++written;
    digits.size *= -1;
  }

  written += uint_print(digits.limbs, digits.size, out + written);

  if (n != 0)
  {
    out[written] = 'e';
    written++;
    written += sprintf(out + written, "%d", n);
  }

  return written;
}

/*@
 * \fn void float_print(const mx_float_t *x)
 * \param input number
 * \brief Prints a floating point number to stdout.
 *
 */
void float_print(const mx_float_t *x)
{
  const int size_estimate = uint_print_size(NULL, x->prec + 3) + 1 + 1 + 9;
  char *buffer = (char*)malloc(size_estimate);

  const mx_size_t written = float_sprint(buffer, size_estimate, x);
  assert(written > 0);

  printf("%s", buffer);

  free(buffer);
}

mx_exp_t float_dec_digits(mx_int_t *digits_out, const mx_float_t *x)
{
  if (x->exp < 0)
  {
    const int k = -1 * x->exp * sizeofbits(mx_limb_t);

    // We rewrite
    //     x = aaaaa * 2^-k
    // as
    //     x = aaaaa * 10^-n
    // this gives
    //     n = k * ln(2) / ln(10)
    // Unfortunately this will give a non-integer value, 
    // so we consider floor(n) and {n} = n - floor(n) instead
    // We have
    //     x = aaaaa * (2^-k * 10^floor(n)) * 10^-floor(n)
    // with (2^-k * 10^floor(n)) almost 1

    int n = (int)floor(k * log(2) / log(10));

    mx_float_t ten_pow_n = get_pow_ten((unsigned int)n, x->prec);

    mx_float_t digits;
    float_init_prec(&digits, 2 * x->prec);

    float_mul(&digits, x, &ten_pow_n);

    float_clear(&ten_pow_n);

    const int sign = digits.size < 0 ? -1 : 1;
    digits.size = abs(digits.size);

    // We should have digits.exp almost 0
    if (digits.exp < 0)
    {
      const int absexp = -digits.exp;
      memmove(digits.limbs, digits.limbs + absexp, (digits.size - absexp) * sizeof(mx_limb_t));
      memset(digits.limbs + digits.size - absexp, 0, absexp * sizeof(mx_limb_t));
      digits.size -= absexp;
    }
    else if (digits.exp > 0)
    {
      // We need to pad digits with zeros (bad)
      float_lshift_assign(&digits, sizeofbits(mx_limb_t) * digits.exp);
    }

    mx_free(digits_out->limbs);
    digits_out->limbs = digits.limbs;
    digits_out->size = digits.size * sign;
    digits_out->alloc = digits.alloc;

    return -n;
  }
  else if (x->exp > 0)
  {
    // Same principle, we rewrite
    //     x = aaaaa * 2^k
    // as
    //     x = (aaaaa * 2^k / 10^n) * 10^n
    // with
    //     n = floor(k * ln(2) / ln(10))

    const int k = x->exp * sizeofbits(mx_limb_t);
    int n = (int)floor(k * log(2) / log(10));

    assert(n >= 0);

    mx_float_t ten_pow_n = get_pow_ten((unsigned int)n, x->prec);

    mx_float_t digits;
    float_init_prec(&digits, x->prec);

    float_div(&digits, x, &ten_pow_n);

    float_clear(&ten_pow_n);

    const int sign = digits.size < 0 ? -1 : 1;
    digits.size = abs(digits.size);

    // digits.exp should be almost zero
    if (digits.exp > 0)
    {
      // We need to pad with zeros (bad)
      float_lshift_assign(&digits, digits.exp * sizeofbits(mx_limb_t));
    }
    else if (digits.exp <= 0)
    {
      const int absexp = -digits.exp;
      assert(absexp <= digits.size);
      memmove(digits.limbs, digits.limbs + absexp, (digits.size - absexp) * sizeof(mx_limb_t));
      memset(digits.limbs + digits.size - absexp, 0, absexp * sizeof(mx_limb_t));
      digits.size -= absexp;
    }

    float_clear(&digits);

    mx_free(digits_out->limbs);
    digits_out->limbs = digits.limbs;
    digits_out->size = digits.size * sign;
    digits_out->alloc = digits.alloc;

    return n;
  }
  else
  {
    assert(x->exp == 0);

    mx_int_t digits;
    digits.size = x->size;
    digits.alloc = x->alloc;
    digits.limbs = x->limbs;

    int_assign(digits_out, &digits);

    return 0;
  }
}
