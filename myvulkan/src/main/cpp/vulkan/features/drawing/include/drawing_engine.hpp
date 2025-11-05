#pragma once

#include "vulkan_context.h"
#include "surface_context.hpp"
#include "frame_context.hpp"
#include "engine.hpp"
#include "view_bounds.hpp"

class DrawingEngine : public Engine {

public:
    DrawingEngine(VulkanContext &vkContext, std::shared_ptr<Surface> surface);
};
