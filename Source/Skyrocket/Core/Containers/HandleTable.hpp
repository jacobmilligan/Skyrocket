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

#include <cstdint>
#include <cinttypes>

namespace sky {

template <typename T>
struct Handle {
    Handle()
        : id(UINT32_MAX), data(T())
    {}

    uint32_t id;
    T data;
};

template <typename T, uint32_t Capacity>
class HandleTable {
public:
    static constexpr uint32_t invalid_id = UINT32_MAX;

    HandleTable()
        : count_(0), next_id_(0)
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

    bool valid(const uint32_t id)
    {
        return indicies_[id] < count_;
    }

    uint32_t create(const T& data)
    {
        if ( count_ + 1 >= Capacity || count_ + 1 == invalid_id ) {
            SKY_ERROR("HandleTable", "Trying to create a new handle while capacity is reached");
            return invalid_id;
        }

        auto index = count_;
        indicies_[next_id_] = index;

        handles_[index].id = next_id_;
        handles_[index].data = data;

        next_id_++;
        count_++;

        return handles_[index].id;
    }

    void destroy(const uint32_t id)
    {
        if ( !valid(id) ) {
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
        if ( !valid(id) ) {
            SKY_ERROR("HandleTable", "Trying to lookup invalid id (%" PRIu32 ")", id);
            return nullptr;
        }

        return &handles_[ indicies_[id] ].data;
    }

private:
    uint32_t count_;
    uint32_t next_id_;
    Handle<T> handles_[Capacity];
    uint32_t indicies_[Capacity];
};


}