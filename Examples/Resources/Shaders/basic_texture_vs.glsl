#stage vs
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 tex;

//    instance_data {
//        mat4 model;
//    };

// Model matrix for instancing
layout (location = 3) in vec4 instance_model0;
layout (location = 4) in vec4 instance_model1;
layout (location = 5) in vec4 instance_model2;
layout (location = 6) in vec4 instance_model3;

out vec4 frag_color;
out vec2 frag_tex;

uniform Params {
    mat4 viewproj;
};

void main() {
    mat4 model = mat4(instance_model0, instance_model1, instance_model2, instance_model3);
    frag_color = color;
    frag_tex = tex;
	gl_Position = viewproj * model * position;
}
