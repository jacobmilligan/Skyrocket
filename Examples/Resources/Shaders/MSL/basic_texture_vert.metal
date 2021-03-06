#include <metal_stdlib>

using namespace metal;

struct Vertex {
    float4 position [[position]];
    float4 color;
    float2 tex_coords;
};

vertex Vertex basic_texture_vert(device Vertex* vertices [[buffer(0)]],
                                 constant float4x4* model [[buffer(1)]],
                                 constant float4x4& view_projection [[buffer(2)]],
                                 uint vid [[vertex_id]],
                                 uint iid [[instance_id]])
{
    Vertex out;
    out.position = view_projection * model[iid] * vertices[vid].position;
    out.color = vertices[vid].color;
    out.tex_coords = vertices[vid].tex_coords;
    
    return out;
}
