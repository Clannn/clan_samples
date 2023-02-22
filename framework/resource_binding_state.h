#pragma once

#include "common/vk_common.h"
#include "core/buffer.h"
#include "core/image_view.h"
#include "core/sampler.h"

namespace clan
{
    /**
     * @brief A resource info is a struct containing the actual resource data.
     * 
     * This will be referenced by a buffer info or image info descriptor inside a descriptor set.
     */
    struct ResourceInfo
    {
        bool dirty{false};

        const Buffer* buffer{nullptr};

        VkDeviceSize offset{0};

        VkDeviceSize range{0};

        const ImageView* image_view{nullptr};

        const Sampler* sampler{nullptr};
    };

    /**
     * @brief A resource set is a set of bindings containing resources that were bound by commond buffer.
     * 
     * The ResourceSet has a one to one mapping with a Descriptor Set
     */
    class ResourceSet
    {
    public:
        void reset();

        bool is_dirty() const;

        void clear_dirty();

        void clear_dirty(uint32_t binding, uint32_t array_element);

        void bind_buffer(const Buffer& buffer, VkDeviceSize offset, VkDeviceSize range, uint32_t binding, uint32_t array_element);

        void bind_image(const ImageView& image_view, const Sampler& sampler, uint32_t binding, uint32_t array_element);

        void bind_image(const ImageView& image_view, uint32_t binding, uint32_t array_element);

        void bind_input(const ImageView& image_view, uint32_t binding, uint32_t array_element);

    	const BindingMap<ResourceInfo> &get_resource_bindings() const;

    private:
        bool dirty{false};

        BindingMap<ResourceInfo> resource_bindings;
    };

    /**
     * @brief The resource binding state of a command buffer.
     * 
     * Keeps track of all the resources bound by the commond buffer. The ResourceBindingState is used by
     * the commond buffer to create the appropriate descriptor sets when it comes to draw.
     */
    class ResourceBindingState
    {
    public:
        void reset();

        bool is_dirty();

        void clear_dirty();

        void clear_dirty(uint32_t set);

        void bind_buffer(const Buffer& buffer, VkDeviceSize offset, VkDeviceSize range, uint32_t set, uint32_t binding, uint32_t array_element);

        void bind_image(const ImageView& image_view, const Sampler& sampler, uint32_t set, uint32_t binding, uint32_t array_element);

        void bind_image(const ImageView& image_view, uint32_t set, uint32_t binding, uint32_t array_element);

        void bind_input(const ImageView& image_view, uint32_t set, uint32_t binding, uint32_t array_element);

        const std::unordered_map<uint32_t, ResourceSet> &get_resource_sets();

    private:
        bool dirty{false};

        std::unordered_map<uint32_t, ResourceSet> resource_sets;
    };
}