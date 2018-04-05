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


template <typename T, uint32_t Size>
class MPSCQueue {
public:
    struct Node {
        std::atomic<Node*> next;
        T value;
    };

    MPSCQueue()
    {
        auto stub = new Node;
        stub->next.store(nullptr);

        stub_.store(stub, std::memory_order_relaxed);
        head_.store(stub, std::memory_order_relaxed);
        tail_.store(stub, std::memory_order_relaxed);
    }

    void push(const T& value)
    {
        auto node = &buffer_[++next_];
        node->next.store(nullptr, std::memory_order_relaxed);
        node->value = value;

        auto prev = tail_.exchange(node, std::memory_order_acq_rel);
        prev->next.store(node, std::memory_order_release);
    }

    bool pop(T& out)
    {
        auto next = next_.load(std::memory_order_acquire);
        if (next == 0) {
            return false;
        }
        next_.store(next - 1, std::memory_order_release);

        auto headcpy = head_.load(std::memory_order_relaxed);
        auto nextnode = headcpy->next.load(std::memory_order_acquire);

        if (nextnode == nullptr) {
            return false;
        }

        out = nextnode->value;
        head_.store(nextnode, std::memory_order_release);
        return true;
    }

    uint32_t size_approx() const
    {
        return next_.load(std::memory_order_relaxed);
    }
private:
    using cacheline_pad_t = char[64];

    Node buffer_[Size];
    cacheline_pad_t pad0{};
    std::atomic<uint32_t> next_{};
    cacheline_pad_t pad1{};
    std::atomic<Node*> stub_;
    cacheline_pad_t pad2{};
    std::atomic<Node*> head_;
    cacheline_pad_t pad3{};
    std::atomic<Node*> tail_;
};


} // namespace sky