#include <metal_stdlib>

using namespace metal;

struct Vertex {
    float4 position [[position]];
    float4 color;
};

vertex Vertex basic_vertex( device Vertex* vertices [[buffer(0)]], uint vid [[vertex_id]] )
{
    return vertices[vid];
}

fragment float4 basic_fragment(Vertex in [[stage_in]])
{
    return in.color;
}
