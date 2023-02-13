cmake_minimum_required(VERSION 3.16)

project(framework LANGUAGES C CXX)

set(FRAMEWORK_FILES
    # header files

    #sources files

)

set(COMMON_FILES
    # header files
    ./common/logging.h
    #sources files
)

set(GEOMETRY_FILES
    # Header Files

    # Source Files

)

set(RENDERING_FILES
    # Header Files

    # Source Files
    
)

set(RENDERING_SUBPASSES_FILES
    # Header Files

    # Source Files
    
)

set(SCENE_GRAPH_FILES
    # Header Files

    # Source Files
    
)

set(SCENE_GRAPH_COMPONENT_FILES
    # Header Files

    # Source Files
    
)

set(SCENE_GRAPH_SCRIPTS_FILES
    # Header Files

    # Source Files
    
)

set(STATS_FILES
    # Header Files

    # Source Files
    
)

set(CORE_FILES
    # Header Files

    # Source Files
    
)

set(PLATFORM_FILES
    # Header Files

    # Source Files
    
)

set(GLFW_FILES
    # Header Files

    # Source Files
    
)

set(PARSER_FILES
    # Header Files

    # Source Files
    
)

set(GRAPHING_FILES
    # Header Files

    # Source Files
    
)

set(WINDOWS_FILES
    # Header Files

    # Source Files
    
)

# TODO: source_group

set(PROJECT_FILES
    ${PLATFORM_FILES}
    ${PARSER_FILES}
    ${COMMON_FILES}
    ${FRAMEWORK_FILES}
    ${CORE_FILES}
    ${GEOMETRY_FILES}
    ${RENDERING_FILES}
    ${RENDERING_SUBPASSES_FILES}
    ${SCENE_GRAPH_FILES}
    ${SCENE_GRAPH_COMPONENT_FILES}
    ${SCENE_GRAPH_SCRIPTS_FILES}
    ${STATS_FILES}
    ${GRAPHING_FILES}
    ${GLFW_FILES}
    ${WINDOWS_FILES}
    test.hpp
    test.cpp
)

# add_definitions(-DNOMINMAX)

add_library(${PROJECT_NAME} STATIC ${PROJECT_FILES})

# target_compile_options(${PROJECT_NAME} PUBLIC /MP)

# message(STATUS "Warnings as Errors Enabled")
# target_compile_options(${PROJECT_NAME} PRIVATE /W3 /WX)

# target_compile_definitions(${PROJECT_NAME} PUBLIC VKB_VALIDATION_LAYERS_GPU_ASSISTED)
# target_compile_definitions(${PROJECT_NAME} PUBLIC VKB_VULKAN_DEBUG)

# if(${VKB_ENABLE_PORTABILITY})
#     message(STATUS "Vulkan Portability extension is enabled")
#     target_compile_definitions(${PROJECT_NAME} PUBLIC VKB_ENABLE_PORTABILITY)
# endif()

# TODO: target_include_directories
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link third party libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
    volk
    ktx
    stb
    astc
    imgui
    tinygltf
    glm
    glslang
    SPIRV
    vma
    hwcpipe
    spirv-cross-glsl
    glslang-default-resource-limits
    spdlog
    ctpl
    CLI11::CLI11
    # apps
    # plugins
    glfw
)

target_precompile_headers(${PROJECT_NAME} PRIVATE <vulkan/vulkan.hpp>)