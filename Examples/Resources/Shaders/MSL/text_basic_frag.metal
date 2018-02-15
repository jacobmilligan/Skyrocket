#include <metal_stdlib>

using namespace metal;

struct Vertex {
    float4 position [[position]];
    float4 color;
    float2 tex_coords;
};


fragment half4 text_basic_frag(Vertex in [[stage_in]],
                               texture2d<half> texture [[texture(0)]])
{
    constexpr sampler s;
    const half tex_sample = texture.sample(s, in.tex_coords).r;
    
//    return tex_sample;
    return half4(in.color.r, in.color.g, in.color.b, tex_sample);
}
