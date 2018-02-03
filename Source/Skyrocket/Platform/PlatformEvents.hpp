//
//  PlatformEvents.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 10/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Input/Keycodes.hpp"
#include "Skyrocket/Core/Containers/Bitset.hpp"

#include <cstdint>
#include <vector>

namespace sky {

class Viewport;

struct KeyState {
    KeyState()
        : is_down(false), state_changes(0)
    {}

    bool is_down;
    uint32_t state_changes;
};

struct KeyboardState {
    KeyboardState()
        : this_frame_mask_(0), last_frame_mask_(0)
    {}

    KeyState key_states[static_cast<uint16_t>(Key::last)];
    uint16_t this_frame_mask_;
    uint16_t last_frame_mask_;
    Bitset<static_cast<size_t>(Key::last)> mask_a;
};

struct WindowData {
    WindowData()
        : close_requested(false)
    {}

    bool close_requested;
};

struct PlatformEvents {
    KeyboardState keyboard;

    PlatformEvents() noexcept;

    void reset_keyboard_state();

    void key_down(uint16_t keycode);

    void key_up(uint16_t keycode);

    void request_window_close(WindowData* window_data);

    void window_moved(WindowData* window_data);

private:
    static uint16_t vk_translation_table[static_cast<uint16_t>(Key::last)];
};


}