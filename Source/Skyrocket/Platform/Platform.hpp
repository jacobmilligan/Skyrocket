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

#include "Skyrocket/IO/Keycodes.hpp"
#include "Skyrocket/Core/Math/Vector2.hpp"

#include <cstdint>

namespace sky {

class Application;
struct PlatformHandle;

class Platform {
public:
    friend class Application;

    Platform(Application* app);

    /// @brief Destroys all windows, applications etc. associated with the
    /// platform
    ~Platform();

    /// @brief Initializes a WindowHandle with the specified caption, width, and height
    /// using a platform-specific implementation.
    /// @param caption The caption to assign to the windows title bar
    /// @param width The width of the window to create
    /// @param height The height of the window to create
    uint16_t create_window(const char* caption, const int width, const int height);

    /// @brief Checks to see if the specified WindowHandle's platform-specific window
    /// is currently open or closed
    /// @return True if the window is open, false otherwise
    bool open_window_count();

    static sky::Key get_vk(const uint16_t native_key);
private:
    void initialize(const char* app_title);

    /// @brief Polls the platform-specific window for events and fills the event queue
    void poll_events();

    /// @brief Sets the specified WindowHandle as the current GL context to use for
    /// render calls
    void set_current_context();

    /// @brief Gets the size of the platform-specific windows framebuffer in pixels.
    /// @details Different platforms implement windows in vastly different ways and not
    /// all platforms define their windows using the same relative sizing. The windows
    /// framebuffer size is a more accurate way to get the relative pixel size of a
    /// given viewport and can be safely used with graphics libraries
    /// @return The framebuffer size
    sky::Vector2i get_framebuffer_size();

    /// @brief Swaps the WindowHandle's back buffer with its front buffer using
    /// platform-specific GL implementation
    void swap_buffers();

    /// @brief Requests a close event from the platform-specific window.
    /// @details A close request isn't the same as actually closing a window, it's up
    /// the platform to define what to do in the event of a close request. It may result
    /// in the window being destroyed on PC platforms, or in returning to the home
    /// screen on mobile devices.
    void request_close();

    /// @brief Creates a keycode translation table for translating the current platforms
    /// keycode specification to Skyrockets Key code implementation
    void create_keycode_table(Key* keys, const uint32_t table_size);

    /// @brief Shows or hides the mouse cursor within the bounds of the window
    /// @param displayed Shows the cursor if true, hides it otherwise
    void show_cursor(const bool displayed);

    /// @brief Moves the mouse cursor to the specified position within the
    /// platform-specific window
    /// @param x The x position to move the mouse cursor to
    /// @param y The y position to move the mouse cursor to
    void move_cursor(const double x, const double y);

    /// @brief Centers the mouse cursor within the platform-specific window
    void center_cursor();

    /// @brief Checks whether or not the specified key is currently held down
    /// @param key The Key to check input for
    /// @return The type of input event that represents the Keys current state
    bool is_key_down(const sky::Key key);

    /// @brief Enables or disables VSync in the platform-specific window.
    /// @param enabled Sets VSync enabled if true, false otherwise
    void set_vsync(const bool enabled);

    void set_title(const char* title);

    void
    resize_window(const uint32_t width, const uint32_t height);

    PlatformHandle* handle_;
    static Application* app_;
    static sky::Key keycode_table_[static_cast<uint16_t>(Key::last)];

    void setup_keycodes();
};


}