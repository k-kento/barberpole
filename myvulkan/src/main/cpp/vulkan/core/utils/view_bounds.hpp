#pragma once

#include "glm/gtc/matrix_transform.hpp"

class ViewBounds {
   public:
    float left;
    float right;
    float bottom;
    float top;

    ViewBounds(float l, float r, float b, float t) : left(l), right(r), bottom(b), top(t) {}

    // NDC上の幅と高さを取得
    [[nodiscard]] float width() const { return right - left; }

    [[nodiscard]] float height() const { return top - bottom; }

    // glm::ortho() に渡せる正射影行列を生成
    [[nodiscard]] glm::mat4 toOrthoMatrix(float nearZ = -1.0f, float farZ = 1.0f) const {
        return glm::ortho(left, right, bottom, top, nearZ, farZ);
    }

    // width/height から短辺に合わせた NDC 範囲を生成
    static ViewBounds fromSize(uint32_t width, uint32_t height) {
        float aspect = static_cast<float>(width) / static_cast<float>(height);

        if (aspect > 1.0f) {
            // 横長 → 横を広げる
            return ViewBounds(-aspect, aspect, -1.0f, 1.0f);
        } else {
            // 縦長 → 縦を広げる
            return ViewBounds(-1.0f, 1.0f, -1.0f / aspect, 1.0f / aspect);
        }
    }
};