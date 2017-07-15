//
//  OS.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 4/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

// Include platform headers
#include "Skyrocket/Platform/Time.hpp"

#include "Skyrocket/Platform/Internal/PlatformEvents.hpp"
#include "Skyrocket/Platform/Config.hpp"
#include "Skyrocket/Input/Keycodes.hpp"

#include <cstdint>
#include <memory>

namespace sky {

class Platform {
public:
	Platform()
	{}

	void launch(const char* app_title);

	static void* create_native_window(const char* caption, const uint16_t width,
                                      const uint16_t height);

	void poll_events();

	bool initialized()
	{
		return initialized_;
	}

	static const PlatformEvents& get_events()
	{
		return events_;
	}

private:
	bool initialized_;
	static PlatformEvents events_;

	const char* app_title_;

	void native_init();
	void native_poll_events();
};


}