#pragma once

#include "../brush.hpp"
#include "../../pipeline/normal_pipeline.hpp"
#include "../../ubo_data.hpp"
#include "../../input_vertex.hpp"
#include "generic_buffer.hpp"
#include "ubo_buffer.hpp"
#include "glm/glm.hpp"
#include "../../renderer_constants.hpp"

class NormalBrush : public Brush {
public:

    struct FrameContext {
        std::unique_ptr<GenericBuffer<InputVertex>> vertexBuffer;
        std::unique_ptr<UboBuffer<UboData>> uboBuffer;
        vk::UniqueDescriptorSet descriptorSet;
        uint32_t writtenVertexCount = 0;
    };

    explicit NormalBrush(VulkanContext &context, BasePipeline &pipeline) : Brush(pipeline), mDevice(context.getDevice()) {
        createDescriptorPool();

        mFrames.reserve(MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            FrameContext frameContext{};

            frameContext.vertexBuffer = std::make_unique<GenericBuffer<InputVertex>>(
                    context,
                    sizeof(InputVertex) * MAX_VERTEX_COUNT,
                    vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
            );

            frameContext.uboBuffer = std::make_unique<UboBuffer<UboData>>(context);
            frameContext.descriptorSet = allocateDescriptorSet(frameContext.uboBuffer->getBuffer(),
                                                                frameContext.uboBuffer->getSize());

            mFrames.emplace_back(std::move(frameContext));
        }
    }

    void setProjection(const glm::mat4 &projection) override {
        for (auto &frame : mFrames) {
            frame.uboBuffer->update({projection});
        }
    }

    void applyStroke(const Stroke &stroke, uint32_t frameIndex) override {
        auto &frame = mFrames[frameIndex];

        // 差分更新
        const auto &src = stroke.getVertices();
        auto strokeVertexCount = static_cast<uint32_t>(src.size());

        if (strokeVertexCount <= frame.writtenVertexCount) return;

        uint32_t newCount = strokeVertexCount - frame.writtenVertexCount;
        const InputVertex *srcPtr = src.data() + frame.writtenVertexCount;

        frame.vertexBuffer->update(srcPtr, newCount, frame.writtenVertexCount);

        frame.writtenVertexCount = strokeVertexCount;
    }

    void record(vk::CommandBuffer cmd, uint32_t frameIndex) override {
        auto &frame = mFrames[frameIndex];
        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline.getPipeline());
        cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                               mPipeline.getLayout(),
                               0, {frame.descriptorSet.get()}, {});

        vk::DeviceSize offsets[] = {0};
        cmd.bindVertexBuffers(0, frame.vertexBuffer->getBuffer(), offsets);

        cmd.draw(frame.writtenVertexCount, 1, 0, 0);
    }

private:
    vk::Device mDevice;
    vk::UniqueDescriptorPool mDescriptorPool;
    std::vector<FrameContext> mFrames;
    static constexpr size_t MAX_VERTEX_COUNT = 4096;

    void createDescriptorPool() {
        vk::DescriptorPoolSize poolSize{};
        poolSize.type = vk::DescriptorType::eUniformBuffer;
        poolSize.descriptorCount = MAX_FRAMES_IN_FLIGHT;

        vk::DescriptorPoolCreateInfo poolInfo{};
        poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;

        mDescriptorPool = mDevice.createDescriptorPoolUnique(poolInfo);
    }

    vk::UniqueDescriptorSet allocateDescriptorSet(vk::Buffer uboBuffer, vk::DeviceSize uboSize) {
        vk::DescriptorSetLayout layout = mPipeline.getDescriptorSetLayout();

        vk::DescriptorSetAllocateInfo allocInfo{};
        allocInfo.descriptorPool = mDescriptorPool.get();
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &layout;

        auto sets = mDevice.allocateDescriptorSetsUnique(allocInfo);
        auto descriptorSet = std::move(sets.front());

        vk::DescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uboBuffer;
        bufferInfo.offset = 0;
        bufferInfo.range = uboSize;

        vk::WriteDescriptorSet descriptorWrite{};
        descriptorWrite.dstSet = descriptorSet.get();
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        mDevice.updateDescriptorSets(1, &descriptorWrite, 0, nullptr);

        return descriptorSet;
    }
};
