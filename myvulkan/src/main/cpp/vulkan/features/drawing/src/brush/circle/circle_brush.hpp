#pragma once

#include "../brush.hpp"
#include "color_utils.hpp"
#include <cmath>
#include <random>

class CircleBrush : public Brush {
public:
    struct Params {
        float pointSize = 50.0f;  // ピクセル単位
    };

    CircleBrush(VulkanContext& ctx, BasePipeline& pipe)
            : Brush(ctx, pipe) {}

    void generateVertices(const std::vector<glm::vec2>& points,
                          std::vector<InputVertex>& out) override {

        // 追加された分だけ送る
        for (size_t i = mLast; i < points.size(); i++) {
            // 虹色
            glm::vec4 color = hsv2rgb(mBaseHue, 0.8f, 1.0f);
            mBaseHue = std::fmod(mBaseHue + 0.05f, 1.0f);

            out.push_back({
                points[i],
                color,
                glm::vec2(0.0f, 0.0f)
            });
        }

        mLast = points.size();
    }

    void updateUboWithTime(uint32_t frameIndex, float time) override {
        UboData ubo{};
        ubo.projection = mProjection;
        ubo.time = time;
        ubo.glowIntensity = mParams.pointSize;  // pointSize として使用
        ubo.glowRadius = 0.0f;
        ubo.pulsePeriod = 1.0f;
        mFrames[frameIndex].uboBuffer->update(ubo);
    }

    void setParams(const Params& params) { mParams = params; }

    void resetFrameContexts() {
        Brush::resetFrameContexts();
        mLast = 0;
        mBaseHue = 0.0f;
    }

private:
    Params mParams;
    size_t mLast = 0;
    float mBaseHue = 0.0f;
};
