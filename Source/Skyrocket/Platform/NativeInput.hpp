//
//  PlatformInput.hpp
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

#include <cstdint>

namespace sky {

struct KeyState {
    KeyState()
        : is_down(false), state_changes(0)
    {}

    bool is_down;
    uint32_t state_changes;
};


class NativeInputListener {
public:
    NativeInputListener();

    const KeyState& get_key_state(const Key key) const;
    const bool is_key_typed(const Key key) const;

    void reset_state();

    void key_down(const uint16_t keycode);

    void key_up(const uint16_t keycode);

private:

    static bool initialized_;

    static uint16_t key_codes_[Key::last];

    KeyState key_states[Key::last];
    uint16_t this_frame_mask_;
    uint16_t last_frame_mask_;

    void assign_keycodes();
};


}