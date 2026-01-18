add_library(memory STATIC
        src/base_buffer.cpp
)

# include: 公開ヘッダー (VertexBuffer, UniformBuffer, StorageBuffer)
# src: 内部実装 (BaseBuffer) - 直接使用しないこと
target_include_directories(memory
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../context/include
        PRIVATE
        ${CMAKE_SOURCE_DIR}/external/Vulkan-Hpp
)

target_link_libraries(memory
        PRIVATE
        vulkan_context
)
