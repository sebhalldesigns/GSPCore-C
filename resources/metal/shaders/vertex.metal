#include <metal_stdlib>

using namespace metal;

struct Vertex {
    float4 position [[position]];
    float2 texCoord;
};

vertex Vertex vertexShader(uint vertexID [[vertex_id]]) {
    float2 positions[4] = {
        float2(-1.0, -1.0),
        float2(1.0, -1.0),
        float2(-1.0, 1.0),
        float2(1.0, 1.0)
    };
    float2 texCoords[4] = {
        float2(0.0, 1.0),
        float2(1.0, 1.0),
        float2(0.0, 0.0),
        float2(1.0, 0.0)
    };
    
    Vertex outVertex;
    outVertex.position = float4(positions[vertexID], 0.0, 1.0);
    outVertex.texCoord = texCoords[vertexID];
    return outVertex;
}