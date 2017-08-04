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
#include <Skyrocket/Graphics/Core/Vertex.hpp>
#include <Skyrocket/Graphics/GraphicsDriver.hpp>
#include <Skyrocket/Input/Keyboard.hpp>
#include <Skyrocket/Platform/Filesystem.hpp>

#include <vector>
#include <Skyrocket/Core/Containers/Buffer.hpp>

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
//    std::string name = "Jacob";
//    std::string last = "Milligan";
//
//    sky::Buffer<sizeof(std::string) * 32> buf;
//
//    std::string str;
//    auto running = true;
//
//    while ( running ) {
//        buf.write<std::string>(&name);
//        buf.write<std::string>(&last);
//
//        buf.reset();
//
//        while ( buf.cursor_pos() != buf.end() ) {
//            auto str = buf.read<std::string>();
//
//            if ( str->empty() ) {
//                running = false;
//            }
//
//            auto correct = true;
//            auto pos = buf.cursor_pos() / sizeof(std::string);
//            if ( pos == 1 && *str != name ) {
//                correct = false;
//            }
//            if ( pos == 2 && *str != last ) {
//                correct = false;
//            }
//            printf("%s\r", str->c_str() );
//        }
//
//        buf.reset();
//    }
//
//    return 0;


    const char* app_name = "Skyrocket Graphics Example";
    sky::Platform platform;
	platform.launch(app_name);
	sky::Viewport view;
	view.open(app_name, 800, 600);

    sky::GraphicsDriver renderer(sky::GraphicsDriver::ThreadSupport::multithreaded);

    if ( !renderer.initialize(view) ) {
        SKY_ERROR(app_name, "Couldn't initialize graphics device interface");
        return 0;
    }

	sky::Keyboard keyboard;

    std::vector<sky::Vertex> vertices = {
        sky::Vertex {
            .position = sky::Vector4f(0.0f, 0.5f, 0.0f, 1.0f),
            .color = sky::Vector4f(1.0f, 0.0f, 0.0f, 1.0f)
        },
        sky::Vertex {
            .position = sky::Vector4f(-0.5f, -0.5f, 0.0f, 1.0f),
            .color = sky::Vector4f(0.0f, 1.0f, 0.0f, 1.0f)
        },
        sky::Vertex {
            .position = sky::Vector4f(0.5f, -0.5f, 0.0f, 1.0f),
            .color = sky::Vector4f(0.0f, 0.0f, 1.0f, 1.0f)
        }
    };

    sky::MemoryBlock block = {
        .data = vertices.data(),
        .size = sizeof(sky::Vertex) * 3
    };

    auto vbuf_id = renderer.create_vertex_buffer(block, sky::BufferUsage::staticbuf);

    renderer.set_shaders(0, 0);

    double total = 0;
    double std_total = 0.0;
    double frames = 0.0;

    while ( sky::Viewport::open_viewports() > 0 ) {
        auto before = sky::high_resolution_time();

		platform.poll_events();

		if ( keyboard.key_down(sky::Key::escape) || view.close_requested() ) {
			break;
		}

		if ( keyboard.key_typed(sky::Key::space) ) {
			printf("Open windows: %d\n", sky::Viewport::open_viewports());
		}

        renderer.set_vertex_buffer(vbuf_id);
        renderer.present();

		auto dt = sky::Timespan(sky::high_resolution_time() - before);

        printf("%f\n", dt.total_milliseconds());
        total += dt.total_milliseconds();
        frames++;
    }

//    printf("Average: %fms STD: %fms\n", total / frames, std_total / frames);

    return 0;
}
