#include <metal_stdlib>

using namespace metal;

struct Vertex {
    float4 position [[position]];
    float4 color;
    float2 tex_coords;
};

fragment float4 basic_frag(Vertex in [[stage_in]])
{
    return in.color;
}
