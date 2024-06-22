#include <metal_stdlib>
using namespace metal;

struct Vertex {
    float4 position [[position]];
    float2 texCoord;
};

fragment float4 fragmentShader(Vertex in [[stage_in]], texture2d<float> tex [[texture(0)]]) {
    //return float4(0.0, 0.0, 1.0, 1.0); // Clear color: blue
    constexpr sampler texSampler (mag_filter::linear, min_filter::linear);
    return tex.sample(texSampler, in.texCoord);
}