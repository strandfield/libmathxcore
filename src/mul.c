// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mathx/core/mul.h"

#include "mathx/core/memory.h"
#include "mathx/core/umul.h"

#include <stdlib.h>
#include <string.h>

/*@
 * \fn void int_mul(mx_int_t *product, const mx_int_t *factor1, const mx_int_t *factor2)
 * \brief Multiplies two integers.
 * \param variable that will receive the product
 * \param first factor
 * \param second factor
 *
 * Informally, performs \c{product = factor1 * factor2}.
 */
void int_mul(mx_int_t *product, const mx_int_t *factor1, const mx_int_t *factor2)
{
  if (factor1->size == 0 || factor2->size == 0)
  {
    int_assign_zero(product);
  }
  else
  {
    int_ensure_alloc_zero(product, abs(factor1->size) + abs(factor2->size));
    product->size = uint_mul(factor1->limbs, abs(factor1->size), factor2->limbs, abs(factor2->size), product->limbs);
    product->size *= int_sign(factor1) * int_sign(factor2);
  }
}
