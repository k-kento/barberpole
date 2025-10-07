#pragma once

#include "vulkan_renderer.h"
#include "graphics_pipeline.h"
#include "memory/vulkan_buffer.h"
#include <glm/glm.hpp>
#include "renderer/descriptor/descriptor.hpp"
#include "memory/ubo_buffer.hpp"
#include <optional>
#include <vector>
#include "regular_polygon.hpp"

struct InstanceData {
    glm::mat4 model;
};

struct UBO {
    glm::mat4 projection;
};

class KaleidoscopeRenderer : public VulkanRenderer {
public:
    KaleidoscopeRenderer(VulkanContext *vkContext, ANativeWindow *window);

protected:
    void recordDrawCommands(vk::CommandBuffer cmdBuffer, uint32_t imageIndex) override;

    void renderFrame() override;

private:
    std::unique_ptr<RegularPolygon> mMesh;
    std::unique_ptr<VulkanBuffer> mVertexBuffer;
    std::unique_ptr<VulkanBuffer> mInstanceBuffer;
    std::unique_ptr<VulkanBuffer> mIndexBuffer;
    std::unique_ptr<VulkanBuffer> mUboBuffer;
    std::unique_ptr<GraphicsPipeline> mGraphicsPipeline;
    std::unique_ptr<Descriptor> mDescriptorManager;
    vk::DescriptorSet mDescriptorSet;
    glm::mat4 mProjectionMatrix{};
    std::optional<UboBuffer<UBO>> mUboWrapper;


private:
    void updateProjectionMatrix(uint32_t width, uint32_t height);

    void createBuffer();

    void createGraphicsPipeline();

    void createDescriptorSet();

    static PipelineConfig createPipeLineConfig();
};
