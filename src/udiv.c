
#include "mathx/core/udiv.h"

#include "mathx/core/memory.h"
#include "mathx/core/shift.h"

#include <assert.h>

static const unsigned char bits_in_digit_table[32] = {
  0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
  5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
};

static mx_size_t bits_in_digit(mx_limb_t d)
{
  mx_size_t d_bits = 0;
  while (d >= 32) {
    d_bits += 6;
    d >>= 6;
  }
  d_bits += (mx_size_t)bits_in_digit_table[d];
  return d_bits;
}

/*@
 * \fn void unbr_knuth_div(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *quo, mx_size_t *quos, mx_limb_t *rem, mx_size_t *rems)
 * \brief Performs euclidean division of unsigned integers using Knuth division algorithm.
 * \param pointer to dividend
 * \param size of dividend
 * \param pointer to divisor
 * \param size of divisor
 * \param location to write quotient
 * \param location to write size of quotient
 * \param location to write remainder
 * \param location to write size of remainder
 *
 * This function assumes:
 * \begin{list}
 *   \li as >= bs >= 2
 *   \li at least 1+as-bs limbs can be written in quo
 *   \li at least as+1 limbs can be written in rem
 * \end{list}
 *
 * It should be noted that, although the remainder will fit in bs limbs, as+1 limbs 
 * must be available for rem. 
 * This is because rem is used as a buffer to perform intermediate computations that 
 * involve a number slightly larger than the dividend.
 * 
 * A thorough description of the algorithm is available in section 4.3.1 of
 * The Art of Computer Programming, Vol. 2 by Donald Knuth.
 */
void unbr_knuth_div(const mx_limb_t *a, mx_size_t as, const mx_limb_t *b, mx_size_t bs, mx_limb_t *quo, mx_size_t *quos, mx_limb_t *rem, mx_size_t *rems)
{
  /* The following is an implementation of Algorithm D of section 4.3.1 
   * of The Art of Computer Programming, Vol. 2 by Knuth.
   *
   * The algorithm works much like 'long division' taught in gradeschool.
   * Given $u$ an $n+m$ digits integer and $v$ an $n$ digits number, 
   * the operation $u / v$ is computed by iteratively diving the $n+1$
   * most significant digits of $u$ by $v$, which yields a single digit 
   * of the quotient and a partial remainder.
   *
   * The division of an $n+1$ digits number by an $n$ digits number is 
   * done by computing an approximate quotient q_tilde from the two most 
   * significant digits of $u$ and the most significant digits of $v$.
   * It can be shown that, given prior normalization of $u$ and $v$, 
   * and a few additional checks, q_tilde is always a very good approximation
   * of the actual digits of the quotient.
   *
   * The complete algorithm, as well as various proofs, can be found in Knuth.
   */

  mx_limb_t *u, *v, *q;
  mx_size_t size_diff, size_u, size_v;
  mx_size_t d; // the amount by which u and v are shifted during normalisation
  mx_limb_t *uk, *qk;
  mx_longlimb_t q_tilde;
  const mx_longlimb_t base = 1 << sizeofbits(mx_limb_t);
  const mx_longlimb_t single_digit_mask = base - 1;

  // We allocate space to store the shifted divisor, and 
  // initialize alias variable to have notations similar to Knuth.
  size_u = as;
  u = rem;
  size_v = bs;
  q = quo;
  v = mx_malloc(size_v, NULL);
  assert(size_u >= size_v && size_v >= 2); 

  // D1. Normalization
  // We shift both the dividend $u$ and divisor $v$ left by the same amount 
  // (thus the quotient does not change) so that the highest bit of the 
  // divisor is set.
  // We also check that the highest digits of $v$ is bigger than 
  // the highest digit of $u$, if that is not the case, we add a leading 
  // zero to $u$.
  // This step ensures that q_tilde is always a very good approximation 
  // of the true quotient digit.
  d = sizeofbits(mx_limb_t) - bits_in_digit(v[size_v-1]);
  mx_limb_t normalization_output = unbr_lshift(b, bs, d, v);
  assert(normalization_output == 0);
  normalization_output = unbr_lshift(a, as, d, u);
  if (normalization_output != 0 || u[size_u - 1] >= v[size_v - 1]) 
  {
    u[size_u] = normalization_output;
    size_u++;
  }

  // We initialize several variables that will be used later.
  // Note that the quotient will have at most (and usually exactly)
  // size_diff digits.
  size_diff = size_u - size_v;
  assert(size_diff >= 0);
  const mx_longlimb_t vm1 = v[size_v - 1];
  const mx_longlimb_t vm2 = v[size_v - 2];

  // D2. -> D7. Loop over the digits of the quotient
  // An iteration consists of dividing uk[0:size_v+1] (i.e. the 
  // size_v+1 most significant digits of u) by v, giving a single-digit 
  // quotient and a remainder that is stored in uk[0:size_v].
  for (uk = u + size_diff, qk = q + size_diff; uk-- > u;) 
  {
    // D3. Calculate approximate quotient q_tilde
    // We may rarely overestimate by 1.
    const mx_limb_t utop = uk[size_v];
    assert(utop <= vm1);
    const mx_limb_t uu = ((mx_longlimb_t)utop << sizeofbits(mx_limb_t)) | uk[size_v - 1];
    q_tilde = (uu / vm1);
    mx_longlimb_t r = (uu - vm1 * q_tilde); /* r = uu % vm1 */
    while (vm2 * q_tilde > ((r << sizeofbits(mx_limb_t)) | uk[size_v - 2]))
    {
      --q_tilde;
      r += vm1;
      if (r >= base)
        break;
    }
    assert(q_tilde <= base);
    if (q_tilde == base)
      q_tilde -= 1;
    assert(q_tilde < base);

    // D4. Multiply and subtract
    // We subtract q_tilde * v[0:size_v] from uk[0:size_v+1]
    mx_limb_t zhi = 0;
    for (mx_size_t i = 0; i < size_v; ++i) {
      const mx_longlimb_t z = zhi + q_tilde * (mx_longlimb_t)v[i];
      zhi = z >> sizeofbits(mx_limb_t);
      if (uk[i] < (z & single_digit_mask)) { zhi += 1; }
      uk[i] -= (z & single_digit_mask);
    }

    // D5. Test remainder
    // If q_tilde is one too large, step D4 ought to yield 
    // a negative remainder.
    // Since we don't process the most significant digit of uk 
    // in the previous step, we can detect that by testing zhi against utop.
    assert(zhi - utop == 1 || utop - zhi == 0);
    if (utop < zhi) 
    {
      // D6. Add back
      // We overestimated the quotient by one, and so subtracted too 
      // many at step D4, we need to add v[0:size_v] back to uk[0:size_v]
      mx_longlimb_t carry = 0;
      for (mx_size_t i = 0; i < size_v; ++i)
      {
        carry += uk[i] + v[i];
        uk[i] = (mx_limb_t)carry;
        carry >>= sizeofbits(mx_limb_t);
      }
      --q_tilde;
    }


    // We write the computed digit of the quotient.
    assert(q_tilde < base);
    *--qk = (mx_limb_t)q_tilde;
  }
  
  // D8. Unormalize
  // We un-normalize u to get the correct remainder.
  normalization_output = unbr_rshift(u, size_v, d, u);
  assert(normalization_output == 0);

  while (size_u > 0 && u[size_u - 1] == 0) --size_u;
  while (size_diff > 0 && q[size_diff - 1] == 0) --size_diff;

  *quos = size_diff;
  *rems = size_u;

  mx_free(v);
}

/*@
 * \fn mx_limb_t unbr_limb_div(const mx_limb_t *a, mx_size_t as, const mx_limb_t b, mx_limb_t *quo)
 * \brief Performs euclidean division of an unsigned integer by a single limb.
 * \param pointer to dividend
 * \param size of dividend
 * \param divisor
 * \param location to write quotient
 * \returns remainder
 *
 * This algorithm performs division by one digit as taught in gradeschool.
 * The inputs \c a and \c quo can be equal, but shouldn't otherwise overlap.
 */
mx_limb_t unbr_limb_div(const mx_limb_t *a, mx_size_t as, const mx_limb_t b, mx_limb_t *quo)
{
  mx_longlimb_t rem = 0;

  a += as;
  quo += as;
  while (as-- > 0)
  {
    mx_limb_t hi;
    rem = (rem << sizeofbits(mx_limb_t)) | *--a;
    *--quo = hi = (mx_limb_t)(rem / b);
    rem -= (mx_longlimb_t)hi * b;
  }
  return (mx_limb_t)rem;
}
