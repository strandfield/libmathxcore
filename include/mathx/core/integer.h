
#ifndef LIBMATHXCORE_INTEGER_H
#define LIBMATHXCORE_INTEGER_H

#include "mathx/core/defs.h"

struct LIBMATHXCORE_API mx_integer_struct
{
  mx_ssize_t size;
  mx_limb_t *limbs;
  mx_size_t alloc;
};

typedef struct mx_integer_struct mx_int_t;

#endif // LIBMATHXCORE_INTEGER_H
