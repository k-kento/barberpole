add_subdirectory(memory)

add_library(core STATIC
        pipeline/pipeline_builder.cpp
        renderpass/render_pass.cpp

        engine.cpp

        surface/surface.cpp
        swap_chain/swap_chain.cpp
        texture/texture.cpp
        utils/vulkan_utils.cpp

        surface_context.cpp
)

target_include_directories(core
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/mesh
        ${CMAKE_CURRENT_SOURCE_DIR}/pipeline
        ${CMAKE_CURRENT_SOURCE_DIR}/surface
        ${CMAKE_CURRENT_SOURCE_DIR}/swap_chain
        ${CMAKE_CURRENT_SOURCE_DIR}/texture
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/context/include
        PRIVATE
        ${CMAKE_SOURCE_DIR}/external/Vulkan-Hpp
        ${CMAKE_SOURCE_DIR}/util
        ${CMAKE_SOURCE_DIR}/external/glm
        ${CMAKE_SOURCE_DIR}/external/stb
)

target_link_libraries(core
        PUBLIC
        memory
        PRIVATE
        vulkan_context
)
