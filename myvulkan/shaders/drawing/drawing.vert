#version 450
layout (location = 0) in vec2 inPos;

layout (binding = 0) uniform UBO {
    mat4 projection;
} ubo;

void main() {
    gl_Position = ubo.projection * vec4(inPos, 0.0, 1.0);
}
