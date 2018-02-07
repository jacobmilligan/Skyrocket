#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 1) in vec2 tex;

out vec4 frag_color;
out vec2 frag_tex;

uniform mat4 viewproj;

void main() {
    frag_color = color;
    frag_tex = tex;
	gl_Position = viewproj * position;
}
