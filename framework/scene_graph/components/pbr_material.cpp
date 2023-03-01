#include "pbr_material.h"

namespace clan
{
    namespace sg
    {
        PBRMaterial::PBRMaterial(const std::string &name) :
            Material{name}
        {}

        std::type_index PBRMaterial::get_type()
        {
            return typeid(PBRMaterial);
        }
    }
}