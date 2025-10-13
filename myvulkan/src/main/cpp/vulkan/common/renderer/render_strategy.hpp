#pragma once

#include <vulkan/vulkan.hpp>

class RenderStrategy {
public:
    virtual void recordDrawCommands(vk::CommandBuffer cmdBuffer) = 0;

    virtual void renderFrame() = 0;  // 新たに追加
};