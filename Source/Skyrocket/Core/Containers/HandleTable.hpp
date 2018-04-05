//
//  HTable.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 21/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Diagnostics/Error.hpp"

#include <algorithm>
#include <cinttypes>
#include <cstdint>

namespace sky {


/// @brief Groups some data with an integer ID
/// @tparam T The type of data stored in the handle
template<typename T>
struct Handle {
    uint32_t id{0};
    T data{T()};
};

/// @brief A static table of integer id -> data mappings. Avoids the overhead of a hashmap
/// by allocating upfront and using array indexing for faster lookup. It doesn't handle
/// generation of ID's but only maps them to handles.
/// @tparam T The type of data to store in the table
/// @tparam Capacity The maximum capacity of the table
template<typename T, uint32_t Capacity>
class HandleTable {
public:
    /// @brief ID number that represents an invalid handle. This should be used for error
    /// checking
    static constexpr uint32_t invalid_id = 0;

    /// @brief Allocates a number of default instances of the specified data type
    /// equal to the capacity of the table.
    HandleTable()
        :
        indicies_()
    {
        for ( uint32_t i = 0; i < Capacity; ++i ) {
            indicies_[i] = i;
        }
    }

    /// @brief Gets the tables capacity
    /// @return Capacity
    uint32_t capacity()
    {
        return Capacity;
    }

    /// @brief Gets the number of elements active in the table
    /// @return The size of the table
    uint32_t size()
    {
        return count_;
    }

    /// @brief Checks if the table contains the id specified
    /// @param id
    /// @return
    bool contains(const uint32_t id)
    {
        return id < Capacity && indicies_[id] < count_;
    }

    /// @brief Creates a new handle with the id specified, copying the data into
    /// its entry in the table
    /// @param id
    /// @param data
    T* create(const uint32_t id, const T& data)
    {
        if ( count_ + 1 >= Capacity || count_ + 1 == invalid_id ) {
            SKY_ERROR("HandleTable", "Creating new handle while capacity (%"
                PRIu32
                ") is reached", Capacity);
            return nullptr;
        }

        if ( contains(id) ) {
            SKY_ERROR("HandleTable", "Creating new handle with existing (%"
                PRIu32
                ")", id);
            return nullptr;
        }

        if ( id == invalid_id || id >= Capacity ) {
            SKY_ERROR("HandleTable", "Creating new handle with invalid ID (%"
                PRIu32
                ")", id);
            return nullptr;
        }

        auto index = count_;
        indicies_[id] = index;

        handles_[index].id = id;
        handles_[index].data = data;

        count_++;

        return get(id);
    }

    /// @brief Creates a new handle in the table with the ID specified using the default
    /// constructor of the tables data type to create the data
    /// @param id
    T* create(const uint32_t id)
    {
        return create(id, T());
    }

    /// @brief Destroys an element and erases it from the table
    /// @param id
    void destroy(const uint32_t id)
    {
        if ( !contains(id) ) {
            SKY_ERROR("HandleTable", "Trying to destroy invalid id (%"
                PRIu32
                ")", id);
            return;
        }

        auto index = indicies_[id];
        std::swap(handles_[index], handles_[count_ - 1]);
        indicies_[index] = invalid_id;
        indicies_[count_ - 1] = index;
        --count_;
    }

    /// @brief Returns a pointer to the first element in the raw data buffer
    /// @return Pointer to the first raw data element
    Handle<T>* begin()
    {
        return &handles_[0];
    }

    /// @brief Returns a pointer to one past the last element in the raw data buffer
    /// @return Pointer to one past the last data element
    Handle<T>* end()
    {
        return handles_ + count_;
    }

    /// @brief Returns a pointer to the element associated with the given id
    /// @param id
    /// @return Pointer to the element
    T* get(const uint32_t id)
    {
        SKY_ASSERT(contains(id), "(%" PRIu32 ") is a valid ID", id);
        return &handles_[indicies_[id]].data;
    }

private:
    uint32_t count_{0};
    Handle<T> handles_[Capacity];
    uint32_t indicies_[Capacity];
};


} // namespace sky
