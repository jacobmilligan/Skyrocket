//
//  BitsetTest.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 3/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Core/Containers/Bitset.hpp>
#include <Skyrocket/Core/Diagnostics/Stopwatch.hpp>

#include "catch/catch.hpp"

constexpr size_t bitset_size = UINT8_MAX;
using test_bitset_t = sky::Bitset<bitset_size>;

TEST_CASE("Bits are set correctly", "[bitset]")
{
    test_bitset_t bitset;

    for (size_t i = 0; i < bitset_size; ++i) {
        bitset.set_bit(i);
        REQUIRE(bitset.is_set(i));
        if (i < bitset_size - 1) {
            REQUIRE(!bitset.is_set(i + 1));
        }
    }
}

TEST_CASE("Setting all bits is done correctly", "[bitset]")
{
    test_bitset_t bitset;
    bitset.set_all();
    for (int i = 0; i < bitset_size; ++i) {
        REQUIRE(bitset.is_set(i));
    }
}

TEST_CASE("Clearing all bits is done correctly", "[bitset]")
{
    test_bitset_t bitset;
    bitset.clear_all();
    for (int i = 0; i < bitset_size; ++i) {
        REQUIRE(!bitset.is_set(i));
    }
}

TEST_CASE("Bits are cleared correctly", "[bitset]")
{
    test_bitset_t bitset;
    bitset.set_all();
    for (size_t i = 0; i < bitset_size; ++i) {
        bitset.clear_bit(i);
        REQUIRE(!bitset.is_set(i));
        if (i < bitset_size - 1) {
            REQUIRE(bitset.is_set(i + 1));
        }
    }
}

TEST_CASE("Bits are toggled correctly", "[bitset]")
{
    test_bitset_t bitset;
    bitset.set_all();
    for (size_t i = 0; i < bitset_size; ++i) {
        bitset.toggle_bit(i);
        REQUIRE(!bitset.is_set(i));
        bitset.toggle_bit(i);
        REQUIRE(bitset.is_set(i));
    }
}