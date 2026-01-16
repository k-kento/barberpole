#version 450

layout(location = 0) in vec4 vColor;

layout(location = 0) out vec4 outColor;

void main() {
    // 点の中のUV (-1~1)
    vec2 uv = gl_PointCoord * 2.0 - 1.0;
    float d = length(uv);

    // 円以外カット
    if (d > 1.0) discard;

    float a = smoothstep(1.0, 0.8, d);

    outColor = vec4(vColor.rgb, a * vColor.a);
}
