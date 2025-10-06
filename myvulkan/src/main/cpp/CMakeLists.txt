# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("myvulkan")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        platform/android/jni/vulkan_context_jni.cpp
        core/context/vulkan_context.cpp
        core/context/physical_device/physical_device.cpp
        core/context/device/device.cpp
        core/utils/vulkan_utils.cpp
        core/context/vulkan_context.cpp
        core/pipeline/graphics_pipeline.cpp
        core/surface/surface.cpp
        core/surface/swap_chain.cpp
        platform/android/jni/vulkan_renderer_jni.cpp
        core/renderer/kaleidoscope_renderer.cpp
        core/renderer/descriptor/descriptor.cpp
        core/pipeline/render_pass.cpp
        core/renderer/vulkan_renderer.cpp
        core/memory/vulkan_buffer.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/core/context
        ${CMAKE_CURRENT_SOURCE_DIR}/core/context/physical_device
        ${CMAKE_CURRENT_SOURCE_DIR}/core/context/device
        ${CMAKE_CURRENT_SOURCE_DIR}/core/renderer
        ${CMAKE_CURRENT_SOURCE_DIR}/core/renderer/descriptor
        ${CMAKE_CURRENT_SOURCE_DIR}/core/renderer/kaleidoscope
        ${CMAKE_CURRENT_SOURCE_DIR}/core/pipeline
        ${CMAKE_CURRENT_SOURCE_DIR}/core/memory
        ${CMAKE_CURRENT_SOURCE_DIR}/core/surface
        ${CMAKE_CURRENT_SOURCE_DIR}/core/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/android/jni
        ${CMAKE_CURRENT_SOURCE_DIR}/util
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Vulkan-Hpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR)

find_library(VULKAN_LIB vulkan PATHS ${ANDROID_NDK}/sysroot/usr/lib/${ANDROID_TOOLCHAIN_NAME}/)

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        ${VULKAN_LIB}
        android
        log)