#include "buffer.h"

#include "device.h"

namespace clan
{
    Buffer::Buffer(Device const &device, VkDeviceSize size, VkBufferUsageFlags buffer_usage, VmaMemoryUsage memory_usage, VmaAllocationCreateFlags flags, const std::vector<uint32_t> &queue_family_indices) :
        VulkanResource{VK_NULL_HANDLE, &device},
        size{size}
    {
        persistent = (flags & VMA_ALLOCATION_CREATE_MAPPED_BIT) != 0;

        VkBufferCreateInfo buffer_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        buffer_info.usage = buffer_usage;
        buffer_info.size  = size;

        if(queue_family_indices.size() >= 2){
            buffer_info.sharingMode           = VK_SHARING_MODE_CONCURRENT;
            buffer_info.queueFamilyIndexCount = to_u32(queue_family_indices.size());
            buffer_info.pQueueFamilyIndices   = queue_family_indices.data();
        }

        VmaAllocationCreateInfo memory_info{};
        memory_info.flags = flags;
        memory_info.usage = memory_usage;

        VmaAllocationInfo allocation_info{};
        auto result = vmaCreateBuffer(device.get_memory_allocator(),
                         &buffer_info, &memory_info,
                         &handle, &allocation,
                         &allocation_info);

        if (result != VK_SUCCESS){
            throw VulkanException{result, "Cannot create Buffer"};
        }

        memory = allocation_info.deviceMemory;

        if (persistent){
            mapped_data = static_cast<uint8_t *>(allocation_info.pMappedData);
        }
    }

    Buffer::Buffer(Buffer &&other) :
        VulkanResource{other.handle, other.device},
        allocation{other.allocation},
        memory{other.memory},
        size{other.size},
        mapped_data{other.mapped_data},
        mapped{other.mapped}
    {
        // Reset other handles to avoid releasing on destruction
        other.allocation  = VK_NULL_HANDLE;
        other.memory      = VK_NULL_HANDLE;
        other.mapped_data = nullptr;
        other.mapped      = false;
    }

    Buffer::~Buffer()
    {
        if(handle != VK_NULL_HANDLE && allocation != VK_NULL_HANDLE){
            unmap();
            vmaDestroyBuffer(device->get_memory_allocator(), handle, allocation);
        }
    }

    const VkBuffer *Buffer::get() const
    {
        return &handle;
    }

    VmaAllocation Buffer::get_allocation() const
    {
        return allocation;
    }

    VkDeviceMemory Buffer::get_memory() const
    {
        return memory;
    }

    VkDeviceSize Buffer::get_size() const
    {
        return size;
    }

    uint8_t *Buffer::map()
    {
        if (!mapped && !mapped_data){
            VK_CHECK(vmaMapMemory(device->get_memory_allocator(), allocation, reinterpret_cast<void **>(&mapped_data)));
            mapped = true;
        }
        return mapped_data;
    }

    void Buffer::unmap()
    {
        if(mapped){
            vmaUnmapMemory(device->get_memory_allocator(), allocation);
            mapped_data = nullptr;
            mapped      = false;
        }
    }

    void Buffer::flush() const
    {
        vmaFlushAllocation(device->get_memory_allocator(), allocation, 0, size);
    }

    void Buffer::update(const std::vector<uint8_t> &data, size_t offset)
    {
        update(data.data(), data.size(), offset);
    }

    uint64_t Buffer::get_device_address()
    {
        VkBufferDeviceAddressInfoKHR buffer_device_address_info{};
        buffer_device_address_info.sType  = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
        buffer_device_address_info.buffer = handle;
        return vkGetBufferDeviceAddressKHR(device->get_handle(), &buffer_device_address_info);
    }

    void Buffer::update(void *data, size_t size, size_t offset)
    {
        update(reinterpret_cast<const uint8_t *>(data), size, offset);
    }

    void Buffer::update(const uint8_t *data, const size_t size, const size_t offset)
    {
        if(persistent){
            std::copy(data, data + size, mapped_data + offset);
            flush();
        }
        else{
            map();
            std::copy(data, data + size, mapped_data + offset);
            flush();
            unmap();
        }
    }
}