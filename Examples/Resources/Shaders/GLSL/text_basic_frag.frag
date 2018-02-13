#version 330 core

in vec4 frag_color;
in vec2 frag_tex;

out vec4 color_out;

uniform sampler2D text_texture;

void main() {
    vec4 text_sample = texture(text_texture, frag_tex);
    color_out = vec4(frag_color.rgb, text_sample.r);
}
