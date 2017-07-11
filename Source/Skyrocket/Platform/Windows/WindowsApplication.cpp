//
//  WindowsApplication.cpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 11/07/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Diagnostics/Error.hpp"
#include "Skyrocket/Platform/NativeInput.hpp"
#include "Skyrocket/Platform/Windows/WindowsApplication.hpp"

namespace sky {

NativeInputListener* WindowsApplication::input_ = nullptr;

WindowsApplication::WindowsApplication(NativeInputListener* input_listener)
{
	input_ = input_listener;
}

void WindowsApplication::initialize()
{

}

LRESULT CALLBACK WindowsApplication::window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	LRESULT result = 0;
	switch ( message ) {
		case WM_CLOSE:
		{
			auto viewport = reinterpret_cast<Viewport*>(GetPropW(window, L"SKY_VIEWPORT"));
			input_->request_window_close(viewport);
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

void WindowsApplication::pump_messages()
{
	AssertGuard guard("Pumping windows messages", nullptr);

	SKY_ASSERT(input_ != nullptr, "Application has a input listener");

	MSG msg;
	// Remove message from queue after processing
	while ( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) ) {
		switch ( msg.message ) {
			case WM_KEYUP:
			{
				input_->key_up(msg.wParam);
			} break;

			case WM_KEYDOWN:
			{
				input_->key_down(msg.wParam);
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