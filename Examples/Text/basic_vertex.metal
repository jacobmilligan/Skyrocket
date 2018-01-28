#include <metal_stdlib>

using namespace metal;

struct Vertex {
    float4 position [[position]];
    float4 color;
    float2 tex_coords;
};

vertex Vertex basic_vertex(device Vertex* vertices [[buffer(0)]],
                           constant float4x4& viewproj [[ buffer(1) ]],
                           uint vid [[vertex_id]])
{
    Vertex out;
    out.position = viewproj * vertices[vid].position;
    out.color = vertices[vid].color;
    out.tex_coords = vertices[vid].tex_coords;
    return out;
}
