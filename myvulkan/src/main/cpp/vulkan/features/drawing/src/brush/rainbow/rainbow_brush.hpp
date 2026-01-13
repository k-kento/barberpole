#pragma once

#include "../brush.hpp"
#include <cmath>
#include "color_utils.hpp"

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
            glm::vec2 uv(0.0f, 0.0f);  // 非グローブラシは UV 未使用

            outVertices.push_back({left0, color, uv});
            outVertices.push_back({right0, color, uv});
            outVertices.push_back({left1, color, uv});
            outVertices.push_back({right1, color, uv});
        }
    }

private:
    static constexpr float HALF_WIDTH = 0.02f;
    float mBaseHue = 0.0f;
};
