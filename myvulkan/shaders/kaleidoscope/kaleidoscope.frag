#version 450
layout (location = 0) in vec2 fragUV;
layout (binding = 1) uniform sampler2D texSampler;
layout (location = 0) out vec4 outColor;

void main() {
    // UV が [0,1] から外れたら描画しない
    if (fragUV.x < 0.0 || fragUV.x > 1.0 || fragUV.y < 0.0 || fragUV.y > 1.0) {
        discard;
    }

    vec4 texColor = texture(texSampler, fragUV);
    outColor = texColor;
}