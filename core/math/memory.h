//
// memory.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef MATH_MEMORY_H_
#define MATH_MEMORY_H_

#include <cstdlib>
#include <cstring>
#include <memory>

///
/// @brief If preferred cache line size is not defined, set it to 32 bytes with
/// AVX instruction set. Otherwise use default value of 16 bytes.
///
#ifndef MATH_CACHE_LINE_SIZE
#ifdef __AVX__
#define MATH_CACHE_LINE_SIZE 32
#else
#define MATH_CACHE_LINE_SIZE 16
#endif
#endif // MATH_CACHE_LINE_SIZE

namespace math {

/// ---- Aligned memory allocation --------------------------------------------
/// @brief Allocate a block of memory with size bytes; posix_memalign places the
/// block on an address multiple of alignment. The alignment must be a power of
/// two and a multiple of sizeof(void *).
///
inline void *align_alloc(size_t size)
{
    if (size == 0) {
        return nullptr;
    }

    void *ptr = nullptr;
#if defined(_WIN32)
    ptr = _aligned_malloc(size, MATH_CACHE_LINE_SIZE);
#else
    int ret = posix_memalign(&ptr, MATH_CACHE_LINE_SIZE, size);
    if (ret == EINVAL || ret == ENOMEM) {
        return nullptr;
    }
#endif
    if (!ptr) {
        return nullptr;
    }
    std::memset(ptr, 0, size);

    return ptr;
}

///
/// @brief Free a memory block.
///
inline void align_free(void *ptr)
{
#if defined(_WIN32)
    _aligned_free(ptr);
#else
    std::free(ptr);
#endif
}

///
/// @brief Reallocate an array with oldsize = (oldelms * size) bytes to a new
/// array with newsize = (newelms * size) bytes on a boundary specified by the
/// default alignment.
///
/// The address of a block returned by malloc or realloc in 32-bit systems is a
/// multiple of eight (or a multiple of sixteen on 64-bit systems).
/// The function reimplements the realloc specification to the new boundary size.
///
/// @note Summary of the relevant requirements for realloc:
/// - The contents of the object shall remain unchanged up to the lesser of the
///   new and old sizes. If the new block size requires movement of the object,
///   the space for the previous instantiation of the object is freed.
///
/// - If the new block size is smaller than the old block size, align_realloc
///   will allocate a new block of the appriate size, copy the contents of the
///   original block up to the new size, free the original block and return a
///   pointer to the newly created block.
///   If the new block size is larger, the contents of the newly allocated extra
///   portion are initialised to 0 as set by align_alloc.
///
/// - If the input pointer is null, align_realloc behaves exactly as if
///   align_alloc has been called.
///
/// - If the newsize is 0 and ptr is not a null pointer, align_realloc
///   behaves exactly as if align_free has been called and return a null
///   value.
///
/// - If the space cannot be allocated, the object shall remain unchanged.
///   If the input pointer is null and the requested size is 0, then the
///   result is undefined.
///
inline void *align_realloc(void *ptr, size_t oldsize, size_t newsize)
{
    // If new size is 0, free the ptr and return null.
    if (newsize == 0) {
        align_free(ptr);
        return nullptr;
    }

    // If ptr is a null pointer, return a newly allocated block.
    if (ptr == nullptr) {
        return align_alloc(newsize);
    }

    //
    // Otherwise, create a new block and copy the contents of the original
    // block up to the lesser of the new and old sizes.
    //
    size_t size = newsize > oldsize ? oldsize : newsize;
    void *mem = align_alloc(newsize);
    if (!mem) {
        return nullptr;
    }
    std::memcpy(mem, ptr, size);

    // Free the original block and return the newly created block.
    align_free(ptr);

    return mem;
}

/// ---- Templated aligned memory allocation ----------------------------------
/// @brief Allocate a block of memory of with a single object of type T, with
/// size sizeof(T) bytes and default alignment. Initialize the object by calling
/// directly the T constructor at the newly allocated address with placement new.
///
template<typename T, typename... Args>
T *align_alloc(Args&&... args)
{
    T *ptr = (T *) align_alloc(sizeof(T));
    if (!ptr) {
        throw std::runtime_error("failed to allocate");
    }

    try {
        ::new(static_cast<void *>(ptr)) T(static_cast<Args&&>(args)...);
    } catch (std::exception& e) {
        align_free(ptr);
        throw std::runtime_error(e.what());
    }

    return ptr;
}

///
/// @brief Delete a block of memory allocated by align_alloc<T> by manually
/// calling the T destructor for the object and freeing the block.
///
template<typename T>
void align_free(T *ptr)
{
    if (ptr && std::is_destructible<T>::value) {
        (*ptr).~T();
    }

    align_free(static_cast<void *>(ptr));
}

/// ---- Templated aligned array allocation -----------------------------------
/// @brief Allocate an aligned block of memory with n objects of type T, with
/// size n * sizeof(T) bytes. Initialize each element in the array by calling
/// the T constructor at the newly allocated addresses with placement new.
///
template<typename T, typename... Args>
T *align_array_alloc(size_t n, Args&&... args)
{
    if (n == 0) {
        return nullptr;
    }

    if (n > static_cast<size_t>(-1) / sizeof(T)) {
        throw std::runtime_error("invalid array length");
    }

    T *ptr = (T *) align_alloc(n * sizeof(T));
    if (!ptr) {
        throw std::runtime_error("failed to allocate");
    }

    size_t i = 0;
    try {
        for (; i < n; ++i) {
            ::new(static_cast<void *>(ptr + i)) T(static_cast<Args&&>(args)...);
        }
    } catch (std::exception& e) {
        for (size_t j = 0; j < i; ++j) {
           ptr[j].~T();
        }
        align_free(ptr);
        throw std::runtime_error(e.what());
    }

    return ptr;
}

///
/// @brief Delete a block of memory with n objects of type T, each allocated by
/// align_alloc<T> by manually calling the T destructor for each object in the
/// array before freeing the block.
///
template<typename T>
void align_array_free(T *ptr, size_t n)
{
    if (ptr == nullptr) {
        return;
    }

    if (std::is_destructible<T>::value) {
        for (size_t i = 0; i < n; ++i) {
            ptr[i].~T();
        }
    }

    align_free(ptr);
}

/// ---- Templated aligned allocator ------------------------------------------
/// @brief Minimal allocator targetting C++ Standard Library. The allocator only
/// implements the following members:
///  - copy constructor
///  - operator==
///  - operator!=
///  - allocate
///  - deallocate
/// At compile time, the C++ Standard Library uses the allocator_traits class
/// to detect which members you have explicitly provided and provides a default
/// implementation for any members that are not present.
/// @see https://learn.microsoft.com/en-us/cpp/standard-library/allocators
///
template <typename T>
struct align_allocator
{
    typedef T value_type;

    align_allocator() noexcept {}

    template<typename U>
    align_allocator(const align_allocator<U>&) noexcept {}
    template<typename U>
    bool operator==(const align_allocator<U>&) const noexcept { return true; }
    template<typename U>
    bool operator!=(const align_allocator<U>&) const noexcept { return false; }

    T *allocate(const size_t n) const;
    void deallocate(T * const p, size_t) const noexcept;
};

template <typename T>
T *align_allocator<T>::allocate(const size_t n) const
{
    if (n == 0) {
        return nullptr;
    }

    if (n > static_cast<size_t>(-1) / sizeof(T)) {
        throw std::runtime_error("invalid array length");
    }

    void * const ptr = align_alloc(n * sizeof(T));
    if (!ptr) {
        throw std::runtime_error("failed to allocate");
    }

    return static_cast<T *>(ptr);
}

template<class T>
void align_allocator<T>::deallocate(T * const ptr, size_t) const noexcept
{
    align_free(ptr);
}

} // math

#endif // MATH_MEMORY_H_
