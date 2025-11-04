#pragma once

#include "vulkan/vulkan.hpp"
#include "render_message.hpp"

class Renderer {
public:
    virtual void renderFrame(float deltaTimeMs) = 0;

    virtual void handleMessage(std::unique_ptr<RenderMessage> message) = 0;
};
