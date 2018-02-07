//
//  main.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 27/10/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Framework/Application.hpp>
#include <Skyrocket/Input/Keyboard.hpp>
#include <Skyrocket/Resource/Font.hpp>
#include <Skyrocket/Graphics/Renderer/Vertex.hpp>
#include <Skyrocket/Framework/Camera.hpp>
#include <Skyrocket/Graphics/TextBuffer.hpp>

#include <Common.hpp>

class TextApplication : public sky::Application {
public:
    TextApplication()
        : Application("Skyrocket Text Rendering Example"),
          tb_(&renderer, &font_),
          cam_speed_(4.5f),
          program_(0),
          viewproj_(0)
    {
        set_frame_limit(144);
    }

    void create_graphics_resources()
    {
        auto cmdlist = renderer.make_command_list();

        tb_.init();

        auto vert_path = common::get_vertex_shader(resinfo_, "text_basic_vert");
        auto frag_path = common::get_fragment_shader(resinfo_, "text_basic_frag");
        program_ = cmdlist.create_program(vert_path, frag_path);

        tb_.set_program(program_);

        viewproj_ = cmdlist.create_uniform(sky::UniformType::mat4, sizeof(sky::Matrix4f));

        renderer.submit(cmdlist);
    }

    void on_startup(int argc, const char** argv) override
    {
        common::get_resource_info(renderer.active_backend(), &resinfo_);

        renderer.set_vsync_enabled(false);
        primary_view.set_backing_color(sky::Color::cornflower_blue);
        cam_.setup(primary_view.size(), 0.01f, 1000.0f);
        cam_.set_position({0.0f, 0.0f});

        font_.load_from_file(common::get_font(resinfo_, "Go-Regular.ttf"), font_size);

        textpos_ = sky::Vector3f(10.0f, primary_view.size().y - (font_size + 10.0f), 1.0f);

        create_graphics_resources();

        renderer.commit_frame();
    }

    sky::Vertex vertices[3] = {
        sky::Vertex(0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f),
        sky::Vertex(-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
        sky::Vertex(0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f)
    };
    uint32_t test_vbuf{0};

    void on_update(const double dt) override
    {
        if (keyboard_.key_typed(sky::Key::escape)) {
            primary_view.close();
        }

        if (keyboard_.key_typed(sky::Key::space)) {
            if (renderer.active_backend() == sky::RendererBackend::Metal) {
                renderer.set_backend(sky::RendererBackend::OpenGL);
            } else {
                renderer.set_backend(sky::RendererBackend::Metal);
            }

            create_graphics_resources();
        }

        auto& frame = renderer.get_frame_info(1);

        char dtbuffer[4096];
        snprintf(dtbuffer, 4096, "Average FPS: %.*f\nFPS: %.*f\nFrame time: %.*f\nFrames queued: %d\nSim time: %.*f\n"
                     "GDI time: %.*f\nCPU time: %.*f\nGPU time: %.*f",
                 4, frame.running_fps_average,
                 4, frame.total_fps,
                 4, frame.total_time,
                 renderer.frames_queued(),
                 4, frame.sim_time,
                 4, frame.gdi_time,
                 4, frame.cpu_time,
                 4, frame.gpu_time);

        tb_.set_text(dtbuffer, strlen(dtbuffer), textpos_);

        cam_mat_ = cam_.get_matrix();

        auto cmdlist = renderer.make_command_list();

        cmdlist.set_program(program_);
        cmdlist.set_state(sky::RenderPipelineState::culling_frontface);
        cmdlist.update_uniform(viewproj_, sky::MemoryBlock {
            sizeof(sky::Matrix4f), &cam_mat_
        });

        renderer.submit(cmdlist);

        tb_.submit(viewproj_);

        cmdlist = renderer.make_command_list();

        cmdlist.set_program(0);

        if (test_vbuf == 0) {
            test_vbuf = cmdlist.create_vertex_buffer(sky::MemoryBlock {
                sizeof(sky::Vertex) * 3, &vertices
            }, sky::BufferUsage::staticbuf);
        }
        cmdlist.set_vertex_buffer(test_vbuf, 0, 3);
        cmdlist.draw();

        renderer.submit(cmdlist);

        renderer.commit_frame();
    }

    void on_shutdown() override
    {
        renderer.set_backend(sky::RendererBackend::unknown);
    }

private:
    static constexpr float font_size = 16.0f;

    sky::Keyboard keyboard_;
    sky::Font font_;
    common::ResourceInfo resinfo_;

    uint32_t program_, viewproj_;

    sky::TextBuffer tb_;

    sky::Camera2D cam_;
    float cam_speed_;
    sky::Matrix4f cam_mat_;
    sky::Vector3f textpos_;
};

int main(int argc, const char** argv)
{
    auto app = std::make_unique<TextApplication>();
    app->start(sky::Renderer::ThreadSupport::single_threaded, sky::RendererBackend::OpenGL);
    return 0;
}
