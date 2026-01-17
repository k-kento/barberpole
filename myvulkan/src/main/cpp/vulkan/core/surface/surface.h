#pragma once

#include <android/native_window.h>

#include <vulkan/vulkan.hpp>

#include "vulkan_context.h"

class Surface {
   public:
    Surface(vk::Instance vkInstance, ANativeWindow* window);

    ~Surface();

    [[nodiscard]] vk::SurfaceKHR getSurface() const { return mSurface.get(); }

    [[nodiscard]] uint32_t getWidth() const {
        return mWindow ? static_cast<uint32_t>(ANativeWindow_getWidth(mWindow)) : 0;
    }

    [[nodiscard]] uint32_t getHeight() const {
        return mWindow ? static_cast<uint32_t>(ANativeWindow_getHeight(mWindow)) : 0;
    }

   private:
    vk::UniqueSurfaceKHR mSurface;
    ANativeWindow* mWindow = nullptr;
};
