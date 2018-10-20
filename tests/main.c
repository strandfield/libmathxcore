
#include "test-framework.h"

#include "mathx/core/integer.h"
#include "mathx/core/memory.h"

#include <string.h>


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


Test(integer_subtraction, signs)
{
  mx_int_t a, b, c;

  nbr_limb_init(&a, 4);
  nbr_limb_init(&b, 0);
  nbr_init(&c);

  // 4 - 0 = 4
  nbr_sub(&c, &a, &b);
  Assert(nbr_comp(&a, &c) == 0);
 
  // 0 - 4 = -4
  nbr_sub(&c, &b, &a);
  Assert(c.size == -1);
  Assert(c.limbs[0] == 4);

  // 4 - 2 = 2
  nbr_clear(&b);
  nbr_limb_init(&b, 2);
  nbr_sub(&c, &a, &b);
  Assert(c.size == 1);
  Assert(c.limbs[0] == 2);

  // 2 - 4 = -2
  nbr_sub(&c, &b, &a);
  Assert(c.size == -1);
  Assert(c.limbs[0] == 2);

  // -4 - -2 = -(4-2) = -2
  a.size *= -1;
  b.size *= -1;
  nbr_sub(&c, &a, &b);
  Assert(c.size == -1);
  Assert(c.limbs[0] == 2);

  // -2 - -4 = 4 - 2 = 2
  nbr_sub(&c, &b, &a);
  Assert(c.size == 1);
  Assert(c.limbs[0] == 2);

  // -4 - -4 = 0
  nbr_sub(&c, &a, &a);
  Assert(c.size == 0);

  // -2 - 4 = -6
  b.size = -1;
  a.size = 1;
  nbr_sub(&c, &b, &a);
  Assert(c.size == -1);
  Assert(c.limbs[0] == 6);

  // 2 - -4 = 6
  b.size = 1;
  a.size = -1;
  nbr_sub(&c, &b, &a);
  Assert(c.size == 1);
  Assert(c.limbs[0] == 6);

  nbr_clear(&a);
  nbr_clear(&b);
  nbr_clear(&c);
}

Test(integer_subtraction, borrow)
{
  mx_int_t a, b, c;

  mx_limb_t max_digit = 0;
  max_digit = ~max_digit;


  // 1*b^2 - 1 = b*(b-1) + (b-1)
  nbr_raw_init(&a, 3, mx_malloc_zero(4, NULL), 4);
  a.limbs[2] = 1;
  nbr_limb_init(&b, 1);
  nbr_init(&c);
  nbr_sub(&c, &a, &b);
  Assert(c.size == 2);
  Assert(c.limbs[0] == max_digit);
  Assert(c.limbs[1] == max_digit);

  nbr_clear(&a);
  nbr_clear(&b);
  nbr_clear(&c);
}


Test(integer_subtraction, cancelation)
{
  mx_int_t a, b, c;

  mx_limb_t max_digit = 0;
  max_digit = ~max_digit;

  nbr_raw_init(&a, 3, mx_malloc_zero(4, NULL), 4);
  nbr_limb_init(&b, 1);
  nbr_init(&c);

  // (1*b^2 + 1*b + 2) - (1*b^2 + 1*b + 1)
  a.limbs[2] = 1;
  a.limbs[1] = 1;
  a.limbs[0] = 2;
  a.size = 3;
  nbr_clear(&b);
  nbr_raw_init(&b, 3, mx_malloc_zero(4, NULL), 4);
  b.limbs[2] = 1;
  b.limbs[1] = 1;
  b.limbs[0] = 1;
  nbr_sub(&c, &a, &b);
  Assert(c.size == 1);
  Assert(c.limbs[0] == 1);

  // (1*b^2 + 1*b + 1) - (1*b^2 + 1*b + 1)
  a.limbs[0] = 1;
  nbr_sub(&c, &a, &b);
  Assert(c.size == 0);

  // (1*b^3 + 1*b^2 + 2*b + 1) - (1*b^3 + 1*b^2 + 1*b + 1)
  a.limbs[3] = 1;
  a.limbs[1] = 2;
  a.size = 4;
  b.limbs[3] = 1;
  b.size = 4;
  nbr_sub(&c, &a, &b);
  Assert(c.size == 2);
  Assert(c.limbs[1] == 1);
  Assert(c.limbs[0] == 0);

  nbr_clear(&a);
  nbr_clear(&b);
  nbr_clear(&c);
}

TestSuite(integer_subtraction, signs, borrow, cancelation);


Test(integer_multiplication, basics)
{
  mx_int_t a, b, c;

  mx_limb_t max_digit = 0;
  max_digit = ~max_digit;

  nbr_limb_init(&a, 6);
  nbr_init(&b);
  nbr_init(&c);

  // 6 * 0
  nbr_mul(&c, &a, &b);
  Assert(c.size == 0);

  // 6 * (1*b)
  nbr_clear(&b);
  nbr_raw_init(&b, 2, mx_malloc_zero(2, NULL), 2);
  b.limbs[1] = 1;
  nbr_mul(&c, &a, &b);
  Assert(c.size == 2);
  Assert(c.limbs[0] == 0);
  Assert(c.limbs[1] == 6);

  // (1*b^2 + 1) * (1*b^3 + b)
  nbr_clear(&a);
  nbr_raw_init(&a, 3, mx_malloc_zero(3, NULL), 3);
  a.limbs[0] = 1;
  a.limbs[2] = 1;
  nbr_clear(&b);
  nbr_raw_init(&b, 4, mx_malloc_zero(4, NULL), 4);
  b.limbs[1] = 1;
  b.limbs[3] = 1;
  nbr_mul(&c, &a, &b);
  Assert(c.size == 6);
  Assert(c.limbs[0] == 0);
  Assert(c.limbs[1] == 1);
  Assert(c.limbs[2] == 0);
  Assert(c.limbs[3] == 2);
  Assert(c.limbs[4] == 0);
  Assert(c.limbs[5] == 1);

  // (b/2) * (b/2)
  nbr_clear(&a);
  nbr_limb_init(&a, 1 << (sizeofbits(mx_limb_t) / 2));
  nbr_clear(&b);
  nbr_limb_init(&b, 1 << (sizeofbits(mx_limb_t) / 2));
  nbr_mul(&c, &a, &b);
  Assert(c.size == 2);
  Assert(c.limbs[0] == 0);
  Assert(c.limbs[1] == 1);

  // (b/2) * (1*b + b/2)
  nbr_clear(&a);
  nbr_limb_init(&a, 1 << (sizeofbits(mx_limb_t) / 2));
  nbr_clear(&b);
  nbr_raw_init(&b, 2, mx_malloc_zero(2, NULL), 2);
  b.limbs[0] = 1 << (sizeofbits(mx_limb_t) / 2);
  b.limbs[1] = 1;
  nbr_mul(&c, &a, &b);
  Assert(c.size == 2);
  Assert(c.limbs[0] == 0);
  Assert(c.limbs[1] == 1 + (1 << (sizeofbits(mx_limb_t) / 2)));


  nbr_clear(&a);
  nbr_clear(&b);
  nbr_clear(&c);
}

Test(integer_multiplication, signs)
{
  mx_int_t a, b, c;

  mx_limb_t max_digit = 0;
  max_digit = ~max_digit;

  nbr_limb_init(&a, 6);
  nbr_limb_init(&b, 7);
  nbr_init(&c);

  // 6 * 7
  nbr_mul(&c, &a, &b);
  Assert(c.size == 1);
  Assert(c.limbs[0] == 42);

  // -6 * -7
  a.size = -1;
  b.size = -1;
  nbr_mul(&c, &a, &b);
  Assert(c.size == 1);
  Assert(c.limbs[0] == 42);

  // -6 * 7
  b.size = 1;
  nbr_mul(&c, &a, &b);
  Assert(c.size == -1);
  Assert(c.limbs[0] == 42);

  // 6 * -7
  a.size = 1;
  b.size = -1;
  nbr_mul(&c, &a, &b);
  Assert(c.size == -1);
  Assert(c.limbs[0] == 42);

  nbr_clear(&a);
  nbr_clear(&b);
  nbr_clear(&c);
}

TestSuite(integer_multiplication, basics, signs);


Test(integer_division, basics)
{
  mx_int_t a, b, q, r;

  mx_limb_t max_digit = 0;
  max_digit = ~max_digit;

  nbr_raw_init(&a, 2, mx_malloc(2, NULL), 2);
  a.limbs[0] = 1;
  a.limbs[1] = 1;
  nbr_limb_init(&b, 2);
  nbr_init(&q);
  nbr_init(&r);

  // (1*b + 1) / 2 -> rem = 1, quo = b >> 1
  nbr_div(&q, &r, &a, &b);
  Assert(r.size == 1);
  Assert(r.limbs[0] == 1);
  Assert(q.size == 1);
  Assert(q.limbs[0] == (1 << (sizeofbits(mx_limb_t) - 1)));

  //
  nbr_ensure_alloc(&q, 2);
  nbr_ensure_alloc(&r, 3);
  q.size = 2;
  q.limbs[0] = 1;
  q.limbs[1] = 1;
  r.size = 3;
  r.limbs[0] = 2;
  r.limbs[1] = 2;
  r.limbs[2] = 2;
  nbr_mul(&a, &q, &r);
  nbr_limb_abs_incr(&a, 64);
  nbr_assign(&b, &r);
  nbr_div(&q, &r, &a, &b);
  Assert(r.size == 1);
  Assert(r.limbs[0] == 64);
  Assert(q.size == 2);
  Assert(q.limbs[0] == 1);
  Assert(q.limbs[1] == 1);

  nbr_clear(&a);
  nbr_clear(&b);
  nbr_clear(&q);
  nbr_clear(&r);
}

TestSuite(integer_division, basics);


Test(integer_utils, to_string)
{
  const mx_size_t buffer_size = 1024;
  char buffer[1024] = { 0 };

  mx_int_t a;
  nbr_init(&a);

  // 0
  mx_size_t written = nbr_print(buffer, buffer_size, &a);
  Assert(written == 1);
  Assert(buffer[0] == '0');

  // -128
  nbr_ensure_alloc(&a, 1);
  a.size = -1;
  a.limbs[0] = 128;
  written = nbr_print(buffer, buffer_size, &a);
  Assert(written == 4);
  Assert(strcmp(buffer, "-128") == 0);

  // 128*128 = 16384
  mx_int_t b;
  nbr_init(&b);
  nbr_mul(&b, &a, &a);
  nbr_print(buffer, buffer_size, &b);
  Assert(strcmp(buffer, "16384") == 0);

  // 16384*16384 = 268435456
  nbr_mul(&a, &b, &b);
  nbr_print(buffer, buffer_size, &a);
  Assert(strcmp(buffer, "268435456") == 0);

  // 268435456 * 268435456 = 72057594037927936
  nbr_mul(&b, &a, &a);
  nbr_print(buffer, buffer_size, &b);
  Assert(strcmp(buffer, "72057594037927936") == 0);

  // 72057594037927936 * 72057594037927936 = 5192296858534827628530496329220096
  nbr_mul(&a, &b, &b);
  written = nbr_print(buffer, buffer_size, &a);
  Assert(written == 34);
  Assert(strcmp(buffer, "5192296858534827628530496329220096") == 0);

  nbr_clear(&a);
}

TestSuite(integer_utils, to_string);


int main(int argc, char *argv[])
{
  init_test_framework();

  register_test(&integer_comparison);
  register_test(&integer_addition);
  register_test(&integer_subtraction);
  register_test(&integer_multiplication);
  register_test(&integer_division);
  register_test(&integer_utils);

  run_all_tests();

  return 0;
}