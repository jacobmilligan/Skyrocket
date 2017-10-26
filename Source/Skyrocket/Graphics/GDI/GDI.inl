//
//  GDI.inl
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 26/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <chrono>
#include <thread>

namespace sky {


template<typename T>
void GDI::write_command(T* cmd)
{
    auto is_base = std::is_base_of<rc::Command, T>::value;
    SKY_ASSERT(is_base, "Command is derived from RenderCommand");

    auto header = static_cast<rc::Command*>(cmd)->type;
    
    cmdbufs_.get_write().write(&header);
    cmdbufs_.get_write().write(cmd);
}


}