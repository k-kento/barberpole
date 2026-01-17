#pragma once

#include "render_message.hpp"
#include "vulkan/vulkan.hpp"

class Renderer {
   public:
    virtual void renderFrame(float deltaTimeMs) = 0;

    virtual void handleMessage(std::unique_ptr<RenderMessage> message) = 0;
};
