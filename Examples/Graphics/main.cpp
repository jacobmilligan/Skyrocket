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
#include <Skyrocket/Graphics/Core/Vertex.hpp>
#include <Skyrocket/Graphics/GraphicsDriver.hpp>
#include <Skyrocket/Input/Keyboard.hpp>
#include <Skyrocket/Platform/Filesystem.hpp>

#include <Skyrocket/Core/Math.hpp>
#include <Skyrocket/Graphics/Color.hpp>
#include <iostream>
#include <Skyrocket/Graphics/Font.hpp>

struct Rectangle {
    Rectangle()
    {
        vertices = {
            sky::Vertex(-1.0f, 1.0f, 0.0f, 1, 0, 1, 1, 1),
            sky::Vertex(1.0f, 1.0f, 0.0f, 1, 0, 0, 1, 1),
            sky::Vertex(1.0f, -1.0f, 0.0f, 1, 1, 0, 1, 1),
            sky::Vertex(-1.0f, -1.0f, 0.0f, 1, 1, 1, 1, 1)
        };

        indices = {
            0, 1, 3,
            3, 2, 1
        };
    }

    sky::Matrix4f model;
    std::vector<sky::Vertex> vertices;
    std::vector<uint32_t> indices;
};

struct ModelViewProjection {
    ModelViewProjection() = default;
    sky::Matrix4f model_view;
    sky::Matrix4f projection;
};

int main(int argc, char** argv)
{
    // Setup paths and string data
    const char* app_name = "Skyrocket Graphics Example";
    auto root_path = sky::Path::executable_path().relative_path("../../../../Examples/Graphics");
    root_path.make_real();

    // Skyrocket platform and input handling data
    sky::Platform platform;
    sky::Viewport view;
    sky::Keyboard keyboard;

    platform.launch(app_name);
	view.open(app_name, 800, 600);
    view.set_backing_color(sky::Color::gray);
    sky::GraphicsDriver driver(sky::GraphicsDriver::ThreadSupport::single_thread);

    if ( !driver.initialize(view) ) {
        SKY_ERROR(app_name, "Couldn't initialize graphics device interface");
        return 0;
    }

    // Geometry
    Rectangle rect;
    ModelViewProjection mvp;
    sky::Matrix4f identity;
    sky::Vector3f pos;
    sky::Vector3f scale(10.0f, 10.0f, 0.0f);

    auto translation_mat = identity.translate(pos);
    auto scale_mat = identity.scale(scale);
    mvp.model_view = translation_mat * scale_mat;
    mvp.projection = identity.ortho(0.0f, view.size().x, view.size().y, 0.0f, -1.0f, 1.0f);

    // Graphics resources
    sky::Font font;
    font.load_from_file(root_path.relative_path("Go-Regular.ttf"), 32);

    auto vbuf_id = driver.create_vertex_buffer(sky::MemoryBlock {
        static_cast<uint32_t>(sizeof(sky::Vertex) * rect.vertices.size()),
        rect.vertices.data()
    }, sky::BufferUsage::staticbuf);
    auto ibuf_id = driver.create_index_buffer(sky::MemoryBlock {
        static_cast<uint32_t>(sizeof(uint32_t) * rect.indices.size()),
        rect.indices.data()
    });
    auto ubuf_id = driver.create_uniform(sky::UniformType::mat4, 2);

    driver.update_uniform(ubuf_id, sky::MemoryBlock{ sizeof(ModelViewProjection), &mvp});
    driver.set_shaders(0, 0);

    // Main loop
    auto dt = 0.0;
    uint64_t frame_start = 0;
    while ( sky::Viewport::open_viewports() > 0 ) {
        frame_start = sky::high_resolution_time();

		platform.poll_events();

		if ( keyboard.key_down(sky::Key::escape) || view.close_requested() ) {
            break;
		}

		if ( keyboard.key_typed(sky::Key::space) ) {
			printf("Open windows: %d\n", sky::Viewport::open_viewports());
		}

        if ( keyboard.key_down(sky::Key::down) ) {
            pos.y += 0.5f * dt;
        }
        if ( keyboard.key_down(sky::Key::right) ) {
            pos.x += 0.5f * dt;
        }
        if ( keyboard.key_down(sky::Key::up) ) {
            pos.y -= 0.5f * dt;
        }
        if ( keyboard.key_down(sky::Key::left) ) {
            pos.x -= 0.5f * dt;
        }

        translation_mat = identity.translate(pos);
        scale_mat = identity.scale(scale);
        mvp.model_view = translation_mat * scale_mat;

        driver.update_uniform(ubuf_id, sky::MemoryBlock{ sizeof(ModelViewProjection), &mvp});
        driver.set_vertex_buffer(vbuf_id, 0, static_cast<uint32_t>(rect.vertices.size()));
        driver.set_uniform(ubuf_id, 1);
        driver.set_index_buffer(ibuf_id, 0, static_cast<uint32_t>(rect.indices.size()));
        driver.draw_primitives();
        driver.present();

        dt = sky::Timespan(sky::high_resolution_time() - frame_start).total_milliseconds();
        printf("%f\n", dt);
    }

    return 0;
}
