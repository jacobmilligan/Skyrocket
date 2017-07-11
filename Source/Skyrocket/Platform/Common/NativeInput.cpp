//
//  PlatformInput.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 10/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Core/Diagnostics/Error.hpp"
#include "Skyrocket/Platform/Platform.hpp"

namespace sky {

uint16_t NativeInputListener::key_codes_[Key::last];

bool NativeInputListener::initialized_ = false;

NativeInputListener::NativeInputListener()
    : this_frame_mask_(0), last_frame_mask_(0)
{
    if ( !initialized_ ) {
        assign_keycodes();
        initialized_ = true;
    }
}

void NativeInputListener::reset_state()
{
    last_frame_mask_ = this_frame_mask_;
    this_frame_mask_ = 0;

	if ( !window_events_.empty() ) {
		window_events_.clear();
	}
}

void NativeInputListener::request_window_close(Viewport* viewport)
{
	window_events_.push_back(WindowEvent{ viewport, EventType::window_closed });
}

void NativeInputListener::window_moved(Viewport* viewport)
{
	window_events_.push_back(WindowEvent{ viewport, EventType::window_moved });
}

void NativeInputListener::key_down(const uint16_t keycode)
{
    auto vk = key_codes_[keycode];

    key_states[vk].is_down = true;
    key_states[vk].state_changes++;

    this_frame_mask_ |= 1 << vk;
}

void NativeInputListener::key_up(const uint16_t keycode)
{
    auto vk = key_codes_[keycode];

    key_states[vk].is_down = false;
    key_states[vk].state_changes++;

    this_frame_mask_ &= ~(1 << vk);
}

const KeyState& NativeInputListener::get_key_state(const Key key) const
{
    return key_states[key];
}

const bool NativeInputListener::is_key_typed(const Key key) const
{
    return ((this_frame_mask_ >> key) & 1) == 1
        && ((last_frame_mask_ >> key) & 1) == 0;
}

bool NativeInputListener::window_event_occurred(const Viewport* viewport, const EventType event) const
{
	if ( window_events_.empty() )
		return false;

	for ( auto& ev : window_events_ ) {
		if ( ev.event == event && ev.viewport == viewport ) {
			return true;
		}
	}

	return false;
}

}
