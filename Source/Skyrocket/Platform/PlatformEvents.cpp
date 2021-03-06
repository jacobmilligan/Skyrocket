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
#include "PlatformEvents.hpp"

namespace sky {

uint16_t PlatformEvents::vk_translation_table[static_cast<uint16_t>(Key::last)];

void PlatformEvents::reset_keyboard_state()
{
    std::swap(keyboard.last_frame_mask_.bits, keyboard.this_frame_mask_.bits);
    keyboard.last_frame_mask_.clear_all();
}

void PlatformEvents::key_down(const uint16_t keycode)
{
    auto vk = vk_translation_table[keycode];

    keyboard.key_states[vk].is_down = true;
    keyboard.key_states[vk].state_changes++;

    keyboard.this_frame_mask_.set_bit(vk);
}

void PlatformEvents::key_up(const uint16_t keycode)
{
    auto vk = vk_translation_table[keycode];

    keyboard.key_states[vk].is_down = false;
    keyboard.key_states[vk].state_changes++;

    keyboard.this_frame_mask_.clear_bit(vk);
}

void PlatformEvents::request_window_close(WindowData* window_data)
{
    window_data->close_requested = true;
}

void PlatformEvents::window_moved(WindowData* window_data)
{
    //window_event_store.push_back(WindowEvent{
    //	viewport,
    //	EventType::window_moved
    //});
}


}
