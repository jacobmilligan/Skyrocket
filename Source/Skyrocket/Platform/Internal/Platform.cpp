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

bool Platform::initialized_ = false;
PlatformEvents Platform::events_;

void Platform::launch(const char* app_title)
{
	app_title_ = app_title;
	native_init();
	initialized_ = true;
}

void Platform::poll_events()
{
	events_.reset_keyboard_state();
	native_poll_events();
}




}