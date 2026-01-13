#version 450

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec2 vUV;

layout(binding = 0) uniform UBO {
    mat4 projection;
    float time;
    float glowIntensity;
    float glowRadius;
    float pulsePeriod;
} ubo;

void main() {
    vColor = inColor;
    vUV = inUV;
    gl_Position = ubo.projection * vec4(inPos, 0.0, 1.0);
}
