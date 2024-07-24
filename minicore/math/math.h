//
// math.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MATH_H_
#define MATH_H_

///
/// @brief Vector and matrix data representations:
///
///  1x2 vector is interpreted as one 128-bit (2x64) memory block.
///  1x3 vector is interpreted as one 256-bit (4x64) memory block with the last
///  64-bit double padded to zero.
///  1x4 vector is interpreted as one 256-bit (4x64) memory block.
///
///  2x2 matrix is interpreted as two 128-bit (2x64) memory block.
///  3x3 matrix is interpreted as three 256-bit (4x64) memory blocks with the
///  last 64-bit double padded to zero.
///  4x4 matrix is interpreted as four 256-bit (4x64) memory blocks.
///
/// @see https://stackoverflow.com/questions/4421706
///      https://stackoverflow.com/questions/36955576
///      https://gamedev.stackexchange.com/questions/33142
///      https://stackoverflow.com/questions/36211864
///      https://gcc.gnu.org/onlinedocs/gcc-6.5.0/gcc/Common-Type-Attributes.html
///
#include "io.h"
#include "matrix.h"
#include "ortho.h"
#include "random.h"
#include "transform.h"
#include "vector.h"

#endif // MATH_H_