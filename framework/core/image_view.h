#pragma once

#include "common/helpers.h"
#include "common/vk_common.h"
#include "core/image.h"
#include "core/vulkan_resource.h"

namespace clan
{
    class ImageView : public VulkanResource<VkImageView, VK_OBJECT_TYPE_IMAGE_VIEW, const Device>
    {
    public:
        ImageView(Image& image, VkImageViewType view_type, VkFormat format = VK_FORMAT_UNDEFINED,
                  uint32_t base_mip_level = 0, uint32_t base_array_layer = 0,
                  uint32_t n_mip_levels = 0, uint32_t n_array_layers = 0
        );

        ImageView(const ImageView&) = delete;
        ImageView& operator=(const ImageView&) = delete;
        ImageView& operator=(Image&&) = delete;

        ImageView(ImageView&& other);

        ~ImageView() override;

        const Image& get_image() const;

        /**
         * @brief Update the image this view is referring to used on image move
         */
        void set_image(Image& image);

        VkFormat get_format() const;

        VkImageSubresourceRange get_subresource_range() const;

        VkImageSubresourceLayers get_subresource_layers() const;


    private:
        Image* image{};

        VkFormat format{};

        VkImageSubresourceRange subresource_range{};
    };
}