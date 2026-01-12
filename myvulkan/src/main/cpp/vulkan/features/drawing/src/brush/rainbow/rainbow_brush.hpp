#pragma once

#include "../brush.hpp"
#include <cmath>

/**
 * RainbowBrush - 虹色ブラシ
 *
 * HSV色空間を使って虹色のグラデーションで描画するブラシ
 */
class RainbowBrush : public Brush {
public:
    RainbowBrush(VulkanContext& context, BasePipeline& pipeline)
        : Brush(context, pipeline) {}

    void generateVertices(const std::vector<glm::vec2>& points,
                          std::vector<InputVertex>& outVertices) override {
        const auto total = static_cast<uint32_t>(points.size());
        if (total < 2) return;

        // 既存の頂点数から、新しく追加すべき区間を計算
        uint32_t existingSegments = outVertices.empty() ? 0 : (outVertices.size() / 4);
        uint32_t startIndex = existingSegments;

        for (uint32_t i = startIndex; i < total - 1; ++i) {
            glm::vec2 p0 = points[i];
            glm::vec2 p1 = points[i + 1];

            glm::vec2 dir = p1 - p0;
            float len = glm::length(dir);
            if (len < 1e-6f) continue;
            dir /= len;

            glm::vec2 normal(-dir.y, dir.x);
            normal *= HALF_WIDTH;

            glm::vec2 left0 = p0 + normal;
            glm::vec2 right0 = p0 - normal;
            glm::vec2 left1 = p1 + normal;
            glm::vec2 right1 = p1 - normal;

            // 虹色
            glm::vec4 color = hsv2rgb(mBaseHue, 1.0f, 1.0f);
            mBaseHue = std::fmod(mBaseHue + 0.005f, 1.0f);

            outVertices.push_back({left0, color});
            outVertices.push_back({right0, color});
            outVertices.push_back({left1, color});
            outVertices.push_back({right1, color});
        }
    }

private:
    static constexpr float HALF_WIDTH = 0.02f;
    float mBaseHue = 0.0f;

    static glm::vec4 hsv2rgb(float h, float s, float v) {
        h = glm::fract(h);
        float c = v * s;
        float x = c * (1.0f - std::fabs(std::fmod(h * 6.0f, 2.0f) - 1.0f));
        float m = v - c;
        glm::vec3 rgb;
        if (h < 1.0f / 6.0f)      rgb = {c, x, 0};
        else if (h < 2.0f / 6.0f) rgb = {x, c, 0};
        else if (h < 3.0f / 6.0f) rgb = {0, c, x};
        else if (h < 4.0f / 6.0f) rgb = {0, x, c};
        else if (h < 5.0f / 6.0f) rgb = {x, 0, c};
        else                      rgb = {c, 0, x};
        return glm::vec4(rgb + glm::vec3(m), 1.0f);
    }
};
