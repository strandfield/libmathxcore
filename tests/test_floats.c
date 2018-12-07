
#include "test-framework.h"
#include "test-framework-float.h"

#include "mathx/core/float.h"
#include "mathx/core/integer.h"

#include <stdio.h>
#include <string.h>


mx_float_t new_float(const char *str, mx_exp_t exp)
{
  mx_int_t a;
  int_string_init(&a, str);

  mx_float_t ret;
  float_raw_init(&ret, a.size, a.limbs, a.alloc, exp, float_default_prec());
  return ret;
}

Test(Floats, add)
{
  mx_float_t a;
  mx_float_t b;

  float_init(&a);
  float_init(&b);

  float_ensure_alloc(&a, 4);
  float_ensure_alloc(&b, 4);

  for (int i = 0; i < 4; ++i)
  {
    a.limbs[i] = 1;
    b.limbs[i] = 1;
  }

  a.size = 4;
  b.size = 4;

  // aaaa
  //  bbbb
  b.exp = -1;

  mx_float_t c;
  float_init_prec(&c, 6);

  float_add(&c, &b, &a);
  
  Assert(c.exp == b.exp);
  Assert(c.size == 5);

  Assert(c.limbs[0] == 1);
  for (int i = 0; i < 3; ++i)
  {
    Assert(c.limbs[1 + i] == 2);
  }
  Assert(c.limbs[c.size - 1] == 1);

  // aaaa
  //      bbbb
  c.prec = 9;
  b.exp = -5;
  float_add(&c, &a, &b);

  Assert(c.exp == b.exp);
  Assert(c.size == 9);

  for (int i = 0; i < 4; ++i)
  {
    Assert(c.limbs[i] == 1);
  }
  Assert(c.limbs[4] == 0);
  for (int i = 5; i < 9; ++i)
  {
    Assert(c.limbs[i] == 1);
  }

  // aaaa
  //      bbbb
  // rrrrrrr
  c.prec = 7;

  float_add(&c, &b, &a);

  Assert(c.exp == b.exp + 2);
  Assert(c.size == 7);

  for (int i = 0; i < 2; ++i)
  {
    Assert(c.limbs[i] == 1);
  }
  Assert(c.limbs[2] == 0);
  for (int i = 3; i < 7; ++i)
  {
    Assert(c.limbs[i] == 1);
  }

  // aaaa
  //      bbbb
  // rrrrr
  c.prec = 5;

  float_add(&c, &a, &b);
  // b is completely ignored, 
  // rand the result is simply a
  Assert(float_comp(&a, &c) == 0);

  // aaaa
  //   b
  b.size = 1;
  b.exp = 1;

  float_add(&c, &a, &b);

  Assert(c.exp == a.exp);
  Assert(c.size == 4);

  Assert(c.limbs[0] == 1);
  Assert(c.limbs[1] == 2);
  Assert(c.limbs[2] == 1);
  Assert(c.limbs[3] == 1);

  // aaaa
  //   b
  // rrr
  c.prec = 3;

  float_add(&c, &a, &b);

  Assert(c.exp == a.exp + 1);
  Assert(c.size == 3);

  Assert(c.limbs[0] == 2);
  Assert(c.limbs[1] == 1);
  Assert(c.limbs[2] == 1);

  // Cleanup
  float_clear(&a);
  float_clear(&b);
  float_clear(&c);
}


Test(Floats, sub)
{
  const mx_limb_t maxdigit = ~((mx_limb_t)0);

  mx_float_t a;
  mx_float_t b;

  float_init(&a);
  float_init(&b);

  float_ensure_alloc_zero(&a, 8);
  float_ensure_alloc_zero(&b, 8);

  //    2 . .
  // -  . . 1
  // --------
  //    1 f f
  a.limbs[0] = 2;
  a.size = 1;
  a.exp = 2;
  b.limbs[0] = 1;
  b.size = 1;

  mx_float_t c;
  float_init_prec(&c, 4);

  float_sub(&c, &a, &b);

  Assert(c.exp == b.exp);
  Assert(c.size == 3);

  Assert(c.limbs[0] == maxdigit);
  Assert(c.limbs[1] == maxdigit);
  Assert(c.limbs[2] == 1);

  //    3 . . .
  // -  2 f f f
  // ----------
  //    0 0 0 1
  a.limbs[0] = 3;
  a.size = 1;
  a.exp = 3;
  b.limbs[0] = maxdigit;
  b.limbs[1] = maxdigit;
  b.limbs[2] = maxdigit;
  b.limbs[3] = 2;
  b.size = 4;

  float_sub(&c, &a, &b);

  Assert(c.exp == b.exp);
  Assert(c.size == 1);

  Assert(c.limbs[0] == 1);

  //    3 0 . .
  // -  2 f f f
  // ----------
  //    0 0 0 1 
  a.limbs[0] = 0;
  a.limbs[1] = 3;
  a.size = 2;
  a.exp = 2;
  b.limbs[0] = maxdigit;
  b.limbs[1] = maxdigit;
  b.limbs[2] = maxdigit;
  b.limbs[3] = 2;
  b.size = 4;

  float_sub(&c, &a, &b);

  Assert(c.exp == b.exp);
  Assert(c.size == 1);

  Assert(c.limbs[0] == 1);

  //    1 0 0 . .
  // -  0 f f 1 1
  //   -----------
  //    0 0 0 e f
  a.limbs[0] = 0;
  a.limbs[1] = 0;
  a.limbs[2] = 1;
  a.size = 3;
  a.exp = 1;
  b.limbs[0] = 1;
  b.limbs[1] = 1;
  b.limbs[2] = maxdigit;
  b.limbs[3] = maxdigit;
  b.size = 4;
  b.exp = -1;

  float_sub(&c, &a, &b);

  Assert(c.exp == b.exp);
  Assert(c.size == 2);

  Assert(c.limbs[0] == maxdigit);
  Assert(c.limbs[1] == maxdigit - 1);

  //    1 0 0 1 1 1 1
  // -    f f 1 1 . .
  //   --------------
  //    0 0 1 0 0 1 1
  set_limbs_value_7(a.limbs, 1, 1, 1, 1, 0, 0, 1);
  a.size = 7;
  a.exp = -1;
  set_limbs_value_4(b.limbs, 1, 1, maxdigit, maxdigit);
  b.size = 4;
  b.exp = a.exp + 2;

  c.prec = 5;
  float_sub(&c, &a, &b);

  Assert(c.exp == a.exp);
  Assert(c.size == 5);

  Assert(c.limbs[0] == 1);
  Assert(c.limbs[1] == 1);
  Assert(c.limbs[2] == 0);
  Assert(c.limbs[3] == 0);
  Assert(c.limbs[4] == 1);

  //    1 0 0 1 1 . .
  // -    f f 1 1 1 1
  //   --------------
  //    0 0 0 f f e f
  set_limbs_value_5(a.limbs, 1, 1, 0, 0, 1);
  a.size = 5;
  a.exp = -1;
  set_limbs_value_6(b.limbs, 1, 1, 1, 1, maxdigit, maxdigit);
  b.size = 6;
  b.exp = a.exp - 2;

  c.prec = 5;
  float_sub(&c, &a, &b);

  Assert(c.exp == b.exp);
  Assert(c.size == 4);

  Assert(c.limbs[0] == maxdigit);
  Assert(c.limbs[1] == maxdigit - 1);
  Assert(c.limbs[2] == maxdigit);
  Assert(c.limbs[3] == maxdigit);

  //    1 1 2 1 . .
  // -      1 1 1 1
  //   ------------
  //    1 1 0 f e f
  set_limbs_value_4(a.limbs, 1, 2, 1, 1);
  a.size = 4;
  a.exp = -2;
  set_limbs_value_4(b.limbs, 1, 1, 1, 1);
  b.size = 4;
  b.exp = a.exp - 2;

  c.prec = 6;
  float_sub(&c, &a, &b);

  Assert(c.exp == b.exp);
  Assert(c.size == 6);

  Assert(c.limbs[0] == maxdigit);
  Assert(c.limbs[1] == maxdigit - 1);
  Assert(c.limbs[2] == maxdigit);
  Assert(c.limbs[3] == 0);
  Assert(c.limbs[4] == 1);
  Assert(c.limbs[5] == 1);

  //    1 1 2 1 0 0 0
  // -      1 f . . .
  //   --------------
  //    1 1 0 2 0 0 0
  set_limbs_value_7(a.limbs, 0, 0, 0, 1, 2, 1, 1);
  a.size = 7;
  a.exp = -2;
  set_limbs_value_2(b.limbs, maxdigit, 1);
  b.size = 2;
  b.exp = a.exp + 3;

  c.prec = 6;
  Assert(c.prec < a.size);

  float_sub(&c, &a, &b);

  Assert(c.exp == a.exp + (a.size - c.prec));
  Assert(c.size == 6);

  Assert(c.limbs[0] == 0);
  Assert(c.limbs[1] == 0);
  Assert(c.limbs[2] == 2);
  Assert(c.limbs[3] == 0);
  Assert(c.limbs[4] == 1);
  Assert(c.limbs[5] == 1);

  //    3 1 1 . . . .
  // -  1 0 1 1 0 1 1
  //   --------------
  //    2 0 f e f e f

  //    x x x x x x
  //    3 1 1 . . .
  // -  1 0 1 1 0 1
  //   ------------
  //    2 0 f e f f
  set_limbs_value_3(a.limbs, 1, 1, 3);
  a.size = 3;
  a.exp = 2;
  set_limbs_value_7(b.limbs, 1, 1, 0, 1, 1, 0, 1);
  b.size = 7;
  b.exp = a.exp - 4;

  c.prec = 6;
  Assert(c.prec < b.size);

  float_sub(&c, &a, &b);

  Assert(c.exp == b.exp + (b.size - c.prec));
  Assert(c.size == 6);

  Assert(c.limbs[0] == maxdigit);
  Assert(c.limbs[1] == maxdigit);
  Assert(c.limbs[2] == maxdigit - 1);
  Assert(c.limbs[3] == maxdigit);
  Assert(c.limbs[4] == 0);
  Assert(c.limbs[5] == 2);

  // Cleanup
  float_clear(&a);
  float_clear(&b);
  float_clear(&c);
}


Test(Floats, mul)
{
  mx_float_t a;
  mx_float_t b;

  float_init(&a);
  float_init(&b);

  float_ensure_alloc(&a, 4);
  float_ensure_alloc(&b, 4);

  for (int i = 0; i < 4; ++i)
  {
    a.limbs[i] = 1;
    b.limbs[i] = 1;
  }

  a.size = 4;
  b.size = 4;

  // aaaa
  //   bbbb
  a.exp = 1;
  b.exp = -1;

  mx_float_t c;
  float_init_prec(&c, 8);

  float_mul(&c, &a, &b);

  Assert(c.exp == 0);
  Assert(c.size == 7);
  Assert(c.limbs[0] == 1);
  Assert(c.limbs[1] == 2);
  Assert(c.limbs[2] == 3);
  Assert(c.limbs[3] == 4);
  Assert(c.limbs[4] == 3);
  Assert(c.limbs[5] == 2);
  Assert(c.limbs[6] == 1);

  // aaaa
  //   bbbb
  c.prec = 5;

  float_mul(&c, &a, &b);

  Assert(c.exp == 2);
  Assert(c.size == 5);
  Assert(c.limbs[0] == 3);
  Assert(c.limbs[1] == 4);
  Assert(c.limbs[2] == 3);
  Assert(c.limbs[3] == 2);
  Assert(c.limbs[4] == 1);

  // aaaa
  //   bbbb
  c.prec = 2;

  float_mul(&c, &a, &b);

  Assert(c.exp == 5);
  Assert(c.size == 2);
  Assert(c.limbs[0] == 2);
  Assert(c.limbs[1] == 1);

  // aaaa
  //    0
  c.prec = 2;
  b.size = 0;

  float_mul(&c, &a, &b);

  Assert(c.size == 0);

  // Cleanup
  float_clear(&a);
  float_clear(&b);
  float_clear(&c);
}

Test(Floats, test1)
{
  mx_float_t a = new_float("123456789", -1);
  mx_float_t b = new_float("9876543210", -3);

  mx_float_t c;
  float_init(&c);

  printf_float(&a);
  printf_float(&b);

  float_mul(&c, &a, &b);

  printf_float(&c);

  float_clear(&a);
  float_clear(&b);
  float_clear(&c);
}


Test(Floats, test2)
{
  mx_float_t a;
  float_init(&a);
  a.exp = -1 * (float_default_prec() / 2);
  float_ensure_alloc(&a, float_default_prec());
  for (mx_size_t i = 0; i < float_default_prec(); ++i)
    a.limbs[i] = 1;
  a.size = float_default_prec();

  mx_float_t c;
  float_init(&c);

  printf_float(&a);
  printf_float(&a);

  float_mul(&c, &a, &a);

  printf_float(&c);

  float_clear(&a);
  float_clear(&c);
}


Test(Floats, print1)
{
  mx_float_t a;
  float_init(&a);
  float_assign_limb(&a, 66);
  a.exp = -2;

  const mx_size_t buffer_size = 1024;
  char buffer[1024] = { 0 };
  float_sprint(buffer, buffer_size, &a);
  printf("%s\n", buffer);

  float_clear(&a);
}

Test(Floats, print2)
{
  mx_float_t a;
  float_init(&a);
  float_assign_limb(&a, 107);
  a.exp = 1;

  const mx_size_t buffer_size = 1024;
  char buffer[1024] = { 0 };
  float_sprint(buffer, buffer_size, &a);
  printf("%s\n", buffer);

  float_clear(&a);
}


TestSuite(Floats, add, sub, mul, print1, print2);


void register_floats_test_suite()
{
  register_test(&Floats);
}
