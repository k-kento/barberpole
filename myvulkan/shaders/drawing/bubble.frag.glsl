#version 450
layout(location = 0) in vec4 vPosition; // 世界 or スクリーン座標
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform Params {
    float time;
} pc;

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

float bubble(vec2 p, float t) {
    vec2 cell = floor(p);
    vec2 local = fract(p);
    vec2 center = vec2(hash(cell), hash(cell + 1.3));
    center.y = fract(center.y + t * 0.2); // 上昇
    float d = distance(local, center);
    return smoothstep(0.2, 0.15, d);
}

void main() {
    vec2 pos = vPosition.xy * 0.01; // グローバル座標スケーリング

    // ストローク範囲外は描かない
//    if (vPosition.x < pc.strokeMin.x || vPosition.x > pc.strokeMax.x ||
//    vPosition.y < pc.strokeMin.y || vPosition.y > pc.strokeMax.y) {
//        discard;
//    }

    float b = bubble(pos, pc.time);

    vec3 base = vec3(0.1, 0.2, 0.4);
    vec3 color = mix(base, vec3(1.0), b);

    outColor = vec4(color, 1.0);
}


//#version 450
//
//layout(location = 0) in vec2 vUV;          // 頂点シェーダで渡されたUVや座標
//layout(location = 1) in float vPressure;   // 筆圧などがあるなら使える
//layout(location = 0) out vec4 outColor;
//
//layout(push_constant) uniform Params {
//    float time;        // アニメーション用
//    float bubbleScale; // 泡のスケール
//    float bubbleDensity; // 泡の密度（確率）
//    float brightness;  // 泡の明るさ
//} params;
//
////------------------------------------
//// 2Dノイズ関数（高速なフラクショナルノイズ）
////------------------------------------
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
////------------------------------------
//// 泡マスクの生成
////------------------------------------
//float bubbleNoise(vec2 uv, float t) {
//    // 時間とともにノイズが上昇する（泡が上に上がるように）
//    uv.y += t * 0.1;
//
//    // ノイズ空間を広げる
//    float n = noise(uv * params.bubbleScale);
//
//    // 泡っぽい閾値化（高い値のところだけ泡にする）
//    n = smoothstep(1.0 - params.bubbleDensity, 1.0, n);
//
//    // エッジを柔らかく（丸い泡の輪郭）
//    n = pow(n, 3.0);
//
//    return n;
//}
//
////------------------------------------
//// メイン
////------------------------------------
//void main() {
//    // UV座標や筆圧に応じて泡ノイズを生成
//    float t = params.time;
//    float bubbles = bubbleNoise(vUV, t);
//
//    // 泡の光っぽいハイライト
//    float rim = smoothstep(0.3, 1.0, bubbles);
//
//    // ベース色（帯の色とブレンド）
//    vec3 baseColor = vec3(0.2, 0.5, 0.8);
//    vec3 bubbleColor = vec3(1.0) * params.brightness;
//
//    // 泡を加算ブレンド気味に重ねる
//    vec3 color = baseColor + bubbleColor * rim;
//
//    outColor = vec4(color, 1.0);
//}
