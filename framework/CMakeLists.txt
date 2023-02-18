cmake_minimum_required(VERSION 3.16)

project(framework LANGUAGES C CXX)

set(FRAMEWORK_FILES
    # header files
    timer.h
    debug_info.h

    #sources files
    timer.cpp
    debug_info.cpp

)

set(COMMON_FILES
    # header files
    ./common/logging.h
    ./common/helpers.h
    ./common/error.h
    ./common/tags.h
    ./common/strings.h
    ./common/glm_common.h
    ./common/vk_common.h
    #sources files
    ./common/error.cpp
    ./common/tags.cpp
    ./common/vk_common.cpp
    ./common/strings.cpp
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
    ./core/instance.h
    ./core/physical_device.h

    # Source Files
    ./core/instance.cpp
    ./core/physical_device.cpp
    
)

set(PLATFORM_FILES
    # Header Files
    ./platform/parser.h
    ./platform/input_events.h
    ./platform/filesystem.h
    ./platform/configuration.h
    ./platform/application.h
    ./platform/parsers/help_formatter.h
    ./platform/parsers/CLI11.h
    ./platform/plugins/plugin.h

    # Source Files
    ./platform/parser.cpp
    ./platform/filesystem.cpp
    ./platform/configuration.cpp
    ./platform/input_events.cpp
    ./platform/application.cpp
    ./platform/plugins/plugin.cpp
    ./platform/parsers/help_formatter.cpp
    ./platform/parsers/CLI11.cpp

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
)

# mask out the min/max macros from minwindef.h
add_definitions(-DNOMINMAX)

add_library(${PROJECT_NAME} STATIC ${PROJECT_FILES})

if(${VKB_VULKAN_DEBUG})
    target_compile_definitions(${PROJECT_NAME} PUBLIC VKB_VULKAN_DEBUG)
endif()

if(${VKB_VALIDATION_LAYERS})
    target_compile_definitions(${PROJECT_NAME} PUBLIC VKB_VALIDATION_LAYERS)
endif()
# target_compile_options(${PROJECT_NAME} PUBLIC /MP)

# message(STATUS "Warnings as Errors Enabled")
# target_compile_options(${PROJECT_NAME} PRIVATE /W3 /WX)

# target_compile_definitions(${PROJECT_NAME} PUBLIC VKB_VALIDATION_LAYERS_GPU_ASSISTED)
# target_compile_definitions(${PROJECT_NAME} PUBLIC VKB_VULKAN_DEBUG)

# if(${VKB_ENABLE_PORTABILITY})
#     message(STATUS "Vulkan Portability extension is enabled")
#     target_compile_definitions(${PROJECT_NAME} PUBLIC VKB_ENABLE_PORTABILITY)
# endif()

target_include_directories(${PROJECT_NAME} PUBLIC $<TARGET_PROPERTY:plugins,INCLUDE_DIRECTORIES> ${CMAKE_CURRENT_SOURCE_DIR})

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
    apps
    plugins
    glfw
)

target_precompile_headers(${PROJECT_NAME} PRIVATE <vulkan/vulkan.hpp>)