#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 tex;
layout (location = 3) in mat4 sky_instance__model;

out vec4 frag_color;
out vec2 frag_tex;

uniform mat4 viewproj;

void main() {
    frag_color = color;
    frag_tex = tex;
	gl_Position = viewproj * sky_instance__model * position;
}
