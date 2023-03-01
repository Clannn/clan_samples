#pragma once

#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "common/glm_common.h"

#include "common/error.h"
#include "scene_graph/components/material.h"

namespace clan
{
    namespace sg
    {
        class PBRMaterial : public Material
        {
        public:
            PBRMaterial(const std::string &name);

            virtual ~PBRMaterial() = default;

            virtual std::type_index get_type() override;

            glm::vec4 base_color_factor{0.0f, 0.0f, 0.0f, 0.0f};

            float metallic_factor{0.0f};

            float roughness_factor{0.0f};
        };
    }
}