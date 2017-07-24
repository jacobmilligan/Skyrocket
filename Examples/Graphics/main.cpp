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

#include <Skyrocket/Core/Containers/HandleTable.hpp>
#include <Skyrocket/Core/Diagnostics/Timespan.hpp>
#include <Skyrocket/Framework/Application.hpp>
#include <Skyrocket/Graphics/Viewport.hpp>
#include <Skyrocket/Input/Keyboard.hpp>
#include <Skyrocket/Platform/Platform.hpp>

#include <Skyrocket/Graphics/Core/Vertex.hpp>
#include <Skyrocket/Platform/Filesystem.hpp>

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

int sum(int a)
{
    return a;
}

int main(int argc, char** argv)
{
//    GraphicsApp app;
//    app.start();
    auto resources = sky::Path(sky::Path::executable_path().parent());
    resources.append("../../..");
    resources.append("Resources");
    printf("%s\n", resources.str());

    sky::Platform platform;
	platform.launch("Graphics app");
	sky::Viewport view;
	view.open("Graphics App", 800, 600);
    auto graphics = sky::create_graphics_device_interface();
    graphics->initialize();
    graphics->set_viewport(&view);

    sky::Timespan now(sky::high_resolution_time());
    sky::Timespan after;

	sky::Keyboard keyboard;

    sky::Vertex vertices[3];
    vertices[0].position = sky::Vector4f(0.0f, 1.0f, 0.0f);
    vertices[1].position = sky::Vector4f(-1.0f, -1.0f, 0.0f);
    vertices[2].position = sky::Vector4f(1.0f, 1.0f, 0.0f);

    sky::MemoryBlock mem = {};
    mem.size = sizeof(sky::Vertex) * 3;
    mem.data = malloc(mem.size);
    memcpy(mem.data, static_cast<void*>(vertices), static_cast<size_t>(mem.size));

    auto vbuf_id = graphics->create_vertex_buffer(mem, sky::BufferUsage::staticbuf);

    while ( sky::Viewport::open_viewports() > 0 ) {
        now = sky::high_resolution_time();

		platform.poll_events();

		if ( keyboard.key_down(sky::Key::escape) || view.close_requested() ) {
			break;
		}

		if ( keyboard.key_typed(sky::Key::space) ) {
			printf("Open windows: %d\n", sky::Viewport::open_viewports());
		}

		after = sky::high_resolution_time();
    }

    free(mem.data);

    return 0;
}
