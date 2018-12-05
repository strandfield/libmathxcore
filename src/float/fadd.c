
#include "mathx/core/float/fadd.h"

#include "mathx/core/float/fsub.h"

#include "mathx/core/uadd.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/*@
 * \fn void float_add(mx_float_t *sum, const mx_float_t *addend1, const mx_float_t *addend2)
 * \brief Adds two floating-point numbers.
 * \param variable that will receive the sum
 * \param first addend
 * \param second addend
 *
 */
void float_add(mx_float_t *sum, const mx_float_t *addend1, const mx_float_t *addend2)
{
  if (addend1->size == 0)
    return float_assign(sum, addend2);
  else if (addend2->size == 0)
    return float_assign(sum, addend1);

  if (float_sign(addend1) != float_sign(addend2))
  {
    mx_float_t neg = *addend2;
    neg.size *= -1;
    float_sub(sum, addend1, &neg);
    return;
  }
  
  assert(float_sign(addend1) == float_sign(addend2) && float_sign(addend1) != 0);

  // Make u the largest input
  mx_float_t u, v;
  if (addend1->exp + abs(addend1->size) < addend2->exp + abs(addend2->size))
    u = *addend2, v = *addend1;
  else
    u = *addend1, v = *addend2;

  u.size = abs(u.size);
  v.size = abs(v.size);

  const mx_ssize_t hediff = (u.exp + u.size) - (v.exp + v.size);
  assert(hediff >= 0);

  //  uuuuuuuuu
  //         vvvvvvv
  // ^______^         
  //  hediff

  const mx_ssize_t prec = sum->prec;
  // We are going to write at most 'prec' digits to sum,
  // we may ignore some digits of 'u' and 'v' if they have too many.
  if (u.size > prec)
  {
    u.limbs += u.size - prec;
    u.exp += u.size - prec;
    u.size = prec;
  }
  if (v.size + hediff > prec)
  {
    //  uuuuuuuuu
    //         vvvvvvv
    //  rrrrrrrrrrr
    // ^______^    ^__^    
    //  hediff     v.size + hediff - prec

    v.limbs += v.size + hediff - prec;
    v.exp += v.size + hediff - prec;
    v.size = prec - hediff;
  }

  if (hediff >= prec)
  {
    // 'v' is completely ignored
    assert(v.size <= 0);
    float_assign(sum, &u);
    sum->size *= float_sign(addend1);
    return;
  }
  else
  {
    mx_float_t *r = sum;

    if (u.size > hediff)
    {
      // 'u' and 'v' partially overlap
      if (v.size + hediff <= u.size)
      {
        // uuuu  
        //   v 
        float_ensure_alloc(r, u.size + 1);
        r->limbs[u.size] = 0;
        mx_size_t size = u.size - hediff - v.size;
        memcpy(r->limbs, u.limbs, sizeof(mx_limb_t) * size);
        r->size = uint_add(u.limbs + size, u.size - size, v.limbs, v.size, r->limbs + size);
        r->size += size;
        r->exp = u.exp;
      }
      else
      {
        // uuuu   
        //   vvvvv
        float_ensure_alloc(r, v.size + hediff + 1);
        r->limbs[v.size + hediff] = 0;
        mx_size_t size = v.size + hediff - u.size;
        memcpy(r->limbs, v.limbs, sizeof(mx_limb_t) * size);
        r->size = uint_add(u.limbs, u.size, v.limbs + size, v.size - size, r->limbs + size);
        r->size += size;
        r->exp = v.exp;
      }
    }
    else
    {
      // 'u' and 'v' do not overlap, but the precision
      // is large enough so that the two must be taken into account
      // uuuu   
      //      vv
      float_ensure_alloc(r, v.size + hediff);
      mx_size_t size = v.size + hediff - u.size;
      memcpy(r->limbs, v.limbs, sizeof(mx_limb_t) * v.size);
      memset(r->limbs + v.size, 0, hediff - u.size);
      memcpy(r->limbs + size, u.limbs, sizeof(mx_limb_t) * u.size);
      r->size = size + u.size;
      r->exp = v.exp;
    }

    r->size *= float_sign(addend1);
  }
}
