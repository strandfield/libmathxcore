
#include "test-framework-int.h"

void set_limbs_value_2(mx_limb_t *limbs, mx_limb_t a, mx_limb_t b)
{
  limbs[0] = a;
  limbs[1] = b;
}

void set_limbs_value_3(mx_limb_t *limbs, mx_limb_t a, mx_limb_t b, mx_limb_t c)
{
  set_limbs_value_2(limbs, a, b);
  limbs[2] = c;
}

void set_limbs_value_4(mx_limb_t *limbs, mx_limb_t a, mx_limb_t b, mx_limb_t c, mx_limb_t d)
{
  set_limbs_value_3(limbs, a, b, c);
  limbs[3] = d;
}

void set_limbs_value_5(mx_limb_t *limbs, mx_limb_t a, mx_limb_t b, mx_limb_t c, mx_limb_t d, mx_limb_t e)
{
  set_limbs_value_4(limbs, a, b, c, d);
  limbs[4] = e;
}

void set_limbs_value_6(mx_limb_t *limbs, mx_limb_t a, mx_limb_t b, mx_limb_t c, mx_limb_t d, mx_limb_t e, mx_limb_t f)
{
  set_limbs_value_5(limbs, a, b, c, d, e);
  limbs[5] = f;
}

void set_limbs_value_7(mx_limb_t *limbs, mx_limb_t a, mx_limb_t b, mx_limb_t c, mx_limb_t d, mx_limb_t e, mx_limb_t f, mx_limb_t g)
{
  set_limbs_value_6(limbs, a, b, c, d, e, f);
  limbs[6] = g;
}
