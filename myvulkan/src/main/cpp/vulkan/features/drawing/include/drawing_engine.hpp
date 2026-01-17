#pragma once

#include "engine.hpp"
#include "surface_context.hpp"
#include "view_bounds.hpp"
#include "vulkan_context.h"

class DrawingEngine : public Engine {
   public:
    DrawingEngine(VulkanContext& vkContext, std::shared_ptr<Surface> surface);
};
