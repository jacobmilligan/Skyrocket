//
//  OS.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 4/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

// Include platform headers
#include "Skyrocket/Platform/Time.hpp"

#include "Skyrocket/Platform/Internal/PlatformEvents.hpp"
#include "Skyrocket/Platform/Config.hpp"
#include "Skyrocket/Input/Keycodes.hpp"

#include <cstdint>
#include <memory>

namespace sky {


/// @brief Platform initializes the native application and window and manages OS-specific
/// application calls.
class Platform {
public:
    Platform() = default;

    /// @brief Launches a new application and window for the current platform
    /// @param app_title
    void launch(const char* app_title);

    /// @brief Creates a new native window object, returning it as a void*
    /// @param caption
    /// @param width
    /// @param height
    /// @return raw void* to the native window object
    static void* create_native_window(const char* caption, const uint16_t width,
                                      const uint16_t height);

    /// @brief Polls the input and registers all events
    void poll_events();

    /// @brief Checks whether the platform was initialized or not
    /// @return True if initialized, false otherwise
    bool initialized()
    {
        return initialized_;
    }

    /// @brief Gets a reference to the struct that holds all the events that were registered in the
    /// previous frame
    /// @return
    static const PlatformEvents& get_events()
    {
        return events_;
    }

private:
    bool initialized_{};
    static PlatformEvents events_;

    const char* app_title_{};

    void native_init();
    void native_poll_events();
};


} // namespace sky