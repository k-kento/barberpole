add_library(vulkan_context STATIC
        src/device_helper.cpp
        src/physical_device_helper.cpp
        src/vulkan_context.cpp
)

target_include_directories(vulkan_context
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external/Vulkan-Hpp
        ${CMAKE_SOURCE_DIR}/util
)
