//
//  Stopwatch.hpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 13/12/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "SkyTime.hpp"

namespace sky {

/// @brief Stopwatch defines a set of functionality for recording
/// elapsed time values
class Stopwatch {
public:
    /// @brief Initializes the Stopwatch with a value of zero
    Stopwatch();
    
    /// @brief Starts the Stopwatch and begins recording elapsed time
    void start();
    
    /// @brief Restarts the stopwatch and returns the recorded elapsed time value
    /// @return The elapsed time
    Time restart();
    
    /// @brief Gets the currently elapsed time value
    /// @return The elapsed time
    Time elapsed();
private:
    /// @brief Internal representation of the currently elapsed time value
    Time time_;
};


}
