#ifndef TEST_MATH_COMMON_H_
#define TEST_MATH_COMMON_H_

///
/// @brief OpenMP interface.
///
#if defined(_OPENMP)
#include <omp.h>
#else
inline int omp_get_level(void) { return 0; }
inline int omp_get_thread_num(void) { return 0; }
inline int omp_get_num_threads(void) { return 1; }
inline int omp_get_max_threads(void) { return 1; }
#endif // _OPENMP

#endif // TEST_MATH_COMMON_H_