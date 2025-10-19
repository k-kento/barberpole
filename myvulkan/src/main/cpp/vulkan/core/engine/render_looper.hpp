#pragma once

#include <functional>
#include <thread>
#include "render_message.hpp"

class RenderLooper {
public:
    RenderLooper(std::function<void()> frameCallback,
                 std::function<void(std::unique_ptr<RenderMessage>)> handleMessage)
            : mFrameCallback(frameCallback),
              mHandleMessage(handleMessage) {
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

    void postTask(std::function<void()> task) {
        std::lock_guard<std::mutex> lock(mMutex);
        mTasks.push(std::move(task));
    }

    void postMessage(std::unique_ptr<RenderMessage> message) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMessageQueue.push(std::move(message));
    }

private:
    void loop() {
        while (mRunning.load()) {
            mFrameCallback();


            // ----------------------------
            // タスク処理
            // ----------------------------
            std::function<void()> task;
            {
                std::lock_guard<std::mutex> lock(mMutex);
                if (!mTasks.empty()) {
                    task = std::move(mTasks.front());
                    mTasks.pop();
                }
            }
            if (task) task();


            // ----------------------------
            // メッセージ処理
            // ----------------------------
            std::unique_ptr<RenderMessage> message;
            {
                std::lock_guard<std::mutex> lock(mMutex);
                if (!mMessageQueue.empty()) {
                    message = std::move(mMessageQueue.front());
                    mMessageQueue.pop();
                }
            }
            if (message) mHandleMessage(std::move(message));

            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // 60FPS
        }
    }

    std::atomic<bool> mRunning{false};
    std::thread mThread;
    std::function<void()> mFrameCallback;
    std::function<void(std::unique_ptr<RenderMessage>)> mHandleMessage;
    std::queue<std::function<void()>> mTasks;
    std::queue<std::unique_ptr<RenderMessage>> mMessageQueue;
    std::mutex mMutex;
};
