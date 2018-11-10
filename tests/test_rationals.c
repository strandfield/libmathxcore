
#include "test-framework.h"

#include "mathx/core/rational.h"

#include <string.h>

int rat_str_eq(const mx_rat_t *x, const char *str)
{
  const mx_size_t buffer_size = 1024;
  char buffer[1024] = { 0 };
  rat_print(buffer, buffer_size, x);
  return strcmp(buffer, str) == 0;
}

Test(Rationals, basics)
{
  mx_rat_t a;
  mx_rat_t b;
  mx_rat_t c;

  rat_limb_init(&a, 1);
  rat_limb_init(&b, 2);

  rat_init(&c);

  /// 1/2
  rat_div(&c, &a, &b);
  Assert(rat_str_eq(&c, "1/2"));

  /// 1/2 + 1/2 == 1
  rat_add(&a, &c, &c);
  Assert(rat_str_eq(&a, "1"));

  /// 1/2 * 1/2 == 1/4
  rat_mul(&a, &c, &c);
  Assert(rat_str_eq(&a, "1/4"));
  Assert(rat_sign(&a) == 1);

  /// 1/4 < 1/2
  Assert(rat_comp(&a, &c) == -1);

  /// 1/4 - 1/2 == -1/4
  rat_sub(&b, &a, &c);
  Assert(rat_str_eq(&b, "-1/4"));
  Assert(rat_sign(&b) == -1);

  /// -1/4 < 1/2
  Assert(rat_comp(&b, &c) == -1);

  /// abs(-1/4) == 1/4
  rat_abs(&c, &b);
  Assert(rat_str_eq(&c, "1/4"));

  /// neg(-1/4) == 1/4
  rat_negate(&c, &b);
  Assert(rat_str_eq(&c, "1/4"));

  /// is_zero(1/4)
  Assert(rat_is_zero(&c) == 0);

  /// 1/4 - 1/4 == 0
  /// TODO:fix: the following fails because of division by zero
  //rat_sub(&a, &c, &c);
  //Assert(rat_str_eq(&a, "0"));
  //Assert(rat_is_zero(&a));

  rat_clear(&a);
  rat_clear(&b);
  rat_clear(&c);
}

TestSuite(Rationals, basics);

void register_rationals_test_suite()
{
  register_test(&Rationals);
}
