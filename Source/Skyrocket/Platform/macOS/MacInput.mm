//
//  Input.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 10/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Platform/NativeInput.hpp"

namespace sky {

void NativeInputListener::assign_keycodes()
{
    key_codes_[0x00] = sky::Key::A;
    key_codes_[0x01] = sky::Key::S;
    key_codes_[0x02] = sky::Key::D;
    key_codes_[0x03] = sky::Key::F;
    key_codes_[0x04] = sky::Key::H;
    key_codes_[0x05] = sky::Key::G;
    key_codes_[0x06] = sky::Key::Z;
    key_codes_[0x07] = sky::Key::X;
    key_codes_[0x08] = sky::Key::C;
    key_codes_[0x09] = sky::Key::V;
    key_codes_[0x0B] = sky::Key::B;
    key_codes_[0x0C] = sky::Key::Q;
    key_codes_[0x0D] = sky::Key::W;
    key_codes_[0x0E] = sky::Key::E;
    key_codes_[0x0F] = sky::Key::R;
    key_codes_[0x10] = sky::Key::Y;
    key_codes_[0x11] = sky::Key::T;
    key_codes_[0x12] = sky::Key::num1;
    key_codes_[0x13] = sky::Key::num2;
    key_codes_[0x14] = sky::Key::num3;
    key_codes_[0x15] = sky::Key::num4;
    key_codes_[0x16] = sky::Key::num6;
    key_codes_[0x17] = sky::Key::num5;
    key_codes_[0x18] = sky::Key::equal;
    key_codes_[0x19] = sky::Key::num9;
    key_codes_[0x1A] = sky::Key::num7;
    key_codes_[0x1B] = sky::Key::minus;
    key_codes_[0x1C] = sky::Key::num8;
    key_codes_[0x1D] = sky::Key::num0;
    key_codes_[0x1E] = sky::Key::right_bracket;
    key_codes_[0x1F] = sky::Key::O;
    key_codes_[0x20] = sky::Key::U;
    key_codes_[0x21] = sky::Key::left_bracket;
    key_codes_[0x22] = sky::Key::I;
    key_codes_[0x23] = sky::Key::P;
    key_codes_[0x25] = sky::Key::L;
    key_codes_[0x26] = sky::Key::J;
    key_codes_[0x27] = sky::Key::apostrophe;
    key_codes_[0x28] = sky::Key::K;
    key_codes_[0x29] = sky::Key::semicolon;
    key_codes_[0x2A] = sky::Key::backslash;
    key_codes_[0x2B] = sky::Key::comma;
    key_codes_[0x2C] = sky::Key::slash;
    key_codes_[0x2D] = sky::Key::N;
    key_codes_[0x2E] = sky::Key::M;
    key_codes_[0x2F] = sky::Key::period;
    key_codes_[0x32] = sky::Key::grave_accent;
    key_codes_[0x41] = sky::Key::keypad_decimal;
    key_codes_[0x43] = sky::Key::keypad_multiply;
    key_codes_[0x45] = sky::Key::keypad_plus;
    key_codes_[0x47] = sky::Key::num_lock;
    key_codes_[0x4B] = sky::Key::keypad_divide;
    key_codes_[0x4C] = sky::Key::keypad_enter;
    key_codes_[0x4E] = sky::Key::keypad_minus;
    key_codes_[0x51] = sky::Key::keypad_equals;
    key_codes_[0x52] = sky::Key::keypad_0;
    key_codes_[0x53] = sky::Key::keypad_1;
    key_codes_[0x54] = sky::Key::keypad_2;
    key_codes_[0x55] = sky::Key::keypad_3;
    key_codes_[0x56] = sky::Key::keypad_4;
    key_codes_[0x57] = sky::Key::keypad_5;
    key_codes_[0x58] = sky::Key::keypad_6;
    key_codes_[0x59] = sky::Key::keypad_7;
    key_codes_[0x5B] = sky::Key::keypad_8;
    key_codes_[0x5C] = sky::Key::keypad_9;

// Keys independent of keyboard layout
    key_codes_[0x24] = sky::Key::enter;
    key_codes_[0x30] = sky::Key::tab;
    key_codes_[0x31] = sky::Key::space;
    key_codes_[0x33] = sky::Key::delete_key;
    key_codes_[0x35] = sky::Key::escape;
    key_codes_[0x37] = sky::Key::left_super;
    key_codes_[0x38] = sky::Key::left_shift;
    key_codes_[0x39] = sky::Key::caps_lock;
    key_codes_[0x3A] = sky::Key::left_alt; // osx - left option
    key_codes_[0x3B] = sky::Key::left_control;
    key_codes_[0x36] = sky::Key::right_super;
    key_codes_[0x3C] = sky::Key::right_shift;
    key_codes_[0x3D] = sky::Key::right_alt;
    key_codes_[0x3E] = sky::Key::right_control;
//  key_codes_[0x3F] = sky::Key::function;
    key_codes_[0x40] = sky::Key::f17;
//  key_codes_[0x48] = sky::Key::volumeup;
//  key_codes_[0x49] = sky::Key::volumedown;
//  key_codes_[0x4A] = sky::Key::mute;
    key_codes_[0x4F] = sky::Key::f18;
    key_codes_[0x50] = sky::Key::f19;
    key_codes_[0x5A] = sky::Key::f20;
    key_codes_[0x60] = sky::Key::f5;
    key_codes_[0x61] = sky::Key::f6;
    key_codes_[0x62] = sky::Key::f7;
    key_codes_[0x63] = sky::Key::f3;
    key_codes_[0x64] = sky::Key::f8;
    key_codes_[0x65] = sky::Key::f9;
    key_codes_[0x67] = sky::Key::f11;
    key_codes_[0x69] = sky::Key::f13;
    key_codes_[0x6A] = sky::Key::f16;
    key_codes_[0x6B] = sky::Key::f14;
    key_codes_[0x6D] = sky::Key::f10;
    key_codes_[0x6F] = sky::Key::f12;
    key_codes_[0x71] = sky::Key::f15;
    key_codes_[0x72] = sky::Key::insert;
    key_codes_[0x73] = sky::Key::home;
    key_codes_[0x74] = sky::Key::page_up;
    key_codes_[0x75] = sky::Key::delete_key;
    key_codes_[0x76] = sky::Key::f4;
    key_codes_[0x77] = sky::Key::end;
    key_codes_[0x78] = sky::Key::f2;
    key_codes_[0x79] = sky::Key::page_down;
    key_codes_[0x7A] = sky::Key::f1;
    key_codes_[0x7B] = sky::Key::left;
    key_codes_[0x7C] = sky::Key::right;
    key_codes_[0x7D] = sky::Key::down;
    key_codes_[0x7E] = sky::Key::up;
}


}
