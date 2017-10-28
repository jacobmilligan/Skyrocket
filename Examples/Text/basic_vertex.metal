#include <metal_stdlib>

using namespace metal;

struct Vertex {
    float4 position [[position]];
    float4 color;
    float2 tex_coords;
};

vertex Vertex basic_vertex(device Vertex* vertices [[buffer(0)]],
                           uint vid [[vertex_id]])
{
    return vertices[vid];
}
