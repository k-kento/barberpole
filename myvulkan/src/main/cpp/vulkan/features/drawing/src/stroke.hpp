#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <cmath>
#include "input_vertex.hpp"

class Stroke {

public:
    void addPoint(InputVertex &point) {
        mPoints.push_back(point);
        generateNewSegment();
    }

    const std::vector<InputVertex> &getVertices() const { return mVertices; }

    const uint32_t getVertexCount() const { return static_cast<uint32_t>(mVertices.size()); }

private:
    // 三角ストリップ方式で、ポリゴンを作成する
    void generateNewSegment() {
        const uint32_t total = static_cast<uint32_t>(mPoints.size());
        if (total < 2) return;

        // 直近追加された1区間だけ生成
        const uint32_t i = total - 2;

        glm::vec2 p0 = glm::vec2(mPoints[i].position);
        glm::vec2 p1 = glm::vec2(mPoints[i + 1].position);

        glm::vec2 dir = p1 - p0;
        float len = glm::length(dir);
        if (len < 1e-6f) return;
        dir /= len;

        glm::vec2 normal(-dir.y, dir.x);
        normal *= HALF_WIDTH;

        glm::vec2 left0 = p0 + normal;
        glm::vec2 right0 = p0 - normal;
        glm::vec2 left1 = p1 + normal;
        glm::vec2 right1 = p1 - normal;

        float u0 = mTotalLength;
        mTotalLength += len;
        float u1 = mTotalLength;

        float hueOrigin = mBaseHue;
        mBaseHue = fmod(mBaseHue + 0.005f, 1.0f);

        glm::vec4 rgb = hsv2rgb(hueOrigin, 1.0f, 1.0f);

        mVertices.push_back({glm::vec4(left0, 0.0f, 1.0f), rgb});
        mVertices.push_back({glm::vec4(right0, 0.0f, 1.0f), rgb});
        mVertices.push_back({glm::vec4(left1, 0.0f, 1.0f), rgb});
        mVertices.push_back({glm::vec4(right1, 0.0f, 1.0f), rgb});
    }

private:
    static constexpr float HALF_WIDTH = 0.2f;

    std::vector<InputVertex> mPoints;
    std::vector<InputVertex> mVertices;

    float mTotalLength = 0.0f; // u軸計算用に累積距離を保存
    float mBaseHue = 0.0f;

    glm::vec4 hsv2rgb(float h, float s, float v) {
        h = glm::fract(h);
        float c = v * s;
        float x = c * (1 - fabs(fmod(h * 6.0f, 2.0f) - 1));
        float m = v - c;
        glm::vec3 rgb;
        if (h < 1.0f/6.0f)      rgb = {c, x, 0};
        else if (h < 2.0f/6.0f) rgb = {x, c, 0};
        else if (h < 3.0f/6.0f) rgb = {0, c, x};
        else if (h < 4.0f/6.0f) rgb = {0, x, c};
        else if (h < 5.0f/6.0f) rgb = {x, 0, c};
        else                    rgb = {c, 0, x};
        return glm::vec4(rgb + glm::vec3(m), 1.0f);
    }
};
