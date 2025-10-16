#pragma once

#include "vulkan/vulkan.hpp"
#include "device_buffer.h"
#include "vulkan_context.h"
#include "../../common/mesh/regular_polygon.hpp"
#include "../../common/mesh/mesh_buffer.hpp"

class KaleidoscopeMeshManager {

public:

    explicit KaleidoscopeMeshManager(VulkanContext &context) {
        auto meshData = RegularPolygon(4, 0.5f).getMeshData();
        auto square = std::make_unique<MeshBuffer>(context, meshData);
        mMeshBuffers.push_back(std::move(square));
    }

    void bind(vk::CommandBuffer& cmd, uint32_t binding) {
        for (auto &buffer : mMeshBuffers) {
            buffer->bind(cmd, binding);
        }
    }

    void draw(vk::CommandBuffer& cmd, uint32_t instanceCount) {
        for (auto &buffer : mMeshBuffers) {
            buffer->draw(cmd, instanceCount);
        }
    }

private:
    std::vector<std::unique_ptr<MeshBuffer>> mMeshBuffers;
};
