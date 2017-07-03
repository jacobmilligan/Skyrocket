//
//  Keyboard.hpp
//  Launchpad
//
//  Created by Jacob Milligan on 22/06/2017.
//  Copyright © 2017 Jacob Milligan. All rights reserved.
//

#pragma once

#include <cstdint>

namespace sky {


class Keyboard {
public:
    static const uint16_t keycount = 400;
    
    Keyboard();
private:
    static uint16_t keycodes_[keycount];
};
    
    
}
