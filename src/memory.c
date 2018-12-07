// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mathx/core/memory.h"

#include <assert.h>
#include <string.h> // memset
#include <stdlib.h> // malloc, free

/*@
 * \fn mx_limb_t* mx_malloc(mx_size_t size, mx_size_t *alloc)
 * \brief Allocates limbs.
 * \param The number of limbs to allocate
 * \param Possibly-null memory location to write the actual number of limbs allocated.
 *
 * The actual number of limbs allocated can be bigger than \c size, but is never smaller.
 */
mx_limb_t* mx_malloc(mx_size_t size, mx_size_t *alloc)
{
  if (alloc)
    *alloc = size;

  return (mx_limb_t*) malloc(size * sizeof(mx_limb_t));
}

/*@
 * \fn mx_limb_t* mx_malloc_zero(mx_size_t size, mx_size_t *alloc)
 * \brief Allocates and initializes limbs.
 * \param The number of limbs to allocate
 * \param Possibly-null memory location to write the actual number of limbs allocated.
 *
 * Much like \m mx_malloc, this function allocates limbs, but it also zero-initializes them.
 */
mx_limb_t* mx_malloc_zero(mx_size_t size, mx_size_t *alloc)
{
  if (alloc)
    *alloc = size;

  mx_limb_t *ret = (mx_limb_t*) malloc(size * sizeof(mx_limb_t));
  memset(ret, 0, size * sizeof(mx_limb_t));
  return ret;
}

/*@
 * \fn void mx_free(mx_limb_t *mem)
 * \brief Releases memory.
 * \param Pointer to the first limb of a dynamically allocated block.
 *
 * This function frees memory allocated with either \m mx_malloc or 
 * \m mx_malloc_zero.
 */
void mx_free(mx_limb_t *mem)
{
  free((void*)mem);
}
