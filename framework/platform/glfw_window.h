#pragma once

#include "common/vk_common.h"
#include "platform/window.h"

struct GLFWwindow;

namespace clan
{
    class Platform;
    /**
     * @brief An implementation of GLFW, inheriting the behaviour of the Window interface
     */
    class GlfwWindow : public Window
    {
    public:
        GlfwWindow(Platform *platform, const Window::Properties &properties);

        virtual ~GlfwWindow();

        VkSurfaceKHR create_surface(Instance &instance) override;

        VkSurfaceKHR create_surface(VkInstance instance, VkPhysicalDevice physical_device) override;

        bool should_close() override;

        void process_events() override;

        void close() override;

        float get_dpi_factor() const override;

        float get_content_scale_factor() const override;

    private:
        GLFWwindow *handle = nullptr;
    };
}