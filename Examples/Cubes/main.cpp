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
#include <Skyrocket/Core/Hash.hpp>
#include <Skyrocket/Core/Math.hpp>
#include <Skyrocket/Graphics/Color.hpp>
#include <Skyrocket/Graphics/GraphicsDriver.hpp>
#include <Skyrocket/Graphics/Vertex.hpp>
#include <Skyrocket/Input/Keyboard.hpp>
#include <Skyrocket/Resource/Font.hpp>

#include <Skyrocket/Framework/Application.hpp>

#include <iostream>
#include <random>
#include <Skyrocket/Framework/Camera.hpp>
#include <Skyrocket/Graphics/Image.hpp>


struct Cube {
    sky::Vector3f pos;
    sky::Vector3f scale;
    sky::Vector2f angle;

    Cube()
    {
        xaxis_ = sky::Vector3f(0.0f, 1.0f, 0.0f);
        yaxis_ = sky::Vector3f(1.0f, 0.0f, 0.0f);
        pos = sky::Vector3f(0.0f, 0.0f, 0.0f);
        scale = sky::Vector3f(100.0f, 100.0f, 100.0f);
    }

    sky::Matrix4f& get_transform()
    {
        auto translation_mat = sky::Matrix4f::translate(pos);
        auto scale_mat = sky::Matrix4f::scale(scale);
        auto rotation_mat = sky::Matrix4f::rotate(angle.x, xaxis_) * sky::Matrix4f::rotate(angle.y, yaxis_);

        transform_ = translation_mat * rotation_mat * scale_mat;

        return transform_;
    }

private:
    sky::Vector3f xaxis_, yaxis_;
    sky::Matrix4f transform_;

};

class CubeApp : public sky::Application {
public:
    CubeApp()
        : sky::Application("Skyrocket Cubes Example"),
          rand_gen(rd()),
          neg_dist(-1.0f, 1.0f),
          dist(0.04f, 0.07f),
          big_dist(-300.0f, 300.0f)
    {
        root_path_ = sky::Path::executable_path().relative_path("../../../../Examples/Cubes");
        if ( sky::target_platform == sky::OS::macos ) {
            root_path_ = sky::Path("/Users/Jacob/Dev/Repos/Skyrocket/Examples/Cubes");
        }
        root_path_.make_real();
        set_frame_limit(60);
    }

    void on_startup(int argc, const char** argv) override
    {
        graphics_driver.set_vsync_enabled(true);
        auto cmdqueue = graphics_driver.command_list();

        primary_view.set_backing_color(sky::Color::cornflower_blue);
        auto vert_path = root_path_.relative_path("basic_vertex.metal");
        auto frag_path = root_path_.relative_path("basic_fragment.metal");
        program_ = cmdqueue->create_program(vert_path, frag_path);
        cmdqueue->set_program(program_);

        auto xpos = 0;
        auto ypos = 0;

        for ( int i = 0; i < cubes.size(); ++i ) {
            xpos += 500;
            if ( xpos > num_cubes_ * 2 ) {
                ypos += 500;
                xpos = 0;
            }
            cubes[i].pos = sky::Vector3f(xpos, ypos, 0.0f);
        }

        // Cube vertices

        //Front
        auto a = sky::Vertex(-1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.25f);
        auto b = sky::Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.25f, 0.50f);
        auto c = sky::Vertex( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.50f, 0.50f);
        auto d = sky::Vertex( 1.0f,  1.0f,  1.0f, 1.0f, 0.1f, 0.6f, 0.4f, 1.0f, 0.50f, 0.25f);
        //Left
        auto e = sky::Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.00f, 0.25f);
        auto f = sky::Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.00f, 0.50f);
        auto g = sky::Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.25f, 0.50f);
        auto h = sky::Vertex(-1.0f,  1.0f,  1.0f, 1.0f, 0.1f, 0.6f, 0.4f, 1.0f, 0.25f, 0.25f);
        //Right
        auto i = sky::Vertex( 1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.50f, 0.25f);
        auto j = sky::Vertex( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.50f, 0.50f);
        auto k = sky::Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.75f, 0.50f);
        auto l = sky::Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 0.1f, 0.6f, 0.4f, 1.0f, 0.75f, 0.25f);
        //Top
        auto m = sky::Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.00f);
        auto n = sky::Vertex(-1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.25f, 0.25f);
        auto o = sky::Vertex( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.50f, 0.25f);
        auto p = sky::Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 0.1f, 0.6f, 0.4f, 1.0f, 0.50f, 0.00f);
        //Bottom
        auto q = sky::Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.50f);
        auto r = sky::Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.25f, 0.75f);
        auto s = sky::Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.50f, 0.75f);
        auto t = sky::Vertex( 1.0f, -1.0f,  1.0f, 1.0f, 0.1f, 0.6f, 0.4f, 1.0f, 0.50f, 0.50f);
        //Back
        auto u = sky::Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.75f, 0.25f);
        auto v = sky::Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.75f, 0.50f);
        auto w = sky::Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.00f, 0.50f);
        auto x = sky::Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.1f, 0.6f, 0.4f, 1.0f, 1.00f, 0.25f);

        vertices_ = {
            a, b, c, a, c, d,   //front
            e, f, g, e, g, h,   //left
            i, j, k, i, k, l,   //right
            m, n, o, m, o, p,   //top
            q, r, s, q, s, t,   //bot
            u, v, w, u, w, x    //back
        };

        // Setup matrices

        auto vbuf_mem = sky::MemoryBlock {
            static_cast<uint32_t>(sizeof(sky::Vertex) * vertices_.size()),
            vertices_.data()
        };
        vbuf_id_ = cmdqueue->create_vertex_buffer(vbuf_mem, sky::BufferUsage::staticbuf);

        sky::Matrix4f identity;
        auto fovy = static_cast<float>(sky::math::to_radians(90.0f));

        auto aspect = primary_view.size().x / primary_view.size().y;

        cam_.set_position(sky::Vector3f(5000.0f, 5000.0f, 2000.0f));
        cam_.setup(90.0f, aspect, 0.1f, 20000.0f);

        model_ubuf_ = cmdqueue->create_uniform(sky::UniformType::mat4, num_cubes_ * sizeof(sky::Matrix4f));
        view_proj_ubuf_ = cmdqueue->create_uniform(sky::UniformType::mat4, num_cubes_ * sizeof(sky::Matrix4f));

        // Main loop
        sky::Timespan dt;
        uint64_t frame_start = 0;
        auto cam_speed = 10.0f;
        auto target_frametime = 16.6;

        sky::Image img;
        img.load_from_file(root_path_.relative_path("cube.png"));

        texture_ = cmdqueue->create_texture(img.width, img.height, img.pixel_format);
        cmdqueue->create_texture_region(texture_, sky::UIntRect(0, 0, img.width, img.height), img.pixel_format, img.data);

        graphics_driver.commit_frame();
        graphics_driver.set_viewport(&primary_view);
    }

    void on_update(const double dt) override
    {
        if ( keyboard_.key_down(sky::Key::escape) || primary_view.close_requested() ) {
            primary_view.close();
        }

        sky::Vector3f cam_movement;
        if ( keyboard_.key_down(sky::Key::up) ) {
            cam_movement.z -= 1.0f;
        }
        if ( keyboard_.key_down(sky::Key::down) ) {
            cam_movement.z += 1.0f;
        }
        if ( keyboard_.key_down(sky::Key::left) ) {
            cam_movement.x -= 1.0f;
        }
        if ( keyboard_.key_down(sky::Key::right) ) {
            cam_movement.x += 1.0f;
        }

        auto cmdlist = graphics_driver.command_list();

        cmdlist->set_state(sky::RenderPipelineState::culling_frontface);

        cmdlist->set_vertex_buffer(vbuf_id_, 0, static_cast<uint32_t>(vertices_.size()));

        cmdlist->set_uniform(model_ubuf_, 1);
        cmdlist->set_uniform(view_proj_ubuf_, 2);
        cmdlist->set_texture(texture_, 0);

        cam_.move(cam_movement * cam_speed_ * static_cast<float>(dt));

        cam_mat_ = cam_.get_matrix();

        cmdlist->update_uniform(view_proj_ubuf_, sky::MemoryBlock {
            sizeof(sky::Matrix4f), &cam_mat_
        });

        uint32_t cube_index = 0;

        for ( auto& c : cubes ) {
            c.angle += dist(rand_gen) * dt * 0.1;

            sky::MemoryBlock mb {
                sizeof(sky::Matrix4f),
                &c.get_transform()
            };

            cmdlist->update_uniform(model_ubuf_, mb, cube_index * sizeof(sky::Matrix4f));

            cube_index++;
        }

        cmdlist->draw_instanced(cube_index);

        graphics_driver.commit_frame();
    }

    void on_shutdown() override
    {

    }

private:
    static constexpr uint32_t num_cubes_ = 10000;
    sky::Keyboard keyboard_;

    sky::Path root_path_;

    std::random_device rd;
    std::mt19937 rand_gen;
    std::uniform_real_distribution<float> neg_dist;
    std::uniform_real_distribution<float> dist;
    std::uniform_real_distribution<float> big_dist;

    sky::Camera3D cam_;
    sky::Matrix4f cam_mat_;
    float cam_speed_{2.0f};

    uint32_t program_{}, vbuf_id_{}, model_ubuf_{}, view_proj_ubuf_{}, texture_{};
    std::vector<sky::Vertex> vertices_;
    std::array<Cube, num_cubes_> cubes;
};

int main(int argc, char** argv)
{
    auto app = std::make_unique<CubeApp>();
    app->start(sky::GraphicsDriver::ThreadSupport::multi_threaded);

    return 0;
}
