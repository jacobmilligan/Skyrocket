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

#include <cinttypes>
#include <cstdint>
#include <algorithm>

namespace sky {

template <typename T>
struct Handle {
    uint32_t id { 0 };
    T data { T() };
};

template <typename T, uint32_t Capacity>
class HandleTable {
public:
    static constexpr uint32_t invalid_id = 0;

    HandleTable()
        : indicies_()
    {
        for ( uint32_t i = 0; i < Capacity; ++i ) {
            indicies_[i] = i;
        }
    }

    uint32_t capacity()
    {
        return Capacity;
    }

    uint32_t size()
    {
        return count_;
    }

    bool contains(const uint32_t id)
    {
        return id < Capacity && indicies_[id] < count_;
    }

    void create(const uint32_t id, const T& data)
    {
        if ( count_ + 1 >= Capacity || count_ + 1 == invalid_id ) {
            SKY_ERROR("HandleTable", "Creating new handle while capacity (%" PRIu32 ") is reached", Capacity);
            return;
        }

        if ( contains(id) ) {
            SKY_ERROR("HandleTable", "Creating new handle with existing (%" PRIu32 ")", id);
            return;
        }

        if ( id == invalid_id || id >= Capacity ) {
            SKY_ERROR("HandleTable", "Creating new handle with invalid ID (%" PRIu32 ")", id);
            return;
        }

        auto index = count_;
        indicies_[id] = index;

        handles_[index].id = id;
        handles_[index].data = data;

        count_++;
    }

    void create(const uint32_t id)
    {
        create(id, T());
    }

    void destroy(const uint32_t id)
    {
        if ( !contains(id) ) {
            SKY_ERROR("HandleTable", "Trying to destroy invalid id (%" PRIu32 ")", id);
            return;
        }

        auto index = indicies_[id];
        std::swap(handles_[index], handles_[count_ - 1]);
        indicies_[index] = invalid_id;
        indicies_[count_ - 1] = index;
        --count_;
    }

    Handle<T>* begin()
    {
        return &handles_[0];
    }

    Handle<T>* end()
    {
        return handles_ + count_;
    }

    T* lookup(const uint32_t id)
    {
        if ( !contains(id) ) {
            SKY_ERROR("HandleTable", "Trying to lookup invalid id (%" PRIu32 ")", id);
            return nullptr;
        }

        return &handles_[ indicies_[id] ].data;
    }

private:
    uint32_t count_ { 0 };
    Handle<T> handles_[Capacity];
    uint32_t indicies_[Capacity];
};


} // namespace sky