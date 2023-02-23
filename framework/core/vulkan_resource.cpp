#include "vulkan_resource.h"

#include "device.h"

namespace clan
{
    namespace detail
    {
        void set_debug_name(const Device *device, VkObjectType object_type, uint64_t handle, const char *debug_name)
        {
            if (!debug_name || *debug_name == '\0' || !device){
                return;
            }
            device->get_debug_utils().set_debug_name(device->get_handle(), object_type, handle, debug_name);
        }
    }
}