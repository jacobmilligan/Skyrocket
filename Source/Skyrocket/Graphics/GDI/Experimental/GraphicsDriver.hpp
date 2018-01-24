//
//  GraphicsDriver.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 23/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Graphics/GDI/Experimental/CommandBuffer.hpp"

namespace sky {
namespace experimental {


class GraphicsDriver {
public:
    CommandBuffer* make_command_buffer();
private:
    static constexpr size_t cmdbuf_max_ = 16;

    std::array<CommandBuffer, cmdbuf_max_> cmdbufs_;
};


} // namespace experimental
} // namespace sky