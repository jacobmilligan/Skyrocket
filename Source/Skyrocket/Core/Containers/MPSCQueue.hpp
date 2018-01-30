//
//  MPSCQueue.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 30/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Memory/Allocator.hpp"

#include <cstddef>
#include <atomic>

namespace sky {


template <typename T>
class MPSCQueue {
public:
    struct Node {
        std::atomic<Node*> next;
        T value;
    };

    explicit MPSCQueue(Allocator& allocator)
        : allocator_(&allocator)
    {
        auto stub = static_cast<Node*>(allocator_->allocate(sizeof(Node), 0));
        stub->next.store(nullptr);

        stub_.store(stub, std::memory_order_relaxed);
        head_.store(stub, std::memory_order_relaxed);
        tail_.store(stub, std::memory_order_relaxed);
    }

    void push(const T& value)
    {
        auto node = static_cast<Node*>(allocator_->allocate(sizeof(Node), 0));
        node->next.store(nullptr, std::memory_order_relaxed);
        node->value = value;

        auto prev = tail_.exchange(node, std::memory_order_acq_rel);
        prev->next.store(node, std::memory_order_release);
    }

    bool pop(T& out)
    {
        auto headcpy = head_.load(std::memory_order_relaxed);
        auto next = headcpy->next.load(std::memory_order_acquire);

        if (next == nullptr) {
            return false;
        }

        out = next->value;
        head_.store(next, std::memory_order_release);
        allocator_->free(headcpy);
        return true;
    }
private:
    using cacheline_pad_t = char[64];

    Allocator* allocator_;
    cacheline_pad_t pad1{};
    std::atomic<Node*> stub_;
    cacheline_pad_t pad2{};
    std::atomic<Node*> head_;
    cacheline_pad_t pad3{};
    std::atomic<Node*> tail_;
};


} // namespace sky