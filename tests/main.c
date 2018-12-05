
#include "test-framework.h"

#include "mathx/core/integer.h"
#include "mathx/core/memory.h"

#include <string.h>


Test(integer_comparison, same_size)
{
  mx_int_t a, b;

  int_limb_init(&a, 4);
  int_limb_init(&b, 4);

  int comp = int_comp(&a, &b);
  Assert(comp == 0);

  int_clear(&b);
  int_limb_init(&b, 6);
  comp = int_comp(&a, &b);
  Assert(comp == -1);

  a.size *= -1;
  b.size *= -1;

  comp = int_comp(&a, &b);
  Assert(comp == 1);
}

Test(integer_comparison, same_sign_different_size)
{
  mx_int_t a, b;

  int_limb_init(&a, 4);

  int_raw_init(&b, 2, mx_malloc_zero(2, NULL), 2);
  b.limbs[0] = 2;
  b.limbs[1] = 1;

  int comp = int_comp(&a, &b);
  Assert(comp == -1);

  a.size *= -1;
  b.size *= -1;
  comp = int_comp(&a, &b);
  Assert(comp == 1);

  a.limbs[0] = 0;
  a.size = 0;
  b.size *= -1;
  comp = int_comp(&a, &b);
  Assert(comp == -1);
}

Test(integer_comparison, different_signs)
{
  mx_int_t a, b;

  int_limb_init(&a, 4);
  int_limb_init(&b, 4);
  b.size *= -1;

  int comp = int_comp(&a, &b);
  Assert(comp == 1);

  a.size *= -1;
  b.size *= -1;
  comp = int_comp(&a, &b);
  Assert(comp == -1);
}

TestSuite(integer_comparison, same_size, same_sign_different_size, different_signs);


Test(integer_addition, carry_propagation_single_limb) 
{
  mx_int_t a, b, c;

  int_limb_init(&a, 1);
  int_limb_init(&b, ~((mx_limb_t) 0));
  int_init(&c);

  int_add(&c, &a, &b);

  Assert(c.size == 2);
  Assert(c.limbs[1] == 1);

  int_clear(&a);
  int_clear(&b);
  int_clear(&c);
}

Test(integer_addition, carry_propagation_two_limbs)
{
  mx_int_t a, b, c;

  mx_limb_t l = 0;
  l = ~l;

  int_limb_init(&a, 1);
  int_raw_init(&b, 2, mx_malloc_zero(2, NULL), 2);
  b.limbs[0] = l;
  b.limbs[1] = l;
  int_init(&c);

  int_add(&c, &b, &a);

  Assert(c.size == 3);
  Assert(c.limbs[0] == 0);
  Assert(c.limbs[1] == 0);
  Assert(c.limbs[2] == 1);

  int_clear(&a);
  int_clear(&b);
  int_clear(&c);
}

Test(integer_addition, add_zero)
{
  mx_int_t a, b, c;

  int_limb_init(&a, 4);
  int_init(&b);
  int_init(&c);

  int_add(&c, &a, &b);
  Assert(c.size == a.size);
  Assert(c.limbs[0] == a.limbs[0]);

  int_add(&c, &b, &a);
  Assert(c.size == a.size);
  Assert(c.limbs[0] == a.limbs[0]);

  int_clear(&a);
  int_clear(&b);
  int_clear(&c);
}

Test(integer_addition, opposite_signs)
{
  mx_int_t a, b, c;

  int_limb_init(&a, 4);
  int_limb_init(&b, 2);
  b.size *= -1;
  int_init(&c);

  int_add(&c, &b, &a);

  Assert(c.size == 1);
  Assert(c.limbs[0] == 2);

  int_clear(&b);
  
  int_limb_init(&b, 4);
  b.size *= -1;

  int_add(&c, &a, &b);

  Assert(c.size == 0);

  int_clear(&a);
  int_clear(&b);
  int_clear(&c);
}

TestSuite(integer_addition, carry_propagation_single_limb, carry_propagation_two_limbs, add_zero, opposite_signs);


Test(integer_subtraction, signs)
{
  mx_int_t a, b, c;

  int_limb_init(&a, 4);
  int_limb_init(&b, 0);
  int_init(&c);

  // 4 - 0 = 4
  int_sub(&c, &a, &b);
  Assert(int_comp(&a, &c) == 0);
 
  // 0 - 4 = -4
  int_sub(&c, &b, &a);
  Assert(c.size == -1);
  Assert(c.limbs[0] == 4);

  // 4 - 2 = 2
  int_clear(&b);
  int_limb_init(&b, 2);
  int_sub(&c, &a, &b);
  Assert(c.size == 1);
  Assert(c.limbs[0] == 2);

  // 2 - 4 = -2
  int_sub(&c, &b, &a);
  Assert(c.size == -1);
  Assert(c.limbs[0] == 2);

  // -4 - -2 = -(4-2) = -2
  a.size *= -1;
  b.size *= -1;
  int_sub(&c, &a, &b);
  Assert(c.size == -1);
  Assert(c.limbs[0] == 2);

  // -2 - -4 = 4 - 2 = 2
  int_sub(&c, &b, &a);
  Assert(c.size == 1);
  Assert(c.limbs[0] == 2);

  // -4 - -4 = 0
  int_sub(&c, &a, &a);
  Assert(c.size == 0);

  // -2 - 4 = -6
  b.size = -1;
  a.size = 1;
  int_sub(&c, &b, &a);
  Assert(c.size == -1);
  Assert(c.limbs[0] == 6);

  // 2 - -4 = 6
  b.size = 1;
  a.size = -1;
  int_sub(&c, &b, &a);
  Assert(c.size == 1);
  Assert(c.limbs[0] == 6);

  int_clear(&a);
  int_clear(&b);
  int_clear(&c);
}

Test(integer_subtraction, borrow)
{
  mx_int_t a, b, c;

  mx_limb_t max_digit = 0;
  max_digit = ~max_digit;


  // 1*b^2 - 1 = b*(b-1) + (b-1)
  int_raw_init(&a, 3, mx_malloc_zero(4, NULL), 4);
  a.limbs[2] = 1;
  int_limb_init(&b, 1);
  int_init(&c);
  int_sub(&c, &a, &b);
  Assert(c.size == 2);
  Assert(c.limbs[0] == max_digit);
  Assert(c.limbs[1] == max_digit);

  int_clear(&a);
  int_clear(&b);
  int_clear(&c);
}


Test(integer_subtraction, cancelation)
{
  mx_int_t a, b, c;

  mx_limb_t max_digit = 0;
  max_digit = ~max_digit;

  int_raw_init(&a, 3, mx_malloc_zero(4, NULL), 4);
  int_limb_init(&b, 1);
  int_init(&c);

  // (1*b^2 + 1*b + 2) - (1*b^2 + 1*b + 1)
  a.limbs[2] = 1;
  a.limbs[1] = 1;
  a.limbs[0] = 2;
  a.size = 3;
  int_clear(&b);
  int_raw_init(&b, 3, mx_malloc_zero(4, NULL), 4);
  b.limbs[2] = 1;
  b.limbs[1] = 1;
  b.limbs[0] = 1;
  int_sub(&c, &a, &b);
  Assert(c.size == 1);
  Assert(c.limbs[0] == 1);

  // (1*b^2 + 1*b + 1) - (1*b^2 + 1*b + 1)
  a.limbs[0] = 1;
  int_sub(&c, &a, &b);
  Assert(c.size == 0);

  // (1*b^3 + 1*b^2 + 2*b + 1) - (1*b^3 + 1*b^2 + 1*b + 1)
  a.limbs[3] = 1;
  a.limbs[1] = 2;
  a.size = 4;
  b.limbs[3] = 1;
  b.size = 4;
  int_sub(&c, &a, &b);
  Assert(c.size == 2);
  Assert(c.limbs[1] == 1);
  Assert(c.limbs[0] == 0);

  int_clear(&a);
  int_clear(&b);
  int_clear(&c);
}

TestSuite(integer_subtraction, signs, borrow, cancelation);


Test(integer_multiplication, basics)
{
  mx_int_t a, b, c;

  mx_limb_t max_digit = 0;
  max_digit = ~max_digit;

  int_limb_init(&a, 6);
  int_init(&b);
  int_init(&c);

  // 6 * 0
  int_mul(&c, &a, &b);
  Assert(c.size == 0);

  // 6 * (1*b)
  int_clear(&b);
  int_raw_init(&b, 2, mx_malloc_zero(2, NULL), 2);
  b.limbs[1] = 1;
  int_mul(&c, &a, &b);
  Assert(c.size == 2);
  Assert(c.limbs[0] == 0);
  Assert(c.limbs[1] == 6);

  // (1*b^2 + 1) * (1*b^3 + b)
  int_clear(&a);
  int_raw_init(&a, 3, mx_malloc_zero(3, NULL), 3);
  a.limbs[0] = 1;
  a.limbs[2] = 1;
  int_clear(&b);
  int_raw_init(&b, 4, mx_malloc_zero(4, NULL), 4);
  b.limbs[1] = 1;
  b.limbs[3] = 1;
  int_mul(&c, &a, &b);
  Assert(c.size == 6);
  Assert(c.limbs[0] == 0);
  Assert(c.limbs[1] == 1);
  Assert(c.limbs[2] == 0);
  Assert(c.limbs[3] == 2);
  Assert(c.limbs[4] == 0);
  Assert(c.limbs[5] == 1);

  // (b/2) * (b/2)
  int_clear(&a);
  int_limb_init(&a, 1 << (sizeofbits(mx_limb_t) / 2));
  int_clear(&b);
  int_limb_init(&b, 1 << (sizeofbits(mx_limb_t) / 2));
  int_mul(&c, &a, &b);
  Assert(c.size == 2);
  Assert(c.limbs[0] == 0);
  Assert(c.limbs[1] == 1);

  // (b/2) * (1*b + b/2)
  int_clear(&a);
  int_limb_init(&a, 1 << (sizeofbits(mx_limb_t) / 2));
  int_clear(&b);
  int_raw_init(&b, 2, mx_malloc_zero(2, NULL), 2);
  b.limbs[0] = 1 << (sizeofbits(mx_limb_t) / 2);
  b.limbs[1] = 1;
  int_mul(&c, &a, &b);
  Assert(c.size == 2);
  Assert(c.limbs[0] == 0);
  Assert(c.limbs[1] == 1 + (1 << (sizeofbits(mx_limb_t) / 2)));


  int_clear(&a);
  int_clear(&b);
  int_clear(&c);
}

Test(integer_multiplication, signs)
{
  mx_int_t a, b, c;

  mx_limb_t max_digit = 0;
  max_digit = ~max_digit;

  int_limb_init(&a, 6);
  int_limb_init(&b, 7);
  int_init(&c);

  // 6 * 7
  int_mul(&c, &a, &b);
  Assert(c.size == 1);
  Assert(c.limbs[0] == 42);

  // -6 * -7
  a.size = -1;
  b.size = -1;
  int_mul(&c, &a, &b);
  Assert(c.size == 1);
  Assert(c.limbs[0] == 42);

  // -6 * 7
  b.size = 1;
  int_mul(&c, &a, &b);
  Assert(c.size == -1);
  Assert(c.limbs[0] == 42);

  // 6 * -7
  a.size = 1;
  b.size = -1;
  int_mul(&c, &a, &b);
  Assert(c.size == -1);
  Assert(c.limbs[0] == 42);

  int_clear(&a);
  int_clear(&b);
  int_clear(&c);
}

TestSuite(integer_multiplication, basics, signs);


Test(integer_division, basics)
{
  mx_int_t a, b, q, r;

  mx_limb_t max_digit = 0;
  max_digit = ~max_digit;

  int_raw_init(&a, 2, mx_malloc(2, NULL), 2);
  a.limbs[0] = 1;
  a.limbs[1] = 1;
  int_limb_init(&b, 2);
  int_init(&q);
  int_init(&r);

  // (1*b + 1) / 2 -> rem = 1, quo = b >> 1
  int_div(&q, &r, &a, &b);
  Assert(r.size == 1);
  Assert(r.limbs[0] == 1);
  Assert(q.size == 1);
  Assert(q.limbs[0] == (1 << (sizeofbits(mx_limb_t) - 1)));

  //
  int_ensure_alloc(&q, 2);
  int_ensure_alloc(&r, 3);
  q.size = 2;
  q.limbs[0] = 1;
  q.limbs[1] = 1;
  r.size = 3;
  r.limbs[0] = 2;
  r.limbs[1] = 2;
  r.limbs[2] = 2;
  int_mul(&a, &q, &r);
  int_limb_abs_incr(&a, 64);
  int_assign(&b, &r);
  int_div(&q, &r, &a, &b);
  Assert(r.size == 1);
  Assert(r.limbs[0] == 64);
  Assert(q.size == 2);
  Assert(q.limbs[0] == 1);
  Assert(q.limbs[1] == 1);

  int_clear(&a);
  int_clear(&b);
  int_clear(&q);
  int_clear(&r);
}

Test(integer_division, signs)
{
  mx_int_t a, b, q, r;

  int_limb_init(&a, 7);
  int_limb_init(&b, 3);
  int_init(&q);
  int_init(&r);

  // 7 = 3 * 2 + 1
  int_div(&q, &r, &a, &b);
  Assert(r.size == 1);
  Assert(r.limbs[0] == 1);
  Assert(q.size == 1);
  Assert(q.limbs[0] == 2);

  // 7 = -3 * -2 + 1
  b.size = -1;
  int_div(&q, &r, &a, &b);
  Assert(r.size == 1);
  Assert(r.limbs[0] == 1);
  Assert(q.size == -1);
  Assert(q.limbs[0] == 2);

  // -7 = 3 * -3 + 2
  a.size = -1;
  b.size = 1;
  int_div(&q, &r, &a, &b);
  Assert(r.size == 1);
  Assert(r.limbs[0] == 2);
  Assert(q.size == -1);
  Assert(q.limbs[0] == 3);

  // -7 = -3 * 3 + 2
  a.size = -1;
  b.size = -1;
  int_div(&q, &r, &a, &b);
  Assert(r.size == 1);
  Assert(r.limbs[0] == 2);
  Assert(q.size == 1);
  Assert(q.limbs[0] == 3);

  int_clear(&a);
  int_clear(&b);
  int_clear(&q);
  int_clear(&r);
}

TestSuite(integer_division, basics, signs);


Test(integer_utils, to_string)
{
  const mx_size_t buffer_size = 1024;
  char buffer[1024] = { 0 };

  mx_int_t a;
  int_init(&a);

  // 0
  mx_size_t written = int_print(buffer, buffer_size, &a);
  Assert(written == 1);
  Assert(buffer[0] == '0');

  // -128
  int_ensure_alloc(&a, 1);
  a.size = -1;
  a.limbs[0] = 128;
  written = int_print(buffer, buffer_size, &a);
  Assert(written == 4);
  Assert(strcmp(buffer, "-128") == 0);

  // 128*128 = 16384
  mx_int_t b;
  int_init(&b);
  int_mul(&b, &a, &a);
  int_print(buffer, buffer_size, &b);
  Assert(strcmp(buffer, "16384") == 0);

  // 16384*16384 = 268435456
  int_mul(&a, &b, &b);
  int_print(buffer, buffer_size, &a);
  Assert(strcmp(buffer, "268435456") == 0);

  // 268435456 * 268435456 = 72057594037927936
  int_mul(&b, &a, &a);
  int_print(buffer, buffer_size, &b);
  Assert(strcmp(buffer, "72057594037927936") == 0);

  // 72057594037927936 * 72057594037927936 = 5192296858534827628530496329220096
  int_mul(&a, &b, &b);
  written = int_print(buffer, buffer_size, &a);
  Assert(written == 34);
  Assert(strcmp(buffer, "5192296858534827628530496329220096") == 0);

  int_clear(&a);
}

Test(integer_utils, from_string)
{
  const mx_size_t buffer_size = 1024;
  char buffer[1024] = { 0 };

  mx_int_t a;
  int_string_init(&a, "10");
  int_print(buffer, buffer_size, &a);
  Assert(strcmp(buffer, "10") == 0);
  int_clear(&a);

  int_string_init(&a, "123456789");
  int_print(buffer, buffer_size, &a);
  Assert(strcmp(buffer, "123456789") == 0);
  int_clear(&a);

  int_string_init(&a, "+15248675986452315264857956842153624578568");
  int_print(buffer, buffer_size, &a);
  Assert(strcmp(buffer, "15248675986452315264857956842153624578568") == 0);
  int_clear(&a);

  int_string_init(&a, "-1235648575472136548045");
  int_print(buffer, buffer_size, &a);
  Assert(a.size < 0);
  Assert(strcmp(buffer, "-1235648575472136548045") == 0);
  int_clear(&a);
}

TestSuite(integer_utils, to_string, from_string);



int eq(const mx_int_t *x, const char *str)
{
  const mx_size_t buffer_size = 1024;
  char buffer[1024] = { 0 };
  int_print(buffer, buffer_size, x);
  return strcmp(buffer, str) == 0;
}

Test(integer_functions, sign)
{
  mx_int_t a;
  mx_int_t b;
  mx_int_t c;

  int_limb_init(&a, 42);
  int_limb_init(&b, 65);
  int_init(&c);

  Assert(int_is_odd(&a) == 0);
  Assert(int_is_even(&a) == 1);
  Assert(int_is_odd(&b) == 1);
  Assert(int_is_even(&b) == 0);

  int_negate(&c, &a);
  Assert(int_sign(&c) == -1);
  int_abs(&b, &c);
  Assert(int_comp(&b, &a) == 0);
}

#include "mathx/core/factorial.h"

Test(integer_functions, factorial)
{
  const mx_size_t buffer_size = 1024;
  char buffer[1024] = { 0 };

  mx_int_t a;
  mx_int_t b;

  int_init(&a);
  int_limb_init(&b, 1);
  b.size = -1;

  // negative number -> returns 0
  int_factorial(&a, &b);
  int_print(buffer, buffer_size, &a);
  Assert(strcmp(buffer, "0") == 0);

  // 0! = 1
  int_limb_assign(&b, 0);
  int_factorial(&a, &b);
  int_print(buffer, buffer_size, &a);
  Assert(strcmp(buffer, "1") == 0);

  // 1! = 1
  int_limb_assign(&b, 1);
  int_factorial(&a, &b);
  int_print(buffer, buffer_size, &a);
  Assert(strcmp(buffer, "1") == 0);

  // 2! = 2
  int_limb_assign(&b, 2);
  int_factorial(&a, &b);
  int_print(buffer, buffer_size, &a);
  Assert(strcmp(buffer, "2") == 0);

  // 3! = 6
  int_limb_assign(&b, 3);
  int_factorial(&a, &b);
  int_print(buffer, buffer_size, &a);
  Assert(strcmp(buffer, "6") == 0);

  // 30! = 265252859812191058636308480000000
  int_limb_assign(&b, 30);
  int_factorial(&a, &b);
  int_print(buffer, buffer_size, &a);
  Assert(strcmp(buffer, "265252859812191058636308480000000") == 0);
}

#include "mathx/core/gcd.h"

Test(integer_functions, gcd)
{
  const mx_size_t buffer_size = 1024;
  char buffer[1024] = { 0 };

  mx_int_t a;
  mx_int_t b;
  mx_int_t gcd;

  int_limb_init(&a, 2);
  int_limb_init(&b, 1);
  int_init(&gcd);

  // gcd(2, 1) = 1
  int_gcd(&gcd, &a, &b);
  int_print(buffer, buffer_size, &gcd);
  Assert(strcmp(buffer, "1") == 0);

  // gcd(10, 5) = 5
  int_limb_assign(&a, 10);
  int_limb_assign(&b, 5);
  int_gcd(&gcd, &a, &b);
  int_print(buffer, buffer_size, &gcd);
  Assert(strcmp(buffer, "5") == 0);

  // gcd(9, 7) = 1
  int_limb_assign(&a, 9);
  int_limb_assign(&b, 7);
  int_gcd(&gcd, &a, &b);
  int_print(buffer, buffer_size, &gcd);
  Assert(strcmp(buffer, "1") == 0);

  // gcd(12, 28) = 4
  int_limb_assign(&a, 12);
  int_limb_assign(&b, 28);
  int_gcd(&gcd, &a, &b);
  int_print(buffer, buffer_size, &gcd);
  Assert(strcmp(buffer, "4") == 0);
}

#include "mathx/core/bezout.h"

Test(integer_functions, bezout)
{
  mx_int_t a;
  mx_int_t b;
  mx_int_t u;
  mx_int_t v;
  mx_int_t gcd;

  int_limb_init(&a, 2);
  int_limb_init(&b, 1);
  int_init(&u);
  int_init(&v);
  int_init(&gcd);

  // gcd(2, 1) = 1 = 0 * 2 + 1 * 1
  int_gcd_bezout(&gcd, &a, &b, &u, &v);
  Assert(eq(&u, "0"));
  Assert(eq(&v, "1"));

  // gcd(12, 28) = 4 = (-2) * 12 + 1 * 28
  int_limb_assign(&a, 12);
  int_limb_assign(&b, 28);
  int_gcd_bezout(&gcd, &a, &b, &u, &v);
  Assert(eq(&u, "-2"));
  Assert(eq(&v, "1"));

  // gcd(-12, 28) = 4 = 2 * (-12) + 1*28
  int_limb_assign(&a, 12);
  a.size = -1;
  int_limb_assign(&b, 28);
  int_gcd_bezout(&gcd, &a, &b, &u, &v);
  Assert(eq(&u, "2"));
  Assert(eq(&v, "1"));
}

#include "mathx/core/isqrt.h"

Test(integer_functions, isqrt)
{
  mx_int_t a;
  mx_int_t b;

  int_limb_init(&a, 0);
  int_init(&b);

  // isqrt(0) = 0
  int_isqrt(&b, &a);
  Assert(eq(&b, "0"));

  // isqrt(44) = 6
  int_limb_assign(&a, 44);
  int_isqrt(&b, &a);
  Assert(eq(&b, "6"));

  // isqrt(144) = 12
  int_limb_assign(&a, 144);
  int_isqrt(&b, &a);
  Assert(eq(&b, "12"));
}

Test(integer_functions, pow)
{
  mx_int_t a;
  mx_int_t b;
  mx_int_t c;

  int_limb_init(&a, 2);
  int_limb_init(&b, 32);
  int_init(&c);

  // 2^32
  int_pow(&c, &a, &b);
  Assert(eq(&c, "4294967296"));
}

Test(integer_functions, modpow)
{
  mx_int_t a;
  mx_int_t b;
  mx_int_t c;
  mx_int_t d;

  int_limb_init(&a, 2);
  int_limb_init(&b, 32);
  int_limb_init(&c, 3);
  int_init(&d);

  // 2^32 % 3 == 1
  int_modpow(&d, &a, &b, &c);
  Assert(eq(&d, "1"));
}

TestSuite(integer_functions, sign, factorial, gcd, bezout, isqrt, pow, modpow);


extern void register_rationals_test_suite();
extern void register_floats_test_suite();

int main(int argc, char *argv[])
{
  init_test_framework();

  register_test(&integer_comparison);
  register_test(&integer_addition);
  register_test(&integer_subtraction);
  register_test(&integer_multiplication);
  register_test(&integer_division);
  register_test(&integer_utils);
  register_test(&integer_functions);

  register_rationals_test_suite();
  register_floats_test_suite();

  run_all_tests();

  return 0;
}