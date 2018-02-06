//
//  Context.HPP
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <cstdint>

namespace sky {

class Viewport;


class GLContext {
public:
    GLContext()
        : glcontext_(nullptr)
    {}

    ~GLContext()
    {
        destroy();
    }

    void create(Viewport* viewport);
    void destroy();
    void make_current();
    void set_size(uint32_t width, uint32_t height);
    void swap_buffers();
    void set_swap_interval(int interval);

private:
    void* glcontext_;
};


}