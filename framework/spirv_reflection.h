#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "common/error.h"
#include "common/vk_common.h"
#include "core/shader_module.h"

#include <spirv_glsl.hpp>

namespace clan
{
    /// Generate a list of shader resource based on SPIRV reflection code, and provided ShaderVariant
    class SPIRVReflection
    {
    public:
        /// @brief Reflects shader resources from SPIRV code
        /// @param stage The Vulkan shader stage flag
        /// @param spirv The SPIRV code of shader
        /// @param[out] resources The list of reflected shader resources
        /// @param variant ShaderVariant used for reflection to specify the size of the runtime arrays in Storage Buffers
        bool reflect_shader_resources(VkShaderStageFlagBits        stage,
                                      const std::vector<uint32_t> &spirv,
                                      std::vector<ShaderResource> &resources,
                                      const ShaderVariant &        variant);

    private:
        void parse_shader_resources(const spirv_cross::Compiler &compiler,
                                    VkShaderStageFlagBits        stage,
                                    std::vector<ShaderResource> &resources,
                                    const ShaderVariant &        variant);

        void parse_push_constants(const spirv_cross::Compiler &compiler,
                                  VkShaderStageFlagBits        stage,
                                  std::vector<ShaderResource> &resources,
                                  const ShaderVariant &        variant);

        void parse_specialization_constants(const spirv_cross::Compiler &compiler,
                                            VkShaderStageFlagBits        stage,
                                            std::vector<ShaderResource> &resources,
                                            const ShaderVariant &        variant);
    };
}