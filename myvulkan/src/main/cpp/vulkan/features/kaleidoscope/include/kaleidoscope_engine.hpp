#pragma once

#include "vulkan_context.h"
#include "surface_context.hpp"
#include "frame_context.hpp"
#include "engine.hpp"
#include "view_bounds.hpp"

class KaleidoscopeEngine : public Engine {

public:
    KaleidoscopeEngine(VulkanContext &vkContext,
                       std::shared_ptr<Surface> surface,
                       uint32_t deviceRotationDegree,
                       std::string &texturePath);
};
