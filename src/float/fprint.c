
#include "mathx/core/float/fprint.h"

#include "mathx/core/memory.h"
#include "mathx/core/print.h"

#include "mathx/core/float/fpow.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
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
 * \fn mx_size_t float_print(char *out, mx_size_t s, const mx_float_t *x)
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
mx_size_t float_print(char *out, mx_size_t s, const mx_float_t *x)
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

    int n = (int) floor(k * log(2) / log(10));

    mx_float_t ten;
    float_init(&ten);
    float_assign_limb(&ten, 10);

    mx_float_t tenN;
    float_init_prec(&tenN, 2 * x->prec);
    float_pow_uint(&tenN, &ten, (unsigned int)n);

    mx_float_t digits;
    float_init_prec(&digits, 2 * x->prec);

    mx_float_t u = *x;
    u.exp = 0;

    mx_float_t mul = tenN;
    //mul.limbs += (k / sizeofbits(mx_limb_t)); // dividing by 2^k
    //mul.size -= (k / sizeofbits(mx_limb_t));

    float_mul(&digits, &u, &mul);

    digits.exp += (k / sizeofbits(mx_limb_t));

    // We should have digits.exp almost equal to k
    memmove(digits.limbs, digits.limbs + digits.exp, (digits.size - digits.exp) * sizeof(mx_limb_t));
    digits.size -= digits.exp;
    digits.exp = 0;

    const mx_size_t size_estimate = uint_print_size(digits.limbs, abs(digits.size)) + 1 + 1 + 9;
    if (s < size_estimate)
    {
      float_clear(&ten);
      float_clear(&tenN);
      float_clear(&digits);
      return 0;
    }

    mx_size_t written = 0;

    if (digits.size < 0)
    {
      out[written] = '-';
      ++written;
      digits.size *= -1;
    }

    written += uint_print(digits.limbs, abs(digits.size), out + written);
    out[written] = 'e';
    written++;
    written += sprintf(out + written, "%d", -n);

    float_clear(&ten);
    float_clear(&tenN);
    float_clear(&digits);

    return written;
  }
  else if (x->exp > 0)
  {
    // We rewrite
    //     x = aaaaa * 2^k
    // as
    //     x = (aaaaa * 2^k / 10^n) * 10^n
    // with
    //     n = floor(k * ln(2) / ln(10))

    const int k = x->exp * sizeofbits(mx_limb_t);
    int n = (int) floor(k * log(2) / log(10));

    assert(n >= 0);

    mx_float_t ten_pow_n = get_pow_ten((unsigned int)n, x->prec);

    mx_float_t digits;
    float_init_prec(&digits, x->prec);

    float_div(&digits, x, &ten_pow_n);

    float_clear(&ten_pow_n);

    mx_size_t written = 0;
    if (digits.size < 0)
    {
      out[written] = '-';
      ++written;
      digits.size *= -1;
    }

    if (digits.exp > 0)
    {
      if (digits.alloc >= digits.size + digits.exp)
      {
        memmove(digits.limbs + digits.exp, digits.limbs, digits.size);
      }
      else
      {
        mx_limb_t *limbs = mx_malloc(digits.size + digits.exp, &digits.alloc);
        memcpy(limbs + digits.exp, digits.limbs, sizeof(mx_limb_t) * digits.size);
        mx_free(digits.limbs);
        digits.limbs = limbs;
      }

      memset(digits.limbs, 0, digits.exp * sizeof(mx_limb_t));
      digits.size += digits.exp;
      digits.exp = 0;

      const mx_size_t size_estimate = uint_print_size(digits.limbs, digits.size) + 1 + 1 + 9;
      if (s < size_estimate)
      {
        float_clear(&digits);
        return 0;
      }

      written += uint_print(digits.limbs, digits.size, out);
    }
    else if (digits.exp <= 0)
    {
      mx_float_t print_digits = digits;
      print_digits.limbs += -digits.exp;
      print_digits.size += digits.exp;

      const mx_size_t size_estimate = uint_print_size(print_digits.limbs, print_digits.size) + 1 + 1 + 9;
      if (s < size_estimate)
      {
        float_clear(&digits);
        return 0;
      }

      written += uint_print(print_digits.limbs, print_digits.size, out);
    }

    float_clear(&digits);

    out[written] = 'e';
    written++;
    written += sprintf(out + written, "%d", n);

    return written;
  }
  else
  {
    assert(x->exp == 0);

    mx_float_t digits = *x;

    const mx_size_t size_estimate = uint_print_size(digits.limbs, abs(digits.size)) + 1 + 1 + 9;
    if (s < size_estimate)
      return 0;

    mx_size_t written = 0;
    if (digits.size < 0)
    {
      out[written] = '-';
      ++written;
      digits.size *= -1;
    }
    written += uint_print(digits.limbs, digits.size, out);

    return written;
  }
}
