//
//  Common.HPP
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 7/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

namespace common {

struct ResourceInfo {
    sky::Path root_directory;
    sky::Path shader_directory;
    sky::Path font_directory;
    sky::Path image_directory;
    char vertex_extension[6];
    char fragment_extension[6];
};

void get_resource_info(const sky::RendererBackend backend, ResourceInfo* info)
{
    info->root_directory = sky::Path::executable_path().parent().parent().parent().parent().parent().relative_path("Examples").relative_path("Resources");
    if ( sky::target_platform == sky::OS::macos ) {
        info->root_directory = sky::Path("/Users/Jacob/Dev/Repos/Skyrocket/Examples/Resources");
    }
    info->root_directory.make_real();

    info->font_directory = info->root_directory.relative_path("Fonts");
    info->image_directory = info->root_directory.relative_path("Images");

    info->shader_directory = info->root_directory.relative_path("Shaders");
    switch (backend) {

        case sky::RendererBackend::unknown:
            break;
        case sky::RendererBackend::none:
            break;
        case sky::RendererBackend::Metal:
            info->shader_directory = info->shader_directory.relative_path("MSL");
            strcpy(info->vertex_extension, "metal");
            strcpy(info->fragment_extension, "metal");
            break;
        case sky::RendererBackend::OpenGL:
            info->shader_directory = info->shader_directory.relative_path("GLSL");
            strcpy(info->vertex_extension, "vert");
            strcpy(info->fragment_extension, "frag");
            break;
        case sky::RendererBackend::D3D9:
        case sky::RendererBackend::D3D11:
        case sky::RendererBackend::D3D12:
            info->shader_directory = info->shader_directory.relative_path("HSL");
            strcpy(info->vertex_extension, "hlsl");
            strcpy(info->fragment_extension, "hlsl");
            break;
        case sky::RendererBackend::Vulkan:
            info->shader_directory = info->shader_directory.relative_path("SPIRV");
            strcpy(info->vertex_extension, "vert");
            strcpy(info->fragment_extension, "frag");
            break;
        case sky::RendererBackend::last:
            break;
    }
}

sky::Path get_vertex_shader(const ResourceInfo& info, const char* shader_name)
{
    auto path = info.shader_directory.relative_path(shader_name);
    path.set_extension(info.vertex_extension);
    SKY_ASSERT(path.exists(), "Vertex shader exists at the path specified: %s", path.str());
    return path;
}

sky::Path get_fragment_shader(const ResourceInfo& info, const char* shader_name)
{
    auto path = info.shader_directory.relative_path(shader_name);
    path.set_extension(info.fragment_extension);
    SKY_ASSERT(path.exists(), "Fragment shader exists at the path specified: %s", path.str());
    return path;
}

sky::Path get_image(const ResourceInfo& info, const char* image_name)
{
    auto path = info.image_directory.relative_path(image_name);
    SKY_ASSERT(path.exists(), "Image exists at the path specified: %s", path.str());
    return path;
}

sky::Path get_font(const ResourceInfo& info, const char* font_name)
{
    auto path = info.font_directory.relative_path(font_name);
    SKY_ASSERT(path.exists(), "Font exists at the path specified: %s", path.str());
    return path;
}

} // namespace common