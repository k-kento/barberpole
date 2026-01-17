#include "engine.hpp"

Engine::Engine() {
    mRenderLooper = std::make_unique<RenderLooper>([this]() { onRenderFrame(); },
                                                   [this](std::unique_ptr<RenderMessage> msg) {
                                                       if (mRenderer) {
                                                           mRenderer->handleMessage(std::move(msg));
                                                       }
                                                   });
}

void Engine::start() {
    mRenderLooper->start();
}

void Engine::setRenderer(std::unique_ptr<Renderer>&& renderer) {
    mRenderer = std::move(renderer);
}

void Engine::stop() {
    mRenderLooper->stop();
}

void Engine::postTask(std::function<void()> task) {
    mRenderLooper->postTask(task);
}

void Engine::postMessage(std::unique_ptr<RenderMessage> message) {
    mRenderLooper->postMessage(std::move(message));
}

void Engine::onRenderFrame() {
    // 経過時間計算
    auto now = std::chrono::high_resolution_clock::now();
    float deltaTimeMs = std::chrono::duration<float, std::milli>(now - mLastFrameTime).count();
    mLastFrameTime = now;
    if (mRenderer) {
        mRenderer->renderFrame(deltaTimeMs);
    }
}
