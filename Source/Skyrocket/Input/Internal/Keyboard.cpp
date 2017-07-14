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
	auto this_frame = Platform::get_events().keyboard.this_frame_mask_;
	auto last_frame = Platform::get_events().keyboard.last_frame_mask_;

	return ((this_frame >> code) & 1) == 1
		&& ((last_frame >> code) & 1) == 0;
}


}