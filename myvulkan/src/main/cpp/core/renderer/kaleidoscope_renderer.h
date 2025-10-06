#pragma once

#include "vulkan_renderer.h"
#include "graphics_pipeline.h"
#include "memory/vulkan_buffer.h"
#include <glm/glm.hpp>
#include "renderer/descriptor/descriptor.hpp"
#include "memory/ubo_buffer.hpp"
#include <optional>
#include <vector>

struct Vertex {
    float pos[2];
};

struct InstanceData {
    glm::mat4 model;
};

struct UBO {
    glm::mat4 projection;
};

inline constexpr std::array<Vertex, 4> vertices = {{
                                                           {{-0.5f, 0.5f}}, // 0
                                                           {{0.5f, 0.5f}}, // 1
                                                           {{0.5f, -0.5f}}, // 2
                                                           {{-0.5f, -0.5f}}  // 3
                                                   }};

inline constexpr std::array<uint16_t, 6> indices = {
        0, 1, 2,
        2, 3, 0
};

class KaleidoscopeRenderer : public VulkanRenderer {
public:
    KaleidoscopeRenderer(VulkanContext* vkContext, ANativeWindow* window);
protected:
    void recordDrawCommands(vk::CommandBuffer cmdBuffer, uint32_t imageIndex) override;

    void renderFrame() override;
private:

    std::unique_ptr<VulkanBuffer> mVertexBuffer;
    std::unique_ptr<VulkanBuffer> mInstanceBuffer;
    std::unique_ptr<VulkanBuffer> mIndexBuffer;
    std::unique_ptr<VulkanBuffer> mUboBuffer;
    std::unique_ptr<GraphicsPipeline> mGraphicsPipeline;
    std::unique_ptr<Descriptor> mDescriptorManager;
    vk::DescriptorSet mDescriptorSet;
    glm::mat4 mProjectionMatrix;
    std::optional<UboBuffer<UBO>> mUboWrapper;



private:
    void updateProjectionMatrix(uint32_t width, uint32_t height);
    void createBuffer();
    void createGraphicsPipeline();
    void createDescriptorSet();
    static PipelineConfig createPipeLineConfig();
};
