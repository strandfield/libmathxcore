// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_DEFS_H
#define LIBMATHXCORE_DEFS_H

#if (defined(WIN32) || defined(_WIN32)) && !defined(LIBMATHXCORE_STATIC_LINKING)
#if defined(LIBMATHXCORE_BUILD_LIB)
#  define LIBMATHXCORE_API __declspec(dllexport)
#else
#  define LIBMATHXCORE_API __declspec(dllimport)
#endif
#else
#define LIBMATHXCORE_API
#endif


#include <stdint.h>

typedef int mx_ssize_t;
typedef size_t mx_size_t;

#if LIBMATHXCORE_LIMB_SIZE == 8

typedef uint8_t mx_limb_t;
#define LIBMATHXCORE_HAS_LONGLIMB_TYPE
typedef uint16_t mx_longlimb_t;

#elif LIBMATHXCORE_LIMB_SIZE == 16

typedef uint16_t mx_limb_t;
#define LIBMATHXCORE_HAS_LONGLIMB_TYPE
typedef uint32_t mx_longlimb_t;

#elif LIBMATHXCORE_LIMB_SIZE == 32

typedef uint32_t mx_limb_t;
#define LIBMATHXCORE_HAS_LONGLIMB_TYPE
typedef uint64_t mx_longlimb_t;

#elif LIBMATHXCORE_LIMB_SIZE == 64

typedef uint64_t mx_limb_t;

#else
# error "libmathxcore : no limb size defined"
#endif

#define sizeofbits(x) (8 * sizeof(x))

#endif // LIBMATHXCORE_DEFS_H
