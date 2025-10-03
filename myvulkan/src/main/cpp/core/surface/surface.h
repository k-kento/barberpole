#pragma once

#include <vulkan/vulkan.hpp>
#include <android/native_window.h>
#include "vulkan_context.h"

class Surface {
public:
    Surface(vk::Instance vkInstance, ANativeWindow *window);

    ~Surface();

    [[nodiscard]] vk::SurfaceKHR getSurface() const {
        return mSurface.get();
    }

private:
    vk::UniqueSurfaceKHR mSurface;
    ANativeWindow *mWindow = nullptr;
};
