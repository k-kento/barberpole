#pragma once

#include <memory>
#include <vector>

#include "../brush/brush.hpp"
#include "stroke.hpp"

/**
 * StrokeManager クラス
 *
 * 役割:
 * - 現在アクティブな Stroke と Brush を管理する指揮者
 * - マウスイベントを受け取り、Stroke への座標追加と Brush による頂点生成をトリガー
 * - 描画ループから frameIndex を受け取り、GPU へのデータ転送と描画命令の記録を依頼
 */
class StrokeManager {
   public:
    StrokeManager() { startNewStroke(); }

    void setCurrentBrush(Brush* brush) { mCurrentBrush = brush; }

    /**
     * マウス/タッチ移動イベント処理
     */
    void onTouchMove(glm::vec2 pos) {
        if (!mCurrentStroke || !mCurrentBrush) return;

        mCurrentStroke->addPoint(pos);
        mCurrentBrush->generateVertices(mCurrentStroke->getPoints(), mCurrentStroke->getVerticesMutable());
        mNeedsRebuild = true;
    }

    /**
     * ストローク終了（指を離した時）
     */
    void endStroke() {
        if (mCurrentStroke && !mCurrentStroke->isEmpty()) {
            mStrokes.push_back(std::move(mCurrentStroke));
        }
        startNewStroke();
    }

    /**
     * 描画処理（毎フレーム呼ばれる）
     */
    void onDraw(uint32_t frameIndex, vk::CommandBuffer cmd) {
        if (!mCurrentBrush) return;

        rebuildVerticesIfNeeded();

        // GPU バッファへの差分更新
        mCurrentBrush->updateGPUBuffer(frameIndex, mCombinedVertices);

        // 描画コマンド記録
        mCurrentBrush->record(cmd, frameIndex);
    }

    /**
     * プロジェクション行列を設定
     */
    void setProjection(const glm::mat4& projection) {
        if (mCurrentBrush) {
            mCurrentBrush->setProjection(projection);
        }
    }

    /**
     * UBO を更新（time を含む）
     */
    void updateTime(uint32_t frameIndex, float time) {
        if (mCurrentBrush) {
            mCurrentBrush->updateUboWithTime(frameIndex, time);
        }
    }

    [[nodiscard]] const std::vector<InputVertex>& getCombinedVertices() {
        rebuildVerticesIfNeeded();
        return mCombinedVertices;
    }

    [[nodiscard]] uint32_t getVertexCount() {
        rebuildVerticesIfNeeded();
        return static_cast<uint32_t>(mCombinedVertices.size());
    }

    void clear() {
        mStrokes.clear();
        startNewStroke();
        mCombinedVertices.clear();
        mNeedsRebuild = true;
    }

   private:
    void startNewStroke() {
        mCurrentStroke = std::make_unique<Stroke>();
        mNeedsRebuild = true;
    }

    void rebuildVerticesIfNeeded() {
        if (!mNeedsRebuild) return;
        rebuildVertices();
        mNeedsRebuild = false;
    }

    void rebuildVertices() {
        mCombinedVertices.clear();

        for (const auto& stroke : mStrokes) {
            appendStrokeVertices(*stroke);
        }

        if (mCurrentStroke) {
            appendStrokeVertices(*mCurrentStroke);
        }
    }

    void appendStrokeVertices(const Stroke& stroke) {
        const auto& vertices = stroke.getVertices();
        if (vertices.empty()) return;

        // 退化三角形を追加して前のストロークと繋がらないようにする
        if (!mCombinedVertices.empty()) {
            mCombinedVertices.push_back(mCombinedVertices.back());
            mCombinedVertices.push_back(vertices.front());
        }

        mCombinedVertices.insert(mCombinedVertices.end(), vertices.begin(), vertices.end());
    }

    Brush* mCurrentBrush = nullptr;
    std::vector<std::unique_ptr<Stroke>> mStrokes;
    std::unique_ptr<Stroke> mCurrentStroke;
    std::vector<InputVertex> mCombinedVertices;
    bool mNeedsRebuild = true;
};
