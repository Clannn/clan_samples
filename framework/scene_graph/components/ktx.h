#pragma once

#include "scene_graph/components/image.h"

namespace clan
{
    namespace sg
    {
        class Ktx : public Image
        {
        public:
            Ktx(const std::string &name, const std::vector<uint8_t> &data, ContentType content_type);

            virtual ~Ktx() = default;
        };
    }
}