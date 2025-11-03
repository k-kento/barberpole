#include "drawing_engine.hpp"

DrawingEngine::DrawingEngine(VulkanContext &vulkanContext, std::unique_ptr<Surface> surface) : mVulkanContext(
        vulkanContext) {
    mRenderLooper = std::make_unique<RenderLooper>(
            [this]() {
                onRenderFrame();
            },
            [this](std::unique_ptr<RenderMessage> msg) {
                if (mDrawingRenderer) {
                    mDrawingRenderer->handleMessage(std::move(msg));
                }
            });

    auto surfaceShared = std::shared_ptr<Surface>(std::move(surface));

    postTask([this, surfaceShared]() {
        auto surfaceContext = std::make_unique<SurfaceContext>(mVulkanContext, surfaceShared);
        mDrawingRenderer = std::make_unique<DrawingRenderer>(mVulkanContext, std::move(surfaceContext));
    });
}

void DrawingEngine::start() {
    mRenderLooper->start();
}

void DrawingEngine::stop() {
    mRenderLooper->stop();
}

void DrawingEngine::postTask(std::function<void()> task) {
    mRenderLooper->postTask(task);
}

void DrawingEngine::postMessage(std::unique_ptr<RenderMessage> message) {
    mRenderLooper->postMessage(std::move(message));
}

void DrawingEngine::onRenderFrame() {
    // 経過時間計算
    auto now = std::chrono::high_resolution_clock::now();
    float deltaTimeMs = std::chrono::duration<float, std::milli>(now - mLastFrameTime).count();
    mLastFrameTime = now;
    if (mDrawingRenderer) {
        mDrawingRenderer->renderFrame(deltaTimeMs);
    }
}
