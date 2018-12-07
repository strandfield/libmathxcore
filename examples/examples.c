
#include "mathx/core/integer.h"
#include "mathx/core/float.h"

#include "mathx/core/float/pi.h"

#include <stdio.h>
#include <stdlib.h>

void print_pi()
{
  printf("==========================================\n");
  printf("======= Print pi\n");
  printf("==========================================\n");

  mx_float_t pi;

  // We want to compute pi on 128 bytes.
  float_init_prec(&pi, 128 / sizeof(mx_limb_t)); 
  
  float_assign_pi(&pi);

  float_print(&pi);

  float_clear(&pi);
}

int main()
{
  print_pi();

  return 0;
}
