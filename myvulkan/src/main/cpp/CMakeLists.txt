# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

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
        platform/android/jni/vulkan_context_jni.cpp
        platform/android/jni/vulkan_renderer_jni.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/vulkan/features/kaleidoscope/include
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/android/jni
        ${CMAKE_CURRENT_SOURCE_DIR}/util
        ${CMAKE_CURRENT_SOURCE_DIR}/external/Vulkan-Hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
)

add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR)

find_library(VULKAN_LIB vulkan PATHS ${ANDROID_NDK}/sysroot/usr/lib/${ANDROID_TOOLCHAIN_NAME}/)

add_subdirectory(vulkan/core/context)
add_subdirectory(vulkan/core)
add_subdirectory(vulkan/features/kaleidoscope)

target_link_libraries(${CMAKE_PROJECT_NAME}
        vulkan_context
        core
        kaleidoscope
        ${VULKAN_LIB}
        android
        log)
