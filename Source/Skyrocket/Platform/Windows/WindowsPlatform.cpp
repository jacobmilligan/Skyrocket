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
#include "Skyrocket/Graphics/Viewport.hpp"

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

LRESULT CALLBACK window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

void* Platform::create_native_window(const char* caption, const uint16_t width,
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

void Platform::native_init()
{
	AssertGuard assert_guard("Creating Win32 application", nullptr);
	WNDCLASS winclass = {};

	// 1. Setup the winclass with style, classname and callback
	// unique device context per window, vertical and horizontal redaw when moving or resize
	winclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = window_callback;
	winclass.lpszClassName = SKY_WNDCLASS_NAME;

	auto registered = RegisterClassA(&winclass);
	SKY_ASSERT(registered, "WNDCLASS successfully registered");
}

LRESULT CALLBACK window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	auto data = reinterpret_cast<WindowData*>(GetPropW(window, L"SKY_WINDOW"));
	LRESULT result = 0;
	switch ( message ) {
		case WM_CLOSE:
		{
			data->close_requested = true;
		} break;

		case WM_MOVE:
		{

		} break;

		default:
		{
			result = DefWindowProc(window, message, wparam, lparam); // default processing for anything not handled
		} break;
	}
	return result;
}

void Platform::native_poll_events()
{
	MSG msg;
	// Remove message from queue after processing
	while ( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) ) {
		switch ( msg.message ) {
			case WM_KEYUP:
			{
				events_.key_up(msg.wParam);
			} break;

			case WM_KEYDOWN:
			{
				events_.key_down(msg.wParam);
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