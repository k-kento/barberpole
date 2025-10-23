#pragma once

#include <vulkan/vulkan.hpp>
#include "render_message.hpp"

class RendererInterface {
public:
    virtual void recordDrawCommands(vk::CommandBuffer cmdBuffer, uint32_t frameIndex) = 0;

    virtual void renderFrame(float deltaTimeMs, uint32_t currentFrame) = 0;

    virtual void handleMessage(std::unique_ptr<RenderMessage> message) = 0;

    virtual uint32_t getMaxFramesInFlight() = 0;
};
