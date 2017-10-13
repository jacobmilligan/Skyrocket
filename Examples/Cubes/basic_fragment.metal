#include <metal_stdlib>

using namespace metal;

struct GraphicsData {
    float4 position [[position]];
    float4 color;
};


fragment float4 basic_fragment(GraphicsData in [[stage_in]])
{
    return in.color;
}
