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

#include "Skyrocket/Core/Memory.hpp"
#include "Skyrocket/Platform/Platform.hpp"

#include <cstdint>
#include <memory>

namespace sky {

struct Color;
struct PlatformWindow;
struct NativeViewport;
class GDI;

class Viewport {
public:
    Viewport();

	Viewport(const Viewport& other) = delete;
	Viewport& operator=(const Viewport& other) = delete;

	~Viewport();

    void open(const char* caption, const uint16_t width, const uint16_t height);

	void close();

	static uint16_t open_viewports();

    void set_backing_color(const Color& color);

	bool close_requested();

    NativeViewport* get_native_viewport();
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