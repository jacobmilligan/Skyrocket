//
//  Keyboard.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 14/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Platform/Platform.hpp"
#include "Skyrocket/Input/Keyboard.hpp"

namespace sky {


bool Keyboard::key_down(const Key key)
{
    auto code = static_cast<uint16_t>(key);
    return Platform::get_events().keyboard.key_states[code].is_down;
}

bool Keyboard::key_typed(const Key key)
{
    auto code = static_cast<uint16_t>(key);
    auto keyboard = &Platform::get_events().keyboard;

    return keyboard->this_frame_mask_.is_set(code)
        && !keyboard->last_frame_mask_.is_set(code);
}


}