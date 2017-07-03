//
//  IDLookup.hpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 25/02/2017.
//  Copyright (c) 2017 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Diagnostics/SkyAssert.hpp"
#include "Diagnostics/ErrorMessage.hpp"
#include "ConditionalTypes.hpp"

namespace sky {

/// @brief IDHandle is a wrapper used to tag an object or value with an ID for storage
/// inside containers that use integer-based fast ID-tagged storage and lookup of
/// data
/// @tparam T The data type stored within the handle
/// @tparam ID_T The data type used for each ID
template <typename T, typename ID_T>
struct IDHandle {
    ID_T id;
    T item;
};

/// @brief IDLookup is a lookup container that uses indirection to allow fast
/// lookup of id-tagged objects and values while keeping the stored data tightly
/// packed in sequential memory.
/// @details The IDLookup array uses two static buffers, one for indices and another
/// for the actual stored data, all allocated up-front at construction time.
/// This allows for fast iteration, insertion and removal operations at the penalty of a
/// slightly slower lookup time than an std::unordered_map equivalent.
///
/// As a consequence of allocating all memory up-front, objects stored within an
/// IDLookup array must either specify a default constructor or be a POD struct
/// (recommended), furthermore, it's recommended that objects intended to be stored within
/// either be as small as possible or do the minimal amount of allocations within their
/// default constructor.
/// @tparam T The type stored within the data buffer
/// @tparam Size The maximum amount of objects to store in the data buffer
template <typename T, uint64_t Size>
class IDLookup {
    /// @brief The type used for lookups into the index array
    using lookup_t = large_conditional_uint<Size + 1>;
public:
    /// @brief The type used as ID's and indices
    using index_t = conditional_uint<Size  + 1>;
    /// @brief The defined IDHandle type
    using handle_t = IDHandle<T, index_t>;

    /// @brief Default constructor, allocates all default objects and indices
    IDLookup()
        : max_(static_cast<index_t>(Size + 1)), size_(0)
    {
        for ( index_t i = 0; i < max_; ++i ) {
            indices_[i] = i;
        }

        data_[max_ - 1].id = max_;
    }

    /// @brief Gets the upper bounds of the IDLookup
    /// @return The upper bounds
    uint64_t max_size()
    {
        return Size;
    }

    /// @brief Gets the current amount of stored objects
    /// @return The amount of stored objects
    index_t size()
    {
        return size_;
    }

    /// @brief Inserts a new object into the IDLookup data buffer using the objects
    /// copy constructor. This
    /// @param item Item to add to the lookup
    /// @param id The id to assign to the lookup
    void add(const T& item, const lookup_t id)
    {
        SKY_ASSERT(id < Size, "ID %d is out of the IDLookup's bounds (%d)", id, Size);

        if ( contains(id) ) {
            SKY_ERROR("IDLookup", "The IDLookup already contains ID %d", id);
            return;
        }

        data_[size_].id = static_cast<index_t>(id);
        data_[size_].item = item;
        indices_[id] = size_;
        size_++;
    }

    /// @brief Removes the item with the given id from the lookup if it exists.
    /// @param id ID of the item to remove
    void remove(const lookup_t id)
    {
        SKY_ASSERT(id < Size, "ID %d is out of the IDLookup's bounds (%d)", id, Size);

        auto& index = indices_[id];
        if ( !contains(index) ) {
            SKY_ERROR("IDLookup", "Removing non-existent ID %d from IDLookup", id);
            return;
        }

        std::swap(data_[index], data_[size_ - 1]);
        indices_[id] = static_cast<index_t>(Size);
        size_--;
    }

    /// @brief Finds an object in the data buffer if it exists. If it doesn't exist,
    /// the last item in the data buffer will be returned.
    /// @details If an item doesn't exist, the lookup() function returns the item at
    /// position data[max_size() + 1], this allows error checking for a null reference
    /// return without copying or allocating a new handle if the item doesn't exist as
    /// the lookup always keeps an extra handle just for error handling and other
    /// internal operations. This error checking can be done like so:
    /// @code{.cpp}
    /// auto& handle = lookup.lookup(2);
    /// if ( handle.id == lookup.max_size() + 1 ) {
    ///     printf("Doesn't exist!\n");
    /// }
    /// @endcode
    /// lookup() will also print an error if you try and lookup an item that doesn't
    /// exist, which is intended to encourage the programmer to only call lookup() for
    /// items they know definitely exist in the array. For checking if an item exists or
    /// not, prefer using the contains(lookup_t id) function.
    /// @param id The ID of the object to lookup
    /// @return The item with the specified ID if found, otherwise the item at position
    /// data[max_size() + 1] with an id of max_size() + 1
    handle_t& lookup(const lookup_t id)
    {
        SKY_ASSERT(id < Size, "ID %d is out of the IDLookup's bounds (%d)", id, Size);

        auto& index = indices_[id];

        if ( index >= size_ ) {
            SKY_ERROR("IDLookup", "No data found with an ID of %d", index);
            return data_[Size];
        }

        return data_[index];
    }

    /// @brief Gets the index in the data buffer of a specified ID. Returns max_size() + 1
    /// if the item doesn't exist
    /// @param id ID of the object to get the index of
    /// @return The index into the data buffer if it exists, max_size() + 1 otherwise
    index_t index_of(const lookup_t id)
    {
        SKY_ASSERT(id < Size, "ID %d is out of the IDLookup's bounds (%d)", id, Size);

        return indices_[id];
    }

    /// @brief Checks to see if an object with the specified ID exists in the data buffer
    /// @param id ID of the object to check for
    /// @return True if the object exists, false otherwise
    bool contains(const lookup_t id)
    {
        return id < Size && index_of(id) < size_;
    }

    /// @brief Pointer to the start of the data buffer. For compatibility with range-based
    /// for loops and iterators.
    /// @return Pointer to the start of the data buffer
    handle_t* begin()
    {
        return &data_[0];
    }

    /// @brief Pointer to the last active element in the data buffer (position data[size()]).
    /// For compatibility with range-based for loops and iterators.
    /// @return Pointer to the end of the data buffer
    handle_t* end()
    {
        return &data_[size_];
    }

    /// @brief Subscript overload, returns the item at the specified position in the data
    /// array, not the handle
    /// @param index Index to access
    /// @return Item at the specified position in the data buffer, not the handle
    T const& operator[](const lookup_t index)
    {
        SKY_ASSERT(index < size_, "Index %d is greater than the IDLookup's size (%d)",
                   index, size_);
        return data_[index].item;
    }

private:
    index_t max_;
    index_t size_;
    index_t indices_[Size + 1];
    handle_t data_[Size + 1];
};


}