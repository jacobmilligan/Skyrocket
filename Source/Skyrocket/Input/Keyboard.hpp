//
//  Keyboard.hpp
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

namespace sky {

class NativeInputListener;

class Keyboard {
public:
    Keyboard();
    bool key_down(const Key key);

    bool key_typed(const Key key);
private:
    const NativeInputListener* native_listener_;
};


}