#pragma once

#include "engine.hpp"
#include "surface_context.hpp"
#include "view_bounds.hpp"
#include "vulkan_context.h"

class KaleidoscopeEngine : public Engine {
   public:
    KaleidoscopeEngine(VulkanContext& vkContext,
                       std::shared_ptr<Surface> surface,
                       uint32_t deviceRotationDegree,
                       std::string& texturePath);
};
