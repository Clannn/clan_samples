#pragma once

#include "common/helpers.h"
#include "common/vk_common.h"
#include "core/render_pass.h"
#include "rendering/render_target.h"

namespace clan
{
    class Device;

    class Framebuffer
    {
    public:
        Framebuffer(Device& device, const RenderTarget& render_target, const RenderPass& render_pass);

        Framebuffer(Framebuffer&& other);

        ~Framebuffer();

        Framebuffer(const Framebuffer &) = delete;
        Framebuffer &operator=(const Framebuffer &) = delete;
        Framebuffer &operator=(Framebuffer &&) = delete;

        VkFramebuffer get_handle() const;

        const VkExtent2D& get_extent() const;

    private:
        Device& device;
        
        VkFramebuffer handle{VK_NULL_HANDLE};

        VkExtent2D extent{};
    };
}