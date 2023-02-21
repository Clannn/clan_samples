#pragma once

#include "common/helpers.h"
#include "common/vk_common.h"
#include "core/vulkan_resource.h"

namespace clan
{
    struct Attachment;
    class Device;

    struct SubpassInfo
    {
        std::vector<uint32_t> input_attachments;

        std::vector<uint32_t> output_attachments;

        std::vector<uint32_t> color_resolve_attachments;

        bool disable_depth_stencil_attachment;

        uint32_t depth_stencil_resolve_attachment;

        VkResolveModeFlagBits depth_stencil_resolve_mode;

        std::string debug_name;
    };

    class RenderPass : public VulkanResource<VkRenderPass, VK_OBJECT_TYPE_RENDER_PASS>
    {
    public:
        RenderPass(Device&                           device,
                   const std::vector<Attachment>&    attachments,
                   const std::vector<LoadStoreInfo>& load_store_infos,
                   const std::vector<SubpassInfo>&   subpasses

        );

        RenderPass(RenderPass&& other);

        ~RenderPass();

        RenderPass(const RenderPass &) = delete;
        RenderPass &operator=(const RenderPass &) = delete;
        RenderPass &operator=(RenderPass &&) = delete;

        const uint32_t get_color_output_count(uint32_t subpass_index) const;

        const VkExtent2D get_render_area_granularity() const;


    private:
        size_t subpass_count;

        std::vector<uint32_t> color_output_count;

        // if device extention "VK_KHR_create_renderpass2" is enable, it will create Type ....2KHR
        template <typename T_SubpassDescription, typename T_AttachmentDescription, typename T_AttachmentReference, typename T_SubpassDependency, typename T_RenderPassCreateInfo>
        void create_renderpass(const std::vector<Attachment> &attachments, const std::vector<LoadStoreInfo> &load_store_infos, const std::vector<SubpassInfo> &subpasses);
    };
}