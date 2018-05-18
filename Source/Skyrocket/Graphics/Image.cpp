//
//  Image.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 27/09/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/Renderer/Definitions.hpp"
#include "Skyrocket/Graphics/Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace sky {

Image::~Image()
{
    if ( data != nullptr ) {
        stbi_image_free(data);
    }
}

void Image::load_from_file(const Path& path)
{
    int channels_in_file = 0;
    int w = 0;
    int h = 0;

    data = stbi_load(path.str(), &w, &h, &channels_in_file, 0);

    width = static_cast<uint32_t>(w);
    height = static_cast<uint32_t>(h);

    switch (channels_in_file) {
        case 1:
            pixel_format = PixelFormat::r8;
            break;
        case 2:
            pixel_format = PixelFormat::rg8;
            break;
        case 3:
            pixel_format = PixelFormat::rgb8;
            break;
        case 4:
            pixel_format = PixelFormat::rgba8;
            break;
        default:break;
    }
}


} // namespace sky