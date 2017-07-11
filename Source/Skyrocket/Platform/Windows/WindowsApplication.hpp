//
//  WindowsApplication.hpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 11/07/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <windows.h>

namespace sky {

class NativeInputListener;

class WindowsApplication {
public:
	WindowsApplication(NativeInputListener* input_listener);

	void initialize();

	static LRESULT CALLBACK window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

	void pump_messages();
private:
	static NativeInputListener* input_;

};


}