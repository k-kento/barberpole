#pragma once

#include "vulkan_renderer.h"
#include "graphics_pipeline.h"
#include "vulkan_buffer.h"

class KaleidoscopeRenderer : public VulkanRenderer {
public:
    bool init(VulkanContext *vkContext, ANativeWindow *window) override;

protected:
    void recordDrawCommands(VkCommandBuffer cmdBuffer, uint32_t imageIndex) override;

private:

    struct Vertex {
        float pos[2];
    };

    std::unique_ptr<VulkanBuffer> mVertexBuffer;
    VkDeviceMemory mVertexBufferMemory;
    std::unique_ptr<GraphicsPipeline> mGraphicsPipeline;
};
