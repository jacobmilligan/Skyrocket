//
//  Murmur3Tests.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 13/10/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Core/Hash.hpp>

#include "catch.hpp"

TEST_CASE("test_murmur3_x86_32", "[murmur3]")
{
    auto test_string = [](const std::string str, const uint32_t seed, const uint32_t expected)
    {
        auto hash = sky::hash::murmur3_32(str.data(), static_cast<uint32_t>(str.size()), seed);
        REQUIRE(hash == expected);
    };

    SECTION("test empty string with zero seed gives zero")
    {
        test_string("", 0, 0);
    }

    SECTION("test ignores nearly all the math")
    {
        test_string("", 1, 0x514E28B7);
    }

    SECTION("test seed value is handled unsigned")
    {
        test_string("", 0xffffffff, 0x81F16F39);
    }

    SECTION("test one full chunk")
    {
        test_string("aaaa", 0x9747b28c, 0x5A97808A);
    }

    //Endian order within the chunks
    SECTION("test one full chunk")
    {
        test_string("abcd", 0x9747b28c, 0xF0478627);
    } //one full chunk

    SECTION("test utf-8 characters")
    {
        //U+03C0: Greek Small Letter Pi
        test_string("ππππππππ", 0x9747b28c, 0xD58063C1);
    }
}