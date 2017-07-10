//
//  Buttons.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 4/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

namespace sky {


/// @brief Enumeration of all mouse buttons available on the current platform
enum class Button {
    button1,
    button2,
    button3,
    button4,
    button5,
    button6,
    button7,
    button8,
    last = static_cast<int>(button8),
    left = static_cast<int>(button1),
    right = static_cast<int>(button2),
    middle = static_cast<int>(button3)
};


}