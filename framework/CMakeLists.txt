cmake_minimum_required(VERSION 3.16)

project(framework LANGUAGES C CXX)

set(FRAMEWORK_FILES
    # header files
    timer.h
    debug_info.h
    resource_binding_state.h
    fence_pool.h
    resource_record.h
    resource_replay.h
    resource_cache.h
    spirv_reflection.h
    glsl_compiler.h
    buffer_pool.h
    semaphore_pool.h
    gltf_loader.h
    gui.h
    clan_sample.h

    #sources files
    timer.cpp
    debug_info.cpp
    resource_binding_state.cpp
    fence_pool.cpp
    resource_record.cpp
    resource_replay.cpp
    resource_cache.cpp
    spirv_reflection.cpp
    glsl_compiler.cpp
    buffer_pool.cpp
    semaphore_pool.cpp
    gltf_loader.cpp
    gui.cpp
    clan_sample.cpp

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
    ./common/resource_caching.h
    ./common/utils.h

    #sources files
    ./common/error.cpp
    ./common/vk_common.cpp
    ./common/strings.cpp
    ./common/utils.cpp

)

set(RENDERING_FILES
    # Header Files
    ./rendering/render_target.h
    ./rendering/pipeline_state.h
    ./rendering/render_frame.h
    ./rendering/render_context.h
    ./rendering/subpass.h
    ./rendering/render_pipeline.h
    ./rendering/subpasses/forward_subpass.h
    ./rendering/subpasses/geometry_subpass.h

    # Source Files
    ./rendering/render_target.cpp
    ./rendering/pipeline_state.cpp
    ./rendering/render_frame.cpp
    ./rendering/render_context.cpp
    ./rendering/subpass.cpp
    ./rendering/render_pipeline.cpp
    ./rendering/subpasses/forward_subpass.cpp
    ./rendering/subpasses/geometry_subpass.cpp
    
)

set(SCENE_GRAPH_FILES
    # Header Files
    ./scene_graph/component.h
    ./scene_graph/node.h
    ./scene_graph/scene.h
    ./scene_graph/script.h

    # Source Files
    ./scene_graph/component.cpp
    ./scene_graph/node.cpp
    ./scene_graph/scene.cpp
    ./scene_graph/script.cpp
    
)

set(SCENE_GRAPH_COMPONENT_FILES
    # Header Files
    ./scene_graph/components/transform.h
    ./scene_graph/components/light.h
    ./scene_graph/components/sampler.h
    ./scene_graph/components/texture.h
    ./scene_graph/components/sub_mesh.h
    ./scene_graph/components/material.h
    ./scene_graph/components/camera.h
    ./scene_graph/components/perspective_camera.h
    ./scene_graph/components/orthographic_camera.h
    ./scene_graph/components/image.h
    ./scene_graph/components/stb.h
    ./scene_graph/components/pbr_material.h
    ./scene_graph/components/aabb.h
    ./scene_graph/components/mesh.h
    ./scene_graph/components/astc.h
    ./scene_graph/components/ktx.h

    # Source Files
    ./scene_graph/components/transform.cpp
    ./scene_graph/components/light.cpp
    ./scene_graph/components/sampler.cpp
    ./scene_graph/components/texture.cpp
    ./scene_graph/components/sub_mesh.cpp
    ./scene_graph/components/material.cpp
    ./scene_graph/components/camera.cpp
    ./scene_graph/components/perspective_camera.cpp
    ./scene_graph/components/orthographic_camera.cpp
    ./scene_graph/components/image.cpp
    ./scene_graph/components/stb.cpp
    ./scene_graph/components/pbr_material.cpp
    ./scene_graph/components/aabb.cpp
    ./scene_graph/components/mesh.cpp
    ./scene_graph/components/astc.cpp
    ./scene_graph/components/ktx.cpp

)

set(SCENE_GRAPH_SCRIPTS_FILES
    # Header Files
    scene_graph/scripts/free_camera.h
    scene_graph/scripts/animation.h
    scene_graph/scripts/node_animation.h

    # Source Files
    scene_graph/scripts/free_camera.cpp
    scene_graph/scripts/animation.cpp
    scene_graph/scripts/node_animation.cpp
    
)

set(STATS_FILES
    # Header Files
    stats/stats.h
    stats/stats_common.h
    stats/stats_provider.h
    stats/frame_time_stats_provider.h
    stats/vulkan_stats_provider.h

    # Source Files
    stats/stats.cpp
    stats/stats_provider.cpp
    stats/frame_time_stats_provider.cpp
    stats/vulkan_stats_provider.cpp

)

set(CORE_FILES
    # Header Files
    ./core/instance.h
    ./core/physical_device.h
    ./core/swapchain.h
    ./core/queue.h
    ./core/vulkan_resource.h
    ./core/debug.h
    ./core/image.h
    ./core/image_view.h
    ./core/shader_module.h
    ./core/descriptor_set_layout.h
    ./core/descriptor_pool.h
    ./core/descriptor_set.h
    ./core/pipeline_layout.h
    ./core/render_pass.h
    ./core/pipeline.h
    ./core/framebuffer.h
    ./core/buffer.h
    ./core/query_pool.h
    ./core/sampler.h
    ./core/command_buffer.h
    ./core/command_pool.h
    ./core/device.h

    # Source Files
    ./core/instance.cpp
    ./core/physical_device.cpp
    ./core/swapchain.cpp
    ./core/queue.cpp
    ./core/vulkan_resource.cpp
    ./core/debug.cpp
    ./core/image.cpp
    ./core/image_view.cpp
    ./core/shader_module.cpp
    ./core/descriptor_set_layout.cpp
    ./core/descriptor_pool.cpp
    ./core/descriptor_set.cpp
    ./core/pipeline_layout.cpp
    ./core/render_pass.cpp
    ./core/pipeline.cpp
    ./core/framebuffer.cpp
    ./core/buffer.cpp
    ./core/query_pool.cpp
    ./core/sampler.cpp
    ./core/command_buffer.cpp
    ./core/command_pool.cpp
    ./core/device.cpp
    
)

set(PLATFORM_FILES
    # Header Files
    ./platform/parser.h
    ./platform/input_events.h
    ./platform/filesystem.h
    ./platform/configuration.h
    ./platform/application.h
    ./platform/window.h
    ./platform/platform.h
    ./platform/glfw_window.h
    ./platform/windows_platform.h
    ./platform/parsers/help_formatter.h
    ./platform/parsers/CLI11.h
    ./platform/plugins/plugin.h
    ./platform/plugins/plugin_base.h

    # Source Files
    ./platform/parser.cpp
    ./platform/filesystem.cpp
    ./platform/configuration.cpp
    ./platform/input_events.cpp
    ./platform/window.cpp
    ./platform/application.cpp
    ./platform/platform.cpp
    ./platform/glfw_window.cpp
    ./platform/windows_platform.cpp
    ./platform/plugins/plugin.cpp
    ./platform/parsers/help_formatter.cpp
    ./platform/parsers/CLI11.cpp

)


set(PROJECT_FILES
    ${PLATFORM_FILES}
    ${COMMON_FILES}
    ${FRAMEWORK_FILES}
    ${CORE_FILES}
    ${RENDERING_FILES}
    ${SCENE_GRAPH_FILES}
    ${SCENE_GRAPH_COMPONENT_FILES}
    ${SCENE_GRAPH_SCRIPTS_FILES}
    ${STATS_FILES}
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