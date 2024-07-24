//
// memory.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef CORE_ERROR_H_
#define CORE_ERROR_H_

#include <exception>

namespace Base {

///
/// @brief Fast-fail helper function.
///
inline void ThrowIf(bool result) {
#ifndef NDEBUG
    if (result) {
        throw std::exception();
    }
#endif
}

inline void ThrowIfNot(bool result) {
    ThrowIf(!result);
}

} // namespace Base

#endif // CORE_ERROR_H_
