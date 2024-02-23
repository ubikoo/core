add_library(coremath STATIC
    simd/algebra.h
    simd/arithmetic.h
    simd/common.h
    simd/matrix.h
    simd/transform.h
    simd/vector.h
    math.cpp
    algebra.h
    arithmetic.h
    io.h
    math.h
    matrix.h
    memory.h
    ortho.h
    random.h
    transform.h
    vector.h)

target_include_directories(coremath PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# Enable math compile options.
find_library(MATH_LIBRARY m)
mark_as_advanced(MATH_LIBRARY)
if(MATH_LIBRARY)
    target_link_libraries(coremath PUBLIC ${MATH_LIBRARY})
endif(MATH_LIBRARY)

if(WIN32)
    target_compile_definitions(coremath PUBLIC _USE_MATH_DEFINES)
endif(WIN32)

# Enable SIMD/AVX compile options.
option(ENABLE_AVX "Enable AVX optimizations" ON)
if(ENABLE_AVX)
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        target_compile_options(coremath PUBLIC /arch:AVX)
    else()
        target_compile_options(coremath PUBLIC -mavx)
    endif()
endif(ENABLE_AVX)

# Enable OpenMP compile options.
option(ENABLE_OPENMP "Enable OpenMP multithreading" ON)
if(ENABLE_OPENMP)
    find_package(OpenMP REQUIRED)
    target_link_libraries(coremath PUBLIC OpenMP::OpenMP_CXX)
    target_include_directories(coremath PUBLIC ${OpenMP_CXX_INCLUDE_DIRS})
endif(ENABLE_OPENMP)
