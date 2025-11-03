#pragma once

#include "render_looper.hpp"
#include "drawing_renderer.hpp"
#include <memory>

class DrawingEngine {

public:
    DrawingEngine(VulkanContext& vulkanContext, std::unique_ptr<Surface> surface);

    void start();

    void stop();

    void postTask(std::function<void()> task);

    void postMessage(std::unique_ptr<RenderMessage> message);

private:

    VulkanContext& mVulkanContext;
    std::unique_ptr<DrawingRenderer> mDrawingRenderer;
    std::unique_ptr<RenderLooper> mRenderLooper;
    std::chrono::high_resolution_clock::time_point mLastFrameTime;

    void onRenderFrame();

    void onRenderMessage(std::unique_ptr<RenderMessage> message);

};
