//
// common.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MATH_SIMD_COMMON_H_
#define MATH_SIMD_COMMON_H_

///
/// @brief AVX instruction set.
/// SSE vector registers are 16-bytes wide packing 4 integers (4-byte)
/// or 2 doubles (8-byte).
/// AVX vector registers are 32-bytes wide packing 4 integers (8-byte)
/// or 4 doubles (8-byte).
///
#include <immintrin.h>

namespace Math {

/// ---- Vector insert and extract intrinsics ---------------------------------
/// @fn _mm256_set_m128d(__m128d hi, __m128d lo)
///  dst[127:0]   := lo[127:0]
///  dst[255:128] := hi[127:0]
///
/// @fn _mm256_castpd128_pd256 (__m128d a)
/// Cast vector of type __m128d to type __m256d. The upper 128 bits of the
/// result are undefined.
///
/// @fn _mm256_insertf128_pd(__m256d a, __m128d b, int mask)
///  dst[255:0] := a[255:0]
///  CASE mask[7:0] of
///      0: dst[127:0]   := b[127:0]
///      1: dst[255:128] := b[127:0]
///  ESAC
///
#ifndef _mm256_set_m128d
#define _mm256_set_m128d(hi, lo) \
        _mm256_insertf128_pd(_mm256_castpd128_pd256(lo), (hi), 1)
#endif

///
/// @fn _mm_cvtsd_f64 (__m128d a)
///  dst[63:0] := a[63:0]
///
/// @fn _mm256_cvtsd_f64 (__m256d a)
///  dst[63:0] := a[63:0]
///
/// @fn _mm256_extractf128_pd(__m256d a, const int mask)
///  CASE mask[7:0] of
///      0: dst[127:0] := a[127:0]
///      1: dst[127:0] := a[255:128]
///  ESAC
///
#ifndef _mm256_cvtsd_f64
#define _mm256_cvtsd_f64(a) \
        _mm_cvtsd_f64(_mm256_extractf128_pd((a), 0))
#endif

///
/// @fn _mm_cvtsd_f64 (__m128d a)
///  dst[63:0] := a[63:0]
///
/// @fn _mm256_extractf128_pd(__m256d a, const int mask)
///  dst[127:0] := (mask[0] == 0) ? a[127:0]
///  dst[127:0] := (mask[0] == 1) ? a[255:128]
///
/// @fn _mm_permute_pd(__m128d a, int mask)
///  dst[63:0]   := (mask[0] == 0) ? a[63:0] : a[127:64]
///  dst[127:64] := (mask[1] == 0) ? a[63:0] : a[127:64]
///
inline double simd256_extract_(__m256d a, int ix)
{
    //
    // v = (ix == 0 || ix == 1) ? {a1, a0}
    // v = (ix == 3 || ix == 2) ? {a3, a2}
    //
    __m128d v = (ix > 1)
        ? _mm256_extractf128_pd(a, 0b01)
        : _mm256_extractf128_pd(a, 0b00);

    //
    // v = (ix == 0) ? {a1, a0}
    // v = (ix == 1) ? {a0, a1}
    // v = (ix == 2) ? {a3, a2}
    // v = (ix == 3) ? {a2, a3}
    //
    v = (ix%2 == 0)
        ? _mm_permute_pd(v, 0b10)
        : _mm_permute_pd(v, 0b01);
    return _mm_cvtsd_f64(v);
}

///
/// v = (ix == 0) ? {a1, a0}
/// v = (ix == 1) ? {a0, a1}
///
inline double simd128_extract_(__m128d a, int ix)
{
    __m128d v = (ix%2 == 0)
        ? _mm_permute_pd(a, 0b10)
        : _mm_permute_pd(a, 0b01);
    return _mm_cvtsd_f64(v);
}

/// ---- Vector comparison intrinsics -----------------------------------------
/// @fn _mm256_cmp_pd(__m256d a, __m256d b, const int imm8)
///  dst[63:0]    := (a[63:0]    OP b[63:0])    ? 0xFFFFFFFFFFFFFFFF : 0
///  dst[127:64]  := (a[127:64]  OP b[127:64])  ? 0xFFFFFFFFFFFFFFFF : 0
///  dst[191:128] := (a[191:128] OP b[191:128]) ? 0xFFFFFFFFFFFFFFFF : 0
///  dst[255:192] := (a[255:192] OP b[255:192]) ? 0xFFFFFFFFFFFFFFFF : 0
/// CASE(imm8[7:0]) OF
///  0:  OP := _CMP_EQ_OQ        // Q = non-signaling, S = signaling
///  1:  OP := _CMP_LT_OS
///  2:  OP := _CMP_LE_OS
///  3:  OP := _CMP_UNORD_Q
///  4:  OP := _CMP_NEQ_UQ
///  5:  OP := _CMP_NLT_US
///  6:  OP := _CMP_NLE_US
///  7:  OP := _CMP_ORD_Q
///  8:  OP := _CMP_EQ_UQ
///  9:  OP := _CMP_NGE_US
///  10: OP := _CMP_NGT_US
///  11: OP := _CMP_FALSE_OQ
///  12: OP := _CMP_NEQ_OQ
///  13: OP := _CMP_GE_OS
///  14: OP := _CMP_GT_OS
///  15: OP := _CMP_TRUE_UQ
///  16: OP := _CMP_EQ_OS
///  17: OP := _CMP_LT_OQ
///  18: OP := _CMP_LE_OQ
///  19: OP := _CMP_UNORD_S
///  20: OP := _CMP_NEQ_US
///  21: OP := _CMP_NLT_UQ
///  22: OP := _CMP_NLE_UQ
///  23: OP := _CMP_ORD_S
///  24: OP := _CMP_EQ_US
///  25: OP := _CMP_NGE_UQ
///  26: OP := _CMP_NGT_UQ
///  27: OP := _CMP_FALSE_OS
///  28: OP := _CMP_NEQ_OS
///  29: OP := _CMP_GE_OQ
///  30: OP := _CMP_GT_OQ
///  31: OP := _CMP_TRUE_US
///
#ifndef _mm256_cmplt_pd
#define _mm256_cmplt_pd(a, b) \
        _mm256_cmp_pd((a), (b), 1)
#endif

#ifndef _mm256_cmple_pd
#define _mm256_cmple_pd(a, b) \
        _mm256_cmp_pd((a), (b), 2)
#endif

#ifndef _mm256_cmpge_pd
#define _mm256_cmpge_pd(a, b) \
        _mm256_cmp_pd((a), (b), 13)
#endif

#ifndef _mm256_cmpgt_pd
#define _mm256_cmpgt_pd(a, b) \
        _mm256_cmp_pd((a), (b), 14)
#endif

#ifndef _mm256_cmpeq_pd
#define _mm256_cmpeq_pd(a, b) \
        _mm256_cmp_pd((a), (b), 16)
#endif

#ifndef _mm256_cmpne_pd
#define _mm256_cmpne_pd(a, b) \
        _mm256_cmp_pd((a), (b), 28)
#endif

/// ---- Vector bitwise intrinsics --------------------------------------------
/// @fn __m128d _mm_and_pd(__m128d a, __m128d b)
///  FOR j := 0 to 1
///      i := j*64
///      dst[i+63:i] := (a[i+63:i] AND b[i+63:i])
///  ENDFOR
///
/// @fn __m128d _mm_andnot_pd(__m128d a, __m128d b)
///  FOR j := 0 to 1
///      i := j*64
///      dst[i+63:i] := ((NOT a[i+63:i]) AND b[i+63:i])
///  ENDFOR
///
/// @fn __m128d _mm_or_pd(__m128d a, __m128d b)
///  FOR j := 0 to 1
///      i := j*64
///      dst[i+63:i] := a[i+63:i] OR b[i+63:i]
///  ENDFOR
///
/// @fn __m256d _mm256_and_pd(__m256d a, __m256d b)
///  FOR j := 0 to 3
///      i := j*64
///      dst[i+63:i] := (a[i+63:i] AND b[i+63:i])
///  ENDFOR
///  dst[MAX:256] := 0
///
/// @fn __m256d _mm256_andnot_pd(__m256d a, __m256d b)
///  FOR j := 0 to 3
///      i := j*64
///      dst[i+63:i] := ((NOT a[i+63:i]) AND b[i+63:i])
///  ENDFOR
///  dst[MAX:256] := 0
///
/// @fn __m256d _mm256_or_pd(__m256d a, __m256d b)
///  FOR j := 0 to 3
///      i := j*64
///      dst[i+63:i] := a[i+63:i] OR b[i+63:i]
///  ENDFOR
///  dst[MAX:256] := 0
///

/// ---- Vector shuffle intrinsics --------------------------------------------
/// @brief Shuffle double-precision (64-bit) floating-point elements using the
/// control in mask.
///
/// @fn _mm_shuffle_pd(__m128d a, __m128d b, int mask)
///  dst[63:0]   := (mask[0] == 0) ? a[63:0] : a[127:64]
///  dst[127:64] := (mask[1] == 0) ? b[63:0] : b[127:64]
///
inline __m128d simd128_shuffle_(__m128d a, __m128d b, const int mask)
{
    return (mask == 0 ? _mm_shuffle_pd(a, b, 0b00) :
            mask == 1 ? _mm_shuffle_pd(a, b, 0b01) :
            mask == 2 ? _mm_shuffle_pd(a, b, 0b10) :
                        _mm_shuffle_pd(a, b, 0b11));
}

///
/// @brief Shuffle double-precision (64-bit) floating-point elements within
/// 128-bit lanes using the control in mask.
///
/// @fn _mm256_shuffle_pd(__m256d a, __m256d b, const int mask)
///  dst[63:0]    := (mask[0] == 0) ? a[63:0]    : a[127:64]
///  dst[127:64]  := (mask[1] == 0) ? b[63:0]    : b[127:64]
///  dst[191:128] := (mask[2] == 0) ? a[191:128] : a[255:192]
///  dst[255:192] := (mask[3] == 0) ? b[191:128] : b[255:192]
///
inline __m256d simd256_shuffle_(__m256d a, __m256d b, const int mask)
{
    return (mask == 0  ? _mm256_shuffle_pd(a, b, 0b0000) :
            mask == 1  ? _mm256_shuffle_pd(a, b, 0b0001) :
            mask == 2  ? _mm256_shuffle_pd(a, b, 0b0010) :
            mask == 3  ? _mm256_shuffle_pd(a, b, 0b0011) :
            mask == 4  ? _mm256_shuffle_pd(a, b, 0b0100) :
            mask == 5  ? _mm256_shuffle_pd(a, b, 0b0101) :
            mask == 6  ? _mm256_shuffle_pd(a, b, 0b0110) :
            mask == 7  ? _mm256_shuffle_pd(a, b, 0b0111) :
            mask == 8  ? _mm256_shuffle_pd(a, b, 0b1000) :
            mask == 9  ? _mm256_shuffle_pd(a, b, 0b1001) :
            mask == 10 ? _mm256_shuffle_pd(a, b, 0b1010) :
            mask == 11 ? _mm256_shuffle_pd(a, b, 0b1011) :
            mask == 12 ? _mm256_shuffle_pd(a, b, 0b1100) :
            mask == 13 ? _mm256_shuffle_pd(a, b, 0b1101) :
            mask == 14 ? _mm256_shuffle_pd(a, b, 0b1110) :
                         _mm256_shuffle_pd(a, b, 0b1111));
}

///
/// @brief Shuffle 128-bits(composed of 2 packed double-precision (64-bit)
/// floating-point elements) selected by mask from a and b.
///
/// @fn _mm256_set_m128d(__m128d hi, __m128d lo)
///  dst[127:0]   := lo[127:0]
///  dst[255:128] := hi[127:0]
/// @fn _mm256_extractf128_pd(__m256d a, const int mask)
///  dst[127:0] := (mask[0] == 0) ? a[127:0]
///  dst[127:0] := (mask[0] == 1) ? a[255:128]
/// Operation:
///  dst[127:0]   := (masklo[0] == 0) ? a[127:0] : a[255:128]
///  dst[255:128] := (maskhi[0] == 0) ? b[127:0] : b[255:128]
///
inline __m256d simd256_shuffle2f_(
    __m256d a, const int masklo,
    __m256d b, const int maskhi)
{
    __m128d hi = (maskhi == 0)
        ? _mm256_extractf128_pd(b, 0b00)
        : _mm256_extractf128_pd(b, 0b01);

    __m128d lo = (masklo == 0)
        ? _mm256_extractf128_pd(a, 0b00)
        : _mm256_extractf128_pd(a, 0b01);

    return _mm256_set_m128d(hi, lo);
}

/// ---- Vector permute intrinsics --------------------------------------------
/// @brief Shuffle double-precision (64-bit) floating-point elements in a using
/// the control in mask.
///
/// @fn _mm_permute_pd(__m128d a, int mask)
///  dst[63:0]    := (mask[0] == 0) ? a[63:0] : a[127:64]
///  dst[127:64]  := (mask[1] == 0) ? a[63:0] : a[127:64]
///
inline __m128d simd128_permute_(__m128d a, int mask)
{
    return (mask == 0 ? _mm_permute_pd(a, 0b00) :
            mask == 1 ? _mm_permute_pd(a, 0b01) :
            mask == 2 ? _mm_permute_pd(a, 0b10) :
                        _mm_permute_pd(a, 0b11));
}

///
/// @brief Shuffle double-precision (64-bit) floating-point elements within
/// 128-bit lanes using the control in mask.
///
/// @fn _mm256_permute_pd(__m256d a, int mask)
///  dst[63:0]    := (mask[0] == 0) ? a[63:0]    : a[127:64]
///  dst[127:64]  := (mask[1] == 0) ? a[63:0]    : a[127:64]
///  dst[191:128] := (mask[2] == 0) ? a[191:128] : a[255:192]
///  dst[255:192] := (mask[3] == 0) ? a[191:128] : a[255:192]
///
inline __m256d simd256_permute_(__m256d a, int mask)
{
    return (mask == 0  ? _mm256_permute_pd(a, 0b0000) :
            mask == 1  ? _mm256_permute_pd(a, 0b0001) :
            mask == 2  ? _mm256_permute_pd(a, 0b0010) :
            mask == 3  ? _mm256_permute_pd(a, 0b0011) :
            mask == 4  ? _mm256_permute_pd(a, 0b0100) :
            mask == 5  ? _mm256_permute_pd(a, 0b0101) :
            mask == 6  ? _mm256_permute_pd(a, 0b0110) :
            mask == 7  ? _mm256_permute_pd(a, 0b0111) :
            mask == 8  ? _mm256_permute_pd(a, 0b1000) :
            mask == 9  ? _mm256_permute_pd(a, 0b1001) :
            mask == 10 ? _mm256_permute_pd(a, 0b1010) :
            mask == 11 ? _mm256_permute_pd(a, 0b1011) :
            mask == 12 ? _mm256_permute_pd(a, 0b1100) :
            mask == 13 ? _mm256_permute_pd(a, 0b1101) :
            mask == 14 ? _mm256_permute_pd(a, 0b1110) :
                         _mm256_permute_pd(a, 0b1111));
}

///
/// @brief Shuffle 128-bits (composed of 2 packed double-precision (64-bit)
/// floating-point elements) selected by mask from a and b.
///
/// @fn _mm256_permute2f128_pd(__m256d a, __m256d b, int mask)
///  SELECT4(src1, src2, control) {
///      CASE(control[1:0])
///          0:  tmp[127:0] := src1[127:0]
///          1:  tmp[127:0] := src1[255:128]
///          2:  tmp[127:0] := src2[127:0]
///          3:  tmp[127:0] := src2[255:128]
///      ESAC
///      IF control[3]
///          tmp[127:0] := 0
///      FI
///      RETURN tmp[127:0]
///  }
///  dst[127:0]   := SELECT4(a[255:0], b[255:0], mask[3:0])
///  dst[255:128] := SELECT4(a[255:0], b[255:0], mask[7:4])
///
inline __m256d simd256_permute2f_(__m256d a, __m256d b, int mask)
{
    uint8_t mask_lo = (mask >> 2) & 3;
    uint8_t mask_hi = (mask >> 4) & 3;
    uint8_t mask_both = (mask_hi << 2) | (mask_lo);
    uint8_t mask_zero = ((mask >> 3) & 1) | ((mask >> 7) & 1);

    // use both low and high 128-bit fields
    if (mask_zero == 0b00000000) {
        return (mask_both == 0 ? _mm256_permute2f128_pd(a, b, 0b00000000) :
                mask_both == 1 ? _mm256_permute2f128_pd(a, b, 0b00000001) :
                mask_both == 2 ? _mm256_permute2f128_pd(a, b, 0b00000010) :
                mask_both == 3 ? _mm256_permute2f128_pd(a, b, 0b00000011) :

                mask_both == 4 ? _mm256_permute2f128_pd(a, b, 0b00010000) :
                mask_both == 5 ? _mm256_permute2f128_pd(a, b, 0b00010001) :
                mask_both == 6 ? _mm256_permute2f128_pd(a, b, 0b00010010) :
                mask_both == 7 ? _mm256_permute2f128_pd(a, b, 0b00010011) :

                mask_both == 8 ? _mm256_permute2f128_pd(a, b, 0b00100000) :
                mask_both == 9 ? _mm256_permute2f128_pd(a, b, 0b00100001) :
                mask_both == 10 ? _mm256_permute2f128_pd(a, b, 0b00100010) :
                mask_both == 11 ? _mm256_permute2f128_pd(a, b, 0b00100011) :

                mask_both == 12 ? _mm256_permute2f128_pd(a, b, 0b00110000) :
                mask_both == 13 ? _mm256_permute2f128_pd(a, b, 0b00110001) :
                mask_both == 14 ? _mm256_permute2f128_pd(a, b, 0b00110010) :
                                  _mm256_permute2f128_pd(a, b, 0b00110011));
    }

    // zero low 128-bit field
    if (mask_zero == 0b00001000) {
        return (mask_hi == 0 ? _mm256_permute2f128_pd(a, b, 0b00001000) :
                mask_hi == 1 ? _mm256_permute2f128_pd(a, b, 0b00011000) :
                mask_hi == 2 ? _mm256_permute2f128_pd(a, b, 0b00101000) :
                               _mm256_permute2f128_pd(a, b, 0b00111000));
    }

    // zero high 128-bit field
    if (mask_zero == 0b10000000) {
        return (mask_lo == 0 ? _mm256_permute2f128_pd(a, b, 0b10000000) :
                mask_lo == 1 ? _mm256_permute2f128_pd(a, b, 0b10000001) :
                mask_lo == 2 ? _mm256_permute2f128_pd(a, b, 0b10000010) :
                               _mm256_permute2f128_pd(a, b, 0b10000011));
    }

    // zero both low and high 128-bit fields
    return _mm256_permute2f128_pd(a, b, 0b10001000);
}

/// ---- Vector reverse intrinsics --------------------------------------------
/// @brief Reverse the order of two double-precision (64-bit) floating-point
/// elements.
///
/// @fn _mm_permute_pd(__m128d a, int mask)
///  dst[63:0]    := (mask[0] == 0) ? a[63:0] : a[127:64]
///  dst[127:64]  := (mask[1] == 0) ? a[63:0] : a[127:64]
///
inline __m128d simd128_reverse_(__m128d a)
{
    // {a1, a0} -> {a0, a1}
    return _mm_permute_pd(a, 0b01);
}

///
/// @brief Reverse the order of two double-precision (64-bit) floating-point
/// elements within 128-bit lanes.
///
/// @fn _mm256_permute_pd(__m256d a, int mask)
///  dst[63:0]    := dst[127:64] (mask[0] = 1)
///  dst[127:64]  := dst[63:0]   (mask[1] = 0)
///  dst[191:128] := dst[255:192] (mask[2] = 1)
///  dst[255:192] := dst[191:128] (mask[3] = 0)
///
/// @fn _mm256_permute2f128_pd(__m256d a, __m256d b, int mask)
///  dst[127:0]   := a[255:128]  (mask[1:0] = 1, mask[3] = 0)
///  dst[255:128] := a[127:0]    (mask[5:4] = 0, mask[7] = 0)
///
inline __m256d simd256_reverse_(__m256d a)
{
    // {a3, a2, a1, a0} -> {a2, a3, a0, a1}
    __m256d v = _mm256_permute_pd(a, 0b0101);

    // {a3, a2, a1, a0} -> {a1, a0, a3, a2}
    return _mm256_permute2f128_pd(v, v, 0b0001);
}

/// ---- Vector arithmetic intrinsics -----------------------------------------
///  dst[63:0]    := 1.0 / (SQRT(a[63:0])
///  dst[127:64]  := 1.0 / (SQRT(a[127:64])
///
inline __m128d _mm_rsqrt_pd(__m128d a)
{
    return _mm_div_pd(_mm_set1_pd(1.0), _mm_sqrt_pd(a));
}

///
/// @fn _mm256_rsqrt_pd
///  dst[63:0]    := 1.0 / (SQRT(a[63:0])
///  dst[127:64]  := 1.0 / (SQRT(a[127:64])
///  dst[191:128] := 1.0 / (SQRT(a[191:128])
///  dst[255:192] := 1.0 / (SQRT(a[255:192])
///
inline __m256d _mm256_rsqrt_pd(__m256d a)
{
    return _mm256_div_pd(_mm256_set1_pd(1.0), _mm256_sqrt_pd(a));
}

///
/// @brief Inverse square root of two double-precision (64-bit) elements.
/// @fn __m128 _mm_cvtpd_ps(__m128d a)
///  dst[31:0]   := Convert_FP64_To_FP32(a[63:0])
///  dst[63:32]  := Convert_FP64_To_FP32(a[127:64])
///
/// @fn __m128 _mm_rsqrt_ps(__m128 a)
///  dst[31:0]   :=  APPROXIMATE(1.0 / SQRT(a[31:0]))
///  dst[63:32]  :=  APPROXIMATE(1.0 / SQRT(a[63:32]))
///  dst[95:64]  :=  APPROXIMATE(1.0 / SQRT(a[95:64]))
///  dst[127:96] :=  APPROXIMATE(1.0 / SQRT(a[127:96]))
///
/// @fn __m256d _mm256_cvtps_pd(__m128 a)
///  dst[63:0]   := Convert_FP32_To_FP64(a[31:0])
///  dst[127:64] := Convert_FP32_To_FP64(a[63:32])
///
inline __m128d simd128_rsqrt_(__m128d x)
{
    const __m128d half = _mm_set1_pd(0.5);
    const __m128d one_half = _mm_set1_pd(1.5);
    //
    // Convert packed double-precision (64-bit) elements to packed single
    // precision (32-bit) elements.
    // Compute approximate reciprocal square root of packed single-precision
    // (32-bit) elements.
    // Convert packed single-precision (32-bit) elements to packed double
    // precision (64-bit) elements.
    //
    __m128d y1 = _mm_cvtps_pd(_mm_rsqrt_ps(_mm_cvtpd_ps(x)));
    //
    // Newton-Raphson optimization of the inverse square root estimate.
    // Solve the equation f(y) = y^-2 - x = 0, with f'(y) = -2y^-3,
    //      y(k+1) = y(k) - f(k) / f'(k)
    //             = (3/2)*y(k) - (x/2)*y(k)*y(k)*y(k)
    //             = y(k)*(1.5 - 0.5*x*y(k)*y(k))
    //
    __m128d x2  = _mm_mul_pd(half, x);

    __m128d xy1 = _mm_mul_pd(x2, _mm_mul_pd(y1, y1));
    __m128d y2  = _mm_mul_pd(y1, _mm_sub_pd(one_half, xy1));

    __m128d xy2 = _mm_mul_pd(x2, _mm_mul_pd(y2, y2));
    __m128d y3  = _mm_mul_pd(y2, _mm_sub_pd(one_half, xy2));

    __m128d xy3 = _mm_mul_pd(x2, _mm_mul_pd(y3, y3));
    __m128d y4  = _mm_mul_pd(y3, _mm_sub_pd(one_half, xy3));

    return y4;
}

///
/// @brief Inverse square root of four double-precision (64-bit) elements.
/// @fn __m128d _mm256_cvtpd_ps(__m256d a)
///  dst[31:0]    := Convert_FP64_To_FP32(a[63:0])
///  dst[63:32]   := Convert_FP64_To_FP32(a[127:64])
///  dst[95:64]   := Convert_FP64_To_FP32(a[191:128])
///  dst[127:96]  := Convert_FP64_To_FP32(a[255:192])
///
/// @fn __m128 _mm_rsqrt_ps(__m128 a)
///  dst[31:0]   :=  APPROXIMATE(1.0 / SQRT(a[31:0]))
///  dst[63:32]  :=  APPROXIMATE(1.0 / SQRT(a[63:32]))
///  dst[95:64]  :=  APPROXIMATE(1.0 / SQRT(a[95:64]))
///  dst[127:96] :=  APPROXIMATE(1.0 / SQRT(a[127:96]))
///
/// @fn __m256d _mm256_cvtps_pd(__m128 a)
///  dst[63:0]    := Convert_FP32_To_FP64(a[31:0])
///  dst[127:64]  := Convert_FP32_To_FP64(a[63:32])
///  dst[191:128] := Convert_FP32_To_FP64(a[95:64])
///  dst[255:192] := Convert_FP32_To_FP64(a[127:96])
///
inline __m256d simd256_rsqrt_(__m256d x)
{
    const __m256d half = _mm256_set1_pd(0.5);
    const __m256d one_half = _mm256_set1_pd(1.5);
    //
    // Convert packed double-precision (64-bit) elements to packed single
    // precision (32-bit) elements.
    // Compute approximate reciprocal square root of packed single precision
    // (32-bit) elements.
    // Convert packed single-precision (32-bit) elements to packed double
    // precision (64-bit) elements.
    //
    __m256d y1 = _mm256_cvtps_pd(_mm_rsqrt_ps(_mm256_cvtpd_ps(x)));
    //
    // Newton-Raphson optimization of the inverse square root estimate.
    // Solve the equation f(y) = y^-2 - x = 0, with f'(y) = -2y^-3,
    //      y(k+1) = y(k) - f(k) / f'(k)
    //             = (3/2)*y(k) - (x/2)*y(k)*y(k)*y(k)
    //             = y(k)*(1.5 - 0.5*x*y(k)*y(k))
    //
    __m256d x2  = _mm256_mul_pd(half, x);

    __m256d xy1 = _mm256_mul_pd(x2, _mm256_mul_pd(y1, y1));
    __m256d y2  = _mm256_mul_pd(y1, _mm256_sub_pd(one_half, xy1));

    __m256d xy2 = _mm256_mul_pd(x2, _mm256_mul_pd(y2, y2));
    __m256d y3  = _mm256_mul_pd(y2, _mm256_sub_pd(one_half, xy2));

    __m256d xy3 = _mm256_mul_pd(x2, _mm256_mul_pd(y3, y3));
    __m256d y4  = _mm256_mul_pd(y3, _mm256_sub_pd(one_half, xy3));

    return y4;
}

///
/// @brief Dot product of two double precision (64-bit) elements.
/// @fn _mm_mul_pd(__m128d a, __m128d b)
///  dst[63:0]   := a[63:0]   * a[63:0]
///  dst[127:64] := b[127:64] * b[127:64]
///
/// @fn _mm_hadd_pd(__m128d a, __m128d b)
///  dst[63:0]   := a[127:64] + a[63:0]
///  dst[127:64] := b[127:64] + b[63:0]
///
inline __m128d simd128_dot_(__m128d a, __m128d b)
{
    // {a1*b1, a0*b0}
    __m128d ymul = _mm_mul_pd(a, b);

    // {a1*b1 + a0*b0, a1*b1 + a0*b0}
    __m128d ydot = _mm_hadd_pd(ymul, ymul);

    return ydot;
}

///
/// @brief Dot product of four double precision (64-bit) elements.
/// @fn _mm256_mul_pd(__m256d a, __m256d b)
///  dst[63:0]    := a[63:0]    * a[63:0]
///  dst[127:64]  := b[127:64]  * b[127:64]
///  dst[191:128] := b[191:128] * b[191:128]
///  dst[255:192] := b[255:192] * b[255:192]
///
/// @fn _mm256_hadd_pd(__m256d a, __m256d b)
///  dst[63:0]    := a[127:64]  + a[63:0]
///  dst[127:64]  := b[127:64]  + b[63:0]
///  dst[191:128] := a[255:192] + a[191:128]
///  dst[255:192] := b[255:192] + b[191:128]
///
/// @fn _mm256_add_pd(__m128d a, __m128d b)
///  dst[63:0]    := a[63:0]    + a[63:0]
///  dst[127:64]  := b[127:64]  + b[127:64]
///  dst[191:128] := b[191:128] + b[191:128]
///  dst[255:192] := b[255:192] + b[255:192]
///
inline __m256d simd256_dot_(__m256d a, __m256d b)
{
    //
    // {a3*b3, a2*b2, a1*b1, a0*b0}
    //
    __m256d ymul = _mm256_mul_pd(a, b);
    //
    // {a3*b3 + a2*b2,
    //  a3*b3 + a2*b2,
    //  a1*b1 + a0*b0,
    //  a1*b1 + a0*b0}
    //
    __m256d yadd0 = _mm256_hadd_pd(ymul, ymul);
    //
    // {a3, a2, a1, a0} -> {a1, a0, a3, a2}
    //
    __m256d yadd1 = _mm256_permute2f128_pd(yadd0, yadd0, 0b0001);
    //
    // {a3*b3 + a2*b2 + a1*b1 + a0*b0,
    //  a3*b3 + a2*b2 + a1*b1 + a0*b0,
    //  a3*b3 + a2*b2 + a1*b1 + a0*b0,
    //  a3*b3 + a2*b2 + a1*b1 + a0*b0}
    //
    __m256d ydot = _mm256_add_pd(yadd0, yadd1);

    return ydot;
}

///
/// @brief Euclidean norm of two double-precision (64-bit) elements.
///
inline __m128d simd128_norm_(__m128d a)
{
    //
    // sqrt{a1*a1 + a0*a0,
    //      a1*a1 + a0*a0}
    //
    return _mm_sqrt_pd(simd128_dot_(a, a));
}

///
/// @brief Euclidean norm of four double-precision (64-bit) elements.
///
inline __m256d simd256_norm_(__m256d a)
{
    //
    // sqrt{a3*a3 + a2*a2 + a1*a1 + a0*a0,
    //      a3*a3 + a2*a2 + a1*a1 + a0*a0,
    //      a3*a3 + a2*a2 + a1*a1 + a0*a0,
    //      a3*a3 + a2*a2 + a1*a1 + a0*a0}
    //
     return _mm256_sqrt_pd(simd256_dot_(a, a));
}

///
/// @brief Normalize two double precision (64-bit) elements.
///
inline __m128d simd128_normalize_(__m128d a)
{
    //
    // a / sqrt{a1*b1 + a0*b0,
    //          a1*b1 + a0*b0}
    //
    __m128d ydot = simd128_dot_(a, a);
    __m128d ynorm = simd128_rsqrt_(ydot);  // _mm_rsqrt_pd(ydot)
    return _mm_mul_pd(a, ynorm);
}

///
/// @brief Normalize four double precision (64-bit) elements.
///
inline __m256d simd256_normalize_(__m256d a)
{
    //
    // a / sqrt{a3*b3 + a2*b2 + a1*b1 + a0*b0,
    //          a3*b3 + a2*b2 + a1*b1 + a0*b0,
    //          a3*b3 + a2*b2 + a1*b1 + a0*b0,
    //          a3*b3 + a2*b2 + a1*b1 + a0*b0}
    //
    __m256d ydot = simd256_dot_(a, a);
    __m256d ynorm = simd256_rsqrt_(ydot);  // _mm256_rsqrt_pd(ydot)
     return _mm256_mul_pd(a, ynorm);
}

/// ---- Matrix transpose intrinsics ------------------------------------------
/// @brief Return the transpose a 2x2 matrix represented as 2 vector-rows of
/// dimension 2.
///
/// @fn _mm_shuffle_pd(__m128d a, __m128d b, int mask)
///  dst[63:0]   := (mask[0] == 0) ? a[63:0] : a[127:64]
///  dst[127:64] := (mask[1] == 0) ? b[63:0] : b[127:64]
///
/// @par Operation:
/// row0 = {a1, a0} -> {a2, a0}
/// row1 = {a3, a2} -> {a3, a1}
///
inline void simd128_transpose_(__m128d(&row)[2])
{
    //
    // r0 = {a1, a0}
    // r1 = {a3, a2}
    //
    __m128d r0 = row[0];
    __m128d r1 = row[1];
    //
    // t0 = {a2, a0}
    // t1 = {a3, a1}
    //
    __m128d t0 = _mm_shuffle_pd(r0, r1, 0b00);
    __m128d t1 = _mm_shuffle_pd(r0, r1, 0b11);

    row[0] = t0;
    row[1] = t1;
}

///
/// @brief Return the transpose of a 4x4 matrix represented as 4 vector-rows of
/// dimension 4.
///
/// @fn _mm256_permute2f128_pd(__m256d a, __m256d b, int mask)
///  SELECT4(src1, src2, control) {
///      CASE(control[1:0])
///          0:  tmp[127:0] := src1[127:0]
///          1:  tmp[127:0] := src1[255:128]
///          2:  tmp[127:0] := src2[127:0]
///          3:  tmp[127:0] := src2[255:128]
///      ESAC
///      IF control[3]
///          tmp[127:0] := 0
///      FI
///      RETURN tmp[127:0]
///  }
///  dst[127:0]   := SELECT4(a[255:0], b[255:0], mask[3:0])
///  dst[255:128] := SELECT4(a[255:0], b[255:0], mask[7:4])
///
/// @fn _mm256_unpackhi_pd(__m256d a, __m256d b)
///  dst[63:0]    := a[127:64]
///  dst[127:64]  := b[127:64]
///  dst[191:128] := a[255:192]
///  dst[255:192] := b[255:192]
///
/// @fn _mm256_unpacklo_pd(__m256d a, __m256d b)
///  dst[63:0]    := a[63:0]
///  dst[127:64]  := b[63:0]
///  dst[191:128] := a[191:128]
///  dst[255:192] := b[191:128]
///
/// @par Operation:
/// row0 = { a3,  a2,  a1,  a0} -> {a12, a8,  a4, a0}
/// row1 = { a7,  a6,  a5,  a4} -> {a13, a9,  a5, a1}
/// row2 = {a11, a10,  a9,  a8} -> {a14, a10, a6, a2}
/// row3 = {a15, a14, a13, a12} -> {a15, a11, a7, a3}
///
inline void simd256_transpose_(__m256d (&row)[4])
{
    //
    // r0 = { a3,  a2,  a1,  a0}
    // r1 = { a7,  a6,  a5,  a4}
    // r2 = {a11, a10,  a9,  a8}
    // r3 = {a15, a14, a13, a12}
    //
    __m256d r0 = row[0];
    __m256d r1 = row[1];
    __m256d r2 = row[2];
    __m256d r3 = row[3];
    //
    // r4 = { a9,  a8, a11, a10}  (r2, r2) (mask[5:4] = 0, mask[1:0] = 1)
    // r5 = {a13, a12, a15, a14}  (r3, r3) (mask[5:4] = 0, mask[1:0] = 1)
    //
    __m256d r4 = _mm256_permute2f128_pd(r2, r2, 0b00000001);
    __m256d r5 = _mm256_permute2f128_pd(r3, r3, 0b00000001);
    //
    // p1 = { a6, a2,  a4,  a0}
    // p2 = { a7, a3,  a5,  a1}
    // p3 = {a12, a8, a14, a10}
    // p4 = {a13, a9, a15, a11}
    //
    __m256d p1 = _mm256_unpacklo_pd(r0, r1);
    __m256d p2 = _mm256_unpackhi_pd(r0, r1);
    __m256d p3 = _mm256_unpacklo_pd(r4, r5);
    __m256d p4 = _mm256_unpackhi_pd(r4, r5);
    //
    // t0 = {a12, a8,  a4,  a0}   (p1, p3) (mask[5:4] = 3, mask[1:0] = 0)
    // t1 = {a13, a9,  a5,  a1}   (p4, p2) (mask[5:4] = 3, mask[1:0] = 0)
    // t2 = { a6, a2, a14, a10}   (p1, p3) (mask[5:4] = 1, mask[1:0] = 2)
    // t3 = { a7, a3, a15, a11}   (p4, p2) (mask[5:4] = 1, mask[1:0] = 2)
    //
    __m256d t0 = _mm256_permute2f128_pd(p1, p3, 0b00110000);
    __m256d t1 = _mm256_permute2f128_pd(p2, p4, 0b00110000);
    __m256d t2 = _mm256_permute2f128_pd(p1, p3, 0b00010010);
    __m256d t3 = _mm256_permute2f128_pd(p2, p4, 0b00010010);
    //
    // t4 = {a14, a10, a6,  a2}   (t2, t2) (mask[5:4] = 0, mask[1:0] = 1)
    // t5 = {a15, a11, a7,  a3}   (t3, t3) (mask[5:4] = 0, mask[1:0] = 1)
    //
    __m256d t4 = _mm256_permute2f128_pd(t2, t2, 0b00000001);
    __m256d t5 = _mm256_permute2f128_pd(t3, t3, 0b00000001);

    row[0] = t0;
    row[1] = t1;
    row[2] = t4;
    row[3] = t5;
}

/// ---- Matrix determinant intrinsics ----------------------------------------
/// @brief Return the determinant of a 2x2 matrix represented as 4 packed
/// double-precision (64-bit) floating-point elements.
///
/// @fn _mm256_permute_pd(__m256d a, int mask)
///  dst[63:0]    := (mask[0] == 0) ? a[63:0]    : a[127:64]
///  dst[127:64]  := (mask[1] == 0) ? a[63:0]    : a[127:64]
///  dst[191:128] := (mask[2] == 0) ? a[191:128] : a[255:192]
///  dst[255:192] := (mask[3] == 0) ? a[191:128] : a[255:192]
///
/// @fn _mm256_hsub_pd(__m256d a, __m256d b)
///  dst[63:0]    := a[63:0]    - a[127:64]
///  dst[127:64]  := b[63:0]    - b[127:64]
///  dst[191:128] := a[191:128] - a[255:192]
///  dst[255:192] := b[191:128] - b[255:192]
///
inline __m256d simd256_det_(__m256d a)
{
    //
    // r0 = {a3, a2, a1, a0}
    // r1 = {a0, a1, a2, a3}
    // r2 = {a3*a0, a2*a1, a1*a2, a0*a3}
    // r3 = {a2*a1, a3*a0, a1*a2, a0*a3}
    //
    // det = husb(r3, r3)
    //     = {a3*a0 - a2*a1,
    //        a3*a0 - a2*a1,
    //        a0*a3 - a1*a2,
    //        a0*a3 - a1*a2}
    //
    __m256d r0 = a;
    __m256d r1 = simd256_reverse_(r0);
    __m256d r2 = _mm256_mul_pd(r0, r1);
    __m256d r3 = _mm256_permute_pd(r2, 0b0110);

    return _mm256_hsub_pd(r3, r3);
}

} // namespace Math

#endif  // MATH_SIMD_COMMON_H_
