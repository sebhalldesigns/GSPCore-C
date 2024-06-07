#version 300 es

layout (location = 0) in vec2 aPos;

out vec2 UnitLocation;

void main() {
    UnitLocation = aPos;
    gl_Position = vec4((aPos * 2.0) - 1.0, 0.0, 1.0);
}