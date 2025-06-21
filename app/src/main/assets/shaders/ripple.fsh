#version 300 es
precision mediump float;

in vec2 v_TexCoord;
uniform float u_Time;       // 経過時間 (秒)
uniform vec3 u_Color;       // 輪の色

out vec4 fragColor;

void main() {

    float u_Speed = 0.15f; // 波紋の速さ
    float u_Interval = 0.3f; // 波紋の間隔（波長）
    float u_Thickness = 1.0; // 線の太さ (推奨 0.01 ~ 0.05)

    // 中心からの距離
    float dist = length(v_TexCoord);

    // 輪が外に近づいたときにフェードさせるために使用
    float outerFade = smoothstep(0.7, 1.0, dist);

    float cycle = u_Interval / u_Speed; // 1波紋の周期（秒）
    float loopedTime = mod(u_Time, cycle); // 時間をループさせる

    // 進行度(位相)
    float phase = dist - loopedTime * u_Speed;

    // 角度の周期
    float period = 6.283185; // 2 * π

    // この点が波の中でどの位置か(角度)
    float angle = phase * period / u_Interval;

    // 0~1 の範囲に変換
    // |¯¯¯|_|¯¯¯|_|¯¯¯|_
    float wave = abs(sin(angle));

    // 波を滑らかにする
    // /\____/\____/\____
    float ring = smoothstep(u_Thickness, 0.0, wave);

    // 透明度：輪っかの強さ × 外周フェード × 中心フェードの逆
    float alpha = ring * (1.0 - outerFade);

    // ほぼ透明なら描画しない（パフォーマンス向上）
    if (alpha < 0.01) discard;

    fragColor = vec4(u_Color, alpha);
}
