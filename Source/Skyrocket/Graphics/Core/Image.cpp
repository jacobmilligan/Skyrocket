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

#include "Skyrocket/Graphics/Core/Image.hpp"

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
    data = stbi_load(path.str(), &width, &height, &bytes_per_pixel, 0);
}


} // namespace sky