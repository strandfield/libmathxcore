
#include "test-framework-float.h"

#include "mathx/core/float/fprint.h"
#include "mathx/core/print.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printf_float(mx_float_t *x)
{
  const mx_size_t buffer_size = 1024;
  char buffer[1024] = { 0 };
  float_raw_print(buffer, buffer_size, x);
  printf("%s\n", buffer);
}

void printf_float_dec(const mx_float_t *x)
{
  const mx_size_t buffer_size = 1024;
  char buffer[1024] = { 0 };
  float_sprint(buffer, buffer_size, x);
  printf("%s\n", buffer);
}

int compare_to_decrepr(const mx_float_t *x, DecimalRepresentation decrepr)
{
  mx_int_t digits;
  int_init(&digits);

  const mx_exp_t n = float_dec_digits(&digits, x);
 
  if ((digits.size < 0 && decrepr.digits[0] != '-') || (decrepr.digits[0] == '-' && digits.size >= 0))
  {
    printf("Error while comparing with \n%s\n", decrepr.digits);
    printf("Signs do not match\n");
    return 0;
  }
  else
  {
    decrepr.digits += (decrepr.digits[0] == '-');
    digits.size = digits.size < 0 ? -digits.size : digits.size;
  }

  mx_size_t s = uint_print_size(digits.limbs, digits.size) + 2;
  char *buffer = (char*) malloc(s);
  mx_size_t written = int_print(buffer, s, &digits);
  int_clear(&digits);

  const int sizediff = strlen(decrepr.digits) - written;

  if (decrepr.exp + sizediff != n)
  {
    printf("Error while comparing with \n%s\n", decrepr.digits);
    printf("Exponent do not match: got %d instead of %d\n", n, decrepr.exp + sizediff);
    return 0;
  }

  for (mx_size_t i = 0; i < written - 1; ++i)
  {
    if (decrepr.digits[i] != buffer[i])
    {
      printf("Error while comparing with \n%s\n", decrepr.digits);
      printf("got: \n%s\n", buffer);
      printf("digits number %d differ\n", i);
      return 0;
    }
  }

  free(buffer);

  return 1;
}