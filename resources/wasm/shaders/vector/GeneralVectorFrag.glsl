#version 300 es
precision mediump float;

in vec2 UnitLocation;
out vec4 FragColor;

uniform vec2 uSize;

const uint CIRCLE = 1u;
const uint LINE = 2u;

struct VectorGeometry {
    uint type; 
    vec4 color;
    vec4 posAndStyle;
    vec4 paramsA;
    vec4 paramsB;
};


layout(std140) uniform VectorGeometryBlock {
    VectorGeometry geometry[64]; // Array size of 64
};

uniform int numEntries;



float SdCircle(vec2 pixel, vec2 position, float radius) {
    return length(pixel - position) - radius;
}

float SdLine(vec2 pixel, vec2 start, vec2 end) {
    vec2 sToP = pixel - start;
    vec2 sToE = end - start;
    float h = clamp(dot(sToP, sToE) / dot(sToE, sToP), 0.0, 1.0);
    return length(sToP - sToE*h);
}


void main() {

    vec2 pixel = UnitLocation * uSize;

    for (int i = 0; i < numEntries; i++) {

        VectorGeometry entity = geometry[i];

        switch (entity.type) {
            case CIRCLE:
                float dist = SdCircle(pixel, entity.posAndStyle.xy, entity.paramsA.x);

                if (dist < entity.posAndStyle.z) {
                    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
                    return;
                }

                break;
            case LINE:
                
                break;
        }

    }

    
    
    
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    
}
