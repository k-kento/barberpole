#version 450
layout(location = 0) in vec2 vUV;
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
    float time;
} pc;

// 疑似ランダム生成
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    // 基本色（ブラシ全体の色）
    vec3 baseColor = vec3(0.3, 0.5, 1.0); // 水色っぽい筆
    float brightness = 1.0;

    // 各ピクセルごとに乱数で星パターンを決める
    float starSeed = hash(floor(vUV * 50.0)); // 50個程度の星を散らす
    float blinkSpeed = 2.0 + starSeed * 4.0;  // 星ごとに点滅速度を変える

    // sin波で明滅（0〜1）
    float twinkle = 0.5 + 0.5 * sin(pc.time * blinkSpeed + starSeed * 6.28);

    // 星のマスク（vUVの整数格子単位で1つ星）
    float starMask = step(0.98, fract(starSeed * 10.0 + twinkle)); // ランダム点滅

    // キラキラ部分の輝度
    float starGlow = starMask * pow(twinkle, 3.0);

    // 星の色（白〜黄色）
    vec3 starColor = mix(vec3(1.0, 1.0, 0.8), vec3(1.0), starSeed);

    // 合成
    vec3 color = baseColor * 0.8 + starColor * starGlow * 1.5;

    outColor = vec4(color, 1.0);
}
