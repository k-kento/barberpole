#version 450

layout(location = 0) in vec2 inPos;
layout(location = 2) in vec4 inColor;

layout(location = 0) out vec4 vColor;

layout(binding = 0) uniform UBO {
    mat4 projection;
    float time;
    float pointSize;
    float unused2;
    float unused3;
} ubo;

void main() {
    vColor = inColor;
    gl_Position = ubo.projection * vec4(inPos, 0.0, 1.0);
    gl_PointSize = ubo.pointSize;
}
