#pragma once

#include <vulkan/vulkan.hpp>
#include "render_message.hpp"

class RendererInterface {
public:
    virtual void recordDrawCommands(vk::CommandBuffer cmdBuffer) = 0;

    virtual void renderFrame(float deltaTimeMs) = 0;

    virtual void handleMessage(std::unique_ptr<RenderMessage> message) = 0;
};
