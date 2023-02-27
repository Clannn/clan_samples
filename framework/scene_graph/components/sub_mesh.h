#pragma once

#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "common/vk_common.h"
#include "core/buffer.h"
#include "core/shader_module.h"
#include "scene_graph/component.h"

namespace clan
{
    namespace sg
    {
        class Material;

        struct VertexAttribute
        {
            VkFormat format = VK_FORMAT_UNDEFINED;

            std::uint32_t stride = 0;

            std::uint32_t offset = 0;
        };

        class SubMesh : public Component
        {
        public:
            SubMesh(const std::string &name = {});

            virtual ~SubMesh() = default;

            virtual std::type_index get_type() override;

            VkIndexType index_type{};

            std::uint32_t index_offset = 0;

            std::uint32_t vertices_count = 0;

            std::uint32_t vertex_indices = 0;

            std::unordered_map<std::string, Buffer> vertex_buffers;

            std::unique_ptr<Buffer> index_buffer;

            void set_attribute(const std::string &name, const VertexAttribute &attribute);

            bool get_attribute(const std::string &name, VertexAttribute &attribute) const;

            void set_material(const Material &material);

            const Material *get_material() const;

            const ShaderVariant &get_shader_variant() const;

            ShaderVariant &get_mut_shader_variant();

        private:
            std::unordered_map<std::string, VertexAttribute> vertex_attributes;

            const Material* material{nullptr};

            ShaderVariant shader_variant;

            void compute_shader_variant();
        };
    }
}