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

NativeInputListener Platform::input_;
bool Platform::initialized_ = false;

void Platform::poll_events()
{
    input_.reset_state();
    native_poll_event();
}


}