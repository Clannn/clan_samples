#pragma once

#include "common/helpers.h"
#include "common/vk_common.h"
#include "core/vulkan_resource.h"

namespace clan
{
    class Device;

    /**
     * @brief Represents a Vulkan Sampler
     */
    class Sampler : public VulkanResource<VkSampler, VK_OBJECT_TYPE_SAMPLER, const Device>
    {
    public:
        /**
         * @brief Creates a Vulkan Sampler
         * @param device The device to use
         * @param info Creation details
         */
        Sampler(const Device& device, const VkSamplerCreateInfo& info);

        Sampler(Sampler&& other);

        ~Sampler();

        Sampler(const Sampler &) = delete;
        Sampler &operator=(const Sampler &) = delete;
        Sampler &operator=(Sampler &&) = delete;
    };
}