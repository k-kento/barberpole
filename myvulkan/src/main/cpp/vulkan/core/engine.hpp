#pragma once

#include "render_looper.hpp"
#include "renderer.hpp"
#include <memory>

class Engine {

public:
    Engine();

    void start();

    void stop();

    void setRenderer(std::unique_ptr<Renderer>&& renderer);

    void postTask(std::function<void()> task);

    void postMessage(std::unique_ptr<RenderMessage> message);

private:

    std::unique_ptr<Renderer> mRenderer;
    std::unique_ptr<RenderLooper> mRenderLooper;
    std::chrono::high_resolution_clock::time_point mLastFrameTime;

    void onRenderFrame();

    void onRenderMessage(std::unique_ptr<RenderMessage> message);

};
