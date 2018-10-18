
#include "mathx/core/print.h"

#include "mathx/core/memory.h"

#include <assert.h>
#include <stdlib.h>

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

// Computes an upper bound of the number of digits of an integer of 
// size n in base DECIMAL_BASE.
//   number of digits = 1 + floor(log2(a) / log2(DECIMAL_BASE))
// Using 
//   log2(a) < as * sizeof(mx_limb_t)*8
// and
//   log2(DECIMAL_BASE) = log2(10) * DECIMAL_SHIFT > 3 * DECIMAL_SHIFT
mx_size_t mx_print_helper_size_upper_bound(const mx_size_t n)
{
  return 1 + (n * sizeofbits(mx_limb_t)) / (3 * DECIMAL_SHIFT);
}

/*@
 * \fn mx_size_t unbr_print(const mx_limb_t *a, const mx_size_t as, char *output)
 * \brief Prints an unsigned integer.
 * \param pointer to the least-signficant limb of the integer
 * \param size of the integer
 * \param write location
 * \returns the number of char written (excluding null-terminator)
 *
 * The function assumes that there is enough space in \c output to write the result.
 */
mx_size_t unbr_print(const mx_limb_t *a, const mx_size_t as, char *output)
{
  mx_size_t size, i, j;
  mx_limb_t rem;

  // We start by computing a quick upper bound for the number of digits
  // of a in base DECIMAL_BASE and allocate limbs accordingly.
  const mx_size_t size_upper_bound = mx_print_helper_size_upper_bound(as);
  mx_limb_t *out = mx_malloc_zero(size_upper_bound, NULL);

  // Then we do radix conversion from our binary base to our decimal base
  // using Method 1b) of section 4.4 of Knuth.
  // This basically comes down to computing sum(a[i]*b^i) in base DECIMAL_BASE.
  size = 0;
  for (i = as; --i >= 0; ) 
  {
    mx_limb_t hi = a[i];
    for (j = 0; j < size; j++)
    {
      mx_longlimb_t z = ((mx_longlimb_t) out[j] << sizeofbits(mx_limb_t)) | hi;
      hi = (mx_limb_t)(z / DECIMAL_BASE);
      out[j] = (mx_limb_t)(z - (mx_longlimb_t)hi * DECIMAL_BASE);
    }
    while (hi) 
    {
      out[size++] = hi % DECIMAL_BASE;
      hi /= DECIMAL_BASE;
    }
  }
  
  // We ensure we have at least one digit
  if (size == 0)
    out[size++] = 0;

  // Then we compute the exact number of char we need to print the number.
  // This consits of all limbs except the topmost one (DECIMAL_SHIFT digits each) 
  // plus the number of decimal digits in the topmost limb.
  mx_size_t strlen = 1 + (size - 1) * DECIMAL_SHIFT;
  mx_longlimb_t tenpow = 10;
  rem = out[size - 1];
  while (rem >= tenpow) 
  {
    tenpow *= 10;
    strlen++;
  }

  char *outit = output + strlen;

  // Writing decimal digits from all limbs except the topmost one (DECIMAL_SHIFT digits each).
  for (i = 0; i < size - 1; i++) 
  {
    rem = out[i];
    for (j = 0; j < DECIMAL_SHIFT; j++) 
    {
      *--outit = '0' + rem % 10;
      rem /= 10;
    }
  }

  // Writing decimal digits from the topmost limb (at least 1).
  rem = out[i];
  do {
    *--outit = '0' + rem % 10;
    rem /= 10;
  } while (rem != 0);


  mx_free(out);
  output[strlen] = '\0';

  return strlen;
}