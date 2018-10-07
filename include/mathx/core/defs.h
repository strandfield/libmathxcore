
#ifndef LIBMATHXCORE_DEFS_H
#define LIBMATHXCORE_DEFS_H

#if defined(LIBMATHXCORE_BUILD_LIB)
#  define LIBMATHXCORE_API __declspec(dllexport)
#else
#  define LIBMATHXCORE_API __declspec(dllimport)
#endif

#endif // LIBMATHXCORE_DEFS_H
