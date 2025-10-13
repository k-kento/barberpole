#pragma once

#include <functional>
#include <thread>

class RenderLooper {
public:
    RenderLooper(std::function<void()> frameCallback)
            : mFrameCallback(frameCallback) {
    }

    void start() {
        if (mRunning.load()) return;
        mRunning.store(true);
        mThread = std::thread(&RenderLooper::loop, this);
    }

    void stop() {
        if (!mRunning.load()) return;
        mRunning.store(false);
        if (mThread.joinable()) mThread.join();
    }

private:
    void loop() {
        while (mRunning.load()) {
            mFrameCallback();
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // 60FPS
        }
    }

    std::atomic<bool> mRunning{false};
    std::thread mThread;
    std::function<void()> mFrameCallback;
};
