//
//  Playground.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 23/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Framework/Application.hpp>
#include <Skyrocket/Graphics/GraphicsDriver.hpp>
#include <Skyrocket/Graphics/Vertex.hpp>

class App : public sky::Application {
public:
    App()
        : sky::Application("Playground"), vbuf_(0)
    {
        vertices_[0] = sky::Vertex(0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.3f, 1.0f, 0.0f, 0.0f);
        vertices_[1] = sky::Vertex(0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.1f, 1.0f, 0.0f, 0.0f);
        vertices_[2] = sky::Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.2f, 1.0f, 1.0f, 0.0f, 0.0f);
    }

    void on_startup(int argc, const char** argv) override
    {
        root_ = sky::Path("/Users/Jacob/Dev/Repos/Skyrocket/Tests/Experimental");

        auto cmdbuf = graphics_driver.command_list();
        if (cmdbuf != nullptr) {
            cmdbuf->start_recording();

            vbuf_ = cmdbuf->create_vertex_buffer(sky::MemoryBlock{
                sizeof(sky::Vertex) * 3, vertices_
            }, sky::BufferUsage::staticbuf);
            shader_ = cmdbuf->create_program(root_.relative_path("basic_vertex.metal"),
                                             root_.relative_path("basic_fragment.metal"));

            cmdbuf->end();
            graphics_driver.commit_frame(cmdbuf);
        }
    }

    void on_update() override
    {
        auto cmdbuf = graphics_driver.command_list();

        if (cmdbuf != nullptr) {
            cmdbuf->start_recording();

            cmdbuf->set_program(shader_);
            cmdbuf->set_vertex_buffer(vbuf_, 0, 3);
            cmdbuf->draw();

            cmdbuf->end();

            graphics_driver.commit_frame(cmdbuf);
        }
    }

    void on_shutdown() override
    {

    }

private:
    sky::Path root_;
    uint32_t vbuf_;
    uint32_t shader_;
    sky::Vertex vertices_[3];
};

int main(int argc, char** argv)
{
    App app;
    app.start(sky::GraphicsDriver::ThreadSupport::single_thread);
}
