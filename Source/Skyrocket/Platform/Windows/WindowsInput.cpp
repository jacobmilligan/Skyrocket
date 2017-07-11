//
//  WindowsInput.cpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 10/07/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Platform/NativeInput.hpp"

namespace sky {


void NativeInputListener::assign_keycodes()
{
	key_codes_[0x41] = Key::A;
	key_codes_[0x42] = Key::B;
	key_codes_[0x43] = Key::C;
	key_codes_[0x44] = Key::D;
	key_codes_[0x45] = Key::E;
	key_codes_[0x46] = Key::F;
	key_codes_[0x47] = Key::G;
	key_codes_[0x48] = Key::H;
	key_codes_[0x49] = Key::I;
	key_codes_[0x4A] = Key::J;
	key_codes_[0x4B] = Key::K;
	key_codes_[0x4C] = Key::L;
	key_codes_[0x4D] = Key::M;
	key_codes_[0x4E] = Key::N;
	key_codes_[0x4F] = Key::O;
	key_codes_[0x50] = Key::P;
	key_codes_[0x51] = Key::Q;
	key_codes_[0x52] = Key::R;
	key_codes_[0x53] = Key::S;
	key_codes_[0x54] = Key::T;
	key_codes_[0x55] = Key::U;
	key_codes_[0x56] = Key::V;
	key_codes_[0x57] = Key::W;
	key_codes_[0x58] = Key::X;
	key_codes_[0x59] = Key::Y;
	key_codes_[0x5A] = Key::Z;
	key_codes_[0x30] = Key::num0;
	key_codes_[0x31] = Key::num1;
	key_codes_[0x32] = Key::num2;
	key_codes_[0x33] = Key::num3;
	key_codes_[0x34] = Key::num4;
	key_codes_[0x35] = Key::num5;
	key_codes_[0x36] = Key::num6;
	key_codes_[0x37] = Key::num7;
	key_codes_[0x38] = Key::num8;
	key_codes_[0x39] = Key::num9;

	key_codes_[0xBB] = Key::equal;
	key_codes_[0xBD] = Key::minus;
	key_codes_[0xDD] = Key::right_bracket;
	key_codes_[0xDB] = Key::left_bracket;
	key_codes_[0xDE] = Key::apostrophe;
	key_codes_[0xBA] = Key::semicolon;
	key_codes_[0xDB] = Key::backslash;
	key_codes_[0xBC] = Key::comma;
	key_codes_[0xBF] = Key::slash;
	key_codes_[0xBE] = Key::period;
	key_codes_[0xC0] = Key::grave_accent;
	key_codes_[0x90] = Key::num_lock;
	key_codes_[0x6E] = Key::keypad_decimal;
	key_codes_[0x6A] = Key::keypad_multiply;
	key_codes_[0x6B] = Key::keypad_plus;
	key_codes_[0x6F] = Key::keypad_divide;
	// key_codes_[0x] = Key::keypad_enter; windows defines keypad enter as the same as return. Will need to check for this using scancodes
	key_codes_[0x6D] = Key::keypad_minus;
	// key_codes_[0x51] = Key::keypad_equals; No such definition on windows
	key_codes_[0x60] = Key::keypad_0;
	key_codes_[0x61] = Key::keypad_1;
	key_codes_[0x62] = Key::keypad_2;
	key_codes_[0x63] = Key::keypad_3;
	key_codes_[0x64] = Key::keypad_4;
	key_codes_[0x65] = Key::keypad_5;
	key_codes_[0x66] = Key::keypad_6;
	key_codes_[0x67] = Key::keypad_7;
	key_codes_[0x68] = Key::keypad_8;
	key_codes_[0x69] = Key::keypad_9;

	// Keys independent of keyboard layout
	key_codes_[0x0D] = Key::enter;
	key_codes_[0x09] = Key::tab;
	key_codes_[0x20] = Key::space;
	key_codes_[0x2E] = Key::delete_key;
	key_codes_[0x1B] = Key::escape;
	key_codes_[0x5B] = Key::left_super; // left windows key
	key_codes_[0xA0] = Key::left_shift;
	key_codes_[0x14] = Key::caps_lock;
	key_codes_[0xA4] = Key::left_alt;
	key_codes_[0xA2] = Key::left_control;
	key_codes_[0x5C] = Key::right_super;
	key_codes_[0xA1] = Key::right_shift;
	key_codes_[0xA5] = Key::right_alt;
	key_codes_[0xA3] = Key::right_control;

	key_codes_[0x70] = Key::f1;
	key_codes_[0x71] = Key::f2;
	key_codes_[0x72] = Key::f3;
	key_codes_[0x73] = Key::f4;
	key_codes_[0x74] = Key::f5;
	key_codes_[0x75] = Key::f6;
	key_codes_[0x76] = Key::f7;
	key_codes_[0x77] = Key::f8;
	key_codes_[0x78] = Key::f9;
	key_codes_[0x79] = Key::f10;
	key_codes_[0x7A] = Key::f11;
	key_codes_[0x7B] = Key::f12;
	key_codes_[0x7C] = Key::f13;
	key_codes_[0x7D] = Key::f14;
	key_codes_[0x7E] = Key::f15;
	key_codes_[0x7F] = Key::f16;
	key_codes_[0x80] = Key::f17;
	key_codes_[0x81] = Key::f18;
	key_codes_[0x82] = Key::f19;
	key_codes_[0x83] = Key::f20;

	key_codes_[0x2D] = Key::insert;
	key_codes_[0x24] = Key::home;
	key_codes_[0x21] = Key::page_up;
	key_codes_[0x2E] = Key::delete_key;
	key_codes_[0x23] = Key::end;
	key_codes_[0x22] = Key::page_down;

	key_codes_[0x25] = Key::left;
	key_codes_[0x27] = Key::right;
	key_codes_[0x28] = Key::down;
	key_codes_[0x26] = Key::up;
}


}