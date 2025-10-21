add_library(vulkan_context STATIC
        device_helper.cpp
        physical_device_helper.cpp
        vulkan_context.cpp
)

target_include_directories(vulkan_context
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/external/Vulkan-Hpp
        ${CMAKE_SOURCE_DIR}/util
)
