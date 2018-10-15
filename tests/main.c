
#include "test-framework.h"

#include "mathx/core/integer.h"
#include "mathx/core/memory.h"


Test(integer_comparison, same_size)
{
  mx_int_t a, b;

  nbr_limb_init(&a, 4);
  nbr_limb_init(&b, 4);

  int comp = nbr_comp(&a, &b);
  Assert(comp == 0);

  nbr_clear(&b);
  nbr_limb_init(&b, 6);
  comp = nbr_comp(&a, &b);
  Assert(comp == -1);

  a.size *= -1;
  b.size *= -1;

  comp = nbr_comp(&a, &b);
  Assert(comp == 1);
}

Test(integer_comparison, same_sign_different_size)
{
  mx_int_t a, b;

  nbr_limb_init(&a, 4);

  nbr_raw_init(&b, 2, mx_malloc_zero(2, NULL), 2);
  b.limbs[0] = 2;
  b.limbs[1] = 1;

  int comp = nbr_comp(&a, &b);
  Assert(comp == -1);

  a.size *= -1;
  b.size *= -1;
  comp = nbr_comp(&a, &b);
  Assert(comp == 1);

  a.limbs[0] = 0;
  a.size = 0;
  b.size *= -1;
  comp = nbr_comp(&a, &b);
  Assert(comp == -1);
}

Test(integer_comparison, different_signs)
{
  mx_int_t a, b;

  nbr_limb_init(&a, 4);
  nbr_limb_init(&b, 4);
  b.size *= -1;

  int comp = nbr_comp(&a, &b);
  Assert(comp == 1);

  a.size *= -1;
  b.size *= -1;
  comp = nbr_comp(&a, &b);
  Assert(comp == -1);
}

TestSuite(integer_comparison, same_size, same_sign_different_size, different_signs);


Test(integer_addition, carry_propagation_single_limb) 
{
  mx_int_t a, b, c;

  nbr_limb_init(&a, 1);
  nbr_limb_init(&b, ~((mx_limb_t) 0));
  nbr_init(&c);

  nbr_add(&c, &a, &b);

  Assert(c.size == 2);
  Assert(c.limbs[1] == 1);

  nbr_clear(&a);
  nbr_clear(&b);
  nbr_clear(&c);
}

Test(integer_addition, carry_propagation_two_limbs)
{
  mx_int_t a, b, c;

  mx_limb_t l = 0;
  l = ~l;

  nbr_limb_init(&a, 1);
  nbr_raw_init(&b, 2, mx_malloc_zero(2, NULL), 2);
  b.limbs[0] = l;
  b.limbs[1] = l;
  nbr_init(&c);

  nbr_add(&c, &b, &a);

  Assert(c.size == 3);
  Assert(c.limbs[0] == 0);
  Assert(c.limbs[1] == 0);
  Assert(c.limbs[2] == 1);

  nbr_clear(&a);
  nbr_clear(&b);
  nbr_clear(&c);
}

Test(integer_addition, add_zero)
{
  mx_int_t a, b, c;

  nbr_limb_init(&a, 4);
  nbr_init(&b);
  nbr_init(&c);

  nbr_add(&c, &a, &b);
  Assert(c.size == a.size);
  Assert(c.limbs[0] == a.limbs[0]);

  nbr_add(&c, &b, &a);
  Assert(c.size == a.size);
  Assert(c.limbs[0] == a.limbs[0]);

  nbr_clear(&a);
  nbr_clear(&b);
  nbr_clear(&c);
}

Test(integer_addition, opposite_signs)
{
  mx_int_t a, b, c;

  nbr_limb_init(&a, 4);
  nbr_limb_init(&b, 2);
  b.size *= -1;
  nbr_init(&c);

  nbr_add(&c, &b, &a);

  Assert(c.size == 1);
  Assert(c.limbs[0] == 2);

  nbr_clear(&b);
  
  nbr_limb_init(&b, 4);
  b.size *= -1;

  nbr_add(&c, &a, &b);

  Assert(c.size == 0);

  nbr_clear(&a);
  nbr_clear(&b);
  nbr_clear(&c);
}

TestSuite(integer_addition, carry_propagation_single_limb, carry_propagation_two_limbs, add_zero, opposite_signs);


int main(int argc, char *argv[])
{
  init_test_framework();

  register_test(&integer_comparison);
  register_test(&integer_addition);

  run_all_tests();

  return 0;
}