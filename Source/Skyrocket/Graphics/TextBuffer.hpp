//
//  TextBuffer.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 30/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Resource/Font.hpp"
#include "Skyrocket/Graphics/GraphicsDriver.hpp"
#include "Skyrocket/Graphics/Vertex.hpp"

namespace sky {


class TextBuffer {
public:
    static constexpr size_t max_characters = kibibytes(8);

    explicit TextBuffer(GraphicsDriver* gd, Font* font)
        : gd_(gd), font_(font)
    {}

    void init()
    {
        auto cmdlist = gd_->command_list();
        texid_ = cmdlist->create_texture(font_->width(), font_->height(), PixelFormat::Enum::r8);
        for (auto& glyph : *font_) {
            if ( glyph.bounds.width <= 0 || glyph.bounds.height <= 0 || glyph.data == nullptr ) {
                continue;
            }
            cmdlist->create_texture_region(texid_, glyph.bounds, sky::PixelFormat::Enum::r8, glyph.data);
        }

        uint32_t vertex = 0;
        for (uint32_t i = 0; i < max_characters; i += indices_per_char_) {
            indices_[i] = vertex;
            indices_[i + 1] = vertex + 1;
            indices_[i + 2] = vertex + 2;
            indices_[i + 3] = vertex;
            indices_[i + 4] = vertex + 2;
            indices_[i + 5] = vertex + 3;

            vertex += verts_per_char_;
        }

        vbufid_ = cmdlist->create_vertex_buffer(MemoryBlock {
            static_cast<uint32_t>(sizeof(Vertex) * max_characters), vertices_
        }, BufferUsage::dynamic);

        ibufid_ = cmdlist->create_index_buffer(MemoryBlock {
            static_cast<uint32_t>(sizeof(uint32_t) * max_characters), indices_
        });
    }

    void set_text(const char* str, const size_t str_size)
    {
        Glyph glyph;
        auto x = 0.0f;
        auto y = 0.0f;
        num_vertices_ = 0;
        string_size_ = str_size;
        for (size_t c = 0; c < str_size; ++c) {
            glyph = font_->get_glyph(str[c]);
            auto& bounds = glyph.bounds;

            auto ypos = y - (glyph.bounds.height - glyph.bearing.y);
            auto xpos = x;

            vertices_[num_vertices_] = Vertex(xpos, ypos + bounds.height, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, glyph.s, glyph.t);
            vertices_[num_vertices_ + 1] = Vertex(xpos, ypos, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, glyph.s, glyph.t2);
            vertices_[num_vertices_ + 2] = Vertex(xpos + bounds.width, ypos, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, glyph.s2, glyph.t2);
            vertices_[num_vertices_ + 3] = Vertex(xpos + bounds.width, ypos + bounds.height, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, glyph.s2, glyph.t);

            x += glyph.advance.x;
            num_vertices_ += verts_per_char_;
        }

        gd_->command_list()->update_vertex_buffer(vbufid_, MemoryBlock {
            static_cast<uint32_t>(max_characters * sizeof(Vertex)), vertices_
        });
    }

    inline void set_program(const uint32_t program_id)
    {
        programid_ = program_id;
    }

    void draw(const uint32_t viewprojection_matrix)
    {
        if (string_size_ <= 0) {
            return;
        }

        auto cmdlist = gd_->command_list();
        cmdlist->set_program(programid_);
        cmdlist->set_texture(texid_, 0);
        cmdlist->set_vertex_buffer(vbufid_, 0, num_vertices_);
        cmdlist->set_index_buffer(ibufid_, 0, static_cast<uint32_t>(string_size_ * 6));
        cmdlist->set_uniform(viewprojection_matrix, 1);
        cmdlist->draw();
    }
private:
    static constexpr uint32_t verts_per_char_ = 4;
    static constexpr uint32_t indices_per_char_ = 6;

    // State members
    size_t string_size_{0};
    uint32_t num_vertices_{0};

    // GDI resources
    uint32_t texid_{0}, vbufid_{0}, ibufid_{0}, programid_{0};

    // Resources used
    GraphicsDriver* gd_;
    Font* font_;

    // Buffers
    Vertex vertices_[max_characters * verts_per_char_]{};
    uint32_t indices_[max_characters * indices_per_char_]{};
};


}