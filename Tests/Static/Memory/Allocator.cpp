//
//  Allocator.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 22/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Core/Memory/StackAllocator.hpp>
#include <Skyrocket/Core/Memory/PoolAllocator.hpp>

#include "catch/catch.hpp"

TEST_CASE("Fixed stack allocated memory is aligned correctly", "[FixedStackAllocator]")
{
    constexpr size_t alignment = 4;
    sky::FixedStackAllocator allocator(sky::mebibytes(2));

    auto mem = allocator.allocate(sizeof(int), alignment);
    REQUIRE(sky::is_aligned(mem, alignment));

    mem = allocator.allocate(sizeof(int), alignment);
    REQUIRE(sky::is_aligned(mem, alignment));
}

TEST_CASE("Fixed stack allocated memory can only be aligned to powers of 2", "[FixedStackAllocator]")
{
    sky::FixedStackAllocator allocator(sky::mebibytes(2));

    auto mem = allocator.allocate(sizeof(int), 5);
    REQUIRE(mem == nullptr);

    mem = allocator.allocate(sizeof(int), 21);
    REQUIRE(mem == nullptr);
}

TEST_CASE("Fixed stack allocation is sequential", "[FixedStackAllocator]")
{
    constexpr size_t num_ptrs = 12;
    constexpr size_t alignment = 8;
    constexpr size_t increment = alignment / sizeof(int);

    sky::FixedStackAllocator allocator(sky::mebibytes(2));

    int* mem[num_ptrs];
    for (auto& i : mem) {
        i = nullptr;
        i = static_cast<int*>(allocator.allocate(sizeof(int), alignment));
    }

    for (int i = 1; i < num_ptrs; ++i) {
        INFO("Index: " << i);
        REQUIRE(mem[i] == mem[i - 1] + increment);
    }
}

TEST_CASE("Fixed stack freeing to cursor resets memory correctly", "[FixedStackAllocator]")
{
    sky::FixedStackAllocator allocator(sky::mebibytes(2));

    // Test if memory resets to zero on reset()
    auto mem1 = static_cast<int*>(allocator.allocate(sizeof(int)));
    *mem1 = 25;
    allocator.reset();
    mem1 = static_cast<int*>(allocator.allocate(sizeof(int)));
    REQUIRE(*mem1 == 0);

    // Test if memory resets to zero with char
    auto cursor = allocator.cursor();
    auto mem2 = static_cast<char*>(allocator.allocate(sizeof(char)));
    REQUIRE(*mem2 == '\0');

    // Test if memory resets to zero for all memory after cursor but not before
    *mem1 = 25;
    allocator.free_to_cursor(cursor);
    REQUIRE(*mem1 == 25);
    mem2 = static_cast<char*>(allocator.allocate(sizeof(char)));
    REQUIRE(*mem2 == '\0');
}


TEST_CASE("Fixed pool allocation is sequential", "[FixedPoolAllocator]")
{
    constexpr size_t num_ptrs = 12;
    constexpr size_t increment = sizeof(double);

    sky::FixedPoolAllocator allocator(sizeof(double), 100);

    double* mem[num_ptrs];
    for (auto& i : mem) {
        i = nullptr;
        i = static_cast<double*>(allocator.allocate(0));
    }

    for (int i = 1; i < num_ptrs; ++i) {
        INFO("Index: " << i << mem[i - 1]);
        REQUIRE(mem[i - 1] + 1 == mem[i]);
    }
}

TEST_CASE("Fixed pool freeing resets memory correctly", "[FixedPoolAllocator]")
{
    sky::FixedPoolAllocator allocator(sizeof(double), 100);

    // Test if memory resets to zero on reset()
    auto mem1 = static_cast<double*>(allocator.allocate(0));
    *mem1 = 25;
    allocator.reset();
    mem1 = static_cast<double*>(allocator.allocate(0));
    REQUIRE(*mem1 == 0.0);

    for (int i = 0; i < 100; ++i) {
        auto mem = allocator.allocate(0);
        allocator.free(mem);
        auto mem2 = allocator.allocate(0);
        REQUIRE(mem2 == mem);
        allocator.free(mem2);
    }
}

