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
        auto cube_width = 1.0f;
        auto cube_height = 1.0f;
        auto cube_depth = 1.0f;
        vertices = {
            // 0
            sky::Vertex(-cube_width,-cube_height,-cube_depth, 1.0f, 0.0f,1.0f,1.0f,1.0f),
            //1
            sky::Vertex( cube_width,-cube_height,-cube_depth, 1.0f, 0.0f,0.0f,1.0f,1.0f),
            //2
            sky::Vertex( cube_width, cube_height,-cube_depth, 1.0f, 1.0f,0.0f,1.0f,1.0f),
            //3
            sky::Vertex(-cube_width, cube_height,-cube_depth, 1.0f, 1.0f,1.0f,1.0f,1.0f),
            //4
            sky::Vertex(-cube_width,-cube_height, cube_depth, 1.0f, 0.0f,1.0f,0.0f,1.0f),
            //5
            sky::Vertex( cube_width,-cube_height, cube_depth, 1.0f, 0.0f,0.0f,0.0f,1.0f),
            //6
            sky::Vertex( cube_width, cube_height, cube_depth, 1.0f, 1.0f,0.0f,0.0f,1.0f),
            //7
            sky::Vertex(-cube_width, cube_height, cube_depth, 1.0f, 1.0f,1.0f,0.0f,1.0f)
        };

        indices = {
            0, 1, 3, 3, 1, 2,
            1, 5, 2, 2, 5, 6,
            5, 4, 6, 6, 4, 7,
            4, 0, 7, 7, 0, 3,
            3, 2, 7, 7, 2, 6,
            4, 5, 0, 0, 5, 1
        };
    }

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
    sky::Vector3f pos(0.0f, 0.0f, 0.0f);
    sky::Vector3f scale(100.0f, 100.0f, 100.0f);
    auto angle = 90.0f;

    auto cam_pos = sky::Vector3f(0.0f, 0.0f, 110.0f);
    auto cam_front = sky::Vector3f(0.0f, 0.0f, -1.0f);
    auto cam_up = sky::Vector3f(0.0f, 1.0f, 0.0f);

    auto translation_mat = identity.translate(pos);
    auto rotation_mat = identity.rotate(angle, sky::Vector3f(0.0f, 0.0f, 1.0f));
    auto scale_mat = identity.scale(scale);
    auto view_mat = identity.look_at(cam_pos, cam_pos + cam_front, cam_up);
    mvp.model_view = view_mat * translation_mat * rotation_mat * scale_mat;
//    mvp.projection = identity.ortho(0.0f, view.size().x, view.size().y, 0.0f, -1.0f, 1.0f);
    auto projection_mat = identity.perspective(sky::math::to_radians(90.0f), view.size().x / view.size().y, 0.1f, 1000.0f);
    mvp.model_view = projection_mat * mvp.model_view;
//    mvp.projection = identity.perspective(90.0f, view.size().x / view.size().y, 0.1f, 100.0f);

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

        if ( keyboard.key_down(sky::Key::up) ) {
//            pos.y -= 0.5f * dt;
            cam_pos += (cam_front * 1.0f);
        }
        if ( keyboard.key_down(sky::Key::down) ) {
//            pos.y += 0.5f * dt;
            cam_pos -= (cam_front * 1.0f);
        }
        if ( keyboard.key_down(sky::Key::left) ) {
//            pos.x -= 0.5f * dt;
            cam_pos -= (cam_front.cross(cam_up) * 1.0f).get_normalized();
        }
        if ( keyboard.key_down(sky::Key::right) ) {
//            pos.x += 0.5f * dt;
            cam_pos += (cam_front.cross(cam_up) * 1.0f).get_normalized();
        }
        if ( keyboard.key_down(sky::Key::E) ) {
            angle += 0.01f;
        }
        if ( keyboard.key_down(sky::Key::Q) ) {
            angle -= 0.01f;
        }

        translation_mat = identity.translate(pos);
        scale_mat = identity.scale(scale);
        rotation_mat = identity.rotate(angle, sky::Vector3f(0.0f, 1.0f, 0.0f));
        view_mat = identity.look_at(cam_pos, cam_pos + cam_front, cam_up);
        mvp.model_view = projection_mat * view_mat * translation_mat * rotation_mat * scale_mat;
//
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
