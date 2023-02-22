#pragma once

#include "common/helpers.h"
#include "common/vk_common.h"

namespace clan
{
    class Device;

    /**
     * @brief Represents a Vulkan Query Pool
     */
    class QueryPool
    {
    public:
        /**
         * @brief Creates a Vulkan Query Pool
         * @param device The device to use
         * @param info Creation details
         */
        QueryPool(Device& device, const VkQueryPoolCreateInfo& info);

        QueryPool(QueryPool&& other);

        ~QueryPool();

        QueryPool(const QueryPool &) = delete;
        QueryPool &operator=(const QueryPool &) = delete;
        QueryPool &operator=(QueryPool &&) = delete;

        /**
         * @return The vulkan query pool handles
         */
        VkQueryPool get_handle() const;

        /**
         * @brief Reset a range of queries in the query pool. Only call if VK_EXT_host_query_reset is enabled.
         * @param first_query The first query to reset
         * @param query_count The number of queries to reset
         */
        void host_reset(uint32_t first_query, uint32_t query_count);

        /**
         * @brief Get query pool results
         * @param first_query The initial query index
         * @param num_queries The number of queries to read
         * @param result_bytes The number of bytes in the results array
         * @param results Array of bytes
         * @param stride The stride in bytes between results for individual queries
         * @param flags A bitmask of VkQueryResultFlagBits
         */
        VkResult get_results(uint32_t            first_query,
                             uint32_t            num_queries,
                             size_t              result_bytes,
                             uint8_t*            results,
                             VkDeviceSize        stride,
                             VkQueryResultFlags  flags
        );
    
    private:
        Device& device;

        VkQueryPool handle{VK_NULL_HANDLE};
    };
}