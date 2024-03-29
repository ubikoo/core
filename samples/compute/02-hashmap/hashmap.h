//
// hashmap.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <vector>

///
/// @brief Hashmap maintains an array of key-value items using open addressing
/// with linear probing to handle collision resolution of keys with the same
/// hash value.
///
/// During insertion, the key is hashed and the resulting hash value indicates
/// where the corresponding value is to be stored.
/// Collision resolution is handled by iterating through the hash table array,
/// starting at the key's initial slot location.
/// At each slot in the table, perform an atomic compare-and-swap of the slot
/// key value with empty. If true (ie if the slot key value is empty), update
/// the slot with the new key value and return the slot's original key (empty
/// or not).
///
struct Hashmap {
    // Hashmap key-value item type.
    struct KeyValue {
        uint32_t key;
        uint32_t value;
    };

    // Member variables.
    uint32_t mCapacity;             // max number of items in the table
    uint32_t mNumItems;             // number of items in the table
    std::vector<KeyValue> mData;    // hashmap table

    // Return the max numnber of key-value items in the hash table.
    const uint32_t &capacity() const { return mCapacity; }

    // Return the number of key-value items in the hash table.
    const uint32_t &size() const { return mNumItems; }

    // Return a reference to the hashmap underlying data array.
    const std::vector<KeyValue> &data() const { return mData; }

    // Clear the hash table key-value items and set their state to empty.
    void clear();

    // Insert a key-value item into the table.
    void insert(const uint32_t key, const uint32_t value);

    // Return the first slot containing the specified key.
    uint32_t begin(const uint32_t key) const;

    // Return the past-the-end element indicating an empty slot.
    uint32_t end() const { return kEmpty; }

    // Return the next slot containing the specified key.
    uint32_t next(const uint32_t key, uint32_t slot) const;

    // Return the value of the current slot.
    uint32_t get(const uint32_t slot) const { return mData[slot].value; }

    // Compare key with old value and swap with new value.
    uint32_t compare_and_swap(uint32_t *key, uint32_t oldval, uint32_t newval);

    // Hashmap factory function.
    static Hashmap Create(const uint32_t min_capacity);
};

#endif // HASHMAP_H_
