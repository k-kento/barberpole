#pragma once

#include "glm/glm.hpp"

/**
 * @brief HSVカラーをRGBカラーに変換する
 *
 * Hue（色相）、Saturation（彩度）、Value（明度）から
 * glm::vec4 のRGBAカラーを生成します。Alphaは常に1.0fで設定されます。
 *
 * @param h Hue（色相）0.0f〜1.0f
 * @param s Saturation（彩度）0.0f〜1.0f
 * @param v Value（明度）0.0f〜1.0f
 * @return glm::vec4 RGBAカラー（RGBはHSVから計算、Aは1.0f）
 *
 * @note h が1.0f以上の場合は glm::fract() により0.0〜1.0に正規化されます。
 * @note 彩度が0の場合はグレースケール、Valueが0の場合は黒になります。
 */
inline glm::vec4 hsv2rgb(float h, float s, float v) {
    h = glm::fract(h);
    float c = v * s;
    float x = c * (1 - fabs(fmod(h * 6.0f, 2.0f) - 1));
    float m = v - c;
    glm::vec3 rgb;
    if (h < 1.0f / 6.0f) rgb = {c, x, 0};
    else if (h < 2.0f / 6.0f) rgb = {x, c, 0};
    else if (h < 3.0f / 6.0f) rgb = {0, c, x};
    else if (h < 4.0f / 6.0f) rgb = {0, x, c};
    else if (h < 5.0f / 6.0f) rgb = {x, 0, c};
    else rgb = {c, 0, x};
    return {rgb + glm::vec3(m), 1.0f};
}
