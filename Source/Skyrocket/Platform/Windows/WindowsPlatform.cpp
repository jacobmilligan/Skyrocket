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

LRESULT CALLBACK global_window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

struct Platform::PlatformHandle {
	HWND window;
};

Platform::Platform()
	: handle_(new PlatformHandle)
{}

Platform::~Platform()
{
	if ( handle_ ) {
		delete handle_;
	}
}

struct EnumWindowsData {
	DWORD main_process;
	uint16_t count;
};

BOOL CALLBACK enum_windows_callback(HWND hwnd, LPARAM lparam)
{
	EnumWindowsData* data = reinterpret_cast<EnumWindowsData*>(lparam);
	DWORD process_id;
	GetWindowThreadProcessId(hwnd, &process_id);

	if ( process_id == data->main_process ) {
		data->count++;
	}

	return TRUE;
}

void Platform::startup(const char* app_title)
{
	AssertGuard assert_guard("Initializing platform for new application", app_title);

	WNDCLASS winclass = {};

	// 1. Setup the winclass with style, classname and callback

	// unique device context per window, vertical and horizontal redaw when moving or resize
	winclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = global_window_callback;
	winclass.lpszClassName = SKY_WNDCLASS_NAME;

	auto registered = RegisterClassA(&winclass);
	SKY_ASSERT(registered, "WNDCLASS successfully registered");

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

LRESULT CALLBACK global_window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	LRESULT result = 0;
	switch ( message ) {
		case WM_CLOSE:
		{
		} break;
		default:
		{
			result = DefWindowProc(window, message, wparam, lparam); // default processing for anything not handled
		} break;
	}
	return result;
}

void Platform::native_poll_event()
{
	MSG msg;
	// Remove message from queue after processing
	while ( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) ) {
		switch ( msg.message ) {
			case WM_KEYUP:
			{
				input_.key_up(msg.wParam);
			} break;

			case WM_KEYDOWN:
			{
				input_.key_down(msg.wParam);
			} break;

			default:
			{
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}
		}
	}
}


}