#pragma once

#include "../brush.hpp"

/**
 * NormalBrush - 単色ブラシ
 *
 * 白色の単色で描画する基本的なブラシ
 */
class NormalBrush : public Brush {
public:
    NormalBrush(VulkanContext& context, BasePipeline& pipeline)
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

            // 白色
            glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);

            outVertices.push_back({left0, color});
            outVertices.push_back({right0, color});
            outVertices.push_back({left1, color});
            outVertices.push_back({right1, color});
        }
    }

private:
    static constexpr float HALF_WIDTH = 0.02f;
};
