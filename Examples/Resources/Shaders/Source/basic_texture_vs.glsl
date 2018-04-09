#pragma shadecc_stage vert
#pragma shadecc_name "BasicTextureVS"

#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 tex;

#pragma shadecc_instanced layout (location = 3) mat4 model

out vec4 frag_color;
out vec2 frag_tex;

uniform Params {
    mat4 viewproj;
};

void main() {
    frag_color = color;
    frag_tex = tex;
	gl_Position = viewproj * model * position;
}
