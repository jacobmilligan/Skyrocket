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
struct NativeHandle;

class Renderer;

/// @brief Viewport is a window with a framebuffer for drawing to.
class Viewport {
public:
    Viewport();

    Viewport(const Viewport& other) = delete;
    Viewport& operator=(const Viewport& other) = delete;

    ~Viewport();

    /// @brief Creates and opens the viewport - becoming the active one
    /// @param caption
    /// @param width
    /// @param height
    void open(const Renderer& renderer, const char* caption, uint16_t width, uint16_t height);

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
    NativeHandle* get_native_handle();

    /// @brief Gets the size of the viewports framebuffer
    /// @return
    Vector2f size();
private:
    static uint16_t open_windows_;

    const char* caption_;
    uint16_t width_;
    uint16_t height_;

    std::unique_ptr<NativeHandle> handle_;
    WindowData window_data_;

    void create_native_viewport(const Renderer& renderer);
    void destroy_native_viewport();
};


}  // namespace sky