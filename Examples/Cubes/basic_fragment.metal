#include <metal_stdlib>

using namespace metal;

struct Vertex {
    float4 position [[position]];
    float4 color;
    float2 tex_coords;
};


fragment float4 basic_fragment(Vertex in [[stage_in]],
                               texture2d<float> texture [[texture(0)]])
{
    constexpr sampler s(mag_filter::linear, min_filter::linear);
    const float4 tex_sample = texture.sample(s, in.tex_coords);
    return float4(tex_sample);
}
