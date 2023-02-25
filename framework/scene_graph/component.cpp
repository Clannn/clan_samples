#include "component.h"

namespace clan
{
    Component::Component(const std::string& name) :
        name{name}
    { }

    const std::string& Component::get_name() const
    {
        return name;
    }
}