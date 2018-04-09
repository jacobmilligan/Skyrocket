#pragma shadecc_stage frag
#pragma shadecc_name "BasicTextureFS"

#version 330 core

in vec4 frag_color;
in vec2 frag_tex;

out vec4 color_out;

uniform sampler2D tex;

void main() {
    vec4 tex_sample = texture(tex, frag_tex);
    color_out = tex_sample;
}
