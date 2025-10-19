#pragma once

#include "vulkan/vulkan.hpp"
#include "device_buffer.h"
#include "vulkan_context.h"
#include "regular_polygon.hpp"
#include "../../core/mesh/mesh_buffer.hpp"

class KaleidoscopeMeshManager {

public:

    explicit KaleidoscopeMeshManager(VulkanContext &context) {
        auto polygon = RegularPolygon(3, 1.0f);
        auto mesh = std::make_unique<MeshBuffer<Vertex>>(context, polygon.vertices, polygon.indices);
        mMeshBuffers.push_back(std::move(mesh));
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
    std::vector<std::unique_ptr<MeshBuffer<Vertex>>> mMeshBuffers;
};
