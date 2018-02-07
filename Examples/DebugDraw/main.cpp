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

class App : public sky::Application {
public:
    explicit App(const char* name) :
        Application(name)
    {
        root_path_ = sky::Path::executable_path().relative_path("../../../../Examples");
        if ( sky::target_platform == sky::OS::macos ) {
            root_path_ = sky::Path("/Users/Jacob/Dev/Repos/Skyrocket/Examples");
        }
    }

    void on_startup(int argc, const char** argv) override
    {
        shaders_path_ = root_path_.relative_path("Shaders");

        switch (renderer.active_backend()) {

            case sky::RendererBackend::unknown:break;
            case sky::RendererBackend::none:break;
            case sky::RendererBackend::Metal:
                shaders_path_ = shaders_path_.relative_path("MSL");
                strcpy(vert_extension_, "metal");
                strcpy(frag_extension_, "metal");
                break;
            case sky::RendererBackend::OpenGL:
                shaders_path_ = shaders_path_.relative_path("GLSL");
                strcpy(vert_extension_, "vert");
                strcpy(frag_extension_, "frag");
                break;
            case sky::RendererBackend::D3D9:
            case sky::RendererBackend::D3D11:
            case sky::RendererBackend::D3D12:
                shaders_path_ = shaders_path_.relative_path("HSL");
                strcpy(vert_extension_, "hlsl");
                strcpy(frag_extension_, "hlsl");
                break;
            case sky::RendererBackend::Vulkan:
                shaders_path_ = shaders_path_.relative_path("SPIRV");
                strcpy(vert_extension_, "vert");
                strcpy(frag_extension_, "frag");
                break;
            case sky::RendererBackend::last:
                break;
        }

        auto cmdlist = renderer.make_command_list();
        {
            vbuf_ = cmdlist.create_vertex_buffer(sky::MemoryBlock {
                sizeof(sky::Vertex) * 3, &vertices_
            }, sky::BufferUsage::staticbuf);

            auto vert = shaders_path_.relative_path("basic_vertex");
            vert.set_extension(vert_extension_);
            auto frag = shaders_path_.relative_path("basic_frag");
            frag.set_extension(frag_extension_);
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
    sky::Path root_path_;
    sky::Path shaders_path_;

    char vert_extension_[5];
    char frag_extension_[5];
};

int main(int argc, char** argv)
{
    auto app = std::make_unique<App>("Debug Draw Example");
    app->start(sky::Renderer::ThreadSupport::single_threaded, sky::RendererBackend::OpenGL);

    return 0;
}
