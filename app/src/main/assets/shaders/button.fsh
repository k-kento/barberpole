#version 300 es
precision mediump float;

in vec2 v_TexCoord;
out vec4 fragColor;

uniform vec4 u_Color;   // 円の色
uniform float u_Radius; // 半径 (0.0 ~ 0.5)
uniform float u_Smooth; // アンチエイリアス用

void main() {
    vec2 center = vec2(0.5, 0.5);
    float dist = distance(v_TexCoord, center);

    // アンチエイリアス付き α 値計算
    float alpha = smoothstep(u_Radius, u_Radius - u_Smooth, dist);

    fragColor = vec4(u_Color.rgb, alpha * u_Color.a);
}
