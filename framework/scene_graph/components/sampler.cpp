#include "sampler.h"

namespace clan
{
    namespace sg
    {
        Sampler::Sampler(const std::string &name, clan::Sampler &&vk_sampler) :
            Component{name},
            vk_sampler{std::move(vk_sampler)}
        {}

        std::type_index Sampler::get_type()
        {
            return typeid(Sampler);
        }
    }

}