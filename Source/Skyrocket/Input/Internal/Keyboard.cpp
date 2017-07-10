//
//  Keyboard.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 10/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Input/Keyboard.hpp"
#include "Skyrocket/Platform/Platform.hpp"

namespace sky {


Keyboard::Keyboard()
    : native_listener_(Platform::get_native_input_listener())
{}

bool Keyboard::key_down(const Key key)
{
    return native_listener_->get_key_state(key).is_down;
}

bool Keyboard::key_typed(const Key key)
{
    return native_listener_->is_key_typed(key);
}


}