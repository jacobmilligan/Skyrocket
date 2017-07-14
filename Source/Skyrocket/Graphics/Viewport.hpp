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

#include "Skyrocket/Platform/Platform.hpp"
#include "Skyrocket/Core/Memory/Noncopyable.hpp"

#include <cstdint>
#include <memory>

namespace sky {

struct Color;
struct PlatformWindow;

class Viewport : public Noncopyable {
public:
    friend class GraphicsDriver;

    Viewport() {}

	~Viewport()
	{
		close();
	}

    void open(const char* caption, const uint16_t width, const uint16_t height);

	void close();

	static uint16_t open_viewports();

    void set_backing_color(const Color& color);

	bool close_requested();
private:
	static uint16_t open_windows_;

    const char* caption_;
    uint16_t width_;
    uint16_t height_;

    struct NativeViewport;

    NativeViewport* handle_;
	WindowData window_data_;

	void create_native_viewport();
	void destroy_native_viewport();
};


}