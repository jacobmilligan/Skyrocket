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
#include <Skyrocket/Graphics/Renderer/Renderer.hpp>
#include <Skyrocket/Graphics/Renderer/Vertex.hpp>
#include <Skyrocket/Input/Keyboard.hpp>
#include <Skyrocket/Resource/Font.hpp>

#include <Skyrocket/Framework/Application.hpp>

#include <iostream>
#include <random>
#include <Skyrocket/Framework/Camera.hpp>
#include <Skyrocket/Graphics/Image.hpp>
#include <Jobrocket/Source/Jobrocket/JobGroup.hpp>
#include <Common.hpp>


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
    {}

    void on_startup(int argc, const char** argv) override
    {
        common::get_resource_info(renderer.active_backend(), &resinfo_);
        renderer.set_vsync_enabled(true);
        auto cmdlist = renderer.make_command_list();

        auto vert_path = common::get_vertex_shader(resinfo_, "basic_texture_vert");
        auto frag_path = common::get_fragment_shader(resinfo_, "basic_texture_frag");
        program_ = cmdlist.create_program(vert_path, frag_path);
        cmdlist.set_program(program_);

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
        vbuf_id_ = cmdlist.create_vertex_buffer(vbuf_mem, sky::BufferUsage::staticbuf);

        sky::Matrix4f identity;
        auto fovy = static_cast<float>(sky::math::to_radians(90.0f));

        auto aspect = primary_view.size().x / primary_view.size().y;

        cam_.set_position(sky::Vector3f(5000.0f, 5000.0f, 2000.0f));
        cam_.setup(90.0f, aspect, 0.1f, 20000.0f);

        model_ubuf_ = cmdlist.create_instance_buffer(sizeof(sky::Matrix4f), num_cubes_);
        view_proj_ubuf_ = cmdlist.create_uniform(sky::UniformType::mat4, num_cubes_);

        // Main loop
        sky::Timespan dt;
        uint64_t frame_start = 0;
        auto cam_speed = 10.0f;
        auto target_frametime = 16.6;

        sky::Image img;
        img.load_from_file(common::get_image(resinfo_, "cube.png"));

        texture_ = cmdlist.create_texture(img.width, img.height, img.pixel_format);
        cmdlist.create_texture_region(texture_, sky::UIntRect(0, 0, img.width, img.height), img.pixel_format, img.data);

        renderer.submit(cmdlist);
        renderer.commit_frame();
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

        auto cmdlist = renderer.make_command_list();

        cmdlist.set_state(sky::RenderPipelineState::culling_backface);
        cmdlist.set_program(program_);

        uint32_t modelpos = (renderer.active_backend() == sky::RendererBackend::Metal) ? 1 : 3;
        uint32_t viewpos = (renderer.active_backend() == sky::RendererBackend::Metal) ? 2 : 0;

        cmdlist.set_vertex_buffer(vbuf_id_, 0, static_cast<uint32_t>(vertices_.size()));
        cmdlist.set_instance_buffer(model_ubuf_, modelpos);
        cmdlist.set_uniform(view_proj_ubuf_, viewpos);
        cmdlist.set_texture(texture_, 0);

        cam_.move(cam_movement * cam_speed_ * static_cast<float>(dt));

        cam_mat_ = cam_.get_matrix();

        cmdlist.update_uniform(view_proj_ubuf_, sky::MemoryBlock {
            sizeof(sky::Matrix4f), &cam_mat_
        });

        uint32_t cube_index = 0;

        for ( auto& c : cubes ) {
            c.angle += dist(rand_gen) * dt * 0.1;

            sky::MemoryBlock mb {
                sizeof(sky::Matrix4f),
                &c.get_transform()
            };

            cmdlist.update_instance_buffer(model_ubuf_, static_cast<uint8_t*>(mb.data), cube_index);

            cube_index++;
        }

        cmdlist.draw_instanced(cube_index);
        renderer.submit(cmdlist);

        renderer.commit_frame();
    }

    void on_shutdown() override
    {

    }

private:
    static constexpr uint32_t num_cubes_ = 10000;
    sky::Keyboard keyboard_;

    common::ResourceInfo resinfo_;

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
    app->start(sky::Renderer::ThreadSupport::multi_threaded, sky::RendererBackend::Metal);

    return 0;
}
