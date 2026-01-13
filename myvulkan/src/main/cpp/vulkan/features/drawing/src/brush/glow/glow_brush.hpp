#pragma once

#include "../brush.hpp"
#include <cmath>
#include "color_utils.hpp"
#include "log.h"

/**
 * GlowBrush - グローブラシ
 *
 * 発光エフェクト付きの虹色ブラシ
 * - グロー（光の広がり）
 * - 発光パルス（周期的な明滅）
 * - 可変線幅
 */
class GlowBrush : public Brush {
public:
    struct Params {
        float lineWidth = 0.015f;      // コア線の半幅 (normalized)
        float glowRadius = 0.05f;      // グロー半径 (normalized) - 大きく
        float glowIntensity = 1.0f;    // グロー強度 (0.0~1.0) - 最大
        float pulsePeriod = 1.0f;      // パルス周期 (秒)
    };

    GlowBrush(VulkanContext& context, BasePipeline& pipeline)
        : Brush(context, pipeline) {}

    void generateVertices(const std::vector<glm::vec2>& points,
                          std::vector<InputVertex>& outVertices) override {
        const auto total = static_cast<uint32_t>(points.size());
        LOGD("GlowBrush::generateVertices points=%d", total);
        if (total < 2) return;

        // 既存の頂点数から、新しく追加すべき区間を計算
        uint32_t existingSegments = outVertices.empty() ? 0 : (outVertices.size() / 4);
        uint32_t startIndex = existingSegments;

        // グロー込みの総幅
        float totalHalfWidth = mParams.lineWidth + mParams.glowRadius;
        LOGD("GlowBrush: totalHalfWidth=%f, startIndex=%d", totalHalfWidth, startIndex);

        for (uint32_t i = startIndex; i < total - 1; ++i) {
            glm::vec2 p0 = points[i];
            glm::vec2 p1 = points[i + 1];

            glm::vec2 dir = p1 - p0;
            float len = glm::length(dir);
            if (len < 1e-6f) continue;
            dir /= len;

            glm::vec2 normal(-dir.y, dir.x);
            glm::vec2 offset = normal * totalHalfWidth;

            glm::vec2 left0 = p0 + offset;
            glm::vec2 right0 = p0 - offset;
            glm::vec2 left1 = p1 + offset;
            glm::vec2 right1 = p1 - offset;

            // 虹色
            glm::vec4 color = hsv2rgb(mBaseHue, 1.0f, 1.0f);
            mBaseHue = std::fmod(mBaseHue + 0.005f, 1.0f);

            // UV: x = -1 (左端) ~ +1 (右端)、シェーダーで abs() してグロー計算
            glm::vec2 uvLeft(-1.0f, 0.0f);
            glm::vec2 uvRight(1.0f, 0.0f);

            outVertices.push_back({left0, color, uvLeft});
            outVertices.push_back({right0, color, uvRight});
            outVertices.push_back({left1, color, uvLeft});
            outVertices.push_back({right1, color, uvRight});
        }
        LOGD("GlowBrush: generated vertices=%zu", outVertices.size());
    }

    void setParams(const Params& params) { mParams = params; }
    const Params& getParams() const { return mParams; }

    /**
     * UBO を更新（time とグローパラメータを含む）
     */
    void updateUboWithTime(uint32_t frameIndex, float time) override {
        LOGD("GlowBrush::updateUboWithTime frame=%d time=%f", frameIndex, time);
        UboData ubo{};
        ubo.projection = mProjection;
        ubo.time = time;
        ubo.glowIntensity = mParams.glowIntensity;
        ubo.glowRadius = mParams.glowRadius;
        ubo.pulsePeriod = mParams.pulsePeriod;
        mFrames[frameIndex].uboBuffer->update(ubo);
    }

private:
    Params mParams;
    float mBaseHue = 0.0f;
};
