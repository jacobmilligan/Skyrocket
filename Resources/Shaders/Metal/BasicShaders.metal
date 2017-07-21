vertex float4 basic_vertex(const device packed_float* vertex_array [[buffer(0)]],
                           unsigned int vid [[vertex_id]] )
{
    return float4(vertex_array[vid], 1.0);
}

fragment half4 basic_fragment()
{
    return half(1.0);
}
