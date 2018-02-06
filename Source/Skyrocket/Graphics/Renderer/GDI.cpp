//
//  GDI.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 24/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/Renderer/GDI.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Graphics/Renderer/CommandList.hpp"

#if SKY_GRAPHICS_API_OPENGL == 1

#include "Skyrocket/Graphics/Renderer/OpenGL/GLGDI.hpp"

#endif


namespace sky {


struct BackendSupport {
    RendererBackend backend;
    bool supported;
};

void supported_renderer_backends(renderer_backend_list_t& dest)
{
    static BackendSupport graphics_backends[] = {
        { RendererBackend::Metal, SKY_GRAPHICS_API_METAL != 0 },
        { RendererBackend::D3D12, SKY_GRAPHICS_API_D3D12 != 0 },
        { RendererBackend::Vulkan, SKY_GRAPHICS_API_VULKAN != 0 },
        { RendererBackend::D3D11, SKY_GRAPHICS_API_D3D11 != 0 },
        { RendererBackend::OpenGL, SKY_GRAPHICS_API_OPENGL != 0 },
        { RendererBackend::D3D9, SKY_GRAPHICS_API_D3D9 != 0 },
    };

    auto last = static_cast<int>(RendererBackend::last);
    auto next = 0;
    for (int backend = 0; backend < last - 2; ++backend) {
        if (graphics_backends[backend].supported) {
            dest[next++] = graphics_backends[backend].backend;
        }
    }

    for (int i = next; i < last; ++i) {
        dest[i] = RendererBackend::none;
    }

}


#if SKY_GRAPHICS_API_METAL == 1

/// Creates a Metal GDI. The only reason this function exists is to ensure that Objective-C++
/// headers don't pollute the pure C++ code by providing the implemention in <MetalGDI.mm> -
/// This way .mm files are compiled as Objective-C++ and .cpp files as C++.
std::unique_ptr<GDI> gdi_create_metal();

#else

std::unique_ptr<GDI> gdi_create_metal()
{
    return GDI::create(RendererBackend::unknown);
}

#endif


std::unique_ptr<GDI> GDI::create(const RendererBackend backend, GDI* copy) noexcept
{
    std::unique_ptr<GDI> gdi = nullptr;
    auto new_backend = backend;

    switch (backend) {

        case RendererBackend::unknown:
        {
            renderer_backend_list_t backend_list{};
            supported_renderer_backends(backend_list);
            gdi = create(backend_list[0]);
            new_backend = gdi->backend_;
        } break;

        case RendererBackend::Metal:
        {
            gdi = gdi_create_metal();
        } break;

        case RendererBackend::OpenGL:
        {
#if SKY_GRAPHICS_API_OPENGL == 1
            gdi = std::make_unique<OpenGLGDI>();
            break;
#endif
        }

        case RendererBackend::D3D9:
        {

        }

        case RendererBackend::D3D11:
        {

        }

        case RendererBackend::D3D12:
        {

        }

        case RendererBackend::Vulkan:
        {

        }

        case RendererBackend::none:
        {

        }

        case RendererBackend::last:
        {
            gdi = std::make_unique<GDI>();
            new_backend = RendererBackend::none;
        } break;
    }

    gdi->backend_ = new_backend;

    if (copy != nullptr) {
    }

    return std::move(gdi);
}

void GDI::submit(sky::CommandBuffer* cmdbuf)
{
    CommandType* typeptr = nullptr;

    while (cmdbuf->cursor() <= cmdbuf->size()) {

        typeptr = cmdbuf->read_command<CommandType>();
        if (typeptr == nullptr) {
            break;
        }

        switch (*typeptr) {
            case CommandType::unknown:
            {

            } break;

            case CommandType::init:
            {
//                auto
//                auto& view = static_cast<rc::SetViewport&>(next_cmd).viewport;
//                initialize(view);
            } break;

            case CommandType::set_viewport:
            {
                auto view = cmdbuf->read_command<Viewport*>();
                set_viewport(*view);
            } break;

            case CommandType::create_vertex_buffer:
            {
                auto data = cmdbuf->read_command<CreateVertexBufferData>();
                create_vertex_buffer(data->buf_id, data->data, data->buf_usage);
            } break;

            case CommandType::set_vertex_buffer:
            {
                auto data = cmdbuf->read_command<SetVertexBufferData>();
                set_vertex_buffer(data->buf_id);
                state_.vertex_buffer = data->buf_id;
                state_.vertex_count = data->count;
                state_.vertex_offset = data->first_vertex;
            } break;

            case CommandType::update_vertex_buffer:
            {
                auto data = cmdbuf->read_command<UpdateBufferData>();
                update_vertex_buffer(data->buf_id, data->data);
            } break;

            case CommandType::create_index_buffer:
            {
                auto data = cmdbuf->read_command<CreateIndexBufferData>();
                create_index_buffer(data->buf_id, data->data);
            } break;

            case CommandType::set_index_buffer:
            {
                auto data = cmdbuf->read_command<SetIndexBufferData>();
                set_index_buffer(data->buf_id);
                state_.index_buffer = data->buf_id;
                state_.index_count = data->count;
                state_.index_offset = data->first_index;
            } break;

            case CommandType::create_program:
            {
                auto* data = cmdbuf->read_command<CreateProgramData>();

                auto prog_id = data->prog_id;

                Path vs(data->vs);
                Path frag(data->frag);

                create_program(prog_id, vs, frag);
            } break;

            case CommandType::set_program:
            {
                auto idptr = cmdbuf->read_command<uint32_t>();
                set_program(*idptr);

                if ( *idptr == invalid_handle ) {
//                    printf("invali\n");
                } else {
                }

            } break;

            case CommandType::create_uniform:
            {
                auto data = cmdbuf->read_command<CreateUniformData>();
                create_uniform(data->uniform_id, data->size);
            } break;

            case CommandType::set_uniform:
            {
                auto data = cmdbuf->read_command<SetUniformData>();
                set_uniform(data->uniform_id, data->uniform_index);
            } break;

            case CommandType::update_uniform:
            {
                auto data = cmdbuf->read_command<UpdateUniformData>();
//                if ( data->type != CommandType::unknown ) {
                update_uniform(data->uniform_id, data->new_data, data->offset);
//                }
            } break;

            case CommandType::create_texture:
            {
                auto cmd = cmdbuf->read_command<CreateTextureData>();
                create_texture(cmd->tid, cmd->width, cmd->height, cmd->format, cmd->mipmapped);
            } break;

            case CommandType::create_texture_region:
            {
                auto cmd = cmdbuf->read_command<CreateTextureRegionData>();
                create_texture_region(cmd->tex_id, cmd->rect, cmd->format, cmd->data);
            } break;

            case CommandType::set_texture:
            {
                auto cmd = cmdbuf->read_command<SetTextureData>();
                set_texture(cmd->tid, cmd->index);
            } break;

            case CommandType::set_state:
            {
                auto flagsptr = cmdbuf->read_command<uint32_t>();
                set_state(*flagsptr);
            } break;

            case CommandType::draw:
            {
                draw();
            } break;

            case CommandType::draw_instanced:
            {
                auto instanceptr = cmdbuf->read_command<uint32_t>();
                draw_instanced(*instanceptr);
            } break;
        }
    }
}


bool GDI::init(Viewport*  /*viewport*/)
{
    return true;
}

bool GDI::destroy()
{
    return true;
}

bool GDI::begin_frame(sky::FrameInfo* frame_info)
{
    return true;
}

bool GDI::end_frame(sky::FrameInfo* frame_info)
{
    return true;
}

void GDI::set_viewport(Viewport*  /*viewport*/)
{
    // no op
}

bool GDI::create_vertex_buffer(const uint32_t  /*vbuf_id*/, const MemoryBlock&  /*initial_data*/,
                               const BufferUsage  /*usage*/)
{
    // no op
    return true;
}

bool GDI::set_vertex_buffer(const uint32_t  /*vbuf_id*/)
{
    // no op
    return true;
}

bool GDI::update_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& data)
{
    // no op
    return true;
}

bool GDI::create_index_buffer(const uint32_t /*ibuf_id*/, const MemoryBlock& /*initial_data*/)
{
    // no op
    return true;
}

bool GDI::set_index_buffer(const uint32_t /*ibuf_id*/)
{
    // no op
    return true;
}

bool GDI::create_program(const uint32_t /*program_id*/, const Path& /*vs_path*/, const Path& /*frag_path*/)
{
    // no op
    return true;
}

bool GDI::set_program(const uint32_t)
{
    // no op
    return true;
}

bool GDI::create_uniform(const uint32_t  /*u_id*/, const uint32_t /*initial_data*/)
{
    // no op
    return true;
}

bool GDI::set_uniform(const uint32_t  /*u_id*/, const uint32_t  /*index*/)
{
    // no op
    return true;
}

bool GDI::update_uniform(const uint32_t  /*u_id*/, const MemoryBlock&  /*data*/, const uint32_t /*offset*/)
{
    // no op
    return true;
}

bool GDI::create_texture(const uint32_t  /*t_id*/, const uint32_t /*width*/,
                         const uint32_t  /*height*/, const PixelFormat::Enum /*pixel_format*/,
                         const bool  /*mipmapped*/)
{
    // no op
    return true;
}

bool GDI::create_texture_region(const uint32_t /*tex_id*/, const UIntRect& /*region*/,
                                const PixelFormat::Enum /*pixel_format*/, uint8_t* /*data*/)
{
    // no op
    return true;
}

bool GDI::set_texture(const uint32_t /*t_id*/, const uint32_t /*index*/)
{
    // no op
    return true;
}

bool GDI::draw()
{
    // no op
    return true;
}

bool GDI::draw_instanced(const uint32_t /*instance*/)
{
    // no op
    return true;
}

bool GDI::set_state(const uint32_t /*flags*/)
{
    //no op
    return true;
}


} // namespace sky
