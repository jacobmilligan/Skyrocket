//
//  WindowsViewport.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 11/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Diagnostics/Error.hpp"
#include "Skyrocket/Platform/Platform.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"

#include <windows.h>

namespace sky {


struct Viewport::NativeViewport {
	HWND window;
};

void Viewport::destroy_native_viewport()
{
	if ( handle_ ) {
		delete handle_;
	}
}

void Viewport::create_native_viewport()
{
	AssertGuard guard("Creating a native viewport", nullptr);

	SKY_ASSERT(Platform::initialized(), "Platform is initialized");

	handle_ = new NativeViewport;
	handle_->window = static_cast<HWND>(Platform::create_native_window(caption_, width_, height_));
	SetPropW(handle_->window, L"SKY_WINDOW", &window_data_);
}

void Viewport::set_backing_color(const sky::Color &color)
{

}


}