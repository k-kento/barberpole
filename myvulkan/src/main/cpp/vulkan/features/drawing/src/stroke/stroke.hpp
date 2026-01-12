#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "../input_vertex.hpp"

/**
 * Stroke クラス
 *
 * 役割:
 * - ユーザーの入力座標と生成された頂点データを保持する純粋なデータ容器
 * - Vulkan リソースや Brush のロジックは持たない
 * - Undo/Redo の最小単位として機能
 */
class Stroke {
public:
    static constexpr size_t INITIAL_RESERVE = 256;

    Stroke() {
        mPoints.reserve(INITIAL_RESERVE);
        mVertices.reserve(INITIAL_RESERVE * 4);
    }

    void addPoint(glm::vec2 point) {
        mPoints.push_back(point);
    }

    void setVertices(std::vector<InputVertex>&& vertices) {
        mVertices = std::move(vertices);
    }

    void appendVertices(const std::vector<InputVertex>& newVertices) {
        mVertices.insert(mVertices.end(), newVertices.begin(), newVertices.end());
    }

    [[nodiscard]] const std::vector<glm::vec2>& getPoints() const {
        return mPoints;
    }

    [[nodiscard]] const std::vector<InputVertex>& getVertices() const {
        return mVertices;
    }

    [[nodiscard]] std::vector<InputVertex>& getVerticesMutable() {
        return mVertices;
    }

    [[nodiscard]] uint32_t getPointCount() const {
        return static_cast<uint32_t>(mPoints.size());
    }

    [[nodiscard]] uint32_t getVertexCount() const {
        return static_cast<uint32_t>(mVertices.size());
    }

    [[nodiscard]] bool isEmpty() const {
        return mPoints.empty();
    }

    void clear() {
        mPoints.clear();
        mVertices.clear();
    }

private:
    std::vector<glm::vec2> mPoints;
    std::vector<InputVertex> mVertices;
};
