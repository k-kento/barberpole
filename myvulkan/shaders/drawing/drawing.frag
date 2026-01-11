#version 450

layout(location = 0) in vec4 vPosition; // ← vertex shaderから補間されたワールド座標
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform Params {
    float time;
} pc;

void main() {
    vec2 pos = vPosition.xy;

//    // 範囲外カット
//    if (vPosition.x < pc.strokeMin.x || vPosition.x > pc.strokeMax.x ||
//    vPosition.y < pc.strokeMin.y || vPosition.y > pc.strokeMax.y)
//    discard;

//    float bubble = smoothstep(0.25, 0.15, length(fract(pos + vec2(0.0, pc.time * 0.1)) - 0.5));
//
//    // 泡の半径
//    float radius = 0.25;
//    float dist = distance(local, center);
//
//    // ぼかしなしの円（しきい値で分ける）
//    float bubble = 1.0 - step(radius, dist);
//
//    // 色設定
//    vec3 baseColor = vec3(0.1, 0.2, 0.4);
//    vec3 bubbleColor = vec3(1.0);
//    vec3 color = mix(baseColor, bubbleColor, bubble);
//
//    outColor = vec4(color, 1.0);
}

//float hash(vec2 p) {
//    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
//}
//
//float noise(vec2 p) {
//    vec2 i = floor(p);
//    vec2 f = fract(p);
//    float a = hash(i);
//    float b = hash(i + vec2(1.0, 0.0));
//    float c = hash(i + vec2(0.0, 1.0));
//    float d = hash(i + vec2(1.0, 1.0));
//    vec2 u = f * f * (3.0 - 2.0 * f);
//    return mix(a, b, u.x) +
//    (c - a) * u.y * (1.0 - u.x) +
//    (d - b) * u.x * u.y;
//}
//
//void main() {
//    // (1) とにかく u,v からノイズを作ってみる
//    vec2 uv = vUV;
//    float n = noise(uv);
//
//    // (2) 泡マスクしきい値を緩く
//    float bubble = smoothstep(0.3, 0.9, n);
//    bubble = pow(bubble, 3.0);
//
//    // (3) 色を見えるように強調
//    vec3 baseColor = vec3(0.2, 0.3, 0.6);
//    vec3 bubbleColor = vec3(1.0, 1.0, 1.0);
//    vec3 color = baseColor + bubbleColor * bubble * 2.0;
//
//    outColor = vec4(color, 1.0);
//}
