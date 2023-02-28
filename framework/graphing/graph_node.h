#pragma once

#include <json.hpp>

namespace clan
{
    namespace graphing
    {
        class Node
        {
        public:
            Node() { };
            
            Node(size_t id, const char* title, const char* style = NULL, const nlohmann::json& data = {});

            template <typename T>
            static std::uintptr_t handle_to_uintptr_t(T handle)
            {
                return reinterpret_cast<std::uintptr_t>(reinterpret_cast<void *>(handle));
            }

            nlohmann::json attributes;
        };
    }
}