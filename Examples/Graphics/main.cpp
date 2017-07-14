//
//  main.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 3/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Framework/Application.hpp>
#include <Skyrocket/Platform/Platform.hpp>
#include <Skyrocket/Graphics/Viewport.hpp>
#include <Skyrocket/Core/Diagnostics/Timespan.hpp>
#include <Skyrocket/Input/Keyboard.hpp>

class GraphicsApp : public sky::Application {
public:
    GraphicsApp()
        : sky::Application("Skyrocket Graphics Demo")
    {}

    void on_startup(int argc, const char** argv) override
    {
    }

    void on_update() override
    {

    }

    void on_render() override
    {

    }

    void on_keydown(sky::Key keycode) override
    {
        if ( keycode == sky::Key::escape ) {
            shutdown();
        }
    }

    void on_mouse() override
    {

    }

    void on_keyup(sky::Key keycode) override
    {
    }
};

int main(int argc, char** argv)
{
//    GraphicsApp app;
//    app.start();
	sky::Platform plat;
	plat.launch("Graphics app");
	//auto platform = sky::launch_platform("Graphics App");
    auto graphics = std::make_unique<sky::GraphicsDriver>();
	sky::Viewport view;
	view.open("Graphics App", 800, 600);

    sky::Timespan now(plat.high_resolution_time());
    sky::Timespan after;

	sky::Keyboard keyboard;

    while ( sky::Viewport::open_viewports() > 0 ) {
        now = plat.high_resolution_time();

		//sky::poll_events(platform.get());
		plat.poll_events();
        /*if ( platform->keyboard_state.key_states[sky::Key::escape].is_down || view.close_requested() ) {
            break;
        }*/

		if ( keyboard.key_down(sky::Key::escape) || view.close_requested() ) {
			break;
		}

		if ( keyboard.key_typed(sky::Key::space) ) {
			printf("Open windows: %d\n", sky::Viewport::open_viewports());
		}

		after = plat.high_resolution_time();
    }

    return 0;
}
