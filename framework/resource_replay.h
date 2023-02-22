#pragma once

#include "resource_record.h"

namespace clan
{
    class ResourceCache;

    /**
     * @brief Reads Vulkan objects from a memory stream and creates them in the resource cache.
     */
    class ResourceReplay
    {
    public:
        ResourceReplay();

        void play(ResourceCache &resource_cache, ResourceRecord &recorder);

    protected:
        void create_shader_module(ResourceCache &resource_cache, std::istringstream &stream);

        void create_pipeline_layout(ResourceCache &resource_cache, std::istringstream &stream);

        void create_render_pass(ResourceCache &resource_cache, std::istringstream &stream);

        void create_graphics_pipeline(ResourceCache &resource_cache, std::istringstream &stream);

    private:
        using ResourceFunc = std::function<void(ResourceCache &, std::istringstream &)>;

        std::unordered_map<ResourceType, ResourceFunc> stream_resources;

        std::vector<ShaderModule *> shader_modules;

        std::vector<PipelineLayout *> pipeline_layouts;

        std::vector<const RenderPass *> render_passes;

        std::vector<const GraphicsPipeline *> graphics_pipelines;
    };
}