//
//  Memory.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 21/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <cstdint>

namespace sky {


struct MemoryBlock {
    uint32_t size;
    void* data;
};

class Noncopyable {
protected:
    Noncopyable() {};
private:
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};


}