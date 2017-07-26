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

namespace sky {


template <typename T>
void GDI::write_command(T& cmd)
{
    auto is_base = std::is_base_of<rc::Command, T>::value;
    SKY_ASSERT(is_base, "Command is derived from RenderCommand");

    auto header = static_cast<rc::Command&>(cmd).type;
    cmdbufs[cur_buf].write(&header);
    cmdbufs[cur_buf].write(&cmd);
}


}