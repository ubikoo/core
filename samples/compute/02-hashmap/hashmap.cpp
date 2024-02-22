//
// hashmap.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <algorithm>
#include <vector>

#include "common.h"
#include "hashmap.h"

///
/// @brief Clear the hash table key-value items and set their state to empty.
///
void Hashmap::clear()
{
    mNumItems = 0;
    std::fill(mData.begin(), mData.end(), KeyValue{kEmpty, kEmpty});
}

///
/// @brief Insert a key-value item into the table. Start iterating at the slot
/// given by the key hash value. If the slot key is empty, insert the value.
///
void Hashmap::insert(const uint32_t key, const uint32_t value)
{
    uint32_t slot = key % mCapacity;
    while (true) {
        uint32_t prev = compare_and_swap(&mData[slot].key, kEmpty, key);
        if (prev == kEmpty) {
            mNumItems++;
            mData[slot].value = value;
            return;
        }
        slot = (slot + 1) % mCapacity; // slot & (mCapacity - 1)
    }
}

///
/// @brief Return the first slot containing the specified key. If no key is found
/// return the empty state mask. The mask is then used to signal that no further
/// slots in the map contain the specified key.
///
uint32_t Hashmap::begin(const uint32_t key) const
{
    uint32_t slot = key % mCapacity;   // slot & (mCapacity - 1)
    while (true) {
        if (mData[slot].key == key) {
            return slot;
        }

        if (mData[slot].key == kEmpty) {
            return kEmpty;
        }

        slot = (slot + 1) % mCapacity; // slot & (mCapacity - 1)
    }
}

///
/// @brief Return the next slot containing the specified key.
///
uint32_t Hashmap::next(const uint32_t key, uint32_t slot) const
{
    while (true) {
        slot = (slot + 1) % mCapacity; // slot & (mCapacity - 1)

        if (mData[slot].key == key) {
            return slot;
        }

        if (mData[slot].key == kEmpty) {
            return kEmpty;
        }
    }
}

///
/// @brief Compare key with old value and swap with new value.
///
uint32_t Hashmap::compare_and_swap(uint32_t *key,
    uint32_t oldval, uint32_t newval)
{
    uint32_t prev = *key;
    if (*key == oldval) {
        *key = newval;
    }
    return prev;
}

///
/// @brief Create a hashmap with the next power of two of the given capacity.
///
Hashmap Hashmap::Create(const uint32_t min_capacity)
{
    // Compute the next power of two capacity
    uint32_t capacity = kMinSize;
    while (capacity <= min_capacity) {
        capacity = capacity << 1;
    }

    // Setup an empty hash table with no items.
    Hashmap hashmap;
    hashmap.mCapacity = capacity;
    hashmap.mNumItems = 0;
    hashmap.mData.resize(capacity, KeyValue{kEmpty, kEmpty});
    return hashmap;
}
