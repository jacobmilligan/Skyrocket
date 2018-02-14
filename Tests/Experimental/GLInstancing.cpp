//
//  GLInstancing.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 14/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Platform/Platform.hpp>
#include <Skyrocket/Graphics/Viewport.hpp>
#include <Skyrocket/Graphics/Renderer/Renderer.hpp>

#include <Skyrocket/Graphics/Renderer/OpenGL/GLConfig.hpp>
#include <Skyrocket/Graphics/Renderer/OpenGL/GLContext.hpp>
#include <Skyrocket/Graphics/Renderer/OpenGL/GLResource.hpp>
#include <Skyrocket/Graphics/Renderer/Vertex.hpp>
#include <Skyrocket/Core/Math.hpp>
#include <Skyrocket/Framework/Camera.hpp>

#include <Skyrocket/Framework/Application.hpp>

sky::Vertex vertices[] = {
    sky::Vertex(-1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
    sky::Vertex(1.0f,  -1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
    sky::Vertex(-1.0f, -1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f),
    sky::Vertex(-1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
    sky::Vertex(1.0f,  -1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
    sky::Vertex(1.0f,   1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f)
};

constexpr size_t num_instances = 100;
constexpr size_t instance_data_size = sizeof(sky::Matrix4f) * num_instances;

sky::Path shader_path(sky::Path::executable_path().parent().parent().parent().parent().parent());

std::string vs_src;
std::string frag_src;
sky::Camera3D cam;
uint8_t* models;

void raw_gl()
{
    sky::Platform platform;
    sky::Viewport view;
    sky::Renderer renderer;
    sky::GLContext gl;
    sky::GLProgram program;

    platform.launch("GL Instancing Experiment");
    renderer.set_backend(sky::RendererBackend::OpenGL);
    renderer.destroy();

    view.open(renderer, "GL Instancing Experiment", 800, 600);

    gl.create();
    gl.set_view(&view);
    gl.make_current();

    SKY_GL_CHECK(glEnable(GL_DEPTH_TEST));
    SKY_GL_CHECK(glEnable(GL_BLEND));
    SKY_GL_CHECK(glEnable(GL_CULL_FACE));
    SKY_GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    SKY_GL_CHECK(glClearColor(0.42, 0.62, 0.94, 1.00));


    program.create(vs_src.c_str(), frag_src.c_str());

    GLuint vbo, vao;
    SKY_GL_CHECK(glGenVertexArrays(1, &vao));
    SKY_GL_CHECK(glGenBuffers(1, &vbo));
    SKY_GL_CHECK(glBindVertexArray(vao));
    SKY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    SKY_GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    // Position
    SKY_GL_CHECK(glEnableVertexAttribArray(0));
    SKY_GL_CHECK(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sky::Vertex), nullptr));

    // Color
    auto offset = reinterpret_cast<void*>(4 * sizeof(float));
    SKY_GL_CHECK(glEnableVertexAttribArray(1));
    SKY_GL_CHECK(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(sky::Vertex), offset));

    // Texture coordinates
    offset = reinterpret_cast<void*>(8 * sizeof(float));
    SKY_GL_CHECK(glEnableVertexAttribArray(2));
    SKY_GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sky::Vertex), offset));

    // Instance data
    GLuint ivbo;
    GLint loc;
    SKY_GL_CHECK(loc = glGetAttribLocation(program.id, "sky_instance__model"));
    auto model_loc = static_cast<GLuint>(loc);
    SKY_GL_CHECK(glGenBuffers(1, &ivbo));
    SKY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, ivbo));
    SKY_GL_CHECK(glBufferData(GL_ARRAY_BUFFER, instance_data_size, nullptr, GL_DYNAMIC_DRAW));


    auto cam_mat = cam.get_matrix();
    GLint ubuf;
    SKY_GL_CHECK(glUseProgram(program.id));
    SKY_GL_CHECK(ubuf = glGetUniformLocation(program.id, "viewproj"));
    SKY_GL_CHECK(glUniformMatrix4fv(ubuf, 1, GL_FALSE, cam_mat.entries));

    while (view.is_open()) {
        SKY_GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        platform.poll_events();

        // Draw
        SKY_GL_CHECK(glBindVertexArray(vao));
        SKY_GL_CHECK(glUseProgram(program.id));
        SKY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        cam_mat = cam.get_matrix();
        SKY_GL_CHECK(glUniformMatrix4fv(ubuf, 1, GL_FALSE, cam_mat.entries));

        auto x = 0.0f;
        auto y = 0.0f;
        auto num_drawn = 0;
        for (int i = 0; i < num_instances; ++i) {
            auto translate = sky::Matrix4f::translate(sky::Vector3f(x, y, 0.0f));
            auto scale = sky::Matrix4f::scale(sky::Vector3f(50.0f, 50.0f, 1.0f));
            sky::Matrix4f model = translate * scale;
            memcpy(models + i * sizeof(sky::Matrix4f), &model, sizeof(sky::Matrix4f));
            x += 100.0f;
            if (x > view.size().x) {
                y -= 100.0f;
                x = 0.0f;
            }
            ++num_drawn;
        }

        SKY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, ivbo));
        for (int j = 0; j < 4; ++j) {
            SKY_GL_CHECK(glEnableVertexAttribArray(model_loc + j));
            SKY_GL_CHECK(glVertexAttribPointer(model_loc + j, 4, GL_FLOAT, GL_FALSE,
                                               sizeof(float) * 4 * 4,
                                               reinterpret_cast<GLvoid*>(j * 4 * sizeof(float))));
            SKY_GL_CHECK(glVertexAttribDivisor(model_loc + j, 1));
        }

        SKY_GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, num_drawn * sizeof(sky::Matrix4f), models));
        SKY_GL_CHECK(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, num_drawn));

        SKY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        SKY_GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 6));

        gl.swap_buffers();
    }

    SKY_GL_CHECK(glDeleteBuffers(1, &vbo));
    SKY_GL_CHECK(glDeleteBuffers(1, &ivbo));
    SKY_GL_CHECK(glDeleteVertexArrays(1, &vao));
    program.destroy();
    gl.destroy();

    view.close();
}

class App : public sky::Application {
public:
    explicit App(const char* name) :
        Application(name)
    {

    }

    void on_startup(int argc, const char** argv) override
    {
        auto cmdlist = renderer.make_command_list();
        {
            ubuf_ = cmdlist.create_uniform(sky::UniformType::mat4, sizeof(sky::Matrix4f));
            program_ = cmdlist.create_program(shader_path.relative_path("basic_instancing.vert"),
                                   shader_path.relative_path("basic_instancing.frag"));
            vbuf_ = cmdlist.create_vertex_buffer(sky::MemoryBlock {
                sizeof(vertices), vertices
            }, sky::BufferUsage::staticbuf);
            instbuf_ = cmdlist.create_instance_buffer(sizeof(sky::Matrix4f),
                                                      sizeof(sky::Matrix4f) * num_instances);
        }
        renderer.submit(cmdlist);
        renderer.commit_frame();
    }

    void on_update(double dt) override
    {
        cam_mat_ = cam.get_matrix();
        auto cmdlist = renderer.make_command_list();
        {
            cmdlist.set_state(sky::RenderPipelineState::culling_none);
            cmdlist.set_program(program_);
            cmdlist.set_vertex_buffer(vbuf_, 0, 6);
            cmdlist.update_uniform(ubuf_, sky::MemoryBlock{
                sizeof(sky::Matrix4f), &cam_mat_
            });
            cmdlist.set_uniform(ubuf_, 0);
            cmdlist.set_instance_buffer(instbuf_, 3);

            auto x = 0.0f;
            auto y = 0.0f;
            auto num_drawn = 0u;
            for (uint32_t i = 0; i < num_instances; ++i) {
                auto translate = sky::Matrix4f::translate(sky::Vector3f(x, y, 0.0f));
                auto scale = sky::Matrix4f::scale(sky::Vector3f(50.0f, 50.0f, 1.0f));
                sky::Matrix4f model = translate * scale;
                memcpy(models + i * sizeof(sky::Matrix4f), &model, sizeof(sky::Matrix4f));
                cmdlist.update_instance_buffer(instbuf_, models + (i * sizeof(sky::Matrix4f)), i);
                x += 100.0f;
                if (x > primary_view.size().x) {
                    y -= 100.0f;
                    x = 0.0f;
                }
                ++num_drawn;
            }

            cmdlist.draw_instanced(num_drawn);
        }
        renderer.submit(cmdlist);
        renderer.commit_frame();
    }

    void on_shutdown() override
    {

    }

private:
    uint32_t ubuf_, vbuf_, instbuf_, program_;
    sky::Matrix4f cam_mat_;
};

int main(int argc, char** argv)
{
    models = static_cast<uint8_t*>(malloc(instance_data_size));
    shader_path = shader_path.relative_path("Examples").relative_path("Resources").relative_path("Shaders").relative_path("GLSL");
    vs_src = sky::fs::slurp_file(shader_path.relative_path("basic_instancing.vert"));
    frag_src = sky::fs::slurp_file(shader_path.relative_path("basic_instancing.frag"));

    auto aspect = 800.0f / 600.0f;
    cam.setup(90.0f, aspect, 0.1f, 20000.0f);
    cam.set_position(sky::Vector3f(0.0f, 0.0f, 2000.0f));

    auto app = std::make_unique<App>("GL Instancing Test");
    app->start(sky::Renderer::ThreadSupport::single_threaded, sky::RendererBackend::OpenGL);
//    raw_gl();
    return 0;
}