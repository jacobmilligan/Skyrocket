//
//  Align.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 22/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Core/Memory/Memory.hpp>

#include "catch/catch.hpp"

TEST_CASE("Align correctly aligns to powers of two", "[align]")
{
    void* aligned_mem = nullptr;

    for (size_t i = 1; i < sky::mebibytes(1); ++i) {
        aligned_mem = sky::align(malloc(sizeof(int)), i);
        auto correct_alignment = sky::is_aligned(aligned_mem, i);
        if (!sky::is_power_of_two(i)) {
            continue;
        }

        INFO("Current alignment: " << i);
        REQUIRE(correct_alignment);
    }
}

TEST_CASE("Align returns same pointer for single-byte aligned memory", "[align]")
{
    auto ptr = malloc(sizeof(int));
    auto aligned_mem = sky::align(ptr, 1);
    REQUIRE(ptr == aligned_mem);
}

TEST_CASE("Align returns nullptr for zero", "[align]")
{
    auto ptr = malloc(sizeof(int));
    auto aligned_mem = sky::align(ptr, 0);
    REQUIRE(aligned_mem == nullptr);
}

