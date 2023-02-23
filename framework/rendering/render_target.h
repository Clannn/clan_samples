#pragma once

#include "common/helpers.h"
#include "common/vk_common.h"
#include "core/image.h"
#include "core/image_view.h"

namespace clan
{
    class Device;

    /**
     * @brief Description of render pass attachments.
     * Attachment descriptions can be used to automatically create render target images
     */

    struct Attachment
    {
        VkFormat format{VK_FORMAT_UNDEFINED};
        
        VkSampleCountFlagBits samples{VK_SAMPLE_COUNT_1_BIT};

        VkImageUsageFlags usage{VK_IMAGE_USAGE_SAMPLED_BIT};

        VkImageLayout initial_layout{VK_IMAGE_LAYOUT_UNDEFINED};

        Attachment() = default;

        Attachment(VkFormat format, VkSampleCountFlagBits samples, VkImageUsageFlags usage);
    };

    /**
     * @brief RenderTarget contains three vectors for: Image, ImageView and Attachment.
     * The first two are Vulkan images and corresponding image views respectively.
     * Attachments contain a description of the images, which has two main purposes:
     * - RenderPass creation only needs a list of Attachments, not the actual images, so we keep
     *   the minimum amount of information necessary
     * - Creation of a RenderTarget becomes simpler, because the caller can just ask for some
     *   Attachments without having to create the images
     */
    class RenderTarget
    {
    public:
        using CreateFunc = std::function<std::unique_ptr<RenderTarget>(Image&&)>;

        static const CreateFunc DEFAULT_CREATE_FUNC;

        RenderTarget(std::vector<Image>&& images);

        RenderTarget(std::vector<ImageView>&& image_views);

        RenderTarget(const RenderTarget&) = delete;
        RenderTarget(RenderTarget&&) = delete;
        RenderTarget& operator=(RenderTarget&&) = delete;
        RenderTarget& operator=(const RenderTarget&) = delete;

        const VkExtent2D& get_extent() const;

        const std::vector<ImageView>& get_views() const;

        const std::vector<Attachment>& get_attachments() const;

        /**
         * @brief Sets the current input attachments overwriting the current ones
         *        Should be set before beginning the render pass and before starting a new subpass
         * @param input array of attachment reference number to use as input
         */
        void set_input_attachments(std::vector<uint32_t>& input);

        const std::vector<uint32_t>& get_input_attachments() const;

        /**
         * @brief Sets the current output attachments overwritting the current ones
         *        Should be set before beginning the render pass and before starting a new subpass
         * @param output array of attachment reference number to use as output
         */
        void set_output_attachments(std::vector<uint32_t>& output);

        const std::vector<uint32_t>& get_output_attachments() const;

        void set_layout(uint32_t attachment, VkImageLayout layout);

        VkImageLayout get_layout(uint32_t attachment) const;

    private:
        const Device& device;

        VkExtent2D extent{};

        std::vector<Image> images;

        std::vector<ImageView> views;

        std::vector<Attachment> attachments;

        // By default there are no input attachments
        std::vector<uint32_t> input_attachments = {};

        // By default the output attachments is attachment 0
        std::vector<uint32_t> output_attachments = {0};
    };
}