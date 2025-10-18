#pragma once

#include <vulkan/vulkan.hpp>

class RendererInterface {
public:
    virtual void recordDrawCommands(vk::CommandBuffer cmdBuffer) = 0;

    virtual void renderFrame(float deltaTimeMs) = 0;
};
