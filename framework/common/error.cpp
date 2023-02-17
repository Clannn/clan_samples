#include "error.h"

#include "helpers.h"

namespace clan
{
    VulkanException::VulkanException(const VkResult result, const std::string &msg) :
        result{result},
        std::runtime_error{msg}
    {
        m_error_message = std::string(std::runtime_error::what()) + std::string{" : "} + to_string(result);
    }

    const char *VulkanException::what() const noexcept
    {
        return m_error_message.c_str();
    }
}