//
//  Keycodes.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 4/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <cstdint>

namespace sky {


/// @brief Enumeration of all available modifiers to use for keyboard events.
/// A modifier is different to a Key code in that modifiers are considered as
/// a part of the same event as a Key, modifying them, whereas a Key event can only
/// have one keyboard key at any given time
enum class ModifierKey {
    /// @brief The left/right shift key modifier
    shift = 0x0001,
    /// @brief The left/right control key modifier
    control = 0x0002,
    /// @brief The left/right alt key modifier
    alt = 0x0004,
    /// @brief The left/right super key modifier - this is usually the windows key
    /// on Windows or linux systems with a Windows keyboard or the option
    /// key on macOS
    super = 0x0008
};

/// @brief Enumeration of all keycodes on a US standard keyboard. The keycodes map
/// to the ASCII standard with non-ASCII characters allocated to the 256+ range
enum class Key {
    unknown = -1,
    space = 32,
    apostrophe = 39,
    comma = 44,
    minus,
    period,
    slash,
    num0,
    num1,
    num2,
    num3,
    num4,
    num5,
    num6,
    num7,
    num8,
    num9,
    semicolon = 59,
    equal = 61,
    A = 65,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    left_bracket,
    backslash,
    right_bracket,
    grave_accent = 96,
    /// @brief Keycode to use for non-US mapped key
    international_1 = 161,
    /// @brief Keycode to use for non-US mapped key
    international_2,
    escape = 256,
    enter,
    tab,
    backspace,
    insert,
    delete_key,
    right,
    left,
    down,
    up,
    page_up,
    page_down,
    home,
    end,
    caps_lock = 280,
    scroll_lock,
    num_lock,
    print_screen,
    pause,
    f1 = 290,
    f2,
    f3,
    f4,
    f5,
    f6,
    f7,
    f8,
    f9,
    f10,
    f11,
    f12,
    f13,
    f14,
    f15,
    f16,
    f17,
    f18,
    f19,
    f20,
    f21,
    f22,
    f23,
    f24,
    f25,
    keypad_0 = 320,
    keypad_1,
    keypad_2,
    keypad_3,
    keypad_4,
    keypad_5,
    keypad_6,
    keypad_7,
    keypad_8,
    keypad_9,
    keypad_decimal,
    keypad_divide,
    keypad_multiply,
    keypad_minus,
    keypad_plus,
    keypad_enter,
    keypad_equals,
    left_shift = 340,
    left_control,
    left_alt,
    left_super,
    right_shift,
    right_control,
    right_alt,
    right_super,
    menu,
    last = static_cast<int>(menu)
};

}