#pragma once

#include "vulkan/vulkan.hpp"
#include "vulkan_context.h"
#include "storage_buffer.hpp"
#include "glm/glm.hpp"
#include "../stroke.hpp"
#include "../pipeline/base_pipeline.hpp"

class Brush {
public:
    Brush(BasePipeline& pipeline) : mPipeline(pipeline) {}

    virtual ~Brush() = default;

    virtual void setProjection(const glm::mat4 &projection) = 0;

    virtual void applyStroke(const Stroke &stroke, uint32_t frameIndex) = 0;

    virtual void record(vk::CommandBuffer cmd, uint32_t frameIndex) = 0;

protected:
    BasePipeline &mPipeline;
};
