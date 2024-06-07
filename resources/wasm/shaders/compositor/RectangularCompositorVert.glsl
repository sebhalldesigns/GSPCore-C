#version 300 es
layout (location = 0) in vec2 aPos;

out vec2 TexCoord;

uniform vec2 uPos;
uniform vec2 uSize;
uniform vec2 uViewportSize;

void main() {
    vec2 proportionalPosition = vec2((aPos*uSize + uPos)/uViewportSize);
    vec2 normalisedPosition = (proportionalPosition * 2.0 - 1.0); 
    gl_Position = vec4(normalisedPosition  * vec2(1.0, -1.0), 0.0, 1.0);
    TexCoord = aPos;
}