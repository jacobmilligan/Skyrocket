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

#include "Skyrocket/Core/Diagnostics/Error.hpp"
#include "Skyrocket/Platform/Platform.hpp"


namespace sky {


PlatformEvents Platform::events_;

void Platform::launch(const char* app_title, update_callback_t update_callback)
{
    if ( !initialized_ ) {
        app_title_ = app_title;
        update_ = update_callback;
        native_init();
        initialized_ = true;
    }
}

void Platform::poll_events()
{
    AssertGuard guard("Polling events", nullptr);
    SKY_ASSERT(initialized_, "Platform is initialized before polling events");

    events_.reset_keyboard_state();
    native_poll_events();
}


}