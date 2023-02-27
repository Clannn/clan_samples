#pragma once

#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

#include "scene_graph/component.h"
#include "scene_graph/components/sampler.h"

namespace clan
{
    namespace sg{
        class Image;
        class Sampler;

        class Texture : public Component
        {
        public:
            Texture(const std::string &name);

            Texture(Texture &&other) = default;

            virtual ~Texture() = default;

            virtual std::type_index get_type() override;

            void set_image(Image &image);

            Image *get_image();

            void set_sampler(Sampler &sampler);

            Sampler *get_sampler();

        private:
            Image* image{nullptr};

            Sampler* sampler{nullptr};
        };
    }
}