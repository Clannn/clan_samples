#pragma once

#include <cassert>
#include <stdexcept>
#include <string>

#include "common/strings.h"
#include "logging.h"
#include "vk_common.h"

#if defined(_MSC_VER)
#	define VKBP_DISABLE_WARNINGS() \
		__pragma(warning(push, 0))

#	define VKBP_ENABLE_WARNINGS() \
		__pragma(warning(pop))
#endif

namespace clan
{
    /**
     * @brief Vulkan exception structure
     */
    class VulkanException : public std::runtime_error
    {
    public:
        /**
         * @brief Vulkan exception constructor
         */
        VulkanException(VkResult result, const std::string &msg = "Vulkan error");

        /**
         * @brief Returns the Vulkan error code as string
         * @return String message of exception
         */
        const char *what() const noexcept override;

        VkResult result;

    private:
        std::string m_error_message;
    };
}   //namespace clan

/// @brief Helper macro to test the result of Vulkan calls which can return an error.
#define VK_CHECK(x)                                                 \
	do                                                              \
	{                                                               \
		VkResult err = x;                                           \
		if (err)                                                    \
		{                                                           \
			LOGE("Detected Vulkan error: {}", clan::to_string(err)); \
			abort();                                                \
		}                                                           \
	} while (0)

#define ASSERT_VK_HANDLE(handle)        \
	do                                  \
	{                                   \
		if ((handle) == VK_NULL_HANDLE) \
		{                               \
			LOGE("Handle is NULL");     \
			abort();                    \
		}                               \
	} while (0)
