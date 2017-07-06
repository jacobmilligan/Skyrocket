//
//  RenderResource.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/GraphicsInterface/RenderResource.hpp"

namespace sky {


VertexBuffer::VertexBuffer(const uint32_t size, const BufferUsage usage)
    : initialized_(true), size_(size), usage_(usage)
{}

VertexBuffer VertexBuffer::create(const uint32_t size, const BufferUsage usage)
{
    return VertexBuffer(size, usage);
}

bool VertexBuffer::set_data(Vertex* vertices)
{

}


}