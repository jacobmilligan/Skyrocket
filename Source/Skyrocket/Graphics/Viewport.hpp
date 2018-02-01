//
//  Viewport.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 9/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Memory/Memory.hpp"
#include "Skyrocket/Platform/Platform.hpp"
#include "Skyrocket/Core/Math/Vector2.hpp"

#include <cstdint>
#include <memory>

namespace sky {

struct Color;
struct PlatformWindow;
struct NativeViewport;

class GraphicsDriver;

/// @brief Viewport is a window with a framebuffer for drawing to.
class Viewport {
public:
    using render_proc_t = void (GraphicsDriver::*)();

    Viewport();

    Viewport(const Viewport& other) = delete;
    Viewport& operator=(const Viewport& other) = delete;

    ~Viewport();

    /// @brief Creates and opens the viewport - becoming the active one
    /// @param caption
    /// @param width
    /// @param height
    void open(const char* caption, uint16_t width, uint16_t height);

    /// @brief Closes and destroys the viewport
    void close();

    /// @brief Returns the amount of currently open viewports
    /// @return
    static uint16_t open_viewports();

    /// @brief Sets the backing/clear color of the viewport
    /// @param color
    void set_backing_color(const Color& color);

    /// @brief Checks whether the viewports window requested to close in this frame
    /// @return True if close was requested, false otherwise
    bool close_requested();

    /// @brief Gets a pointer to the platform-specific native viewport object handle
    /// @return
    NativeViewport* get_native_viewport();

    /// @brief Gets the size of the viewports framebuffer
    /// @return
    Vector2f size();

    void set_vsync_enabled(GraphicsDriver* gd, render_proc_t render_proc, bool enabled);
private:
    static uint16_t open_windows_;

    const char* caption_;
    uint16_t width_;
    uint16_t height_;

    std::unique_ptr<NativeViewport> handle_;
    WindowData window_data_;

    void create_native_viewport();
    void destroy_native_viewport();
};


}  // namespace sky