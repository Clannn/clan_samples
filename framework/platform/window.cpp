#include "window.h"

#include "platform/platform.h"

namespace clan
{
    Window::Window(const Properties& properties) :
        properties{properties}
    { }

    void Window::process_events()
    { }

    Window::Extent Window::resize(const Extent &new_extent)
    {
        if (properties.resizable){
            properties.extent.width  = new_extent.width;
            properties.extent.height = new_extent.height;
        }
        return properties.extent;
    }

    const Window::Extent &Window::get_extent() const
    {
        return properties.extent;
    }

    float Window::get_content_scale_factor() const
    {
        return 1.0f;
    }

    Window::Mode Window::get_window_mode() const
    {
        return properties.mode;
    }

    bool Window::get_display_present_info(VkDisplayPresentInfoKHR *info,
                                        uint32_t src_width, uint32_t src_height) const
    {
        // Default is to not use the extra present info
        return false;
    }
}