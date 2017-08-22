#include <metal_stdlib>

using namespace metal;

struct Vertex {
    float4 position [[position]];
    float4 color;
};

vertex Vertex basic_vertex(device Vertex* vertices [[buffer(0)]],
                           constant float4x4& uniforms,
                           uint vid [[vertex_id]] )
{
    Vertex out;
    out.position = uniforms * vertices[vid].position;
    out.color = vertices[vid].color;
    
    return out;
}

fragment float4 basic_fragment(Vertex in [[stage_in]])
{
    return in.color;
}
