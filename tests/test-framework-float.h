
#ifndef LIBMATHXCORE_TEST_FRAMEWORK_FLOAT_H
#define LIBMATHXCORE_TEST_FRAMEWORK_FLOAT_H

#include "test-framework-int.h"

#include "mathx/core/float.h"

void printf_float(mx_float_t *x);
void printf_float_dec(const mx_float_t *x);

struct DecimalRepresentation
{
  const char *digits;
  mx_exp_t exp;
};

typedef struct DecimalRepresentation DecimalRepresentation;

int compare_to_decrepr(const mx_float_t *x, DecimalRepresentation decrepr);

#endif // LIBMATHXCORE_TEST_FRAMEWORK_FLOAT_H
