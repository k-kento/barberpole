#version 450

layout(location = 0) in vec4 vColor;
layout(location = 1) in vec2 vUV;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform UBO {
    mat4 projection;
    float time;
    float glowIntensity;
    float glowRadius;
    float pulsePeriod;
} ubo;

void main() {
    // vUV.x: -1 (左端) ~ +1 (右端)、abs で中心からの距離に変換
    float dist = abs(vUV.x);

    // コア線 (中心の30%は完全に不透明)
    float coreEdge = 0.3;
    float coreMask = 1.0 - smoothstep(coreEdge - 0.05, coreEdge, dist);

    // グロー: 中心から外に向かって減衰 (強め)
    float glowFalloff = exp(-dist * 2.0);  // 指数関数で柔らかく減衰

    // パルスアニメーション (0.5 ~ 1.0 の範囲で振動)
    float pulse = 0.5 + 0.5 * sin(ubo.time * 6.28318 / ubo.pulsePeriod);

    // グロー強度を大幅に上げる
    float glowStrength = ubo.glowIntensity * 2.0 * pulse * glowFalloff;

    // コア色
    vec3 coreColor = vColor.rgb;

    // グロー色 (コア色より明るく、白っぽく)
    vec3 glowColor = mix(coreColor, vec3(1.0), 0.3) * glowStrength;

    // 最終色: コア + グロー発光
    vec3 finalColor = coreColor * coreMask + glowColor;

    // アルファ: コアは不透明、グローは強めに残す
    float alpha = max(coreMask, glowStrength * 0.8);

    // 最低限のアルファを確保（完全に消えないように）
    alpha = max(alpha, 0.1 * glowFalloff);

    outColor = vec4(finalColor, alpha);
}
