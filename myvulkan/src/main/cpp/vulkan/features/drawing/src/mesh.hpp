#pragma once

#include "vulkan/vulkan.hpp"
#include "device_buffer.h"
#include "vulkan_context.h"
#include "mesh_buffer.hpp"
#include "vertex.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/constants.hpp>

class Mesh {

public:

    explicit Mesh(VulkanContext &context) {
        uint32_t segments = 32;

        std::vector<drawing::Vertex> vertices;

        // 中心点
        drawing::Vertex centerVertex{};
        centerVertex.position = {0.0f, 0.0f};
        vertices.push_back(centerVertex);

        for (int i = 0; i <= segments; ++i) {
            float angle = i * 2.0f * glm::pi<float>() / segments;
            float x = cos(angle);
            float y = sin(angle);

            drawing::Vertex v{};
            v.position = {x, y};
            vertices.push_back(v);
        }

        std::vector<uint16_t> indices;

        for (int i = 1; i <= segments; ++i) {
            indices.push_back(0);     // 中心
            indices.push_back(i);     // 現在の周辺頂点
            indices.push_back(i + 1); // 次の周辺頂点
        }

        auto mesh = std::make_unique<MeshBuffer<drawing::Vertex>>(context, vertices, indices);
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
    std::vector<std::unique_ptr<MeshBuffer<drawing::Vertex>>> mMeshBuffers;
};
