//
//  GraphicsDriver.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 23/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/GDI/Experimental/GraphicsDriver.hpp"

namespace sky {
namespace experimental {


CommandBuffer* GraphicsDriver::make_command_buffer()
{
    CommandBuffer* buf = nullptr;

    for (int i = 0; i < cmdbuf_max_; ++i) {
        if (cmdbufs_[i].state() == CommandBuffer::State::ready) {
            buf = &cmdbufs_[i];
            buf->reset();
            break;
        }
    }

    if (buf == nullptr) {
        SKY_ERROR("GraphicsDriver", "Could not make new command buffer");
    }

    return buf;
}


} // namespace experimental
} // namespace sky