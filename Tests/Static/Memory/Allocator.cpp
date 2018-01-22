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

#include "catch.hpp"

TEST_CASE("FixedStackAllocator aligns memory correctly", "[fixed_stack_allocator]")
{
    sky::FixedStackAllocator allocator(1024);
    allocator.initialize();
    auto mem = allocator.allocate(sizeof(int), 4);
    auto mem2 = allocator.allocate(sizeof(int), 4);
}