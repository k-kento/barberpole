#version 300 es
precision mediump float;

in vec2 v_TexCoord;
out vec4 fragColor;

uniform vec4 u_Color;   // 円の色

const float RADIUS = 0.5;
const float SMOOTH = 0.01;

void main() {
    vec2 center = vec2(0.5, 0.5);
    float dist = distance(v_TexCoord, center);

    // アンチエイリアス付き α 値計算
    float alpha = smoothstep(RADIUS, RADIUS - SMOOTH, dist);

    fragColor = u_Color;
}
