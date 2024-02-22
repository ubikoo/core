//
// random.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MATH_RANDOM_H_
#define MATH_RANDOM_H_

#include <random>
#include "algebra.h"
#include "arithmetic.h"

namespace math {

/// -----------------------------------------------------------------------------
/// @brief Function object providing an interface std::random_device and
/// returning std::random_device
///
struct random_device {
    std::random_device seed;
    uint64_t operator()() {
        uint64_t lo = seed();
        uint64_t hi = seed();
        return (hi << 32 | lo);
    }
};

///
/// @brief Return a random 64-bit prime number. Sample a random number from the
/// device object and return the largest prime smaler than it.
///
/// Is n a prime number? Search for any divisor p < n. If none is found, then n
/// is a prime. We only need to test i < (sqrt(n) + 1):
///  If n = p * q, then
///      p ≤ sqrt(n) or q ≤ sqrt(n), otherwise p*q > n.
///  If p = q, then
///      n = p * p and p = sqrt(n)
///  If p != q, then
///      p > sqrt(n) and q < sqrt(n)
///  or
///      p < sqrt(n) and q > sqrt(n)
/// If there is no divisor p < sqrt(n), then n%p == 0, and n is a prime. Every
/// even number larger than 2 is not prime.  If 2 does not divide n, then every
/// other even number smaller than n will also not divide n.
///
inline uint64_t random_prime(random_device &device)
{
    auto is_prime = [] (const uint64_t n) -> bool {
        if (n%2 == 0) {         // Even numbers are not prime
            return false;
        }
        uint64_t sqrt_n = std::sqrt((double) n) + 1.0;
        for (uint64_t p = 3; p < sqrt_n; p += 2) {
            if (n%p == 0) {     // If p is a divisor, n is not prime
                return false;
            }
        }
        return true;            // No divisors found, n is prime
    };

    uint64_t number = device();
    for (uint64_t n = number; n >= 3; --n) {
        if (is_prime(n)) {
            return n;
        }
    }
    return number;
}

/// -----------------------------------------------------------------------------
/// @brief Random number generator state based on a 64-bit variant of George
/// Marsaglia's KISS generator. Random number generator is a combination of:
///
///  Linear congruential generator
///      Xn+1 = (m1 * Xn + m2) mod m, where m = 2^64-1.
///
///  First multiply-with-carry generator
///      Xn+1 = (m3 * Xn + c) mod m, and c = Xn+1 >> 32
///
///  Second multiply-with-carry generator
///      Xn+1 = (m4 * Xn + c) mod m, and c = Xn+1 >> 32
///
/// @see http://www.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf
///
struct random_engine {
    uint64_t x  = 123456789123ULL;      // linear congruential generator
    uint64_t y  = 987654321987ULL;      // xor-shift generator
    uint32_t z1 = 43219876U;            // multiply-with-carry generator
    uint32_t c1 = 6543217U;
    uint32_t z2 = 21987643U;            // multiply-with-carry generator
    uint32_t c2 = 1732654U;
};

///
/// @brief Create a random number generator using random initial seeds
///      0 < x  < 2^64,       linear congruential generator
///      0 < y  < 2^64,       xor-shift register generator
///      0 < z1 < 2^32,       multiply-with-carry generator 1
///      0 < c1 < 698769069U
///      0 < z2 < 2^32,       multiply-with-carry generator 2
///      0 < c2 < 698769069U
/// Do not set y = 0 and avoid x = z1 = c1 = z2 = c2 = 0.
///
inline random_engine make_random()
{
    // Generate a random odd number between 0 and n_max
    random_device device;
    auto random_seed = [&](uint64_t n_max) -> uint64_t {
        uint64_t n = device() % (n_max - 1);
        if (n%2 == 0) { ++n; }
        return n;
    };

    // Create a zeroth state.
    random_engine engine = {};

    // Seed the linear congruential generator state.
    engine.x = random_seed(0xffffffffffffffff);

    // Seed the xor-shift register generator state.
    engine.y = random_seed(0xffffffffffffffff);

    // Seed the first multiply-with-carry generator state.
    engine.z1 = random_seed(0xffffffffffffffff);
    engine.c1 = (uint32_t) random_seed(698769069ULL);

    // Seed the second multiply-with-carry generator state.
    engine.z2 = random_seed(0xffffffffffffffff);
    engine.c2 = (uint32_t) random_seed(698769069ULL);

    return engine;
}

///
/// @brief 32-bit random number generator.
///
inline uint32_t random32(random_engine &rng)
{
    static constexpr uint64_t m1 = 1490024343005336237ULL;
    static constexpr uint64_t m2 = 123456789ULL;
    static constexpr uint64_t m3 = 4294584393ULL;

    // Linear congruential generator
    rng.x =  m1 * rng.x + m2;

    // Xorshift register generator - do not set y = 0
    rng.y ^= rng.y << 21;
    rng.y ^= rng.y >> 17;
    rng.y ^= rng.y << 30;

    // First multiply-with-carry generator - avoid z1 = c1 = 0
    uint64_t t1 = m3 * rng.z1 + rng.c1;
    rng.c1 = t1 >> 32;
    rng.z1 = t1;

    // Return 32-bit result
    return (uint32_t) (rng.x >> 32) + (uint32_t) rng.y + rng.z1;
}

///
/// @brief 64-bit random number generator.
///
inline uint64_t random64(random_engine &rng)
{
    static constexpr uint64_t m1 = 1490024343005336237ULL;
    static constexpr uint64_t m2 = 123456789ULL;
    static constexpr uint64_t m3 = 4294584393ULL;
    static constexpr uint64_t m4 = 4246477509ULL;

    // Linear congruential generator
    rng.x =  m1 * rng.x + m2;

    // Xorshift register generator - do not set y = 0
    rng.y ^= rng.y << 21;
    rng.y ^= rng.y >> 17;
    rng.y ^= rng.y << 30;

    // First multiply-with-carry generator - avoid z1 = c1 = 0
    uint64_t t1 = m3 * rng.z1 + rng.c1;
    rng.c1 = t1 >> 32;
    rng.z1 = t1;

    // Second multiply-with-carry generator - avoid z2 = c2 = 0
    uint64_t t2 = m4 * rng.z2 + rng.c2;
    rng.c2 = t2 >> 32;
    rng.z2 = t2;

    // Return 64-bit result
    return rng.x + rng.y + ((uint64_t) rng.z1) + ((uint64_t) rng.z2 << 32);
}

/// ---- Random number generator samplers -------------------------------------
/// @brief Sample a random number from a uniform distribution in interval [a,b].
///
template<typename T>
struct random_uniform {};

template<>
struct random_uniform<float> {
    float operator()(random_engine &rng, float lo = 0.0f, float hi = 1.0f) {
        float r = (float) random32(rng) / (float) UINT32_MAX;
        return (lo + r * (hi - lo));
    }
};

template<>
struct random_uniform<double> {
    double operator()(random_engine &rng, double lo = 0.0, double hi = 1.0) {
        double r = (double) random64(rng) / (float) UINT64_MAX;
        return (lo + r * (hi - lo));
    }
};

template<>
struct random_uniform<uint32_t> {
    random_uniform<float> urand;
    uint32_t operator()(
        random_engine &rng,
        uint32_t lo = 0,
        uint32_t hi = UINT32_MAX - 1) {
        float r = (float) (hi - lo + 1) * urand(rng);
        float n = (float) lo + std::floor(r);
        return ((uint32_t) n);
    }
};

template<>
struct random_uniform<uint64_t> {
    random_uniform<double> urand;
    uint64_t operator()(
        random_engine &rng,
        uint64_t lo = 0,
        uint64_t hi = UINT64_MAX - 1) {
        double r = (double) (hi - lo + 1) * urand(rng);
        double n = (double) lo + std::floor(r);
        return ((uint64_t) n);
    }
};

template<>
struct random_uniform<int32_t> {
    random_uniform<float> urand;
    int32_t operator()(
        random_engine &rng,
        int32_t lo = INT32_MIN,
        int32_t hi = INT32_MAX - 1) {
        float r = (float) (hi - lo + 1) * urand(rng);
        float n = (float) lo + std::floor(r);
        return ((int32_t) n);
    }
};

template<>
struct random_uniform<int64_t> {
    random_uniform<double> urand;
    int64_t operator()(
        random_engine &rng,
        int64_t lo = INT64_MIN,
        int64_t hi = INT64_MAX - 1) {
        double r = (double) (hi - lo + 1) * urand(rng);
        double n = (double) lo + std::floor(r);
        return ((int64_t) n);
    }
};

///
/// @brief Generate a floating point random number from a Gaussian distribution
/// with average mu and standard deviation sig.
///
/// If Z is a standard normal deviate with zero mean and unit variance, then
/// X = sig*Z + mu will be a normal deviate with mean mu and variance sig^2.
/// Sample a random deviate Z from a standard normal distribution, i.e., a
/// Gaussian distribution with zero mean and unit variance using Box-Muller
/// algorithm.
///
/// @see http://en.literateprograms.org/Box-Muller_transform_(C)
/// Compute the normal deviate X using the corresponding linear relation.
///
template<typename T>
struct random_gauss {};

template<>
struct random_gauss<float> {
    static constexpr float zero = (float) 0;
    static constexpr float one  = (float) 1;
    static constexpr float two  = (float) 2;

    bool has_cache = false;     // normal random deviate cache state
    float cache = zero;         // stored normal random deviate
    random_uniform<float> urand;

    float operator()(random_engine &rng, float mu = zero, float sig = one) {
        float value = zero;
        if (has_cache) {
            // Get the stored random deviate and reset cache state.
            value = cache;
            has_cache = false;
        } else {
            // Sample a random point in the unit circle from two samples
            // in the interval [-one, one] using the Box-Muller transform.
            float x, y, r;
            do {
                x = urand(rng, -one, one);
                y = urand(rng, -one, one);
                r = x*x + y*y;
            } while (iseq(r, zero) || !islt(r, one));

            float d = std::sqrt(-two * std::log(r) / r);
            value = x * d;      // use first random deviate
            cache = y * d;      // cache the second random deviate
            has_cache = true;
        }
        return (sig * value + mu);
    }
};

template<>
struct random_gauss<double> {
    static constexpr double zero = (double) 0;
    static constexpr double one  = (double) 1;
    static constexpr double two  = (double) 2;

    bool has_cache = false;     // normal random deviate cache state
    double cache = zero;        // stored normal random deviate
    random_uniform<double> urand;

    double operator()(random_engine &rng, double mu = zero, double sig = one) {
        double value = zero;
        if (has_cache) {
            // Get the stored random deviate and reset cache state.
            value = cache;
            has_cache = false;
        } else {
            // Sample a random point in the unit circle from two samples
            // in the interval [-one, one] using the Box-Muller transform.
            double x, y, r;
            do {
                x = urand(rng, -one, one);
                y = urand(rng, -one, one);
                r = x*x + y*y;
            } while (iseq(r, zero) || !islt(r, one));

            double d = std::sqrt(-two * std::log(r) / r);
            value = x * d;      // use first random deviate
            cache = y * d;      // cache the second random deviate
            has_cache = true;
        }
        return (sig * value + mu);
    }
};

} // math

#endif // MATH_RANDOM_H_
