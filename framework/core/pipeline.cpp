#include "pipeline.h"

#include "device.h"
#include "pipeline_layout.h"
#include "shader_module.h"

namespace clan
{
    Pipeline::Pipeline(Device &device) :
        device{device}
    {}

    Pipeline::Pipeline(Pipeline &&other) :
        device{other.device},
        handle{other.handle},
        state{other.state}
    {
        other.handle = VK_NULL_HANDLE;
    }

    Pipeline::~Pipeline()
    {
        // Destroy pipeline
        if (handle != VK_NULL_HANDLE){
            vkDestroyPipeline(device.get_handle(), handle, nullptr);
        }
    }

    VkPipeline Pipeline::get_handle() const
    {
        return handle;
    }

    const PipelineState &Pipeline::get_state() const
    {
        return state;
    }
}