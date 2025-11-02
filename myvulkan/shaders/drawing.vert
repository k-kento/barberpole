#version 450
layout (location = 0) in vec2 inPos;
layout (location = 1) in vec4 instanceModelCol0;
layout (location = 2) in vec4 instanceModelCol1;
layout (location = 3) in vec4 instanceModelCol2;
layout (location = 4) in vec4 instanceModelCol3;

layout (binding = 0) uniform UBO {
    mat4 projection;
} ubo;

void main() {
    mat4 model = mat4(
        instanceModelCol0,
        instanceModelCol1,
        instanceModelCol2,
        instanceModelCol3
    );

    gl_Position = ubo.projection * model * vec4(inPos, 0.0, 1.0);
}
