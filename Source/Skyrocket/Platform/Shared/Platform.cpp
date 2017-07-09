//
//  Platform.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 8/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Platform/Platform.hpp"

namespace sky {

Key Platform::keycode_table_[RawInputState::num_keys];
RawInputState Platform::input_;
bool Platform::initialized_ = false;

void Platform::poll_events()
{
    RawKeyState* current = input_.current_key_states;
    RawKeyState* last = input_.last_key_states;
    last = current;

    native_poll_event();
}

bool Platform::key_down(const uint16_t keycode)
{
    return input_.current_key_states[keycode].isdown;
}

bool Platform::key_typed(const uint16_t keycode)
{
    return input_.current_key_states[keycode].isdown
        && !input_.last_key_states[keycode].isdown;
}


}