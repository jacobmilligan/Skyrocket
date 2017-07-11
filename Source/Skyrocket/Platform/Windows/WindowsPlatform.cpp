//
//  WindowsPlatform.cpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 10/07/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Diagnostics/Error.hpp"
#include "Skyrocket/Platform/Platform.hpp"
#include "Skyrocket/Platform/Windows/WindowsApplication.hpp"

#include <windows.h>

#define SKY_WNDCLASS_NAME "SkyrocketWindow"

extern int main(int argc, char** argv);

namespace sky {

int CALLBACK WinMain(
	HINSTANCE	instance,
	HINSTANCE	prev_instance,
	LPSTR		cmd_line,
	int			cmd_show
)
{
	return main(__argc, __argv);
}

struct Platform::PlatformHandle {
	WindowsApplication app;
};

Platform::Platform()
{
	handle_ = new PlatformHandle{ WindowsApplication(&input_) };
}

Platform::~Platform()
{
	if ( handle_ ) {
		delete handle_;
	}
}

void Platform::startup(const char* app_title)
{
	AssertGuard assert_guard("Initializing platform for new application", app_title);

	WNDCLASS winclass = {};

	// 1. Setup the winclass with style, classname and callback

	// unique device context per window, vertical and horizontal redaw when moving or resize
	winclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = handle_->app.window_callback;
	winclass.lpszClassName = SKY_WNDCLASS_NAME;

	auto registered = RegisterClassA(&winclass);
	SKY_ASSERT(registered, "WNDCLASS successfully registered");

	handle_->app.initialize();

	initialized_ = true;
}

void* Platform::new_native_window(const char* caption, const uint16_t width,
								  const uint16_t height)
{
	AssertGuard guard("Creating a new skyrocket HWND instance", SKY_WNDCLASS_NAME);

	auto app_module = GetModuleHandle(nullptr);
	SKY_ASSERT(app_module != NULL, "Handle to the apps Win32 module is found");

	HWND window = CreateWindowEx(0, 
								 SKY_WNDCLASS_NAME, 
								 caption, 
								 WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
								 0, 0, 
								 width, height, 
								 nullptr, nullptr, 
								 app_module, 
								 nullptr);

	SKY_ASSERT(window != NULL, "HWND successfully was created with caption %s", caption);

	return window;
}

void Platform::native_poll_event()
{
	handle_->app.pump_messages();
}


}