
#ifndef LIBMATHXCORE_EUCLIDE_H
#define LIBMATHXCORE_EUCLIDE_H

#include "mathx/core/integer.h"

LIBMATHXCORE_API void int_euclidean_algorithm(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b);
LIBMATHXCORE_API void int_extended_euclidean_algorithm(mx_int_t *gcd, const mx_int_t *a, const mx_int_t *b, mx_int_t *u, mx_int_t *v);


#endif // LIBMATHXCORE_EUCLIDE_H
