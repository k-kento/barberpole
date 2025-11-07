#version 450
layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inUv;
layout (location = 2) in vec4 instanceModelCol0;
layout (location = 3) in vec4 instanceModelCol1;
layout (location = 4) in vec4 instanceModelCol2;
layout (location = 5) in vec4 instanceModelCol3;

layout (binding = 0) uniform UBO {
    mat4 projection;
    mat4 uvMatrix;
} ubo;

layout (location = 0) out vec2 fragUv;

void main() {
    mat4 model = mat4(
        instanceModelCol0,
        instanceModelCol1,
        instanceModelCol2,
        instanceModelCol3
    );

    gl_Position = ubo.projection * model * vec4(inPos, 0.0, 1.0);
    fragUv = (ubo.uvMatrix * vec4(inUv, 0.0, 1.0)).xy;
}
