//
//  main.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Framework/Application.hpp>
#include <Skyrocket/Graphics/Renderer/Vertex.hpp>
#include <Skyrocket/Input/Keyboard.hpp>

#include <Common.hpp>

class App : public sky::Application {
public:
    explicit App(const char* name) :
        Application(name)
    {}

    void on_startup(int argc, const char** argv) override
    {
        common::get_resource_info(renderer.active_backend(), &resource_info_);

        auto cmdlist = renderer.make_command_list();
        {
            vbuf_ = cmdlist.create_vertex_buffer(sky::MemoryBlock {
                sizeof(sky::Vertex) * 3, &vertices_
            }, sky::BufferUsage::staticbuf);

            auto vert = common::get_vertex_shader(resource_info_, "basic_vert");
            auto frag = common::get_fragment_shader(resource_info_, "basic_frag");
            program_ = cmdlist.create_program(vert, frag);
        }
        renderer.submit(cmdlist);
        renderer.commit_frame();
    }

    void on_update(double dt) override
    {
        if (keyboard_.key_typed(sky::Key::escape)) {
            primary_view.close();
        }

        printf("Time: %f\n", dt);

        auto cmdlist = renderer.make_command_list();
        {
            cmdlist.set_program(program_);
            cmdlist.set_vertex_buffer(vbuf_, 0, 3);
            cmdlist.draw();
        }
        renderer.submit(cmdlist);

        renderer.commit_frame();
    }

    void on_shutdown() override
    {

    }

private:
    sky::Vertex vertices_[3] = {
        sky::Vertex(0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f),
        sky::Vertex(-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
        sky::Vertex(0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f)
    };

    uint32_t vbuf_{0};
    uint32_t program_{0};

    sky::Keyboard keyboard_;
    common::ResourceInfo resource_info_;
};

int main(int argc, char** argv)
{
    auto app = std::make_unique<App>("Debug Draw Example");
    app->start(sky::Renderer::ThreadSupport::single_threaded, sky::RendererBackend::OpenGL);

    return 0;
}
