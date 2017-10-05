#include <metal_stdlib>

using namespace metal;

struct GraphicsData {
    float4 position [[position]];
    float4 color;
};

vertex GraphicsData basic_vertex(device GraphicsData* vertices [[buffer(0)]],
                                 constant float4x4& model [[buffer(1)]],
                                 constant float4x4& view [[buffer(2)]],
                                 constant float4x4& projection [[buffer(3)]],
                                 uint vid [[vertex_id]] )
{
    GraphicsData out;
    out.position = projection * view * model * vertices[vid].position;
    out.color = vertices[vid].color;
    
    return out;
}

fragment float4 basic_fragment(GraphicsData in [[stage_in]])
{
    return in.color;
}
