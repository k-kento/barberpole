add_library(drawing STATIC
        src/drawing_renderer.cpp
        src/drawing_engine.cpp
)

target_include_directories(drawing
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/vulkan/core
        ${CMAKE_SOURCE_DIR}/external/Vulkan-Hpp
        ${CMAKE_SOURCE_DIR}/util
        ${CMAKE_SOURCE_DIR}/external/glm
)

target_link_libraries(drawing
        PRIVATE core
)
