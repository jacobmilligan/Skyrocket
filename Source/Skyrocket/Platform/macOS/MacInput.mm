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

#include "Skyrocket/Platform/Internal/PlatformEvents.hpp"

namespace sky {

PlatformEvents::PlatformEvents()
{
    vk_translation_table[0x00] = static_cast<uint16_t>(Key::A);
    vk_translation_table[0x01] = static_cast<uint16_t>(Key::S);
    vk_translation_table[0x02] = static_cast<uint16_t>(Key::D);
    vk_translation_table[0x03] = static_cast<uint16_t>(Key::F);
    vk_translation_table[0x04] = static_cast<uint16_t>(Key::H);
    vk_translation_table[0x05] = static_cast<uint16_t>(Key::G);
    vk_translation_table[0x06] = static_cast<uint16_t>(Key::Z);
    vk_translation_table[0x07] = static_cast<uint16_t>(Key::X);
    vk_translation_table[0x08] = static_cast<uint16_t>(Key::C);
    vk_translation_table[0x09] = static_cast<uint16_t>(Key::V);
    vk_translation_table[0x0B] = static_cast<uint16_t>(Key::B);
    vk_translation_table[0x0C] = static_cast<uint16_t>(Key::Q);
    vk_translation_table[0x0D] = static_cast<uint16_t>(Key::W);
    vk_translation_table[0x0E] = static_cast<uint16_t>(Key::E);
    vk_translation_table[0x0F] = static_cast<uint16_t>(Key::R);
    vk_translation_table[0x10] = static_cast<uint16_t>(Key::Y);
    vk_translation_table[0x11] = static_cast<uint16_t>(Key::T);
    vk_translation_table[0x12] = static_cast<uint16_t>(Key::num1);
    vk_translation_table[0x13] = static_cast<uint16_t>(Key::num2);
    vk_translation_table[0x14] = static_cast<uint16_t>(Key::num3);
    vk_translation_table[0x15] = static_cast<uint16_t>(Key::num4);
    vk_translation_table[0x16] = static_cast<uint16_t>(Key::num6);
    vk_translation_table[0x17] = static_cast<uint16_t>(Key::num5);
    vk_translation_table[0x18] = static_cast<uint16_t>(Key::equal);
    vk_translation_table[0x19] = static_cast<uint16_t>(Key::num9);
    vk_translation_table[0x1A] = static_cast<uint16_t>(Key::num7);
    vk_translation_table[0x1B] = static_cast<uint16_t>(Key::minus);
    vk_translation_table[0x1C] = static_cast<uint16_t>(Key::num8);
    vk_translation_table[0x1D] = static_cast<uint16_t>(Key::num0);
    vk_translation_table[0x1E] = static_cast<uint16_t>(Key::right_bracket);
    vk_translation_table[0x1F] = static_cast<uint16_t>(Key::O);
    vk_translation_table[0x20] = static_cast<uint16_t>(Key::U);
    vk_translation_table[0x21] = static_cast<uint16_t>(Key::left_bracket);
    vk_translation_table[0x22] = static_cast<uint16_t>(Key::I);
    vk_translation_table[0x23] = static_cast<uint16_t>(Key::P);
    vk_translation_table[0x25] = static_cast<uint16_t>(Key::L);
    vk_translation_table[0x26] = static_cast<uint16_t>(Key::J);
    vk_translation_table[0x27] = static_cast<uint16_t>(Key::apostrophe);
    vk_translation_table[0x28] = static_cast<uint16_t>(Key::K);
    vk_translation_table[0x29] = static_cast<uint16_t>(Key::semicolon);
    vk_translation_table[0x2A] = static_cast<uint16_t>(Key::backslash);
    vk_translation_table[0x2B] = static_cast<uint16_t>(Key::comma);
    vk_translation_table[0x2C] = static_cast<uint16_t>(Key::slash);
    vk_translation_table[0x2D] = static_cast<uint16_t>(Key::N);
    vk_translation_table[0x2E] = static_cast<uint16_t>(Key::M);
    vk_translation_table[0x2F] = static_cast<uint16_t>(Key::period);
    vk_translation_table[0x32] = static_cast<uint16_t>(Key::grave_accent);
    vk_translation_table[0x41] = static_cast<uint16_t>(Key::keypad_decimal);
    vk_translation_table[0x43] = static_cast<uint16_t>(Key::keypad_multiply);
    vk_translation_table[0x45] = static_cast<uint16_t>(Key::keypad_plus);
    vk_translation_table[0x47] = static_cast<uint16_t>(Key::num_lock);
    vk_translation_table[0x4B] = static_cast<uint16_t>(Key::keypad_divide);
    vk_translation_table[0x4C] = static_cast<uint16_t>(Key::keypad_enter);
    vk_translation_table[0x4E] = static_cast<uint16_t>(Key::keypad_minus);
    vk_translation_table[0x51] = static_cast<uint16_t>(Key::keypad_equals);
    vk_translation_table[0x52] = static_cast<uint16_t>(Key::keypad_0);
    vk_translation_table[0x53] = static_cast<uint16_t>(Key::keypad_1);
    vk_translation_table[0x54] = static_cast<uint16_t>(Key::keypad_2);
    vk_translation_table[0x55] = static_cast<uint16_t>(Key::keypad_3);
    vk_translation_table[0x56] = static_cast<uint16_t>(Key::keypad_4);
    vk_translation_table[0x57] = static_cast<uint16_t>(Key::keypad_5);
    vk_translation_table[0x58] = static_cast<uint16_t>(Key::keypad_6);
    vk_translation_table[0x59] = static_cast<uint16_t>(Key::keypad_7);
    vk_translation_table[0x5B] = static_cast<uint16_t>(Key::keypad_8);
    vk_translation_table[0x5C] = static_cast<uint16_t>(Key::keypad_9);
    
    // Keys independent of keyboard layout
    vk_translation_table[0x24] = static_cast<uint16_t>(Key::enter);
    vk_translation_table[0x30] = static_cast<uint16_t>(Key::tab);
    vk_translation_table[0x31] = static_cast<uint16_t>(Key::space);
    vk_translation_table[0x33] = static_cast<uint16_t>(Key::delete_key);
    vk_translation_table[0x35] = static_cast<uint16_t>(Key::escape);
    vk_translation_table[0x37] = static_cast<uint16_t>(Key::left_super);
    vk_translation_table[0x38] = static_cast<uint16_t>(Key::left_shift);
    vk_translation_table[0x39] = static_cast<uint16_t>(Key::caps_lock);
    vk_translation_table[0x3A] = static_cast<uint16_t>(Key::left_alt); // osx - left option;
    vk_translation_table[0x3B] = static_cast<uint16_t>(Key::left_control);
    vk_translation_table[0x36] = static_cast<uint16_t>(Key::right_super);
    vk_translation_table[0x3C] = static_cast<uint16_t>(Key::right_shift);
    vk_translation_table[0x3D] = static_cast<uint16_t>(Key::right_alt);
    vk_translation_table[0x3E] = static_cast<uint16_t>(Key::right_control);
//  vk_translation_table[0x3F] = static_cast<uint16_t>(Key::function);
    vk_translation_table[0x40] = static_cast<uint16_t>(Key::f17);
//  vk_translation_table[0x48] = static_cast<uint16_t>(Key::volumeup);
//  vk_translation_table[0x49] = static_cast<uint16_t>(Key::volumedown);
//  vk_translation_table[0x4A] = static_cast<uint16_t>(Key::mute);
    vk_translation_table[0x4F] = static_cast<uint16_t>(Key::f18);
    vk_translation_table[0x50] = static_cast<uint16_t>(Key::f19);
    vk_translation_table[0x5A] = static_cast<uint16_t>(Key::f20);
    vk_translation_table[0x60] = static_cast<uint16_t>(Key::f5);
    vk_translation_table[0x61] = static_cast<uint16_t>(Key::f6);
    vk_translation_table[0x62] = static_cast<uint16_t>(Key::f7);
    vk_translation_table[0x63] = static_cast<uint16_t>(Key::f3);
    vk_translation_table[0x64] = static_cast<uint16_t>(Key::f8);
    vk_translation_table[0x65] = static_cast<uint16_t>(Key::f9);
    vk_translation_table[0x67] = static_cast<uint16_t>(Key::f11);
    vk_translation_table[0x69] = static_cast<uint16_t>(Key::f13);
    vk_translation_table[0x6A] = static_cast<uint16_t>(Key::f16);
    vk_translation_table[0x6B] = static_cast<uint16_t>(Key::f14);
    vk_translation_table[0x6D] = static_cast<uint16_t>(Key::f10);
    vk_translation_table[0x6F] = static_cast<uint16_t>(Key::f12);
    vk_translation_table[0x71] = static_cast<uint16_t>(Key::f15);
    vk_translation_table[0x72] = static_cast<uint16_t>(Key::insert);
    vk_translation_table[0x73] = static_cast<uint16_t>(Key::home);
    vk_translation_table[0x74] = static_cast<uint16_t>(Key::page_up);
    vk_translation_table[0x75] = static_cast<uint16_t>(Key::delete_key);
    vk_translation_table[0x76] = static_cast<uint16_t>(Key::f4);
    vk_translation_table[0x77] = static_cast<uint16_t>(Key::end);
    vk_translation_table[0x78] = static_cast<uint16_t>(Key::f2);
    vk_translation_table[0x79] = static_cast<uint16_t>(Key::page_down);
    vk_translation_table[0x7A] = static_cast<uint16_t>(Key::f1);
    vk_translation_table[0x7B] = static_cast<uint16_t>(Key::left);
    vk_translation_table[0x7C] = static_cast<uint16_t>(Key::right);
    vk_translation_table[0x7D] = static_cast<uint16_t>(Key::down);
    vk_translation_table[0x7E] = static_cast<uint16_t>(Key::up);
}


}
