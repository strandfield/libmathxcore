

# `libmathxcore` - A C library for arbitrary-precision arithmetic 

[![Build Status](https://travis-ci.org/RugessNome/libmathxcore.svg?branch=master)](https://travis-ci.org/RugessNome/libmathxcore)
[![codecov](https://codecov.io/gh/RugessNome/libmathxcore/branch/master/graph/badge.svg)](https://codecov.io/gh/RugessNome/libmathxcore)

`libmathxcore` is a library for high-precision integer and floating-point arithmetic.

## Building

```
git clone https://github.com/RugessNome/libmathxcore.git
mkdir && cd build
cmake ..
make
```

## Documentation

The library provides three types:
- `mx_int_t`: arbitrary precision integer arithmetic
- `mx_rat_t`: rational numbers
- `mx_float_t`: floating-point numbers (the radix is a power of 2)

Depending on the type, you will use functions prefixed with either `int_*`, 
`rat_*`, or `float_*`.
For example, `int_init` is used to initialize integers while `rat_add` adds 
two rational numbers.

### Integers

```
#include "mathx/core/integer.h"
```

### Rational numbers

```
#include "mathx/core/rational.h"
```

A rational number is stored as two `mx_int_t` without any common factors 
(rational numbers are always stored in reduced form).

The sign of a rational number is stored in its numerator (the denominator is 
always positive).

### Floating-point numbers

```
#include "mathx/core/float.h"
```

A floating-point number `x` is basically a signed integer `m` and an exponent `n`.
The represented number is: `x = m * B^n` with `B` a power of two 
(currently `2^8`, `2^16` or `^2^32`).

Since not all decimal numbers can be represented exactly in this form 
(for example `0.1` has no finite binary form), and additional `prec` field 
describe on how many limbs the result are to be computed (see example below). 


## Example

The famous constant `pi` can be computed with the following code.

```c
mx_float_t pi;
// We want to compute pi on 128 bytes.
float_init_prec(&pi, float_prec_bytes(128)); 
float_assign_pi(&pi);
float_print(&pi);
float_clear(&pi);
```

On my machine, this prints:

```
3141592653589793238462643383279502884197169
3993751058209749445923078164062862089986280
3482534211706798214808651328230664709384460
9550582231725359408128481117450284102701938
5211055596446229489549303819644288109756659
3344612847564823378678316527120190914564856
6923460348610454326648213393607260249141273
72458e-305
```

We got an approximation of `pi` to 305 decimal places. 
All digits are correct, but the result is not correctly rounded. 
This is because all operations are done in a binary base and
are truncated for performance.
 