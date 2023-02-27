#pragma once 

#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

#include "core/sampler.h"
#include "scene_graph/component.h"

namespace clan
{
    namespace sg
    {
        class Sampler : public Component
        {
        public:
            Sampler(const std::string &name, clan::Sampler &&vk_sampler);

            Sampler(Sampler &&other) = default;

            virtual ~Sampler() = default;

            virtual std::type_index get_type() override;

            clan::Sampler vk_sampler;
        };
    }

}