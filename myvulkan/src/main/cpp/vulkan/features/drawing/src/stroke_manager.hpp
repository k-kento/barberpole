#pragma once

#include <vector>
#include <memory>
#include "stroke.hpp"

class StrokeManager {
public:
    StrokeManager() {
        startNewStroke();
    }

    void addPoint(InputVertex &point) {
        if (mCurrentStroke) {
            mCurrentStroke->addPoint(point);
        }
    }

    void endStroke() {
        if (mCurrentStroke && mCurrentStroke->getVertexCount() > 0) {
            mStrokes.push_back(std::move(mCurrentStroke));
        }
        startNewStroke();
    }

    const std::vector<InputVertex> &getVertices() {
        rebuildVertices();
        return mCombinedVertices;
    }

    uint32_t getVertexCount() {
        rebuildVertices();
        return static_cast<uint32_t>(mCombinedVertices.size());
    }

private:
    void startNewStroke() {
        mCurrentStroke = std::make_unique<Stroke>();
        mNeedsRebuild = true;
    }

    void rebuildVertices() {
        mCombinedVertices.clear();

        for (const auto &stroke : mStrokes) {
            appendStrokeVertices(*stroke);
        }

        if (mCurrentStroke) {
            appendStrokeVertices(*mCurrentStroke);
        }
    }

    void appendStrokeVertices(const Stroke &stroke) {
        const auto &vertices = stroke.getVertices();
        if (vertices.empty()) return;

        // 退化三角形を追加して前のストロークと繋がらないようにする
        if (!mCombinedVertices.empty()) {
            mCombinedVertices.push_back(mCombinedVertices.back());
            mCombinedVertices.push_back(vertices.front());
        }

        for (const auto &v : vertices) {
            mCombinedVertices.push_back(v);
        }
    }

    std::vector<std::unique_ptr<Stroke>> mStrokes;
    std::unique_ptr<Stroke> mCurrentStroke;
    std::vector<InputVertex> mCombinedVertices;
    bool mNeedsRebuild = true;
};
